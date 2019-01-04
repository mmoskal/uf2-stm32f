/************************************************************************
 *
 *   Copyright (c) 2012-2014 PX4 Development Team. All rights reserved.
 *   Copyright (c) 2010 libopencm3 project (Gareth McMullin)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * LICENSE NOTE FOR EXTERNAL LIBOPENCM3 LIBRARY:
 *
 *   The PX4 development team considers libopencm3 to be
 *   still GPL, not LGPL licensed, as it is unclear if
 *   each and every author agreed to the LGPS -> GPL change.
 *
 ***********************************************************************/

/**
 * @file cdcacm.c
 * @author Gareth McMullin <gareth@blacksphere.co.nz>
 * @author David Sidrane <david_s5@nscdg.com>
 */
#include "hw_config.h"

#include <stdlib.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/otg_fs.h>

#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/msc.h>

#include "bl.h"

#include "webusb.h"
#include "winusb.h"

/*
 * ST changed the meaning and sense of a few critical bits
 * in the USB IP block identified as 0x00002000
 * libopencm3 has failed to merge my PR to fix this
 * So the the following are defined to fix the issue
 * herein.
 */
#define OTG_CID_HAS_VBDEN 0x00002000
#define OTG_GCCFG_VBDEN   (1 << 21)

extern char serial_number[];
/* Provide the stings for the Index 1-n as a requested index of 0 is used for the supported langages
 *  and is hard coded in the usb lib. The array below is indexed by requested index-1, therefore
 *  element[0] maps to requested index 1
 */
static const char *usb_strings[] = {
	USBMFGSTRING, /* Maps to Index 1 Index */
	USBDEVICESTRING,
	serial_number,
};
#define NUM_USB_STRINGS (sizeof(usb_strings)/sizeof(usb_strings[0]))

static usbd_device *usbd_dev;

/* Buffer to be used for control requests. */
static uint8_t usbd_control_buffer[128];

static const struct usb_device_descriptor dev = {
	.bLength = USB_DT_DEVICE_SIZE,
	.bDescriptorType = USB_DT_DEVICE,	/**< Specifies he descriptor type */
	.bcdUSB = 0x0210,					/**< The USB interface version, binary coded (2.10) */
	.bDeviceClass = 0,		/**< USB device class, CDC in this case */
	.bDeviceSubClass = 0,
	.bDeviceProtocol = 0,
	.bMaxPacketSize0 = 64,
	.idVendor = USBVENDORID,			/**< Vendor ID (VID) */
	.idProduct = USBPRODUCTID,			/**< Product ID (PID) */
	.bcdDevice = 0x4201,				/**< Product version. Set 0x42XX to indicate HF2 support. */
	.iManufacturer = 1,					/**< Use string with index 1 for the manufacturer string ("3D Robotics") */
	.iProduct = 2,						/**< Use string with index 2 for the product string (USBDEVICESTRING define) */
	.iSerialNumber = 3,					/**< Use string with index 3 for the serial number string (empty) */
	.bNumConfigurations = 1,			/**< Number of configurations (one) */
};


static const struct usb_endpoint_descriptor msc_endp[] = {{
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,
	.bEndpointAddress = MSC_EP_OUT,
	.bmAttributes = USB_ENDPOINT_ATTR_BULK,
	.wMaxPacketSize = 64,
	.bInterval = 0,
}, {
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,
	.bEndpointAddress = MSC_EP_IN,
	.bmAttributes = USB_ENDPOINT_ATTR_BULK,
	.wMaxPacketSize = 64,
	.bInterval = 0,
}};

static const struct usb_endpoint_descriptor hf2_endp[] = {{
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,
	.bEndpointAddress = HF2_EP_OUT,
	.bmAttributes = USB_ENDPOINT_ATTR_BULK,
	.wMaxPacketSize = 64,
	.bInterval = 0,
}, {
	.bLength = USB_DT_ENDPOINT_SIZE,
	.bDescriptorType = USB_DT_ENDPOINT,
	.bEndpointAddress = HF2_EP_IN,
	.bmAttributes = USB_ENDPOINT_ATTR_BULK,
	.wMaxPacketSize = 64,
	.bInterval = 0,
}};

static struct usb_interface_descriptor msc_iface = {
	.bLength = USB_DT_INTERFACE_SIZE,
	.bDescriptorType = USB_DT_INTERFACE,
	.bInterfaceNumber = INTF_MSC,
	.bAlternateSetting = 0,
	.bNumEndpoints = 2,
	.bInterfaceClass = USB_CLASS_MSC,
	.bInterfaceSubClass = USB_MSC_SUBCLASS_SCSI,
	.bInterfaceProtocol = USB_MSC_PROTOCOL_BBB,
	.iInterface = 0,
	.endpoint = msc_endp,
	.extra = NULL,
	.extralen = 0
};

static const struct usb_interface_descriptor hf2_iface = {
	.bLength = USB_DT_INTERFACE_SIZE,
	.bDescriptorType = USB_DT_INTERFACE,
	.bInterfaceNumber = INTF_HF2,
	.bAlternateSetting = 0,
	.bNumEndpoints = 2,
	.bInterfaceClass = 0xFF,
	.bInterfaceSubClass = 42,
	.bInterfaceProtocol = 1,
	.iInterface = 0,
	.endpoint = hf2_endp,
	.extra = NULL,
	.extralen = 0
};

static const struct usb_interface ifaces[] = {
    {
        .num_altsetting = 1,
        .altsetting = &msc_iface,
    },    
    {
        .num_altsetting = 1,
        .altsetting = &hf2_iface,
    },    
};

extern const struct winusb_platform_descriptor winusb_cap;

// WEBUSB It seems the MS capability thingy is missing here
static const struct usb_device_capability_descriptor* capabilities[] = {
    (const struct usb_device_capability_descriptor*)&webusb_platform,
    (const struct usb_device_capability_descriptor*)&winusb_cap,
};

static const struct usb_bos_descriptor bos = {
    .bLength = USB_DT_BOS_SIZE,
    .bDescriptorType = USB_DT_BOS,
    .wTotalLength = USB_DT_BOS_SIZE + sizeof(webusb_platform),
    .bNumDeviceCaps = sizeof(capabilities)/sizeof(capabilities[0]),
    .capabilities = capabilities
};

static const struct usb_config_descriptor config = {
	.bLength = USB_DT_CONFIGURATION_SIZE,
	.bDescriptorType = USB_DT_CONFIGURATION,
	.wTotalLength = 0,
	.bNumInterfaces = 2,
	.bConfigurationValue = 1,
	.iConfiguration = 0,
	.bmAttributes = 0x80,
	.bMaxPower = 0xFA, /* Request 500 mA power (0xFA=250, get doubled in protocol) */

	.interface = ifaces,
};


void
otg_fs_isr(void)
{
	if (usbd_dev) {
		usbd_poll(usbd_dev);
	}
}

void usb_enumerated();

static void usb_set_config(usbd_device *usbd_dev, uint16_t wValue)
{
	(void)usbd_dev;
	(void)wValue;
	usb_enumerated();
}

void hf2_init(usbd_device *usbd_dev);

void
usb_cinit(void)
{
	if (hf2_mode) {
		// disable MSC interface in HF2 mode
		msc_iface.bInterfaceClass = 0xfe;
	}

#if defined(STM32F4)

	rcc_peripheral_enable_clock(&RCC_AHB1ENR, RCC_AHB1ENR_IOPAEN);
	rcc_peripheral_enable_clock(&RCC_AHB2ENR, RCC_AHB2ENR_OTGFSEN);

#if defined(USB_FORCE_DISCONNECT)
	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_OTYPE_OD, GPIO12);
	gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO12);
	gpio_clear(GPIOA, GPIO12);
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
	systick_set_reload(board_info.systick_mhz * 1000);	/* 1ms tick, magic number */
	systick_interrupt_enable();
	systick_counter_enable();
	/* Spec is 2-2.5 uS */
	delay(1);
	systick_interrupt_disable();
	systick_counter_disable(); // Stop the timer
#endif
	/* Configure to use the Alternate IO Functions USB DP,DM */

	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO11 | GPIO12);
	gpio_set_af(GPIOA, GPIO_AF10, GPIO11 | GPIO12);

#if defined(BOARD_USB_VBUS_SENSE_DISABLED)
	OTG_FS_GCCFG |= OTG_GCCFG_NOVBUSSENS;
#endif

	usbd_dev = usbd_init(&otgfs_usb_driver, &dev, &config, &bos, usb_strings, NUM_USB_STRINGS,
			     usbd_control_buffer, sizeof(usbd_control_buffer));

#elif defined(STM32F1)
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPAEN);
	gpio_set(GPIOA, GPIO8);
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO8);

	usbd_dev = usbd_init(&st_usbfs_v1_usb_driver, &dev, &config, usb_strings, NUM_USB_STRINGS,
			     usbd_control_buffer, sizeof(usbd_control_buffer));
#endif

	if (!hf2_mode)
		usb_msc_init(usbd_dev, MSC_EP_IN, 64, MSC_EP_OUT, 64, USBMFGSTRING, "UF2 Bootloader",
				"42.00", UF2_NUM_BLOCKS, read_block, write_block);

	hf2_init(usbd_dev);

	winusb_setup(usbd_dev);

#if defined(STM32F4)

	if (OTG_FS_CID == OTG_CID_HAS_VBDEN) {

		OTG_FS_GCCFG |= OTG_GCCFG_VBDEN | OTG_GCCFG_PWRDWN;

		/* Set the Soft Connect (STMF32446, STMF32469 comes up disconnected) */

		OTG_FS_DCTL &= ~OTG_DCTL_SDIS;
	}

	nvic_enable_irq(NVIC_OTG_FS_IRQ);
#endif

	usbd_register_set_config_callback(usbd_dev, usb_set_config);
}

void
usb_cfini(void)
{
#if defined(STM32F4)
	nvic_disable_irq(NVIC_OTG_FS_IRQ);
#endif

	if (usbd_dev) {
		usbd_disconnect(usbd_dev, true);
		usbd_dev = NULL;
	}

#if defined(STM32F4)
	/* Reset the USB pins to being floating inputs */
	gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO11 | GPIO12);

	/* Disable the OTGFS peripheral clock */
	rcc_peripheral_disable_clock(&RCC_AHB2ENR, RCC_AHB2ENR_OTGFSEN);

#elif defined(STM32F1)
	/* Reset the USB pins to being floating inputs */
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO8);
	gpio_clear(GPIOA, GPIO8);
#endif
}

void usb_callback() {
#if defined(STM32F1)
	usbd_poll(usbd_dev);
#endif
}
