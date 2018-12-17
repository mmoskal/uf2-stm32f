#include "bl.h"

#include <libopencm3/stm32/flash.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/cortex.h>

// https://makecode.com/_0arRi9Ys0h1F

static int menuActive = 0;
#define MENU_SIZE 4

static const char *menu_options[] = {
    "Cancel",
    "Cancel",
    "Cancel",
    "Update",
};

static void draw_menu() {
    int y = 72;
    drawBar(y - 2, 12 * MENU_SIZE, 0);
    for (int i = 0; i < MENU_SIZE; ++i) {
        if (i == menuActive) {
            drawBar(y - 2, 11, 4);
            print(10, y, 9, ">");
        }
        print(26, y, 9, menu_options[i]);
        y += 12;
    }
}

#define WARNING_RTC_SIG 0x01312e8d

void warning_screen(uint32_t bootSig) {
    start_systick();

    if (bootSig == WARNING_RTC_SIG) {
        flash_program_option_bytes(FLASH_OPTCR & ~0x80030000);
        return;
    }

    board_set_rtc_signature(WARNING_RTC_SIG);

    screen_init();

    drawBar(0, 12, 2);
    print(10, 2, 1, "WARNING!  DANGER!");

    print(10, 20, 2,
          " This bootloader\n"
          "update may DEMAGE\n"
          "  your device!");

    draw_menu();
    draw_screen();

    setup_input_pin(CFG_PIN_BTN_A);
    setup_input_pin(CFG_PIN_BTN_B);
    setup_input_pin(CFG_PIN_BTN_UP);
    setup_input_pin(CFG_PIN_BTN_DOWN);

    DMESG("OPTCR: %p", FLASH_OPTCR);

    while (1) {
        delay(100);

        int d = 0;
        if (pin_get(CFG_PIN_BTN_UP) == 0)
            d = -1;
        if (pin_get(CFG_PIN_BTN_DOWN) == 0)
            d = 1;
        if (d) {
            if (0 <= menuActive + d && menuActive + d < MENU_SIZE) {
                menuActive += d;
                draw_menu();
                draw_screen();
            }
        } else {
            if (pin_get(CFG_PIN_BTN_A) == 0 || pin_get(CFG_PIN_BTN_B) == 0) {
                drawBar(0, 120, 2);
                draw_screen();

                if (menuActive == MENU_SIZE - 1) {
                    //DMESG("flashing...");
                    jump_to_app();
                }

                // they chose a different option, or jump to app failed

                // enable write protection
                flash_program_option_bytes(FLASH_OPTCR & ~0x80030000);

                //DMESG("OPTCR2: %p", FLASH_OPTCR);
                //delay(300);

                flash_unlock();

                // disable the update software
                flash_program_word(APP_LOAD_ADDRESS, 0);
                flash_program_word(APP_LOAD_ADDRESS + 4, 0);
                
                //DMESG("cleared: %p", *(int*)APP_LOAD_ADDRESS);
                //delay(300);
                //DMESG("done");

                // exit to bootloader
                return;
            }
        }
    }
}
