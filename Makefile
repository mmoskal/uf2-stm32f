#
# Common Makefile for the PX4 bootloaders
#

-include Makefile.user

#
# Paths to common dependencies
#
export BL_BASE		?= $(wildcard .)
export LIBOPENCM3	?= $(wildcard libopencm3)

#
# Tools
#
export CC	    = arm-none-eabi-gcc
export OBJCOPY	= arm-none-eabi-objcopy
export OPENOCD	?= openocd

JTAGCONFIG ?= interface/stlink-v2.cfg

export BOARD ?= f401
-include boards/$(BOARD)/board.mk

# Default to F401; override in board.mk if needed
FN ?= f4
CPUTYPE ?= STM32F401
CPUTYPE_SHORT ?= STM32F4
CPUFLAGS ?= -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16
# f1: -mcpu=cortex-m3
# f3: as f4
# f7: -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-sp-d16

LINKER_FILE ?= linker/stm32$(FN).ld 
EXTRAFLAGS ?= -D$(CPUTYPE)

#
# Common configuration
#
export FLAGS		 = -std=gnu99 \
			   -Os \
			   -g \
			   -Wundef \
			   -Wall \
			   -fno-builtin \
			   -I$(LIBOPENCM3)/include \
			   -Iboards/$(BOARD) \
			   -ffunction-sections \
			   -nostartfiles \
			   -lnosys \
			   -Wl,-gc-sections \
			   -Wl,-g \
			   -Wno-unused \
			   -Werror

export COMMON_SRCS	 = bl.c usb.c usb_msc.c ghostfat.c dmesg.c screen.c images.c settings.c hf2.c support.c webusb.c winusb.c util.c flashwarning.c


SRCS		 = $(COMMON_SRCS) main_$(FN).c

OBJS		:= $(patsubst %.c,%.o,$(SRCS))
DEPS		:= $(OBJS:.o=.d)


FLAGS		+= -mthumb $(CPUFLAGS) \
       -D$(CPUTYPE_SHORT) \
       -T$(LINKER_FILE) \
	   -L$(LIBOPENCM3)/lib \
	   -lopencm3_stm32$(FN) \
        $(EXTRAFLAGS)


OPENOCDALL = $(OPENOCD) -f $(JTAGCONFIG) -f target/stm32$(FN)x.cfg

#
# Bootloaders to build
#

all:	build-bl sizes

clean:
	cd libopencm3 && make --no-print-directory clean && cd ..
	rm -f *.elf *.bin # Remove any elf or bin files contained directly in the Bootloader directory
	rm -rf build # Remove build directories

# any file generated during libopencm3 build
OCM3FILE = libopencm3/include/libopencm3/stm32/f4/nvic.h

build-bl: $(MAKEFILE_LIST) $(OCM3FILE) do-build

#
# General rules for making dependency and object files
# This is where the compiler is called
#
include rules.mk

flash: upload
burn: upload
b: burn
f: flash

upload: build-bl flash-bootloader


BMP = $(shell ls -1 /dev/cu.usbmodem* | head -1)
BMP_ARGS = -ex "target extended-remote $(BMP)" -ex "mon swdp_scan" -ex "attach 1"
GDB = arm-none-eabi-gdb

flash-ocd:
	$(OPENOCDALL) -c "program build/$(BOARD)/bootloader.elf verify reset exit "

flash-bootloader:
	$(GDB) $(BMP_ARGS) -ex "load" -ex "quit" build/$(BOARD)/bootloader.elf


gdb:
	$(GDB) $(BMP_ARGS) build/$(BOARD)/bootloader.elf

#
# Show sizes
#
.PHONY: sizes
sizes:
	@-find build/*/ -name '*.elf' -type f | xargs size 2> /dev/null || :

drop:
	for f in `cd boards; ls` ; do $(MAKE) BOARD=$$f ; done
	cd build; 7z a uf2-stm32f.zip */bootloader.bin */flasher*uf2
#
# Binary management
#
.PHONY: deploy
deploy:
	zip -j Bootloader.zip build/*/*.bin

#
# Submodule management
#

$(OCM3FILE): 
	${MAKE} checksubmodules
	${MAKE} -C $(LIBOPENCM3) -j10 lib

.PHONY: checksubmodules
checksubmodules: updatesubmodules
	$(Q) ($(BL_BASE)/Tools/check_submodules.sh)

.PHONY: updatesubmodules
updatesubmodules:
	$(Q) (git submodule init)
	$(Q) (git submodule update)

