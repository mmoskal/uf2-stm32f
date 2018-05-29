/****************************************************************************
 *
 *   Copyright (c) 2012-2014 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * @file bl.c
 *
 * Common bootloader logic.
 *
 * Aside from the header includes below, this file should have no board-specific logic.
 */
#include "hw_config.h"

#include <inttypes.h>
#include <stdlib.h>

# include <libopencm3/stm32/rcc.h>
# include <libopencm3/stm32/gpio.h>
# include <libopencm3/stm32/flash.h>

#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/systick.h>

#include "bl.h"
#include "cdcacm.h"
#include "uart.h"

inline void cinit(void *config, uint8_t interface)
{
	if (interface == USB) {
		return usb_cinit();
	}
}

inline void cfini(void)
{
	usb_cfini();
}

static enum led_state {LED_BLINK, LED_ON, LED_OFF} _led_state;

void sys_tick_handler(void);

static void
do_jump(uint32_t stacktop, uint32_t entrypoint)
{
	asm volatile(
		"msr msp, %0	\n"
		"bx	%1	\n"
		: : "r"(stacktop), "r"(entrypoint) :);

	// just to keep noreturn happy
	for (;;) ;
}

void
jump_to_app()
{
	const uint32_t *app_base = (const uint32_t *)APP_LOAD_ADDRESS;

	/*
	 * We refuse to program the first word of the app until the upload is marked
	 * complete by the host.  So if it's not 0xffffffff, we should try booting it.
	 */
	if (app_base[0] == 0xffffffff) {
		return;
	}

	/*
	 * The second word of the app is the entrypoint; it must point within the
	 * flash area (or we have a bad flash).
	 */
	if (app_base[1] < APP_LOAD_ADDRESS) {
		return;
	}

	if (app_base[1] >= (APP_LOAD_ADDRESS + board_info.fw_size)) {
		return;
	}

	/* just for paranoia's sake */
	flash_lock();

	/* kill the systick interrupt */
	systick_interrupt_disable();
	systick_counter_disable();

	/* deinitialise the interface */
	cfini();

	/* reset the clock */
	clock_deinit();

	/* deinitialise the board */
	board_deinit();

	/* switch exception handlers to the application */
	SCB_VTOR = APP_LOAD_ADDRESS;

	/* extract the stack and entrypoint from the app vector table and go */
	do_jump(app_base[0], app_base[1]);
}

volatile unsigned timer[NTIMERS];

void
sys_tick_handler(void)
{
	unsigned i;

	for (i = 0; i < NTIMERS; i++)
		if (timer[i] > 0) {
			timer[i]--;
		}

	if ((_led_state == LED_BLINK) && (timer[TIMER_LED] == 0)) {
		led_toggle(LED_BOOTLOADER);
		timer[TIMER_LED] = 50;
	}
}

void
delay(unsigned msec)
{
	timer[TIMER_DELAY] = msec;

	while (timer[TIMER_DELAY] > 0)
		;
}

static void
led_set(enum led_state state)
{
	_led_state = state;

	switch (state) {
	case LED_OFF:
		led_off(LED_BOOTLOADER);
		break;

	case LED_ON:
		led_on(LED_BOOTLOADER);
		break;

	case LED_BLINK:
		/* restart the blink state machine ASAP */
		timer[TIMER_LED] = 0;
		break;
	}
}

static uint32_t
crc32(const uint8_t *src, unsigned len, unsigned state)
{
	static uint32_t crctab[256];

	/* check whether we have generated the CRC table yet */
	/* this is much smaller than a static table */
	if (crctab[1] == 0) {
		for (unsigned i = 0; i < 256; i++) {
			uint32_t c = i;

			for (unsigned j = 0; j < 8; j++) {
				if (c & 1) {
					c = 0xedb88320U ^ (c >> 1);

				} else {
					c = c >> 1;
				}
			}

			crctab[i] = c;
		}
	}

	for (unsigned i = 0; i < len; i++) {
		state = crctab[(state ^ src[i]) & 0xff] ^ (state >> 8);
	}

	return state;
}

void eraseFlash() {
	// clear the bootloader LED while erasing - it stops blinking at random
			// and that's confusing
			led_set(LED_ON);

			// erase all sectors
			flash_unlock();

			for (int i = 0; flash_func_sector_size(i) != 0; i++) {
				flash_func_erase_sector(i);
			}

			// enable the LED while verifying the erase
			led_set(LED_OFF);

			// verify the erase
			for (address = 0; address < board_info.fw_size; address += 4)
				if (flash_func_read_word(address) != 0xffffffff) {
					goto cmd_fail;
				}

			// resume blinking
			led_set(LED_BLINK);
		

					for (int i = 0; i < arg; i++) {

				// program the word
				flash_func_write_word(address, flash_buffer.w[i]);

				// do immediate read-back verify
				if (flash_func_read_word(address) != flash_buffer.w[i]) {
					goto cmd_fail;
				}

				address += 4;
			}

flash_func_read_sn(index);
get_mcu_id();
get_mcu_desc(len, buffer);
}

void
bootloader(unsigned timeout)
{
	/* (re)start the timer system */
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
	systick_set_reload(board_info.systick_mhz * 1000);	/* 1ms tick, magic number */
	systick_interrupt_enable();
	systick_counter_enable();

	/* if we are working with a timeout, start it running */
	if (timeout) {
		timer[TIMER_BL_WAIT] = timeout;
	}

	/* make the LED blink while we are idle */
	led_set(LED_BLINK);

	while (true) {
			if (timeout && !timer[TIMER_BL_WAIT]) {
				return;
			}

	}
}
