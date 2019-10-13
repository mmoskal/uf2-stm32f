#ifndef BOARD_H
#define BOARD_H

#define OSC_FREQ 12
#define USBDEVICESTRING "Feather F405 Arcade"
#define USBMFGSTRING "Adafruit"
#define BOARD_FLASH_SECTORS 8
#define BOARD_FLASH_SIZE (512 * 1024)

#ifdef DEFINE_CONFIGDATA
__attribute__((section(".config"))) __attribute__((used)) //
const uint32_t configData[] = {
    /* CF2 START */
    513675505, 539130489, // magic
    30, 100,  // used entries, total entries
    4, 0x27, // PIN_BTN_A = PC07 / D5
    5, 0x26, // PIN_BTN_B = PC06 / D6
    26, 0x10, // PIN_SPEAKER_AMP = PB00 // UNUSED!
    32, 0x1D, // PIN_DISPLAY_SCK = PB13
    34, 0x1F, // PIN_DISPLAY_MOSI = PB15
    35, 0x19, // PIN_DISPLAY_CS = PB9 / D10
    36, 0x18, // PIN_DISPLAY_DC = PB8 / D9
    37, 0xa0, // DISPLAY_WIDTH = 160
    38, 0x80, // DISPLAY_HEIGHT = 128
    39, 0x1000040, // DISPLAY_CFG0 = 0x1000040
    40, 0x603, // DISPLAY_CFG1 = 0x603
    41, 0x16, // DISPLAY_CFG2 = 0x16
    43, 0x00, // PIN_DISPLAY_RST = PA00  // UNUSED!
    44, 0x01, // PIN_DISPLAY_BL = PA01   // UNUSED!
    47, 0x06, // PIN_BTN_LEFT = PA06 / A2
    48, 0x07, // PIN_BTN_RIGHT = PA07 / A3
    49, 0x24, // PIN_BTN_UP = PC04 / A4
    50, 0x25, // PIN_BTN_DOWN = PC05 / A4
    51, 0x110000, // PIN_BTN_MENU = 0x110000 / ??
    59, 0x80, // SPEAKER_VOLUME = 128
    60, 0x1A, // PIN_JACK_TX = PB10 / TX
    65, 0x1B, // PIN_JACK_SND = PB11 / RX
    67, 0x21, // PIN_JACK_COMMLED = PC01 / D13
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
