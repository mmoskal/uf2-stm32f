#ifndef BOARD_H
#define BOARD_H

#define OSC_FREQ 8
#define USBDEVICESTRING "ARCADE BOY F4"
#define USBMFGSTRING "moaLAB"
#define BOARD_FLASH_SECTORS 8
#define BOARD_FLASH_SIZE (512 * 1024)


#ifdef DEFINE_CONFIGDATA
__attribute__((section(".config"))) __attribute__((used)) //
const uint32_t configData[] = {
    /* CF2 START */
    513675505, 539130489, // magic
    40, 100,  // used entries, total entries
    1, 0x1, // PIN_ACCELEROMETER_INT = PA01
    2, 0x18, // PIN_ACCELEROMETER_SCL = PB08
    3, 0x17, // PIN_ACCELEROMETER_SDA = PB07
    4, 0x19, // PIN_BTN_A = PB09
    5, 0x32, // PIN_BTN_B = PD02
    9, 0x20, // PIN_FLASH_CS = PC00
    10, 0x14, // PIN_FLASH_MISO = PB04
    11, 0x15, // PIN_FLASH_MOSI = PB05
    12, 0x13, // PIN_FLASH_SCK = PB03
    13, 0x10, // PIN_LED = PB00
    33, 0x25, // PIN_DISPLAY_MISO = PC05
    34, 0x22, // PIN_DISPLAY_MOSI = PC02
    35, 0x1c, // PIN_DISPLAY_CS = PB12
    36, 0x23, // PIN_DISPLAY_DC = PC03
    37, 0x140, // DISPLAY_WIDTH = 320
    38, 0xf0, // DISPLAY_HEIGHT = 240
    39, 0x10000c8, // DISPLAY_CFG0 = 0x10000c8
    40, 0x1b, // DISPLAY_CFG1 = 0x1b
    41, 0x1000004, // DISPLAY_CFG2 = 0x1000004
    43, 0x1f, // PIN_DISPLAY_RST = PB15
    47, 0x1d, // PIN_BTN_LEFT = PB13
    48, 0xf, // PIN_BTN_RIGHT = PA15
    49, 0x27, // PIN_BTN_UP = PC07
    50, 0x2a, // PIN_BTN_DOWN = PC10
    51, 0x0, // PIN_BTN_MENU = PA00
    55, 0x12, // PIN_LED1 = PB02
    59, 0xc0, // SPEAKER_VOLUME = 192
    60, 0x16, // PIN_JACK_TX = PB06
    65, 0x29, // PIN_JACK_SND = PC09
    70, 0x68, // ACCELEROMETER_TYPE = MPU6050
    76, 0x8, // PIN_VIBRATION = PA08
    78, 0x247d, // DISPLAY_TYPE = ILI9341
    81, 0x33, // ACCELEROMETER_SPACE = 51
    204, 0x80000, // FLASH_BYTES = 0x80000
    205, 0x20000, // RAM_BYTES = 0x20000
    208, 0x3f361941, // BOOTLOADER_BOARD_ID = 0x3f361941
    209, 0x57755a57, // UF2_FAMILY = STM32F401
    210, 0x10, // PINS_PORT_SIZE = PA_16
    212, 0x0, // POWER_DEEPSLEEP_TIMEOUT = 0
    214, 0x60, // CPU_MHZ = 96
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /* CF2 END */
};
#endif

#ifdef DEFINE_CUSTOM_LOGO
#define CUSTOM_LOGO print(12,8,13,"ARCADE BOY");print(12,20,11,"by moaLAB");print(12,28,11,"build v1.0426");
#endif

#endif /* BOARD_H */




