#ifndef BOARD_H
#define BOARD_H

#define OSC_FREQ 12
#define USBDEVICESTRING "Feather F405"
#define USBMFGSTRING "Adafruit"
#define BOARD_FLASH_SECTORS 8
#define BOARD_FLASH_SIZE (1024 * 1024)

#ifdef DEFINE_CONFIGDATA
__attribute__((section(".config"))) __attribute__((used)) //
const uint32_t configData[] = {
    /* CF2 START */
    513675505, 539130489, // magic
    22, 100,  // used entries, total entries
    100, 0x4, // PIN_A0 = PA04
    101, 0x5, // PIN_A1 = PA05
    102, 0x6, // PIN_A2 = PA06
    103, 0x7, // PIN_A3 = PA07
    104, 0x24, // PIN_A4 = PC04
    105, 0x25, // PIN_A5 = PC05
    150, 0x1b, // PIN_D0 = PB11
    151, 0x1a, // PIN_D1 = PB10
    155, 0x27, // PIN_D5 = PC07
    156, 0x26, // PIN_D6 = PC06
    159, 0x18, // PIN_D9 = PB08
    160, 0x19, // PIN_D10 = PB09
    161, 0x23, // PIN_D11 = PC03
    162, 0x22, // PIN_D12 = PC02
    163, 0x21, // PIN_D13 = PC01
    204, 0x100000, // FLASH_BYTES = 0x100000
    205, 0x30000, // RAM_BYTES =  192k
    208, 0x33991894, // BOOTLOADER_BOARD_ID = 0x33991894
    209, 0x57755a57, // UF2_FAMILY = STM32F401
    210, 0x10, // PINS_PORT_SIZE = PA_16
    211, 0x0, // BOOTLOADER_PROTECTION = 0
    212, 0x78, // POWER_DEEPSLEEP_TIMEOUT = 120
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /* CF2 END */
};
#endif

#endif /* BOARD_H */
