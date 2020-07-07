#ifndef BOARD_H
#define BOARD_H

#define OSC_FREQ 12
#define USBDEVICESTRING "JM-Brain v2.1"
#define USBMFGSTRING "MSR Prototype"
#define BOARD_FLASH_SECTORS 8
#define BOARD_FLASH_SIZE (512 * 1024)
#define BOARD_ID "STM32F411-JM-v2.1"


#ifdef DEFINE_CONFIGDATA
__attribute__((section(".config"))) __attribute__((used)) //
const uint32_t configData[] = {
    /* CF2 START */
    513675505, 539130489, // magic
    33, 100,  // used entries, total entries
    18, 0x6, // PIN_MISO = PA06
    19, 0x7, // PIN_MOSI = PA07
    23, 0x5, // PIN_SCK = PA05
    24, 0x18, // PIN_SCL = PB08
    25, 0x19, // PIN_SDA = PB09
    35, 0x1c, // PIN_DISPLAY_CS = PB12
    36, 0x14, // PIN_DISPLAY_DC = PB04
    37, 0xa0, // DISPLAY_WIDTH = 160
    38, 0x80, // DISPLAY_HEIGHT = 128
    39, 0x1000040, // DISPLAY_CFG0 = 0x1000040
    40, 0x603, // DISPLAY_CFG1 = 0x603
    41, 0x16, // DISPLAY_CFG2 = 0x16
    43, 0x17, // PIN_DISPLAY_RST = PB07
    44, 0x15, // PIN_DISPLAY_BL = PB05
    60, 0x16, // PIN_JACK_TX = PB06
    67, 0x1e, // PIN_JACK_COMMLED = PB14
    100, 0x0, // PIN_A0 = PA00
    101, 0x1, // PIN_A1 = PA01
    102, 0x2, // PIN_A2 = PA02
    204, 0x80000, // FLASH_BYTES = 0x80000
    205, 0x20000, // RAM_BYTES = 0x20000
    208, 0xcb3f7d3e, // BOOTLOADER_BOARD_ID = 0xcb3f7d3e
    209, 0x57755a57, // UF2_FAMILY = STM32F401
    210, 0x10, // PINS_PORT_SIZE = PA_16
    211, 0x0, // BOOTLOADER_PROTECTION = 0
    1100, 0x11, // PIN_JDPWR_PRE_SENSE = PB01
    1101, 0x10, // PIN_JDPWR_GND_SENSE = PB00
    1102, 0x8, // PIN_JDPWR_PULSE = PA08
    1103, 0x1d, // PIN_JDPWR_OVERLOAD = PB13
    1104, 0x9, // PIN_JDPWR_ENABLE = PA09

    13, 0x1e, // PIN_LED = 
    55, 0x1d, // PIN_LED1 = 

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0,
    /* CF2 END */
};
#endif

#endif /* BOARD_H */

