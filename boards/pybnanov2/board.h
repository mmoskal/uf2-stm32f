#ifndef BOARD_H
#define BOARD_H

#include "../../pins.h"

#define OSC_FREQ 8
#define USBDEVICESTRING "PYB Nano v2"
#define USBMFGSTRING "pybCN"
#define BOARD_FLASH_SECTORS 8
#define BOARD_FLASH_SIZE (512 * 1024)


#ifdef DEFINE_CONFIGDATA
__attribute__((section(".config"))) __attribute__((used)) //
const uint32_t configData[] = {
    /* CF2 START */
    513675505, 539130489, // magic
    32, 100,  // used entries, total entries
    CFG_PIN_LED1, 0, // PA_0 red
    CFG_PIN_LED2, 1, // PA_1 green
    CFG_PIN_LED3, 2, // PA_2 orange
    CFG_PIN_LED4, 3, // PA_3 blue
    CFG_PIN_BTN_A, 0x2D, // PC_13
    204, 0x80000, // FLASH_BYTES = 0x80000
    205, 0x18000, // RAM_BYTES = 0x18000
    208, 0x1f884ff3, // BOOTLOADER_BOARD_ID = 0x1f884ff3
    209, 0x57755a57, // UF2_FAMILY = STM32F401
    210, 0x10, // PINS_PORT_SIZE = PA_16
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    /* CF2 END */
};
#endif

#endif /* BOARD_H */
