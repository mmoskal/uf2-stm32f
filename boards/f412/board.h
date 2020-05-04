#ifndef BOARD_H
#define BOARD_H

#define OSC_FREQ 8
#define USBDEVICESTRING "Arcade F412"
#define USBMFGSTRING "Contoso Ltd."
#define BOARD_FLASH_SECTORS 8
#define BOARD_FLASH_SIZE (512 * 1024)


#ifdef DEFINE_CONFIGDATA
__attribute__((section(".config"))) __attribute__((used)) //
const uint32_t configData[] = {
    /* CF2 START */
    513675505, 539130489, // magic
    29, 100,  // used entries, total entries
    4, 0x20, // PIN_BTN_A = PC00
    5, 0x21, // PIN_BTN_B = PC01
    13, 0x10, // PIN_LED = PB00
    33, 0x34, // PIN_DISPLAY_MISO = PD04
    34, 0x35, // PIN_DISPLAY_MOSI = PD05
    35, 0x37, // PIN_DISPLAY_CS = PD07
    36, 0x50, // PIN_DISPLAY_DC = PF00
    37, 0x140, // DISPLAY_WIDTH = 320
    38, 0xf0, // DISPLAY_HEIGHT = 240
    39, 0x8, // DISPLAY_CFG0 = 0x8
    40, 0x18ff, // DISPLAY_CFG1 = 0x18ff
    41, 0x1000004, // DISPLAY_CFG2 = 0x1000004
    43, 0x3b, // PIN_DISPLAY_RST = PD11
    44, 0x55, // PIN_DISPLAY_BL = PF05
    47, 0x4, // PIN_BTN_LEFT = PA04
    48, 0x29, // PIN_BTN_RIGHT = PC09
    49, 0x15, // PIN_BTN_UP = PB05
    50, 0x12, // PIN_BTN_DOWN = PB02
    51, 0x110000, // PIN_BTN_MENU = 0x110000
    60, 0x16, // PIN_JACK_TX = PB06
    65, 0x8, // PIN_JACK_SND = PA08
    66, 0x10, // PIN_JACK_BUSLED = PB00
    67, 0x11, // PIN_JACK_COMMLED = PB01
    78, 0x247d, // _78 = 9341
    204, 0x80000, // FLASH_BYTES = 0x80000
    205, 0x18000, // RAM_BYTES = 0x18000
    208, 0x1f884ff3, // BOOTLOADER_BOARD_ID = 0x1f884ff3
    209, 0x57755a57, // UF2_FAMILY = STM32F401
    210, 0x10, // PINS_PORT_SIZE = PA_16
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /* CF2 END */
};
#endif

#endif /* BOARD_H */
