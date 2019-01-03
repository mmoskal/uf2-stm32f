/*
 * STM32F4 board support for the bootloader.
 *
 */

#include "hw_config.h"

#include <stdlib.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/pwr.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/scb.h>

#include "bl.h"
#include <string.h>

/* flash parameters that we should not really know */
static struct {
	uint32_t	sector_number;
	uint32_t	size;
} flash_sectors[] = {

	/* Physical FLASH sector 0 is reserved for bootloader and is not
	 * the table below.
	 * N sectors may aslo be reserved for the app fw in which case
	 * the zero based define BOARD_FIRST_FLASH_SECTOR_TO_ERASE must
	 * be defined to begin the erase above of the reserved sectors.
	 * The default value of BOARD_FIRST_FLASH_SECTOR_TO_ERASE is 0
	 * and begins flash erase operations at phsical sector 1 the 0th entry
	 * in the table below.
	 * A value of 1 for BOARD_FIRST_FLASH_SECTOR_TO_ERASE would reserve
	 * the 0th entry and begin erasing a index 1 the third physical sector
	 * on the device.
	 *
	 * When BOARD_FIRST_FLASH_SECTOR_TO_ERASE is defined APP_RESERVATION_SIZE
	 * must also be defined to remove that additonal reserved FLASH space
	 * from the BOARD_FLASH_SIZE. See APP_SIZE_MAX below.
	 */

	{0x01, 16 * 1024},
	{0x02, 16 * 1024},
	{0x03, 16 * 1024},
	{0x04, 64 * 1024},
	{0x05, 128 * 1024},
	{0x06, 128 * 1024},
	{0x07, 128 * 1024},
	{0x08, 128 * 1024},
	{0x09, 128 * 1024},
	{0x0a, 128 * 1024},
	{0x0b, 128 * 1024},
	/* flash sectors only in 2MiB devices */
	{0x10, 16 * 1024},
	{0x11, 16 * 1024},
	{0x12, 16 * 1024},
	{0x13, 16 * 1024},
	{0x14, 64 * 1024},
	{0x15, 128 * 1024},
	{0x16, 128 * 1024},
	{0x17, 128 * 1024},
	{0x18, 128 * 1024},
	{0x19, 128 * 1024},
	{0x1a, 128 * 1024},
	{0x1b, 128 * 1024},
};
#define BOOTLOADER_RESERVATION_SIZE	(16 * 1024)

#define OTP_BASE			0x1fff7800
#define OTP_SIZE			512
#define UDID_START		        0x1FFF7A10

// address of MCU IDCODE
#define DBGMCU_IDCODE		0xE0042000
#define STM32_UNKNOWN	0
#define STM32F40x_41x	0x413
#define STM32F42x_43x	0x419
#define STM32F42x_446xx	0x421

#define REVID_MASK	0xFFFF0000
#define DEVID_MASK	0xFFF

#ifndef BOARD_PIN_VBUS
# define BOARD_PIN_VBUS                 GPIO9
# define BOARD_PORT_VBUS                GPIOA
# define BOARD_CLOCK_VBUS               RCC_AHB1ENR_IOPAEN
#endif

/* magic numbers from reference manual */

typedef enum mcu_rev_e {
	MCU_REV_STM32F4_REV_A = 0x1000,
	MCU_REV_STM32F4_REV_Z = 0x1001,
	MCU_REV_STM32F4_REV_Y = 0x1003,
	MCU_REV_STM32F4_REV_1 = 0x1007,
	MCU_REV_STM32F4_REV_3 = 0x2001
} mcu_rev_e;

typedef struct mcu_des_t {
	uint16_t mcuid;
	const char *desc;
	char  rev;
} mcu_des_t;

// The default CPU ID  of STM32_UNKNOWN is 0 and is in offset 0
// Before a rev is known it is set to ?
// There for new silicon will result in STM32F4..,?
mcu_des_t mcu_descriptions[] = {
	{ STM32_UNKNOWN,	"STM32F???",    '?'},
	{ STM32F40x_41x, 	"STM32F40x",	'?'},
	{ STM32F42x_43x, 	"STM32F42x",	'?'},
	{ STM32F42x_446xx, 	"STM32F446XX",	'?'},
};

char serial_number[32];
#define STM32_UUID ((uint32_t *)UDID_START)

static void initSerialNumber()
{
	writeHex(serial_number, STM32_UUID[0]);
	writeHex(serial_number+8, STM32_UUID[1]);
	writeHex(serial_number+16, STM32_UUID[2]);
}


typedef struct mcu_rev_t {
	mcu_rev_e revid;
	char  rev;
} mcu_rev_t;

#define APP_SIZE_MAX			(BOARD_FLASH_SIZE - (BOOTLOADER_RESERVATION_SIZE + APP_RESERVATION_SIZE))

/* context passed to cinit */
#if INTERFACE_USB
# define BOARD_INTERFACE_CONFIG_USB  	NULL
#endif

/* board definition */
struct boardinfo board_info = {
	.board_type	= BOARD_TYPE,
	.board_rev	= 0,
	.fw_size	= 0,

#ifdef STM32F401
	.systick_mhz	= 84,
#else
	.systick_mhz	= 168,
#endif
};

static void board_init(void);

#define BOOT_RTC_SIGNATURE          0x71a21877
#define APP_RTC_SIGNATURE           0x24a22d12
#define POWER_DOWN_RTC_SIGNATURE    0x5019684f // Written by app fw to not re-power on.
#define HF2_RTC_SIGNATURE           0x39a63a78
#define BOOT_RTC_REG                MMIO32(RTC_BASE + 0x50)

/* standard clocking for all F4 boards */
static struct rcc_clock_scale clock_setup = {
	.pllm = 0,
	.plln = 336,
#if defined(STM32F401)
	.pllp = 4,
	.pllq = 7,
	.pllr = 0,
	.hpre = RCC_CFGR_HPRE_DIV_NONE,
	.ppre1 = RCC_CFGR_PPRE_DIV_2,
	.ppre2 = RCC_CFGR_PPRE_DIV_NONE,
	.flash_config = FLASH_ACR_ICE | FLASH_ACR_DCE | FLASH_ACR_LATENCY_2WS,
	.ahb_frequency  = 84000000,
#else
	.pllp = 2,
	.pllq = 7,
#if defined(STM32F446) || defined(STM32F469)
	.pllr = 2,
#endif
	.hpre = RCC_CFGR_HPRE_DIV_NONE,
	.ppre1 = RCC_CFGR_PPRE_DIV_4,
	.ppre2 = RCC_CFGR_PPRE_DIV_2,
	.flash_config = FLASH_ACR_ICE | FLASH_ACR_DCE | FLASH_ACR_LATENCY_5WS,
#endif
	.power_save = 0,
	.apb1_frequency = 42000000,
	.apb2_frequency = 84000000,
};

static uint32_t
board_get_rtc_signature()
{
	/* enable the backup registers */
	PWR_CR |= PWR_CR_DBP;
	RCC_BDCR |= RCC_BDCR_RTCEN;

	uint32_t result = BOOT_RTC_REG;

	/* disable the backup registers */
	RCC_BDCR &= RCC_BDCR_RTCEN;
	PWR_CR &= ~PWR_CR_DBP;

	return result;
}

void
board_set_rtc_signature(uint32_t sig)
{
	/* enable the backup registers */
	PWR_CR |= PWR_CR_DBP;
	RCC_BDCR |= RCC_BDCR_RTCEN;

	BOOT_RTC_REG = sig;

	/* disable the backup registers */
	RCC_BDCR &= RCC_BDCR_RTCEN;
	PWR_CR &= ~PWR_CR_DBP;
}

static bool
board_test_force_pin()
{
#if defined(BOARD_FORCE_BL_PIN_IN) && defined(BOARD_FORCE_BL_PIN_OUT)
	/* two pins strapped together */
	volatile unsigned samples = 0;
	volatile unsigned vote = 0;

	for (volatile unsigned cycles = 0; cycles < 10; cycles++) {
		gpio_set(BOARD_FORCE_BL_PORT, BOARD_FORCE_BL_PIN_OUT);

		for (unsigned count = 0; count < 20; count++) {
			if (gpio_get(BOARD_FORCE_BL_PORT, BOARD_FORCE_BL_PIN_IN) != 0) {
				vote++;
			}

			samples++;
		}

		gpio_clear(BOARD_FORCE_BL_PORT, BOARD_FORCE_BL_PIN_OUT);

		for (unsigned count = 0; count < 20; count++) {
			if (gpio_get(BOARD_FORCE_BL_PORT, BOARD_FORCE_BL_PIN_IN) == 0) {
				vote++;
			}

			samples++;
		}
	}

	/* the idea here is to reject wire-to-wire coupling, so require > 90% agreement */
	if ((vote * 100) > (samples * 90)) {
		return true;
	}

#endif
#if defined(BOARD_FORCE_BL_PIN)
	/* single pin pulled up or down */
	volatile unsigned samples = 0;
	volatile unsigned vote = 0;

	for (samples = 0; samples < 200; samples++) {
		if ((gpio_get(BOARD_FORCE_BL_PORT, BOARD_FORCE_BL_PIN) ? 1 : 0) == BOARD_FORCE_BL_STATE) {
			vote++;
		}
	}

	/* reject a little noise */
	if ((vote * 100) > (samples * 90)) {
		return true;
	}

#endif
	return false;
}


static void
board_init(void)
{
	/* fix up the max firmware size, we have to read memory to get this */
	board_info.fw_size = APP_SIZE_MAX;

	RCC_APB1ENR |= RCC_APB1ENR_PWREN;
	
	// enable all GPIO clocks
	RCC_AHB1ENR |= RCC_AHB1ENR_IOPAEN|RCC_AHB1ENR_IOPBEN|RCC_AHB1ENR_IOPCEN|BOARD_CLOCK_VBUS;

	// make sure JACDAC line is up, otherwise trashes the bus
	setup_input_pin(CFG_PIN_JACK_TX);

	setup_output_pin(CFG_PIN_LED);
	setup_output_pin(CFG_PIN_LED1);

	initSerialNumber();
}

void
board_deinit(void)
{
	/* disable the power controller clock */
	rcc_peripheral_disable_clock(&RCC_APB1ENR, RCC_APB1ENR_PWREN);

	/* disable the AHB peripheral clocks */
	RCC_AHB1ENR = 0x00100000; // XXX Magic reset number from STM32F4x reference manual
}

/**
  * @brief  Initializes the RCC clock configuration.
  *
  * @param  clock_setup : The clock configuration to set
  */
static inline void
clock_init(void)
{
	uint32_t pllm = BOOT_SETTINGS->hseValue / 1000000;
	if (pllm < 4 || pllm > 60 || pllm * 1000000 != BOOT_SETTINGS->hseValue)
		pllm = OSC_FREQ;
	clock_setup.pllm = pllm;
	rcc_clock_setup_hse_3v3(&clock_setup);
}

/**
  * @brief  Resets the RCC clock configuration to the default reset state.
  * @note   The default reset state of the clock configuration is given below:
  *            - HSI ON and used as system clock source
  *            - HSE, PLL and PLLI2S OFF
  *            - AHB, APB1 and APB2 prescaler set to 1.
  *            - CSS, MCO1 and MCO2 OFF
  *            - All interrupts disabled
  * @note   This function doesn't modify the configuration of the
  *            - Peripheral clocks
  *            - LSI, LSE and RTC clocks
  */
void
clock_deinit(void)
{
	/* Enable internal high-speed oscillator. */
	rcc_osc_on(RCC_HSI);
	rcc_wait_for_osc_ready(RCC_HSI);

	/* Reset the RCC_CFGR register */
	RCC_CFGR = 0x000000;

	/* Stop the HSE, CSS, PLL, PLLI2S, PLLSAI */
	rcc_osc_off(RCC_HSE);
	rcc_osc_off(RCC_PLL);
	rcc_css_disable();

	/* Reset the RCC_PLLCFGR register */
	RCC_PLLCFGR = 0x24003010; // XXX Magic reset number from STM32F4xx reference manual

	/* Reset the HSEBYP bit */
	rcc_osc_bypass_disable(RCC_HSE);

	/* Reset the CIR register */
	RCC_CIR = 0x000000;
}

uint32_t
flash_func_sector_size(unsigned sector)
{
	if (sector < BOARD_FLASH_SECTORS) {
		return flash_sectors[sector].size;
	}

	return 0;
}

static uint8_t erasedSectors[BOARD_FLASH_SECTORS];

static bool is_blank(uint32_t addr, uint32_t size) {
		for (unsigned i = 0; i < size; i += sizeof(uint32_t)) {
			if (*(uint32_t*)(addr + i) != 0xffffffff) {
				DMESG("non blank: %p i=%d/%d", addr, i, size);
				return false;
			}
		}
		return true;
}

void
flash_write(uint32_t dst, const uint8_t *src, int len)
{
	// assume sector 0 (bootloader) is same size as sector 1
	uint32_t addr = flash_func_sector_size(0) + (APP_LOAD_ADDRESS & 0xfff00000);
	uint32_t sector = 0;
	int erased = false;
	uint32_t size = 0;

	for (unsigned i = 0; i < BOARD_FLASH_SECTORS; i++) {
		size = flash_func_sector_size(i);
		if (addr + size > dst) {
			sector = flash_sectors[i].sector_number;
			erased = erasedSectors[i];
			erasedSectors[i] = 1; // don't erase anymore - we will continue writing here!
			break;
		}
		addr += size;
	}

	if (sector == 0) 
		PANIC("invalid sector");


    flash_unlock();

	if (!erased && !is_blank(addr, size)) {
		flash_erase_sector(sector, FLASH_CR_PROGRAM_X32);
		if (!is_blank(addr, size))
			PANIC("failed to erase!");
	}

    for (int i = 0; i < len; i += 4) {
		flash_program_word(dst + i, *(uint32_t*)(src + i));
    }

	if (memcmp((void*)dst, src, len) != 0)
		PANIC("failed to write");
}

void
flash_func_erase_sector(unsigned sector)
{
	if (sector >= BOARD_FLASH_SECTORS || sector < BOARD_FIRST_FLASH_SECTOR_TO_ERASE) {
		return;
	}

	/* Caculate the logical base address of the sector
	 * flash_func_read_word will add APP_LOAD_ADDRESS
	 */
	uint32_t address = 0;

	for (unsigned i = BOARD_FIRST_FLASH_SECTOR_TO_ERASE; i < sector; i++) {
		address += flash_func_sector_size(i);
	}

	/* blank-check the sector */
	unsigned size = flash_func_sector_size(sector);
	bool blank = true;

	for (unsigned i = 0; i < size; i += sizeof(uint32_t)) {
		if (flash_func_read_word(address + i) != 0xffffffff) {
			blank = false;
			break;
		}
	}

	/* erase the sector if it failed the blank check */
	if (!blank) {
		flash_erase_sector(flash_sectors[sector].sector_number, FLASH_CR_PROGRAM_X32);
	}
}

void
flash_func_write_word(uint32_t address, uint32_t word)
{
	flash_program_word(address + APP_LOAD_ADDRESS, word);
}

uint32_t
flash_func_read_word(uint32_t address)
{
	if (address & 3) {
		return 0;
	}

	return *(uint32_t *)(address + APP_LOAD_ADDRESS);
}

uint32_t
flash_func_read_otp(uint32_t address)
{
	if (address & 3) {
		return 0;
	}

	if (address > OTP_SIZE) {
		return 0;
	}

	return *(uint32_t *)(address + OTP_BASE);
}

uint32_t
flash_func_read_sn(uint32_t address)
{
	// read a byte out from unique chip ID area
	// it's 12 bytes, or 3 words.
	return *(uint32_t *)(address + UDID_START);
}

void
led_on(unsigned led)
{
	switch (led) {
	case LED_ACTIVITY:
		pin_set(CFG_PIN_LED, 1);
		break;

	case LED_BOOTLOADER:
		pin_set(CFG_PIN_LED1, 1);
		break;
	}
}

void
led_off(unsigned led)
{
	switch (led) {
	case LED_ACTIVITY:
		pin_set(CFG_PIN_LED, 0);
		break;

	case LED_BOOTLOADER:
		pin_set(CFG_PIN_LED1, 0);
		break;
	}
}

/* we should know this, but we don't */
#ifndef SCB_CPACR
# define SCB_CPACR (*((volatile uint32_t *) (((0xE000E000UL) + 0x0D00UL) + 0x088)))
#endif

void flash_bootloader(void);
int hf2_mode = 0;

void warning_screen(uint32_t);

int
main(void)
{
	bool try_boot = false;
	unsigned timeout = BOOTLOADER_DELAY;	/* if nonzero, drop out of the bootloader after this time */

	/* Enable the FPU before we hit any FP instructions */
	SCB_CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); /* set CP10 Full Access and set CP11 Full Access */

#if defined(BOARD_POWER_PIN_OUT)

	/* Here we check for the app setting the POWER_DOWN_RTC_SIGNATURE
	 * in this case, we reset the signature and wait to die
	 */
	if (board_get_rtc_signature() == POWER_DOWN_RTC_SIGNATURE) {
		board_set_rtc_signature(0);

		while (1);
	}

#endif

	/* do board-specific initialisation */
	board_init();

	/* configure the clock for bootloader activity */
	clock_init();

	#ifdef BL_FLASHER
	
	flash_bootloader();

	#else

	uint32_t bootSig = board_get_rtc_signature();

	if (FLASH_OPTCR & 0x80030000) {
		warning_screen(bootSig);
	}

	DMESG("bootsig: %p", bootSig);

	/*
	* Clear the signature so that if someone resets us while we're
	* in the bootloader we'll try to boot next time.
	*/
	if (bootSig)
		board_set_rtc_signature(0);

	//bootSig = HF2_RTC_SIGNATURE;

	if (bootSig == HF2_RTC_SIGNATURE) {
		try_boot = false;
		timeout = 2000;
		hf2_mode = 1;
	}

	/*
	 * Check the force-bootloader register; if we find the signature there, don't
	 * try booting.
	 */
	if (bootSig == BOOT_RTC_SIGNATURE) {

		/*
		 * Don't even try to boot before dropping to the bootloader.
		 */
		try_boot = false;

		/*
		 * Don't drop out of the bootloader until something has been uploaded.
		 */
		timeout = 0;

	}

	if (bootSig == APP_RTC_SIGNATURE) {
		try_boot = true;
	}

	/*
	 * Check if the force-bootloader pins are strapped; if strapped,
	 * don't try booting.
	 */
	if (board_test_force_pin()) {
		try_boot = false;
	}

	/*
	 * Check for USB connection - if present, don't try to boot, but set a timeout after
	 * which we will fall out of the bootloader.
	 *
	 * If the force-bootloader pins are tied, we will stay here until they are removed and
	 * we then time out.
	 */
#if 0
#if defined(BOARD_USB_VBUS_SENSE_DISABLED)
	try_boot = false;
#else
	if (gpio_get(BOARD_PORT_VBUS, BOARD_PIN_VBUS) != 0) {

		/* don't try booting before we set up the bootloader */
		try_boot = false;
	}
#endif
#endif

	/* Try to boot the app if we think we should just go straight there */
	if (try_boot) {
		/* try to boot immediately */
		jump_to_app();

		/* booting failed, stay in the bootloader forever */
		timeout = 0;
	}


	/* start the interface */
	cinit(BOARD_INTERFACE_CONFIG_USB, USB);


	while (1) {
		DMESG("enter bootloader, tmo=%d", timeout);

		// if they hit reset the second time, go to app
		board_set_rtc_signature(APP_RTC_SIGNATURE);
		
		/* run the bootloader, come back after an app is uploaded or we time out */
		bootloader(timeout);

		/* if the force-bootloader pins are strapped, just loop back */
		if (board_test_force_pin()) {
			continue;
		}

		board_set_rtc_signature(0);

		/* look to see if we can boot the app */
		jump_to_app();

		/* launching the app failed - stay in the bootloader forever */
		timeout = 0;
	}

	#endif
}

void flushFlash(void);

void resetIntoApp() {
	flushFlash();
	board_set_rtc_signature(APP_RTC_SIGNATURE);
	scb_reset_system();
}

void resetIntoBootloader() {
	flushFlash();
	board_set_rtc_signature(BOOT_RTC_SIGNATURE);
	scb_reset_system();
}
