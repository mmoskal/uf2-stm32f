#ifndef BOARD_H
#define BOARD_H

#define OSC_FREQ 12
#define USBDEVICESTRING "ArcadeBrain v1.0"
#define USBMFGSTRING "Microsoft Research"
// needs update (1M board)
#define BOARD_FLASH_SECTORS 8
#define BOARD_FLASH_SIZE (512 * 1024)
#define BOARD_ID "STM32F412-ArcadeBrain-v1.0"

#define BL_F412 1

#ifdef DEFINE_CONFIGDATA
__attribute__((section(".config"))) __attribute__((used)) //
const uint32_t configData[] = {
    /* CF2 START */
    513675505, 539130489, // magic
    36, 100,  // used entries, total entries
    13, 0x21, // PIN_LED = PC01
    24, 0x16, // PIN_SCL = PB06
    25, 0x17, // PIN_SDA = PB07
    33, 0x25, // PIN_DISPLAY_MISO = PC05
    34, 0x22, // PIN_DISPLAY_MOSI = PC02
    35, 0x19, // PIN_DISPLAY_CS = PB09
    36, 0x23, // PIN_DISPLAY_DC = PC03
    37, 0x140, // DISPLAY_WIDTH = 320
    38, 0xf0, // DISPLAY_HEIGHT = 240
    39, 0x8, // DISPLAY_CFG0 = 0x8
    40, 0x10ff, // DISPLAY_CFG1 = 0x10ff
    41, 0x1000004, // DISPLAY_CFG2 = 0x1000004
    43, 0x1, // PIN_DISPLAY_RST = PA01
    44, 0x7, // PIN_DISPLAY_BL = PA07
    51, 0x28 | 0x110000, // PIN_BTN_MENU = PC08
    55, 0x21, // PIN_LED1 = PC01
    60, 0x16, // PIN_JACK_TX = PIN_SCL
    65, 0x8, // PIN_JACK_SND = PA08
    78, 0x247d, // DISPLAY_TYPE = ILI9341
    100, 0x6, // PIN_A0 = PA06
    102, 0x11, // PIN_A2 = PB01
    104, 0x24, // PIN_A4 = PC04
    105, 0x0, // PIN_A5 = PA00
    155, 0x29, // PIN_D5 = PC09
    156, 0x15, // PIN_D6 = PB05
    158, 0x20, // PIN_D8 = PC00
    161, 0x1f, // PIN_D11 = PB15
    162, 0x14, // PIN_D12 = PB04
    163, 0x2a, // PIN_D13 = PC10
    204, 0x100000, // FLASH_BYTES = 0x100000
    205, 0x40000, // RAM_BYTES = 0x40000
    208, 0x599fb1b4, // BOOTLOADER_BOARD_ID = 0x599fb1b4
    209, 0x57755a57, // UF2_FAMILY = STM32F401
    210, 0x10, // PINS_PORT_SIZE = PA_16
    211, 0x0, // BOOTLOADER_PROTECTION = 0
    212, 0x0, // POWER_DEEPSLEEP_TIMEOUT = 0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /* CF2 END */
};
#endif

#endif /* BOARD_H */


