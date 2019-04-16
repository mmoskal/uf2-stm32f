#ifndef HW_CONFIG_H_
#define HW_CONFIG_H_

#include "board.h"

#define UF2_FAMILY 0x57755a57
#define BOOTLOADER_DELAY 3000
#define INTERFACE_USB 1
#define INTERFACE_USART 0

#ifndef APP_LOAD_ADDRESS
#define APP_LOAD_ADDRESS 0x08010000
#endif

#define BOARD_USB_VBUS_SENSE_DISABLED
//# define BOARD_PIN_VBUS                 GPIO5
//# define BOARD_PORT_VBUS                GPIOC

#if !defined(BOARD_FIRST_FLASH_SECTOR_TO_ERASE)
#define BOARD_FIRST_FLASH_SECTOR_TO_ERASE 0
#endif

#ifndef USBVENDORID
#define USBVENDORID 0x26AC
#endif

#ifndef USBPRODUCTID
#define USBPRODUCTID 0x1043
#endif


#endif /* HW_CONFIG_H_ */
