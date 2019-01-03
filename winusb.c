/*
 * Copyright (c) 2016, Devan Lai
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose with or without fee is hereby granted, provided
 * that the above copyright notice and this permission notice
 * appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <libopencm3/usb/usbd.h>
#include "winusb.h"

//#include "usb_conf.h"

#define WINUSB_SIZE 170

static const uint8_t msOS20Descriptor[] = {
    // Microsoft OS 2.0 descriptor set header (table 10)
    0x0A, 0x00,             // Descriptor size (10 bytes)
    0x00, 0x00,             // MS OS 2.0 descriptor set header
    0x00, 0x00, 0x03, 0x06, // Windows version (8.1) (0x06030000)
    WINUSB_SIZE, 0x00,      // Size, MS OS 2.0 descriptor set

    // Microsoft OS 2.0 function subset header
    0x08, 0x00,             // Descriptor size (8 bytes)
    0x02, 0x00,             // MS OS 2.0 function subset header
    0x01,                   // first interface no; msOS20Descriptor[14]
    0x00,                   // Reserved
    WINUSB_SIZE - 10, 0x00, // Size, MS OS 2.0 function subset

    // Microsoft OS 2.0 compatible ID descriptor (table 13)
    20, 0x00,                     // wLength
    0x03, 0x00,                   // MS_OS_20_FEATURE_COMPATIBLE_ID
    'W', 'I', 'N', 'U', 'S', 'B', //
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    // interface guids
    132, 0, 4, 0, 7, 0,
    //
    42, 0,
    //
    'D', 0, 'e', 0, 'v', 0, 'i', 0, 'c', 0, 'e', 0, 'I', 0, 'n', 0, 't', 0, 'e', 0, 'r', 0, 'f', 0,
    'a', 0, 'c', 0, 'e', 0, 'G', 0, 'U', 0, 'I', 0, 'D', 0, 's', 0, 0, 0,
    //
    80, 0,
    //
    '{', 0, '9', 0, '2', 0, 'C', 0, 'E', 0, '6', 0, '4', 0, '6', 0, '2', 0, '-', 0, '9', 0, 'C', 0,
    '7', 0, '7', 0, '-', 0, '4', 0, '6', 0, 'F', 0, 'E', 0, '-', 0, '9', 0, '3', 0, '3', 0, 'B', 0,
    '-', 0, '3', 0, '1', 0, 'C', 0, 'B', 0, '9', 0, 'C', 0, '5', 0, 'A', 0, 'A', 0, '3', 0, 'B', 0,
    'B', 0, '}', 0, 0, 0, 0, 0};


const struct winusb_platform_descriptor winusb_cap = {
    .bLength = sizeof(struct winusb_platform_descriptor),
    .bDescriptorType = USB_DT_DEVICE_CAPABILITY,
    .bDevCapabilityType = USB_DC_PLATFORM,
    .bReserved = 0,
    .platformCapabilityUUID = WINUSB_OS_20_UUID,
    .descriptor_set_information = { {
        .dwWindowsVersion = 0x06030000,
        .wMSOSDescriptorSetTotalLength = WINUSB_SIZE,
        .bMS_VendorCode = WINUSB_MS_VENDOR_CODE,
        .bAltEnumCode = 0x00,
    } }
};

static int winusb_control_vendor_request(usbd_device *usbd_dev,
                                         struct usb_setup_data *req,
                                         uint8_t **buf, uint16_t *len,
                                         usbd_control_complete_callback* complete) {
    (void)complete;
    (void)usbd_dev;

    if (req->bRequest != WINUSB_MS_VENDOR_CODE) {
        return USBD_REQ_NEXT_CALLBACK;
    }

    int status = USBD_REQ_NOTSUPP;
    if (((req->bmRequestType & USB_REQ_TYPE_RECIPIENT) == USB_REQ_TYPE_DEVICE) &&
        (req->wIndex == 0x07)) {
        *buf = (uint8_t*)msOS20Descriptor;
        if (*len > WINUSB_SIZE) {
            *len = WINUSB_SIZE;
        }
        status = USBD_REQ_HANDLED;
    } else if (((req->bmRequestType & USB_REQ_TYPE_RECIPIENT) == USB_REQ_TYPE_INTERFACE) &&
               (req->wIndex == WINUSB_REQ_GET_EXTENDED_PROPERTIES_OS_FEATURE_DESCRIPTOR)) {
        status = USBD_REQ_NOTSUPP;
    } else {
        status = USBD_REQ_NOTSUPP;
    }

    return status;
}

static void winusb_set_config(usbd_device* usbd_dev, uint16_t wValue) {
    (void)wValue;
    usbd_register_control_callback(
        usbd_dev,
        USB_REQ_TYPE_VENDOR,
        USB_REQ_TYPE_TYPE,
        winusb_control_vendor_request);
}

void winusb_setup(usbd_device* usbd_dev) {
    //usbd_register_extra_string(usbd_dev, 0xEE, "MSFT100!");
    usbd_register_set_config_callback(usbd_dev, winusb_set_config);

    /* Windows probes the compatible ID before setting the configuration,
       so also register the callback now */

    usbd_register_control_callback(
        usbd_dev,
        USB_REQ_TYPE_VENDOR,
        USB_REQ_TYPE_TYPE,
        winusb_control_vendor_request);
}
