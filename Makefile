#
# Common Makefile for the PX4 bootloaders
#

#
# Paths to common dependencies
#
export BL_BASE		?= $(wildcard .)
export LIBOPENCM3	?= $(wildcard libopencm3)
MKFLAGS=--no-print-directory
#
# Tools
#
export CC	 	 = arm-none-eabi-gcc
export OBJCOPY		 = arm-none-eabi-objcopy

export BOARD ?= brainpad_arcade

export LINKER_FILE ?= stm32f4.ld 
export EXTRAFLAGS ?= -DSTM32F401
export SPECMAKEFILE ?= Makefile.f4
-include boards/$(BOARD)/board.mk

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

#
# Bootloaders to build
#

#TARGETS	= brainpad_bl braingames_bl braingames_revb_bl
TARGETS	= braingames_revb_bl

all:	build sizes

clean:
	cd libopencm3 && make --no-print-directory clean && cd ..
	rm -f *.elf *.bin # Remove any elf or bin files contained directly in the Bootloader directory
	rm -rf build # Remove build directories

# any file generated during libopencm3 build
OCM3FILE = libopencm3/include/libopencm3/stm32/f4/nvic.h

build: $(MAKEFILE_LIST) $(OCM3FILE)
	${MAKE} ${MKFLAGS} -f $(SPECMAKEFILE) TARGET_FILE_NAME=$(BOARD)

#
# Show sizes
#
.PHONY: sizes
sizes:
	@-find build/*/ -name '*.elf' -type f | xargs size 2> /dev/null || :

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

