#include <libopencm3/stm32/spi.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/f4/nvic.h>
#include <libopencm3/cm3/cortex.h>

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
    if (pincfg <= 0)
        pin = -pincfg;
    if (pin < 0)
        return;
    pin &= 0xff;
    uint32_t port = pinport(pin);
    uint32_t mask = pinmask(pin);
    gpio_mode_setup(port, mode, pull, mask);
    gpio_set_output_options(port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, mask);
}

void enable_exti(int pincfg) {
    int pin = lookupCfg(pincfg, -1);
    if (pincfg <= 0)
        pin = -pincfg;
    if (pin < 0)
        return;
    uint32_t port = pinport(pin);
    uint32_t mask = pinmask(pin);

    exti_select_source(mask, port);
	exti_set_trigger(mask, EXTI_TRIGGER_BOTH);
    exti_enable_request(mask);
    exti_reset_request(mask);

#if 0
    int irq = NVIC_EXTI0_IRQ;
    pin &= 0xf;
    if (pin <= 4) irq += pin;
    else if (pin <= 9) irq = NVIC_EXTI9_5_IRQ;
    else irq = NVIC_EXTI15_10_IRQ;
    nvic_clear_pending_irq(irq);
    nvic_set_priority(irq, 0);
    nvic_enable_irq(irq);
#endif
}

void setup_output_pin(int pincfg) {
    setup_pin(pincfg, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE);
    pin_set(pincfg, 0);
}

bool is_active_high(int pincfg) {
    int pin = lookupCfg(pincfg, -1);
    return (pin >= 0 && (pin & 0x110000) == 0x110000);
}

void setup_input_pin(int pincfg) {
    int pull = is_active_high(pincfg) ? GPIO_PUPD_PULLDOWN : GPIO_PUPD_PULLUP;
    setup_pin(pincfg, GPIO_MODE_INPUT, pull);
}

void pin_set(int pincfg, int v) {
    int pin = lookupCfg(pincfg, -1);
    if (pincfg <= 0)
        pin = -pincfg;
    if (pin < 0)
        return;
    if (pin & 0x10000)
        v = !v;
    pin &= 0xff;
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
    pin &= 0xff;
    int v = gpio_get(pinport(pin), pinmask(pin));
    return is_active_high(pincfg) ? v == 0 : v != 0;
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
