#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/fsmc.h>

#include <string.h>

static void printicon(int x, int y, int col, const uint8_t *icon);

#define DEFINE_CUSTOM_LOGO 1

#include "pins.h"
#include "bl.h"

#ifndef CUSTOM_LOGO
#define CUSTOM_LOGO /* nothing */
#endif

#define DISPLAY_WIDTH 160
#define DISPLAY_HEIGHT 128

#define ST7735_NOP 0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID 0x04
#define ST7735_RDDST 0x09

#define ST7735_SLPIN 0x10
#define ST7735_SLPOUT 0x11
#define ST7735_PTLON 0x12
#define ST7735_NORON 0x13

#define ST7735_INVOFF 0x20
#define ST7735_INVON 0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON 0x29
#define ST7735_CASET 0x2A
#define ST7735_RASET 0x2B
#define ST7735_RAMWR 0x2C
#define ST7735_RAMRD 0x2E

#define ST7735_PTLAR 0x30
#define ST7735_COLMOD 0x3A
#define ST7735_MADCTL 0x36

#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR 0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1 0xC0
#define ST7735_PWCTR2 0xC1
#define ST7735_PWCTR3 0xC2
#define ST7735_PWCTR4 0xC3
#define ST7735_PWCTR5 0xC4
#define ST7735_VMCTR1 0xC5

#define ST7735_RDID1 0xDA
#define ST7735_RDID2 0xDB
#define ST7735_RDID3 0xDC
#define ST7735_RDID4 0xDD

#define ST7735_PWCTR6 0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

#define FSMC_BASE0 ((uint32_t)0x60000000)
#define FSMC_BASE1 ((uint32_t)0x60000001)
#define FSMC_CmdWrite(cmd) *(volatile uint8_t *)(FSMC_BASE0) = (cmd)
#define FSMC_DataWrite(data) *(volatile uint8_t *)(FSMC_BASE1) = (data)

static int isSPI;

bool hasScreen() {
    return lookupCfg(CFG_PIN_DISPLAY_MOSI, -1) != -1;
}

void screen_delay(unsigned msec) {
    int k = msec * 15000;
    while (k--)
        asm("nop");
}

void transfer(uint8_t *ptr, uint32_t len) {
    uint8_t b;
    if (isSPI) {
        int mosi = CFG(PIN_DISPLAY_MOSI);
        int sck = CFG(PIN_DISPLAY_SCK);
        assert(pinport(mosi) == pinport(sck));
        volatile uint32_t *port = &GPIO_BSRR(pinport(mosi));
        uint32_t mosi_mask = pinmask(mosi);
        uint32_t sck_mask = pinmask(sck);

        uint8_t mask = 0;
        for (;;) {
            if (!mask) {
                if (!len--)
                    break;
                mask = 0x80;
                b = *ptr++;
            }
            *port = (b & mask) ? mosi_mask : (mosi_mask << 16);
            *port = sck_mask;
            mask >>= 1;
            *port = sck_mask << 16;
        }
    } else {
        while (len--)
            FSMC_DataWrite(*ptr++);
    }
}

#define DELAY 0x80

static const uint8_t sleepCmds[] = {ST7735_SLPIN, 0, 0, 0};

// clang-format off
static const uint8_t initCmds[] = {
    ST7735_SWRESET,   DELAY,  //  1: Software reset, 0 args, w/delay
      120,                    //     150 ms delay
    ST7735_SLPOUT ,   DELAY,  //  2: Out of sleep mode, 0 args, w/delay
      120,                    //     500 ms delay
    ST7735_INVOFF , 0      ,  // 13: Don't invert display, no args, no delay
    ST7735_COLMOD , 1      ,  // 15: set color mode, 1 arg, no delay:
      0x05,                  //     16-bit color
    ST7735_GMCTRP1, 16      , //  1: Magical unicorn dust, 16 args, no delay:
      0x02, 0x1c, 0x07, 0x12,
      0x37, 0x32, 0x29, 0x2d,
      0x29, 0x25, 0x2B, 0x39,
      0x00, 0x01, 0x03, 0x10,
    ST7735_NORON  ,    DELAY, //  3: Normal display on, no args, w/delay
      10,                     //     10 ms delay
    ST7735_DISPON ,    DELAY, //  4: Main screen turn on, no args w/delay
      10,
    0, 0 // END
};
static const uint8_t initCmds9341[] = {
    0x28, 0,  // display off
    0x20, 0,  // inversion off
    0xCF, 3, // power ctrl b
        0x00, 0xC1, 0x30,
    0xED, 4, // power seq
        0x64, 0x03, 0x12, 0x81,
    0xE8, 3, // driver timing a
        0x85, 0x00, 0x78,
    0xCB, 5, // power ctrl a
        0x39, 0x2C, 0x00, 0x34, 0x02,
    0xF7, 1,  // pump ratio
        0x20,
    0xEA, 2, // driver timing c
        0x00, 0x00,
    0xC0, 1,  // power ctrl1
        0x23,
    0xC1, 1,  // power ctrl2
        0x10,
    0xC5, 2,  // vcom ctrl1
        0x3E, 0x28,
    0xC7, 1,  // vcom ctrl2
        0x86,
    0xB6, 3,  // display func
        0x08, 0x82, 0x27,
    0xF2, 1,  // enable 3gamma
        0x00,
    0x26, 1,  // gamma set
        0x01,
    0xE0, 15,  // positive gamma
        0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,
    0xE1, 15,  // negative gamma
        0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,
    0x3A, 1,  // pixel format
        0x55,
    0x11, DELAY, 100,  // sleep out
    0x29, 0,  // display on
    0, 0  // END
};
static const uint8_t initCmds7789[] = {
    ST7735_DISPOFF, 0,  // display off
    ST7735_INVOFF, 0,  // inversion off
    0xB2, 5,  // porch setting
        0x0c, 0x0c, 0x00, 0x33, 0x33,
    0xB7, 1,  // gate ctrl
        0x75,
    0xBB, 1,  // vcom setting
        0x35,
    0xC0, 1,  // lcm ctrl
        0x2c,
    0xC2, 1,  // vdv/vrh enable
        0x01,
    0xC3, 1,  // vrh set
        0x13,
    0xC4, 1,  // vdv set
        0x20,
    0xC6, 1,  // frame rate
        0x0f,
    0xD0, 2,  // power ctrl1
        0xa4, 0x81,
    ST7735_GMCTRP1, 14,  // positive gamma
        0xd0, 0x08, 0x10, 0x0d, 0x0c, 0x07, 0x37, 0x53, 0x4c, 0x39, 0x15, 0x15, 0x2a, 0x2d,
    ST7735_GMCTRN1, 14, // negative gamma
        0xd0, 0x0d, 0x12, 0x08, 0x08, 0x15, 0x34, 0x34, 0x4a, 0x36, 0x12, 0x13, 0x2b, 0x2f,
    ST7735_COLMOD, 1,  // pixel format
        0x05,
    ST7735_SLPOUT, DELAY, 100,  // sleep out
    ST7735_DISPON, 0,  // display on
    0, 0  // END
};
// clang-format on

static uint8_t cmdBuf[20];

#define SET_DC(v) pin_set(CFG_PIN_DISPLAY_DC, v)
#define SET_CS(v) pin_set(CFG_PIN_DISPLAY_CS, v)

static void sendCmd(uint8_t *buf, int len) {
    // make sure cmd isn't on stack
    if (buf != cmdBuf)
        memcpy(cmdBuf, buf, len);
    buf = cmdBuf;
    int cs = 0;
    if (isSPI) {
        SET_DC(0);
        SET_CS(0);
        cs = 1;
        transfer(buf, 1);
        SET_DC(1);
    } else {
        FSMC_CmdWrite(*buf);
    }
    len--;
    buf++;
    if (len > 0)
        transfer(buf, len);
    if (cs)
        SET_CS(1);
}

static void sendCmdSeq(const uint8_t *buf) {
    while (*buf) {
        cmdBuf[0] = *buf++;
        int v = *buf++;
        int len = v & ~DELAY;
        // note that we have to copy to RAM
        memcpy(cmdBuf + 1, buf, len);
        sendCmd(cmdBuf, len + 1);
        buf += len;
        if (v & DELAY) {
            screen_delay(*buf++);
        }
    }
}

static void setAddrWindow(int x, int y, int w, int h) {
    uint8_t cmd0[] = {ST7735_RASET, (uint8_t)x >> 8, (uint8_t)x, (uint8_t)((x + w - 1) >> 8),
                      (uint8_t)(x + w - 1)};
    uint8_t cmd1[] = {ST7735_CASET, (uint8_t)y >> 8, (uint8_t)y, (uint8_t)((y + h - 1) >> 8),
                      (uint8_t)(y + h - 1)};
    sendCmd(cmd1, sizeof(cmd1));
    sendCmd(cmd0, sizeof(cmd0));
}

static void configure(uint8_t madctl, uint32_t frmctr1, uint32_t palXOR) {
    uint8_t cmd0[] = {ST7735_MADCTL, madctl};
    uint8_t cmd1[] = {ST7735_FRMCTR1, (uint8_t)(frmctr1 >> 16), (uint8_t)(frmctr1 >> 8),
                      (uint8_t)frmctr1};
    sendCmd(cmd0, sizeof(cmd0));
    sendCmd(cmd1, cmd1[3] == 0xff ? 3 : 4);

    if (palXOR) {
        uint8_t cmd2[] = {ST7735_INVON};
        sendCmd(cmd2, sizeof(cmd2));
    }
}

#define COL0(r, g, b) ((((r) >> 3) << 11) | (((g) >> 2) << 5) | ((b) >> 3))
#define COL(c) COL0((c >> 16) & 0xff, (c >> 8) & 0xff, c & 0xff)

const uint16_t palette[] = {
    COL(0x000000), // 0
    COL(0xffffff), // 1
    COL(0xff2121), // 2
    COL(0xff93c4), // 3
    COL(0xff8135), // 4
    COL(0xfff609), // 5
    COL(0x249ca3), // 6
    COL(0x78dc52), // 7
    COL(0x003fad), // 8
    COL(0x87f2ff), // 9
    COL(0x8e2ec4), // 10
    COL(0xa4839f), // 11
    COL(0x5c406c), // 12
    COL(0xe5cdc4), // 13
    COL(0x91463d), // 14
    COL(0x000000), // 15
};

uint8_t fb[168 * 128];
extern const uint8_t font8[];
extern const uint8_t mkcdLogo[];
extern const uint8_t fileLogo[];
extern const uint8_t pendriveLogo[];
extern const uint8_t arrowLogo[];
extern const uint8_t ghiLogo[];

static void printch(int x, int y, int col, const uint8_t *fnt) {
    for (int i = 0; i < 6; ++i) {
        uint8_t *p = fb + (x + i) * DISPLAY_HEIGHT + y;
        uint8_t mask = 0x01;
        for (int j = 0; j < 8; ++j) {
            if (*fnt & mask)
                *p = col;
            p++;
            mask <<= 1;
        }
        fnt++;
    }
}

static void printch4(int x, int y, int col, const uint8_t *fnt) {
    for (int i = 0; i < 6 * 4; ++i) {
        uint8_t *p = fb + (x + i) * DISPLAY_HEIGHT + y;
        uint8_t mask = 0x01;
        for (int j = 0; j < 8; ++j) {
            for (int k = 0; k < 4; ++k) {
                if (*fnt & mask)
                    *p = col;
                p++;
            }
            mask <<= 1;
        }
        if ((i & 3) == 3)
            fnt++;
    }
}

static void printicon(int x, int y, int col, const uint8_t *icon) {
    int w = *icon++;
    int h = *icon++;
    int sz = *icon++;

    uint8_t mask = 0x80;
    int runlen = 0;
    int runbit = 0;
    uint8_t lastb = 0x00;

    for (int i = 0; i < w; ++i) {
        uint8_t *p = fb + (x + i) * DISPLAY_HEIGHT + y;
        for (int j = 0; j < h; ++j) {
            int c = 0;
            if (mask != 0x80) {
                if (lastb & mask)
                    c = 1;
                mask <<= 1;
            } else if (runlen) {
                if (runbit)
                    c = 1;
                runlen--;
            } else {
                if (sz-- <= 0)
                    PANIC("invalid encoding");
                lastb = *icon++;
                if (lastb & 0x80) {
                    runlen = lastb & 63;
                    runbit = lastb & 0x40;
                } else {
                    mask = 0x01;
                }
                --j;
                continue; // restart
            }
            if (c)
                *p = col;
            p++;
        }
    }
}

void print(int x, int y, int col, const char *text) {
    int x0 = x;
    while (*text) {
        char c = *text++;
        if (c == '\r')
            continue;
        if (c == '\n') {
            x = x0;
            y += 10;
            continue;
        }
        /*
        if (x + 8 > DISPLAY_WIDTH) {
            x = x0;
            y += 10;
        }
        */
        if (c < ' ')
            c = '?';
        if (c >= 0x7f)
            c = '?';
        c -= ' ';
        printch(x, y, col, &font8[c * 6]);
        x += 6;
    }
}

void print4(int x, int y, int col, const char *text) {
    while (*text) {
        char c = *text++;
        c -= ' ';
        printch4(x, y, col, &font8[c * 6]);
        x += 6 * 4;
    }
}

void draw_screen() {
    uint32_t cfg0 = CFG(DISPLAY_CFG0);
    uint32_t offX = (cfg0 >> 8) & 0xff;
    uint32_t offY = (cfg0 >> 16) & 0xff;

    uint8_t *p = fb;
    if (lookupCfg(CFG_DISPLAY_TYPE, 7735) == 7735) {
        SET_DC(0);
        cmdBuf[0] = ST7735_RAMWR;
        transfer(cmdBuf, 1);

        SET_DC(1);
        SET_CS(0);
        for (int i = 0; i < DISPLAY_WIDTH; ++i) {
            for (int j = 0; j < DISPLAY_HEIGHT; ++j) {
                uint16_t color = palette[*p++ & 0xf];
                uint8_t cc[] = {color >> 8, color & 0xff};
                transfer(cc, 2);
            }
        }
        SET_CS(1);
    } else {
        // ILI9341/st7789(320*240*p8b) DISPLAY
        FSMC_CmdWrite(0x2C); // mem write
        for (int i = 0; i < DISPLAY_WIDTH; ++i) {
            for (int j = 0; j < DISPLAY_HEIGHT - 8; j++) {
                uint16_t color = palette[*(p + j) & 0xf];
                uint8_t cc[] = {color >> 8, color & 0xff};
                transfer(cc, 2);
                transfer(cc, 2);
            }
            for (int j = 0; j < DISPLAY_HEIGHT - 8; j++) {
                uint16_t color = palette[*(p + j) & 0xf];
                uint8_t cc[] = {color >> 8, color & 0xff};
                transfer(cc, 2);
                transfer(cc, 2);
            }
            p += DISPLAY_HEIGHT;
        }
    }
}

void drawBar(int y, int h, int c) {
    for (int x = 0; x < DISPLAY_WIDTH; ++x) {
        memset(fb + x * DISPLAY_HEIGHT + y, c, h);
    }
}

void draw_hf2() {
    if (!hasScreen())
        return;

    print4(20, 22, 5, "<-->");
    print(40, 110, 7, "flashing...");
    draw_screen();
}

void draw_hold_menu() {
    if (!hasScreen())
        return;

    print4(20, 22, 6, "MENU");
    print(10, 110, 1, "hold MENU to wake");
    draw_screen();
}

static void print4border(int x, int y, int c, const char *str) {
    print4(x - 1, y - 1, 15, str);
    print4(x + 1, y - 1, 15, str);
    print4(x - 1, y + 1, 15, str);
    print4(x + 1, y + 1, 15, str);
    print4(x, y, c, str);
}

void draw_drag() {
    if (!hasScreen())
        return;

    drawBar(0, 52, 10);
    drawBar(52, 55, 8);
    drawBar(107, 14, 4);

    print4border(109, 5, 1, "F4");
    print(112, 40, 8, "v" UF2_VERSION);
    print(23, 110, 1, "arcade.makecode.com");

    CUSTOM_LOGO;

#define DRAG 70
#define DRAGX 10
    printicon(DRAGX + 20, DRAG + 5, 1, fileLogo);
    printicon(DRAGX + 66, DRAG, 1, arrowLogo);
    printicon(DRAGX + 108, DRAG, 1, pendriveLogo);
    print(10, DRAG - 12, 1, "arcade.uf2");
    print(98, DRAG - 12, 1, "ARCADE-F4");

    draw_screen();
}

static bool pre_init() {
    if (!hasScreen())
        return false;

    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);

    setup_output_pin(CFG_PIN_DISPLAY_SCK);
    setup_output_pin(CFG_PIN_DISPLAY_BL);
    setup_output_pin(CFG_PIN_DISPLAY_RST);
    setup_output_pin(CFG_PIN_DISPLAY_CS);

    if (lookupCfg(CFG_DISPLAY_TYPE, 7735) == 7735) {
        setup_output_pin(CFG_PIN_DISPLAY_MOSI);
        setup_output_pin(CFG_PIN_DISPLAY_DC);
        SET_DC(1);
    } else { // init the fsmc for ili9341/st7789(320*240*p8b)
        rcc_peripheral_enable_clock(&RCC_AHB3ENR, RCC_AHB3ENR_FSMCEN);
        FSMC_BCR1 = FSMC_BCR_EXTMOD | FSMC_BCR_WREN | 0x80;
        FSMC_BWTR1 |= (CFG(DISPLAY_CFG2) & 0xff) << 8;
        FSMC_BCR1 |= FSMC_BCR_MBKEN;
        gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2 | GPIO3 | GPIO4 | GPIO5);
        gpio_set_af(GPIOA, GPIO_AF12, GPIO2 | GPIO3 | GPIO4 | GPIO5);
        gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO14);
        gpio_set_af(GPIOB, GPIO_AF10, GPIO14);
        gpio_mode_setup(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO6 | GPIO11 | GPIO12);
        gpio_set_af(GPIOC, GPIO_AF10, GPIO6 | GPIO11 | GPIO12);
        gpio_mode_setup(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2 | GPIO3 | GPIO5);
        gpio_set_af(GPIOC, GPIO_AF12, GPIO2 | GPIO3 | GPIO5);
    }

    SET_CS(0);

    pin_set(CFG_PIN_DISPLAY_RST, 1);
    screen_delay(20);
    pin_set(CFG_PIN_DISPLAY_RST, 0);
    screen_delay(20);
    pin_set(CFG_PIN_DISPLAY_RST, 1);
    screen_delay(20);

    isSPI = lookupCfg(CFG_DISPLAY_TYPE, 7735) == 7735;

    return true;
}

void screen_sleep() {
    if (!pre_init())
        return;
    if (lookupCfg(CFG_DISPLAY_TYPE, 7735) == 7735) {
        sendCmdSeq(sleepCmds);
        // SET_CS(0);
        SET_DC(0);
    } else {
        // SET_CS(0);
        FSMC_CmdWrite(0x10);
    }
}

void screen_init() {
    if (!pre_init())
        return;

    pin_set(CFG_PIN_DISPLAY_BL, 1);
    uint32_t cfg0 = CFG(DISPLAY_CFG0);
    uint32_t cfg2 = CFG(DISPLAY_CFG2);
    uint32_t frmctr1 = CFG(DISPLAY_CFG1);
    uint32_t palXOR = (cfg0 & 0x1000000) ? 0xffffff : 0x000000;
    uint32_t madctl = cfg0 & 0xff;
    uint32_t offX = (cfg0 >> 8) & 0xff;
    uint32_t offY = (cfg0 >> 16) & 0xff;
    uint32_t freq = (cfg2 & 0xff);

    if (lookupCfg(CFG_DISPLAY_TYPE, 7735) == 7735) {
        sendCmdSeq(initCmds);
        DMESG("configure screen: FRMCTR1=%p MADCTL=%p SPI at %dMHz", frmctr1, madctl, freq);
    } else if (lookupCfg(CFG_DISPLAY_TYPE, 7735) == 7789) { // init the screen st7789
        sendCmdSeq(initCmds7789);
        DMESG("configure screen st7789: FRMCTR1=%p MADCTL=%p FSMC", frmctr1, madctl);
    } else { // init the screen ili9341
        sendCmdSeq(initCmds9341);
        DMESG("configure screen st7789: FRMCTR1=%p MADCTL=%p FSMC", frmctr1, madctl);
    }
    configure(madctl, frmctr1, palXOR);
    setAddrWindow(offX, offY, CFG(DISPLAY_WIDTH), CFG(DISPLAY_HEIGHT));
    memset(fb, 0, sizeof(fb));
}
