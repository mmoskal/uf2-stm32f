#define UF2_VERSION "2.8.1"
#ifndef BOARD_ID
#define BOARD_ID "STM32F4xx-Arcade-Generic"
#endif
#define INDEX_URL "https://arcade.makecode.com"
#define UF2_NUM_BLOCKS 8000
#define VOLUME_LABEL "ARCADE-F4"
// where the UF2 files are allowed to write data; we let them write to settings area as well
#define USER_FLASH_START (uint32_t)(APP_LOAD_ADDRESS - 32 * 1024)
#define USER_FLASH_END (0x08000000+BOARD_FLASH_SIZE)

