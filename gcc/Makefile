# possible values: SAME54_XPLAINED_PRO, SYSMOOCTSIM 
SHELL := /bin/bash -o pipefail -e
BOARD ?= SYSMOOCTSIM
CROSSCC ?= arm-none-eabi-
SILENT ?= @

ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

#DBGFLGS = -fdebug-prefix-map=..=$(readlink -f .)
DBGFLGS = -fdebug-prefix-map=..=$(ROOT_DIR)/.. -fno-omit-frame-pointer -ggdb3 -Os # -fdata-sections -fno-common -Wl,--no-undefined -Wl,--strip-discarded #-Wl,--gc-debuginfo
GIT_VERSION=$(shell ../git-version-gen $(TOP)/.tarvers)
DBGFLGS += -DGIT_VERSION=\"$(GIT_VERSION)\"

MK_DIR = mkdir -p

ifeq (, $(shell which xxd))
$(error missing required program "xxd")
endif

OBJS +=  \
hal/src/hal_io.o \
hpl/core/hpl_core_m4.o \
usb/class/dfu/device/dfudf.o \
hal/utils/src/utils_syscalls.o \
hpl/dmac/hpl_dmac.o \
hpl/nvmctrl/hpl_nvmctrl.o \
gcc/system_same54.o \
hpl/usb/hpl_usb.o \
hal/src/hal_delay.o \
hpl/pm/hpl_pm.o \
hpl/core/hpl_init.o \
hpl/gclk/hpl_gclk.o \
hal/utils/src/utils_list.o \
hal/utils/src/utils_assert.o \
usb_start.o \
hpl/oscctrl/hpl_oscctrl.o \
hpl/mclk/hpl_mclk.o \
hpl/ramecc/hpl_ramecc.o \
usb/usb_protocol.o \
hal/src/hal_flash.o \
hal/src/hal_init.o \
gcc/gcc/startup_same54.o \
hal/src/hal_usb_device.o \
hpl/osc32kctrl/hpl_osc32kctrl.o \
driver_init.o \
hal/src/hal_gpio.o \
hal/utils/src/utils_event.o \
hal/src/hal_sleep.o \
hal/src/hal_cache.o \
hpl/cmcc/hpl_cmcc.o \
atmel_start.o \
usb/device/usbdc.o \
hal/src/hal_atomic.o \
bootstate.o \
timer.o

SRC_hosttools = crc_code.c
SRC_dfu = usb_dfu_main.c crc_code.c crc_params.c
SRC_flash = usb_flash_main.c

OBJ_dfu := $(SRC_dfu:.c=.o)
OBJ_flash := $(SRC_flash:.c=.o)

DFU_DEPS := $(OBJ_dfu:.o=.d)
FLASH_DEPS := $(OBJ_flash:.o=.d)


SUB_DIRS := $(sort $(dir $(OBJS)))
DEPS := $(OBJS:%.o=%.d)
OBJS_AS_ARGS = $(patsubst %,"%",$(OBJS))
DEPS_AS_ARGS = $(patsubst %,"%",$(DEPS))

BOARD_LC := $(shell echo $(BOARD) | tr A-Z a-z)
OUTPUT_FILE_NAME := bootloader-$(BOARD_LC)-$(GIT_VERSION)
QUOTE := "
BLSZ = $(shell sed 's/;//g' gcc/blsize.ld | tr -d ' ' | cut -d '=' -f2)
CALC_BLSZ = $(shell echo $$(( $(BLSZ) )))
$(info blsize is $(CALC_BLSZ))
BLSIZE_DEF := -D$(QUOTE)BL_SIZE_BYTE=$(CALC_BLSZ)$(QUOTE)

vpath %.c ../
vpath %.s ../
vpath %.S ../

.PHONY: clean-dfu clean-flash hosttools

all: $(SUB_DIRS) hosttools dfu-merge

hosttools:
	$(CC) -DHOST_TOOL ../$(SRC_hosttools) -o crctool

dfu-merge: $(OUTPUT_FILE_NAME)-dfu

$(OUTPUT_FILE_NAME)-dfu: $(OUTPUT_FILE_NAME)-flash

.SECONDEXPANSION:
$(OUTPUT_FILE_NAME)-%: $(OBJS) $$(OBJ_$$*)
	@echo Building target: $@ $^
	@echo Invoking: ARM/GNU Linker
	$(SILENT)$(QUOTE)$(CROSSCC)gcc$(QUOTE) -o $(OUTPUT_FILE_NAME)-$*.elf $(OBJS_AS_ARGS) $(OBJ_$*) \
	$(DBGFLGS) -Wl,--no-undefined -Wl,--print-memory-usage -Wl,--strip-discarded -Wl,--start-group -lm -Wl,--end-group -mthumb \
	-Wl,-Map="$(OUTPUT_FILE_NAME)-$*.map" --specs=nano.specs -Wl,--gc-sections -mcpu=cortex-m4 \
	-T"../gcc/gcc/same54p20a_$*.ld" \
	-L"../gcc/gcc"
	@echo Finished building target: $@

	$(SILENT)"$(CROSSCC)objcopy" -O binary "$(OUTPUT_FILE_NAME)-$*.elf" "$(OUTPUT_FILE_NAME)-$*.bin"
#	$(SILENT)"$(CROSSCC)objcopy" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "$(OUTPUT_FILE_NAME)-$*.elf" "$(OUTPUT_FILE_NAME)-$*.hex"
#	$(SILENT)"$(CROSSCC)objcopy" -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O binary "$(OUTPUT_FILE_NAME)-$*.elf" "$(OUTPUT_FILE_NAME)-$*.eep" || exit 0
	$(SILENT)"$(CROSSCC)objdump" -h -S "$(OUTPUT_FILE_NAME)-$*.elf" > "$(OUTPUT_FILE_NAME)-$*.lss"
	$(SILENT)"$(CROSSCC)size" "$(OUTPUT_FILE_NAME)-$*.elf"
	$(SILENT)ln -sf $(OUTPUT_FILE_NAME)-$*.bin bootloader-$(BOARD_LC)-$*.bin
	$(SILENT)ln -sf $(OUTPUT_FILE_NAME)-$*.elf bootloader-$(BOARD_LC)-$*.elf

.PHONY: dfu-merge
dfu-merge:
	$(info updating updater section with padded bootloader file..)
	#$(SILENT)dd status=none if=/dev/zero bs=$(CALC_BLSZ) count=1 of=dfu-flash-padded.bin
	#$(SILENT)dd status=none if=$(OUTPUT_FILE_NAME)-flash.bin conv=notrunc of=dfu-flash-padded.bin
	$(SILENT)cp -a $(OUTPUT_FILE_NAME)-flash.bin dfu-flash-padded.bin
	$(SILENT)truncate -s $(CALC_BLSZ) dfu-flash-padded.bin
	$(SILENT)"$(CROSSCC)objcopy" --update-section .blupdate=dfu-flash-padded.bin $(OUTPUT_FILE_NAME)-dfu.elf
	$(SILENT)"$(CROSSCC)objcopy" -O binary $(OUTPUT_FILE_NAME)-dfu.elf $(OUTPUT_FILE_NAME)-dfu.bin
	$(SILENT)rm dfu-flash-padded.bin

	@FLASH_START_ADDR=0x$$($(CROSSCC)nm $(OUTPUT_FILE_NAME)-dfu.elf | grep ' _sfixed$$' | cut -d' ' -f1); \
	CRC_START_ADDR=0x$$($(CROSSCC)nm $(OUTPUT_FILE_NAME)-dfu.elf | grep ' __CRC_Start$$' | cut -d' ' -f1); \
	CRC_END_ADDR=0x$$($(CROSSCC)nm $(OUTPUT_FILE_NAME)-dfu.elf | grep ' _etext$$' | cut -d' ' -f1); \
	CRC_CHECKSUM_ADDR=0x$$($(CROSSCC)nm $(OUTPUT_FILE_NAME)-dfu.elf | grep ' __CRC_Checksum$$' | cut -d' ' -f1); \
	echo $$CRC_START_ADDR $$CRC_END_ADDR $$CRC_CHECKSUM_ADDR; \
	CRC_START_OFFSET=$$(($$CRC_START_ADDR - $$FLASH_START_ADDR)); \
	CRC_END_OFFSET=$$(($$CRC_END_ADDR - $$FLASH_START_ADDR)); \
	CRC_CHECKSUM_OFFSET=$$(($$CRC_CHECKSUM_ADDR - $$FLASH_START_ADDR)); \
	LENGTH=$$(($$CRC_END_OFFSET - $$CRC_START_OFFSET)); \
	CRC_HEX=$$(./crctool $(OUTPUT_FILE_NAME)-dfu.bin $$CRC_START_OFFSET $$LENGTH ); \
	echo len $$LENGTH : $$CRC_START_OFFSET-$$CRC_END_OFFSET, crco: $$CRC_CHECKSUM_OFFSET, start: $$CRC_START_ADDR crcval: $$CRC_HEX; \
	[ $$(($$CRC_START_OFFSET - $$CRC_CHECKSUM_OFFSET)) -ne 4 ] && exit 1; \
	printf $$CRC_HEX | xxd -r -p | xxd -e | xxd -r | dd of=$(OUTPUT_FILE_NAME)-dfu.bin bs=1 seek=$$CRC_CHECKSUM_OFFSET conv=notrunc 2>/dev/null; \
	xxd -s $$(($$CRC_START_ADDR - $$FLASH_START_ADDR -4)) -l 4 -g 1 $(OUTPUT_FILE_NAME)-dfu.bin; \
	"$(CROSSCC)objcopy" -O binary --only-section=.text $(OUTPUT_FILE_NAME)-dfu.elf section_text.bin; \
	printf $$CRC_HEX | xxd -r -p | xxd -e | xxd -r | dd of=section_text.bin bs=1 seek=$$CRC_CHECKSUM_OFFSET conv=notrunc 2>/dev/null; \
	xxd -s $$(($$CRC_START_ADDR - $$FLASH_START_ADDR -4)) -l 4 -g 1 section_text.bin; \
	"$(CROSSCC)objcopy" --update-section .text=section_text.bin $(OUTPUT_FILE_NAME)-dfu.elf;


# Compiler targets
%.o: %.c
	@echo Building file: $<
	@echo ARM/GNU C Compiler
	$(QUOTE)$(CROSSCC)gcc$(QUOTE) $(DBGFLGS) -x c -Wstringop-truncation -Wformat-truncation -mthumb -DDEBUG  -ffunction-sections -mlong-calls  -Wall -c -std=gnu99 \
$(BLSIZE_DEF) -D__SAME54P19A__ -D$(BOARD) -mcpu=cortex-m4 -mfloat-abi=softfp -mfpu=fpv4-sp-d16 \
-I"../" -I"../config" -I"../hal/include" -I"../hal/utils/include" -I"../hpl/cmcc" -I"../hpl/core" -I"../hpl/dmac" -I"../hpl/gclk" -I"../hpl/mclk" -I"../hpl/nvmctrl" -I"../hpl/osc32kctrl" -I"../hpl/oscctrl" -I"../hpl/pm" -I"../hpl/port" -I"../hpl/ramecc" -I"../hpl/usb" -I"../hri" -I"../" -I"../config" -I"../usb" -I"../usb/class/dfu" -I"../usb/class/dfu/device" -I"../usb/device" -I"../" -I"../CMSIS/Include" -I"../include"  \
-MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -o "$@" "$<"
	@echo Finished building: $<

%.o: %.s
	@echo Building file: $<
	@echo ARM/GNU Assembler
	$(QUOTE)$(CROSSCC)as$(QUOTE) $(DBGFLGS) -x c -mthumb -DDEBUG  -ffunction-sections -mlong-calls  -Wall -c -std=gnu99 \
$(BLSIZE_DEF) -D__SAME54P19A__ -D$(BOARD) -mcpu=cortex-m4 -mfloat-abi=softfp -mfpu=fpv4-sp-d16 \
-I"../" -I"../config" -I"../hal/include" -I"../hal/utils/include" -I"../hpl/cmcc" -I"../hpl/core" -I"../hpl/dmac" -I"../hpl/gclk" -I"../hpl/mclk" -I"../hpl/nvmctrl" -I"../hpl/osc32kctrl" -I"../hpl/oscctrl" -I"../hpl/pm" -I"../hpl/port" -I"../hpl/ramecc" -I"../hpl/usb" -I"../hri" -I"../" -I"../config" -I"../usb" -I"../usb/class/dfu" -I"../usb/class/dfu/device" -I"../usb/device" -I"../" -I"../CMSIS/Include" -I"../include"  \
-MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -o "$@" "$<"
	@echo Finished building: $<

%.o: %.S
	@echo Building file: $<
	@echo ARM/GNU Preprocessing Assembler
	$(QUOTE)$(CROSSCC)gcc$(QUOTE) $(DBGFLGS) -x c -mthumb -DDEBUG  -ffunction-sections -mlong-calls  -Wall -c -std=gnu99 \
$(BLSIZE_DEF) -D__SAME54P19A__ -D$(BOARD) -mcpu=cortex-m4 -mfloat-abi=softfp -mfpu=fpv4-sp-d16 \
-I"../" -I"../config" -I"../hal/include" -I"../hal/utils/include" -I"../hpl/cmcc" -I"../hpl/core" -I"../hpl/dmac" -I"../hpl/gclk" -I"../hpl/mclk" -I"../hpl/nvmctrl" -I"../hpl/osc32kctrl" -I"../hpl/oscctrl" -I"../hpl/pm" -I"../hpl/port" -I"../hpl/ramecc" -I"../hpl/usb" -I"../hri" -I"../" -I"../config" -I"../usb" -I"../usb/class/dfu" -I"../usb/class/dfu/device" -I"../usb/device" -I"../" -I"../CMSIS/Include" -I"../include"  \
-MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -o "$@" "$<"
	@echo Finished building: $<

# Detect changes in the dependent files and recompile the respective object files.
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(DEPS)),)
-include $(DEPS)
-include $(DFU_DEPS)
-include $(FLASH_DEPS)
endif
endif

$(SUB_DIRS):
	$(MK_DIR) "$@"

clean-%:
	rm -f $(OUTPUT_FILE_NAME)-$*.a $(OUTPUT_FILE_NAME)-$*.hex $(OUTPUT_FILE_NAME)-$*.bin \
		$(OUTPUT_FILE_NAME)-$*.lss $(OUTPUT_FILE_NAME)-$*.eep $(OUTPUT_FILE_NAME)-$*.map \
		$(OUTPUT_FILE_NAME)-$*.srec bootloader-$(BOARD_LC)-$*.bin bootloader-$(BOARD_LC)-$*.elf

clean: clean-dfu clean-flash
	rm -f $(OBJS_AS_ARGS)
	rm -f $(DEPS_AS_ARGS)

mrproper: clean
	rm -f *.o *.d *.a *.elf *.bin *.hex *.ihex *.eep *.lss *.map *.srec
