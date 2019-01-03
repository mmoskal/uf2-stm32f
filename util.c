#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include <string.h>

#include "pins.h"
#include "settings.h"
#include "bl.h"

void panic() {
    for (;;)
        ;
}

uint32_t pinport(int pin) {
    switch (pin >> 4) {
    case 0:
        return GPIOA;
    case 1:
        return GPIOB;
    case 2:
        return GPIOC;
    default:
        panic();
        return 0;
    }
}

void setup_pin(int pincfg, int mode, int pull) {
    int pin = lookupCfg(pincfg, -1);
    if (pin < 0)
        return;
    uint32_t port = pinport(pin);
    uint32_t mask = pinmask(pin);
    gpio_mode_setup(port, mode, pull, mask);
    gpio_set_output_options(port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, mask);
}

void setup_output_pin(int pincfg) {
    setup_pin(pincfg, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE);
}

void setup_input_pin(int pincfg) {
    setup_pin(pincfg, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP);
}

void pin_set(int pincfg, int v) {
    int pin = lookupCfg(pincfg, -1);
    if (pin < 0)
        return;
    if (v) {
        gpio_set(pinport(pin), pinmask(pin));
    } else {
        gpio_clear(pinport(pin), pinmask(pin));
    }
}

int pin_get(int pincfg) {
    int pin = lookupCfg(pincfg, -1);
    if (pin < 0)
        return 1;
    return gpio_get(pinport(pin), pinmask(pin)) != 0;
}

extern const uint32_t configData[];
uint32_t lookupCfg(uint32_t key, uint32_t defl) {
    const uint32_t *ptr = BOOT_SETTINGS->configValues;
    if (BOOT_SETTINGS->magic0 != SETTINGS_MAGIC_0)
        ptr = configData + 2;
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
