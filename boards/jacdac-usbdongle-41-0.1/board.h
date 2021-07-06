#ifndef BOARD_H
#define BOARD_H

#define OSC_FREQ 24
#define USBDEVICESTRING "Jacdac USB Dongle 41-0.1"
#define USBMFGSTRING "Jacdac USB Dongle"
#define BOARD_FLASH_SECTORS 8
#define BOARD_FLASH_SIZE (512 * 1024)
#define BOARD_ID "Jacdac USB Dongle 41-0.1"

#ifdef DEFINE_CONFIGDATA
__attribute__((section(".config"))) __attribute__((used)) //
const uint32_t configData[] = {
    /* CF2 START */
    513675505, 539130489, // magic
    18, 100,  // used entries, total entries
    13, 0x10006, // reuse LED B as board LED
    52, 0x10007, // LED R
    53, 0x10010, // LED G
    54, 0x10006, // LED B
    60, 0x2, // PIN_JACK_TX = PA02
    100, 0xA, // ILIM_ENABLE A0
    101, 0x8, // ILIM_STATUS A1
    102, 0x16, // ILIM_CTRL A2
    103, 0x7, // LED R A3
    104, 0x10, // LED G A4
    105, 0x6, // LED B A5
    204, 0x80000, // FLASH_BYTES = 0x80000
    205, 0x20000, // RAM_BYTES = 0x20000
    208, 0x69d51bac, // BOOTLOADER_BOARD_ID = 0x69d51bac
    209, 0x57755a57, // UF2_FAMILY = STM32F401
    210, 0x10, // PINS_PORT_SIZE = PA_16
    211, 0x0, // BOOTLOADER_PROTECTION = 0
    212, 0, // POWER_DEEPSLEEP_TIMEOUT = 120
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /* CF2 END */
};
#endif

#endif /* BOARD_H */
