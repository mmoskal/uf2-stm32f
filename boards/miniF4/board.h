#ifndef BOARD_H
#define BOARD_H

#define OSC_FREQ 25
#define USBDEVICESTRING "Arcade MiniF4"
#define USBMFGSTRING "Phillow Ltd."
#define BOARD_FLASH_SECTORS 8
#define BOARD_FLASH_SIZE (512 * 1024)


#ifdef DEFINE_CONFIGDATA
__attribute__((section(".config"))) __attribute__((used)) //
const uint32_t configData[] = {
    /* CF2 START */
    513675505, 539130489, // magic
    27, 100,  // used entries, total entries
    4, 0x18, // PIN_BTN_A = PB08
    5, 0xf, // PIN_BTN_B = PA15
    13, 0x2d, // PIN_LED = PC13
    //26, 0x10, // PIN_SPEAKER_AMP = PB00
    32, 0x5, // PIN_DISPLAY_SCK = PA05
    33, 0x6, // PIN_DISPLAY_MISO = PA06
    34, 0x7, // PIN_DISPLAY_MOSI = PA07
    35, 0x1c, // PIN_DISPLAY_CS = PB12
    36, 0x14, // PIN_DISPLAY_DC = PB04
    37, 320,//0xa0, // DISPLAY_WIDTH = 160
    38, 240,//0x80, // DISPLAY_HEIGHT = 128
    39, 0x08,//0x80, // DISPLAY_CFG0 = 0x80
    40, 0x0010ff,//0x603, // DISPLAY_CFG1 = 0x603
    41, 50,//0x16, // DISPLAY_CFG2 = 0x16
    43, 0x17, // PIN_DISPLAY_RST = PB07
    44, 0x19, // PIN_DISPLAY_BL = PB09
    47, 0x4, // PIN_BTN_LEFT = PA04
    48, 0xa, // PIN_BTN_RIGHT = PA10
    49, 0x15, // PIN_BTN_UP = PB05
    50, 0x12, // PIN_BTN_DOWN = PB02
    51, 0x0, // PIN_BTN_MENU = PA00
    55, 0x2d, // PIN_LED1 = PC13
    //59, 0x100, // SPEAKER_VOLUME = 128
    78, 9341,
    204, 0x80000, // FLASH_BYTES = 0x80000
    205, 0x20000, // RAM_BYTES = 0x20000(128kB)
    208, 0x59ea3b60, // BOOTLOADER_BOARD_ID = 0x59ea3b60
    209, 0x57755a57, // UF2_FAMILY = STM32F401
    210, 0x10, // PINS_PORT_SIZE = PA_16
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /* CF2 END */
};
#endif

#endif /* BOARD_H */
