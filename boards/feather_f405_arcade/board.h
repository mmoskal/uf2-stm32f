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
    42, 100,  // used entries, total entries
    4, 0x27, // PIN_BTN_A = PC07 / D5
    5, 0x26, // PIN_BTN_B = PC06 / D6
    32, 0x1D, // PIN_DISPLAY_SCK = PB13
    34, 0x1F, // PIN_DISPLAY_MOSI = PB15
    35, 0x19, // PIN_DISPLAY_CS = PB9 / D10
    36, 0x18, // PIN_DISPLAY_DC = PB8 / D9
    37, 0xa0, // DISPLAY_WIDTH = 160
    38, 0x80, // DISPLAY_HEIGHT = 128
    39, 0x1000040, // DISPLAY_CFG0 = 0x1000040
    40, 0x603, // DISPLAY_CFG1 = 0x603
    41, 0x16, // DISPLAY_CFG2 = 0x16
    47, 0x06, // PIN_BTN_LEFT = PA06 / A2
    48, 0x07, // PIN_BTN_RIGHT = PA07 / A3
    49, 0x24, // PIN_BTN_UP = PC04 / A4
    50, 0x25, // PIN_BTN_DOWN = PC05 / A4
    51, 0x110000, // PIN_BTN_MENU = 0x110000 / ??
    59, 0x80, // SPEAKER_VOLUME = 128
    60, 0x1A, // PIN_JACK_TX = PB10 / TX
    65, 0x1B, // PIN_JACK_SND = PB11 / RX
    67, 0x21, // PIN_JACK_COMMLED = PC01 / D13
    100, 0x04, // PIN_A0 = PA04
    101, 0x05, // PIN_A1 = PA05
    102, 0x06, // PIN_A2 = PA06
    103, 0x07, // PIN_A3 = PA07
    104, 0x24, // PIN_A4 = PC04
    105, 0x25, // PIN_A5 = PC05
    150, 0x1B, // PIN_D0 = PB11
    151, 0x1A, // PIN_D1 = PB10
    155, 0x27, // PIN_D5 = PC7
    156, 0x26, // PIN_D6 = PC6
    159, 0x18, // PIN_D9 = PB8
    160, 0x19, // PIN_D10 = PB9
    161, 0x23, // PIN_D11 = PC3
    162, 0x22, // PIN_D12 = PC2
    163, 0x21, // PIN_D13 = PC1
    204, 0x100000, // FLASH_BYTES = 1 MByte
    205, 0x1E0000, // RAM_BYTES = 192KB
    208, 0xa73105d1, // BOOTLOADER_BOARD_ID = 0xa73105d1
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
