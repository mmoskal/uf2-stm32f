#include "bl.h"

__attribute__((section(".config"))) __attribute__((used)) //
const uint32_t configData[] = {
    0x1e9e10f1, 0x20227a79, // magic
    30, 0, // num. entries; reserved
    4, 0x11, // PIN_BTN_A
    5, 0x10, // PIN_BTN_B
    13, 0x29, // PIN_LED
    32, 0x1d, // PIN_DISPLAY_SCK
    33, 0x1e, // PIN_DISPLAY_MISO
    34, 0x1f, // PIN_DISPLAY_MOSI
    35, 0x1c, // PIN_DISPLAY_CS
    36, 0x25, // PIN_DISPLAY_DC
    37, 0xa0, // DISPLAY_WIDTH
    38, 0x80, // DISPLAY_HEIGHT
    39, 0x80, // DISPLAY_CFG0
    40, 0x603, // DISPLAY_CFG1
    41, 0x16, // DISPLAY_CFG2
    43, 0x24, // PIN_DISPLAY_RST
    44, 0x27, // PIN_DISPLAY_BL
    47, 0xf, // PIN_BTN_LEFT
    48, 0x2d, // PIN_BTN_RIGHT
    49, 0x5, // PIN_BTN_UP
    50, 0x1a, // PIN_BTN_DOWN
    51, 0x2a, // PIN_BTN_MENU
    55, 0x28, // PIN_LED1
    60, 0x9, // PIN_JACK_TX
    61, 0x6, // PIN_JACK_SENSE
    62, 0x4, // PIN_JACK_HPEN
    63, 0xa, // PIN_JACK_BZEN
    64, 0x7, // PIN_JACK_PWREN
    65, 0x8, // PIN_JACK_SND
    66, 0x28, // PIN_JACK_BUSLED
    67, 0x29, // PIN_JACK_COMMLED
    208, 0x16e42d61, // BOOTLOADER_BOARD_ID
    0, 0
};


uint32_t lookupCfg(uint32_t key, uint32_t defl) {
    const uint32_t *ptr = configData + 4;
    while (*ptr) {
        if (*ptr == key)
            return ptr[1];
        ptr += 2;
    }
    if (defl == 0x42)
        while (1)
            ;
    return defl;
}

extern const char infoUf2File[];
__attribute__((section(".settings"))) __attribute__((used)) //
const struct Settings settings = {
    .magic0 = SETTINGS_MAGIC_0,
    .magic1 = SETTINGS_MAGIC_1,
    .configValues = configData + 4,
    .hseValue = OSC_FREQ * 1000000,
    .infoUF2 = infoUf2File,
    .manufacturer = USBMFGSTRING,
    .device = USBDEVICESTRING,
};
