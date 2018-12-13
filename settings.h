#pragma once

#include "pins.h"

#define SETTINGS_MAGIC_0 0x10476643
#define SETTINGS_MAGIC_1 0x2e9a5026

struct Settings {
    uint32_t magic0;
    uint32_t magic1;
    const uint32_t *configValues;
    uint32_t hseValue;
    const char *infoUF2;
    const char *manufacturer;
    const char *device;
    uint32_t reserved[16 - 7];
};

uint32_t lookupCfg(uint32_t key, uint32_t defl);
#define CFG(v) lookupCfg(CFG_ ## v, 0x42)

#define BOOT_SETTINGS ((struct Settings*)0x8007fc0)

