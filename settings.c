#include "bl.h"

// clang-format off
const uint32_t configValues[] = {
    CFG_PIN_DISPLAY_CS,      PB_12,
    CFG_PIN_DISPLAY_SCK,     PB_13,
    CFG_PIN_DISPLAY_MOSI,    PB_15,
    CFG_PIN_DISPLAY_MISO,    PB_14,
    CFG_PIN_DISPLAY_DC,      PC_5,
    CFG_PIN_DISPLAY_RST,     PC_4,
    CFG_PIN_LED,             PC_9,
    CFG_PIN_LED1,            PC_8,
    CFG_DISPLAY_WIDTH,       160,
    CFG_DISPLAY_HEIGHT,      128,
    CFG_DISPLAY_CFG0,        0x00000080,
    CFG_DISPLAY_CFG1,        0x000603,
    CFG_DISPLAY_CFG2,        22,
#if defined(TARGET_HW_BRAINGAMES) || defined(TARGET_HW_BRAINGAMES_REVB)
    CFG_PIN_BTN_UP,          PA_5,
    CFG_PIN_BTN_LEFT,        PA_15,
    CFG_PIN_BTN_DOWN,        PB_10,
    CFG_PIN_BTN_RIGHT,       PC_13,
    CFG_PIN_BTN_A,           PB_1,
    CFG_PIN_BTN_B,           PB_0,
    CFG_PIN_BTN_MENU,        PC_10,
    CFG_PIN_DISPLAY_BL,      PC_7,

#ifdef TARGET_HW_BRAINGAMES_REVB
    CFG_PIN_JACK_SND,        PA_8,
    CFG_PIN_JACK_BZEN,       PA_10,
    CFG_PIN_JACK_HPEN,       PA_4,
    CFG_PIN_JACK_SENSE,      PA_6,
    CFG_PIN_JACK_TX,         PA_9,
    CFG_PIN_JACK_PWREN,      PA_7,
#else
    CFG_PIN_JACK_SND,        PA_8,
    CFG_PIN_JACK_BZEN,       PA_10,
    CFG_PIN_JACK_HPEN,       PA_2,
    CFG_PIN_JACK_SENSE,      PB_3,
    CFG_PIN_JACK_TX,         PA_9,
    CFG_PIN_JACK_PWREN,      PB_5,
#endif

#else
    CFG_PIN_DISPLAY_BL,      PA_4,
    CFG_PIN_BTN_LEFT,        PB_10,
    CFG_PIN_BTN_UP,          PA_15,
    CFG_PIN_BTN_RIGHT,       PA_5,
    CFG_PIN_BTN_DOWN,        PC_13,
    CFG_PIN_BTN_A,           PB_7,
    CFG_PIN_BTN_B,           PB_6,
#endif
    0, 0
};
// clang-format on

uint32_t lookupCfg(uint32_t key, uint32_t defl) {
    const uint32_t *ptr = configValues;
    while (*ptr) {
        if (*ptr == key)
            return ptr[1];
        ptr += 2;
    }
    if (defl == 0x42004200)
        while (1)
            ;
    return defl;
}

extern const char infoUf2File[];
__attribute__((section(".settings"))) __attribute__((used)) //
const struct Settings settings = {
    .magic0 = SETTINGS_MAGIC_0,
    .magic1 = SETTINGS_MAGIC_1,
    .configValues = configValues,
    .hseValue = OSC_FREQ * 1000000,
    .infoUF2 = infoUf2File,
    .manufacturer = USBMFGSTRING,
    .device = USBDEVICESTRING,
};
