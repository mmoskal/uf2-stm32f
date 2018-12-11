#include "hw_config.h"

#include <stdlib.h>
#include <string.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/otg_fs.h>
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/usb/usbd.h>

#include "bl.h"

#include "uf2hid.h"

#define HF2_BUF_SIZE 512

#define usb_assert assert

#define LOG DMESG

typedef struct {
    uint16_t size;
    union {
        uint8_t buf[HF2_BUF_SIZE];
        uint32_t buf32[HF2_BUF_SIZE / 4];
        uint16_t buf16[HF2_BUF_SIZE / 2];
        HF2_Command cmd;
        HF2_Response resp;
    };
} HF2_Buffer;

HF2_Buffer pkt;

const uint8_t *dataToSend;
volatile uint32_t dataToSendLength;
uint8_t dataToSendFlag;
static usbd_device *_usbd_dev;

static void pokeSend() {
    uint8_t buf[64];
    bool sendIt = false;

    memset(buf, 0, sizeof(buf));

    cm_disable_interrupts();
    if (dataToSendLength) {
        int flag = dataToSendFlag;
        int s = 63;
        if (dataToSendLength <= 63) {
            s = dataToSendLength;
        } else {
            if (flag == HF2_FLAG_CMDPKT_LAST)
                flag = HF2_FLAG_CMDPKT_BODY;
        }

        buf[0] = flag | s;
        memcpy(buf + 1, dataToSend, s);
        dataToSend += s;
        dataToSendLength -= s;
        sendIt = true;
    }
    cm_enable_interrupts();

    if (sendIt)
        usbd_ep_write_packet(_usbd_dev, HF2_EP_IN, buf, sizeof(buf));
}

static void send_hf2_response(int size) {
    dataToSend = pkt.buf;
    dataToSendFlag = HF2_FLAG_CMDPKT_LAST;
    dataToSendLength = 4 + size;
    pokeSend();
}

extern const char infoUf2File[];

#define checkDataSize(str, add) assert(sz == 8 + sizeof(cmd->str) + (add))

static void handle_command() {
    int tmp;

    // one has to be careful dealing with these, as they share memory
    HF2_Command *cmd = &pkt.cmd;
    HF2_Response *resp = &pkt.resp;

    uint32_t cmdId = cmd->command_id;
    int sz = pkt.size;
    resp->tag = cmd->tag;
    resp->status16 = HF2_STATUS_OK;

    switch (cmdId) {
    case HF2_CMD_INFO:
        tmp = strlen(infoUf2File);
        memcpy(pkt.resp.data8, infoUf2File, tmp);
        send_hf2_response(tmp);
        return;

    case HF2_CMD_BININFO:
        resp->bininfo.mode = HF2_MODE_BOOTLOADER;
        resp->bininfo.flash_page_size = 256;
        resp->bininfo.flash_num_pages = BOARD_FLASH_SIZE / 256;
        resp->bininfo.max_message_size = sizeof(pkt.buf);
        send_hf2_response(sizeof(resp->bininfo));
        return;

    case HF2_CMD_RESET_INTO_APP:
        resetIntoApp();
        break;
    case HF2_CMD_RESET_INTO_BOOTLOADER:
        resetIntoBootloader();
        break;
    case HF2_CMD_START_FLASH:
        // userspace app should reboot into bootloader on this command; we just ignore it
        // userspace can also call hf2_handover() here
        break;
    case HF2_CMD_WRITE_FLASH_PAGE:
        checkDataSize(write_flash_page, 256);
        // first send ACK and then start writing, while getting the next packet
        send_hf2_response(0);
        if (VALID_FLASH_ADDR(cmd->write_flash_page.target_addr, 256)) {
            flash_write(cmd->write_flash_page.target_addr,
                        (const uint8_t *)cmd->write_flash_page.data, 256);
        }
        return;
    default:
        // command not understood
        resp->status16 = HF2_STATUS_INVALID_CMD;
        break;
    }

    send_hf2_response(0);
}

static void hf2_data_rx_cb(usbd_device *usbd_dev, uint8_t ep) {
    int len;
    uint8_t buf[64];

    len = usbd_ep_read_packet(usbd_dev, ep, buf, 64);

    DMESG("HF2 read: %d", len);
    if (len <= 0)
        return;

    uint8_t tag = buf[0];
    // serial packets not allowed when in middle of command packet
    usb_assert(pkt.size == 0 || !(tag & HF2_FLAG_SERIAL_OUT));
    int size = tag & HF2_SIZE_MASK;
    usb_assert(pkt.size + size <= (int)sizeof(pkt.buf));
    memcpy(pkt.buf + pkt.size, buf + 1, size);
    pkt.size += size;
    tag &= HF2_FLAG_MASK;
    if (tag != HF2_FLAG_CMDPKT_BODY) {
        if (tag == HF2_FLAG_CMDPKT_LAST) {
            handle_command();
        } else {
            // do something about serial?
        }
        pkt.size = 0;
    }
}

static void hf2_data_tx_cb(usbd_device *usbd_dev, uint8_t ep) {
    (void)usbd_dev;
    (void)ep;
    pokeSend();
}

/** @brief Setup the endpoints to be bulk & register the callbacks. */
static void hf2_set_config(usbd_device *usbd_dev, uint16_t wValue) {
    LOG("HF2 config");

    (void)wValue;

    usbd_ep_setup(usbd_dev, HF2_EP_IN, USB_ENDPOINT_ATTR_INTERRUPT, 64, hf2_data_tx_cb);
    usbd_ep_setup(usbd_dev, HF2_EP_OUT, USB_ENDPOINT_ATTR_INTERRUPT, 64, hf2_data_rx_cb);
}

void hf2_init(usbd_device *usbd_dev) {
    _usbd_dev = usbd_dev;
    usbd_register_set_config_callback(usbd_dev, hf2_set_config);
}
