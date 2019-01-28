#ifndef BOARD_H
#define BOARD_H

#define OSC_FREQ 8
#define USBDEVICESTRING "Arcade F401"
#define USBMFGSTRING "Contoso Ltd."
#define BOARD_FLASH_SECTORS 8
#define BOARD_FLASH_SIZE (512 * 1024)


#ifdef DEFINE_CONFIGDATA
__attribute__((section(".config"))) __attribute__((used)) //
const uint32_t configData[] = {
    /* CF2 START */
    513675505, 539130489, // magic
    62, 100,  // used entries, total entries
    1, 0x2d, // PIN_ACCELEROMETER_INT = PC13
    2, 0x1a, // PIN_ACCELEROMETER_SCL = PB10
    3, 0x13, // PIN_ACCELEROMETER_SDA = PB03
    4, 0x20, // PIN_BTN_A = PC00
    5, 0x21, // PIN_BTN_B = PC01
    13, 0x10, // PIN_LED = PB00
    18, 0x1e, // PIN_MISO = PB14
    19, 0x1f, // PIN_MOSI = PB15
    21, 0x3, // PIN_RX = PA03
    23, 0x1d, // PIN_SCK = PB13
    24, 0x18, // PIN_SCL = PB08
    25, 0x17, // PIN_SDA = PB07
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
    43, 0x2c, // PIN_DISPLAY_RST = PC12
    44, 0x19, // PIN_DISPLAY_BL = PB09
    45, 0x0, // PIN_SERVO_1 = PIN_D8
    46, 0x1, // PIN_SERVO_2 = PIN_D9
    47, 0x4, // PIN_BTN_LEFT = PA04
    48, 0x29, // PIN_BTN_RIGHT = PC09
    49, 0x15, // PIN_BTN_UP = PB05
    50, 0x12, // PIN_BTN_DOWN = PB02
    51, 0x22, // PIN_BTN_MENU = PC02
    55, 0x10, // PIN_LED1 = PB00
    56, 0x11, // PIN_LED2 = PB01
    57, 0x27, // PIN_LED3 = PC07
    58, 0x26, // PIN_LED4 = PC06
    60, 0x16, // PIN_JACK_TX = PB06
    64, 0x2a, // PIN_JACK_PWREN = PC10
    65, 0x8, // PIN_JACK_SND = PA08
    66, 0x10, // PIN_JACK_BUSLED = PB00
    67, 0x11, // PIN_JACK_COMMLED = PB01
    70, 0x38, // ACCELEROMETER_TYPE = MMA8453
    74, 0x23, // PIN_BTN_MENU2 = PC03
    75, 0x24, // PIN_BATTSENSE = PC04
    76, 0x28, // PIN_VIBRATION = PC08
    77, 0xf, // PIN_PWREN = PA15
    151, 0x2, // PIN_D1 = PA02
    152, 0x3, // PIN_D2 = PA03
    153, 0x1f, // PIN_D3 = PB15
    154, 0x1e, // PIN_D4 = PB14
    155, 0x1d, // PIN_D5 = PB13
    156, 0x18, // PIN_D6 = PB08
    157, 0x17, // PIN_D7 = PB07
    158, 0x0, // PIN_D8 = PA00
    159, 0x1, // PIN_D9 = PA01
    160, 0x25, // PIN_D10 = PC05
    161, 0x2b, // PIN_D11 = PC11
    204, 0x80000, // FLASH_BYTES = 0x80000
    205, 0x18000, // RAM_BYTES = 0x18000
    208, 0x59ea3b60, // BOOTLOADER_BOARD_ID = 0x59ea3b60
    209, 0x57755a57, // UF2_FAMILY = STM32F401
    210, 0x10, // PINS_PORT_SIZE = PA_16
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /* CF2 END */
};
#endif

#endif /* BOARD_H */
