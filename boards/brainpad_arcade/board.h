#ifndef BOARD_H
#define BOARD_H

#define OSC_FREQ 8
#define USBDEVICESTRING "BrainPad Arcade"
#define USBMFGSTRING "GHI Electronics"
#define BOARD_FLASH_SECTORS 8
#define BOARD_FLASH_SIZE (512 * 1024)
#define BOARD_ID "STM32F401-BrainPad-Arcade-RevA"


#ifdef DEFINE_CONFIGDATA
__attribute__((section(".config"))) __attribute__((used)) //
const uint32_t configData[] = {
    /* CF2 START */
    513675505, 539130489, // magic
    57, 100,  // used entries, total entries
    1, 0x2e, // PIN_ACCELEROMETER_INT = PC14
    2, 0x16, // PIN_ACCELEROMETER_SCL = PIN_D6
    3, 0x17, // PIN_ACCELEROMETER_SDA = PIN_D7
    4, 0x11, // PIN_BTN_A = PB01
    5, 0x10, // PIN_BTN_B = PB00
    13, 0x29, // PIN_LED = PC09
    18, 0x14, // PIN_MISO = PB04
    19, 0x15, // PIN_MOSI = PB05
    21, 0x3, // PIN_RX = PA03
    23, 0x13, // PIN_SCK = PB03
    24, 0x16, // PIN_SCL = PB06
    25, 0x17, // PIN_SDA = PB07
    28, 0x2, // PIN_TX = PA02
    32, 0x1d, // PIN_DISPLAY_SCK = PB13
    33, 0x1e, // PIN_DISPLAY_MISO = PB14
    34, 0x1f, // PIN_DISPLAY_MOSI = PB15
    35, 0x1c, // PIN_DISPLAY_CS = PB12
    36, 0x25, // PIN_DISPLAY_DC = PC05
    37, 0xa0, // DISPLAY_WIDTH = 160
    38, 0x80, // DISPLAY_HEIGHT = 128
    39, 0x80, // DISPLAY_CFG0 = 0x80
    40, 0x603, // DISPLAY_CFG1 = 0x603
    41, 0x16, // DISPLAY_CFG2 = 0x16
    43, 0x24, // PIN_DISPLAY_RST = PC04
    44, 0x27, // PIN_DISPLAY_BL = PC07
    45, 0x1, // PIN_SERVO_1 = PA01
    46, 0x0, // PIN_SERVO_2 = PA00
    47, 0xf, // PIN_BTN_LEFT = PA15
    48, 0x2d, // PIN_BTN_RIGHT = PC13
    49, 0x5, // PIN_BTN_UP = PA05
    50, 0x1a, // PIN_BTN_DOWN = PB10
    51, 0x2a, // PIN_BTN_MENU = PC10
    55, 0x28, // PIN_LED1 = PC08
    60, 0x9, // PIN_JACK_TX = PA09
    61, 0x6, // PIN_JACK_SENSE = PA06
    62, 0x4, // PIN_JACK_HPEN = PA04
    63, 0xa, // PIN_JACK_BZEN = PA10
    64, 0x7, // PIN_JACK_PWREN = PA07
    65, 0x8, // PIN_JACK_SND = PA08
    66, 0x28, // PIN_JACK_BUSLED = PC08
    67, 0x29, // PIN_JACK_COMMLED = PC09
    70, 0x38, // ACCELEROMETER_TYPE = MMA8453
    74, 0x12, // PIN_BTN_MENU2 = PB02
    151, 0x2, // PIN_D1 = PA02
    152, 0x3, // PIN_D2 = PA03
    153, 0x13, // PIN_D3 = PB03
    154, 0x14, // PIN_D4 = PB04
    155, 0x15, // PIN_D5 = PB05
    156, 0x16, // PIN_D6 = PB06
    157, 0x17, // PIN_D7 = PB07
    204, 0x80000, // FLASH_BYTES = 0x80000
    205, 0x18000, // RAM_BYTES = 0x18000
    208, 0x16e42d61, // BOOTLOADER_BOARD_ID = 0x16e42d61
    209, 0x57755a57, // UF2_FAMILY = STM32F401
    210, 0x10, // PINS_PORT_SIZE = PA_16
    211, 0x1, // BOOTLOADER_PROTECTION = 1
    212, 0x78, // timeout = 120
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /* CF2 END */
};
#endif

#endif /* BOARD_H */
