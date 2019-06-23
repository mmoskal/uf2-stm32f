#ifndef BOARD_H
#define BOARD_H

#define OSC_FREQ 24
#define USBDEVICESTRING "MVD 1.2"
#define USBMFGSTRING "Minimum Viable Arcade Device"
#define BOARD_FLASH_SECTORS 8
#define BOARD_FLASH_SIZE (512 * 1024)
#define BOARD_ID "STM32F411-MVD-v1.2"


#ifdef DEFINE_CONFIGDATA
__attribute__((section(".config"))) __attribute__((used)) //
const uint32_t configData[] = {
    /* CF2 START */
    513675505, 539130489, // magic
    30, 100,  // used entries, total entries
    4, 0x15, // PIN_BTN_A = PB05
    5, 0x14, // PIN_BTN_B = PB04
    26, 0x10, // PIN_SPEAKER_AMP = PB00
    32, 0x5, // PIN_DISPLAY_SCK = PA05
    34, 0x7, // PIN_DISPLAY_MOSI = PA07
    35, 0x1c, // PIN_DISPLAY_CS = PB12
    36, 0x4, // PIN_DISPLAY_DC = PA04
    37, 0xa0, // DISPLAY_WIDTH = 160
    38, 0x80, // DISPLAY_HEIGHT = 128
    39, 0x1000040, // DISPLAY_CFG0 = 0x1000040
    40, 0x603, // DISPLAY_CFG1 = 0x603
    41, 0x16, // DISPLAY_CFG2 = 0x16
    43, 0x1, // PIN_DISPLAY_RST = PA01
    44, 0x3, // PIN_DISPLAY_BL = PA03
    47, 0x12, // PIN_BTN_LEFT = PB02
    48, 0xf, // PIN_BTN_RIGHT = PA15
    49, 0x6, // PIN_BTN_UP = PA06
    50, 0xa, // PIN_BTN_DOWN = PA10
    51, 0x110000, // PIN_BTN_MENU = 0x110000
    59, 0x80, // SPEAKER_VOLUME = 128
    60, 0x2, // PIN_JACK_TX = PA02
    65, 0x8, // PIN_JACK_SND = PA08
    67, 0x1e, // PIN_JACK_COMMLED = PB14
    204, 0x80000, // FLASH_BYTES = 0x80000
    205, 0x20000, // RAM_BYTES = 0x20000
    208, 0x69d51bac, // BOOTLOADER_BOARD_ID = 0x69d51bac
    209, 0x57755a57, // UF2_FAMILY = STM32F401
    210, 0x10, // PINS_PORT_SIZE = PA_16
    211, 0x0, // BOOTLOADER_PROTECTION = 0
    212, 0x78, // POWER_DEEPSLEEP_TIMEOUT = 120
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /* CF2 END */
};
#endif

#endif /* BOARD_H */
