#
# Common rules for makefiles for the PX4 bootloaders
#

BUILD_DIR	 = build/$(BOARD)

OBJS		:= $(addprefix $(BUILD_DIR)/, $(patsubst %.c,%.o,$(SRCS)))
DEPS		:= $(OBJS:.o=.d)

ELF		 = $(BUILD_DIR)/bootloader.elf
BINARY		 = $(BUILD_DIR)/bootloader.bin
UF2		 = $(BUILD_DIR)/flasher.uf2

FL_OBJS = $(addprefix $(BUILD_DIR)/, flasher.o main_f4-flasher.o util.o dmesg.o screen.o images.o settings.o)

do-build:		$(BUILD_DIR) $(ELF) $(BINARY) $(UF2)

# Compile and generate dependency files
$(BUILD_DIR)/%.o:	%.c
	@echo Generating object $@
	$(CC) -c -MMD $(FLAGS) -o $@ $*.c

# Compile and generate dependency files
$(BUILD_DIR)/%-flasher.o:	%.c
	@echo Generating object $@
	$(CC) -c -MMD $(FLAGS) -DBL_FLASHER=1 -o $@ $<

# Make the build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(ELF):		$(OBJS) $(MAKEFILE_LIST)
	$(CC) -o $@ $(OBJS) $(FLAGS)  -Wl,-Map=$(ELF).map

$(BINARY):	$(ELF)
	$(OBJCOPY) -O binary $(ELF) $(BINARY)

$(UF2): $(FL_OBJS) $(BINARY)
	python uf2/utils/uf2conv.py --carray $(BINARY) -o $(BUILD_DIR)/bootloader.c
	$(CC) -c $(FLAGS) -o $(BUILD_DIR)/bootloader.o $(BUILD_DIR)/bootloader.c
	$(CC) -o $(BUILD_DIR)/flasher.elf $(FL_OBJS) $(BUILD_DIR)/bootloader.o $(FLAGS:.ld=-flasher.ld)
	$(OBJCOPY) -O binary $(BUILD_DIR)/flasher.elf $(BUILD_DIR)/flasher.bin
	python uf2/utils/uf2conv.py -c -f 0x57755a57 -b 0x08010000 $(BUILD_DIR)/flasher.bin -o $(BUILD_DIR)/flasher.uf2
	$(CC) -o $(BUILD_DIR)/flasher16.elf $(FL_OBJS) $(BUILD_DIR)/bootloader.o $(FLAGS:.ld=-flasher16.ld)
	$(OBJCOPY) -O binary $(BUILD_DIR)/flasher16.elf $(BUILD_DIR)/flasher16.bin
	python uf2/utils/uf2conv.py -c -f 0x57755a57 -b 0x08008000 $(BUILD_DIR)/flasher16.bin -o $(BUILD_DIR)/flasher16.uf2

# Dependencies for .o files
-include $(DEPS)
