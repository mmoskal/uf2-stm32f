#ifndef BOARD_H
#define BOARD_H

#define OSC_FREQ 8
#define USBDEVICESTRING "Retro Arcade"
#define USBMFGSTRING "MSR Prototype"
#define BOARD_FLASH_SECTORS 8
#define BOARD_FLASH_SIZE (512 * 1024)
#define BOARD_ID "STM32F411-Retro-Arcade-Patch-v0"


#ifdef DEFINE_CONFIGDATA
__attribute__((section(".config"))) __attribute__((used)) //
const uint32_t configData[] = {
    /* CF2 START */
    513675505, 539130489, // magic
    50, 100,  // used entries, total entries
    1, 0x2d, // PIN_ACCELEROMETER_INT = PC13
    2, 0x1a, // PIN_ACCELEROMETER_SCL = PIN_SCL
    3, 0x13, // PIN_ACCELEROMETER_SDA = PIN_SDA
    4, 0x18, // PIN_BTN_A = PB08
    5, 0xf, // PIN_BTN_B = PA15
    18, 0x1e, // PIN_MISO = PB14
    19, 0x1f, // PIN_MOSI = PB15
    21, 0x3, // PIN_RX = PA03
    23, 0x1d, // PIN_SCK = PB13
    24, 0x1a, // PIN_SCL = PB10
    25, 0x13, // PIN_SDA = PB03
    26, 0x10, // PIN_SPEAKER_AMP = PB00
    28, 0x2, // PIN_TX = PA02
    32, 0x5, // PIN_DISPLAY_SCK = PA05
    33, 0x6, // PIN_DISPLAY_MISO = PA06
    34, 0x7, // PIN_DISPLAY_MOSI = PA07
    35, 0x1c, // PIN_DISPLAY_CS = PB12
    36, 0x14, // PIN_DISPLAY_DC = PB04
    37, 0xa0, // DISPLAY_WIDTH = 160
    38, 0x80, // DISPLAY_HEIGHT = 128
    39, 0x80, // DISPLAY_CFG0 = 0x80
    40, 0x603, // DISPLAY_CFG1 = 0x603
    41, 0x16, // DISPLAY_CFG2 = 0x16
    43, 0x17, // PIN_DISPLAY_RST = PB07
    44, 0x19, // PIN_DISPLAY_BL = PB09
    47, 0x4, // PIN_BTN_LEFT = PA04
    48, 0xa, // PIN_BTN_RIGHT = PA10
    49, 0x15, // PIN_BTN_UP = PB05
    50, 0x12, // PIN_BTN_DOWN = PB02
    51, 0x11, // PIN_BTN_MENU = PB01
    59, 0x100, // SPEAKER_VOLUME = 128
    60, 0x16, // PIN_JACK_TX = PB06
    64, 0x0, // PIN_JACK_PWREN = PA00
    65, 0x8, // PIN_JACK_SND = PA08
    66, 0x10, // PIN_JACK_BUSLED = PB00
    70, 0x32, // ACCELEROMETER_TYPE = LIS3DH
    151, 0x2, // PIN_D1 = PA02
    152, 0x3, // PIN_D2 = PA03
    153, 0x1f, // PIN_D3 = PB15
    154, 0x1e, // PIN_D4 = PB14
    155, 0x1d, // PIN_D5 = PB13
    156, 0x1a, // PIN_D6 = PB10
    157, 0x13, // PIN_D7 = PB03
    204, 0x80000, // FLASH_BYTES = 0x80000
    205, 0x20000, // RAM_BYTES = 0x20000
    208, 0x59ea3b60, // BOOTLOADER_BOARD_ID = 0x59ea3b60
    209, 0x57755a57, // UF2_FAMILY = STM32F401
    210, 0x10, // PINS_PORT_SIZE = PA_16
    211, 0x0, // BOOTLOADER_PROTECTION = 0
    212, 0x78, // POWER_DEEPSLEEP_TIMEOUT = 120
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
    /* CF2 END */
};
#endif

#endif /* BOARD_H */




