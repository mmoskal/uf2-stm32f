#define UF2_VERSION "1.1.0"
#define BOARD_ID "STM32F401-generic-v0"
#define INDEX_URL "https://pxt.io"
#define UF2_NUM_BLOCKS 8000
#define VOLUME_LABEL "ARCADE"
// where the UF2 files are allowed to write data - we allow MBR, since it seems part of the softdevice .hex file
#define USER_FLASH_START (uint32_t)(APP_LOAD_ADDRESS)
#define USER_FLASH_END (0x08000000+BOARD_FLASH_SIZE)

