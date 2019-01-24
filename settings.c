#include <stdint.h>
#define DEFINE_CONFIGDATA 1
#include "bl.h"

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
