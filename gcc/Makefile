################################################################################
# User configuration. Can be edited
################################################################################

# Set for which board the bootloader should be compiled
# run `make clean` for the change to be effective
# possible values: SAME54_XPLAINED_PRO, SYSMOOCTSIM 
BOARD ?= SYSMOOCTSIM

GIT_VERSION=$(shell ../git-version-gen $(TOP)/.tarvers)

################################################################################
# Automatically-generated file. Do not edit!
################################################################################

ifdef SystemRoot
	SHELL = cmd.exe
	MK_DIR = mkdir
else
	ifeq ($(shell uname), Linux)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), CYGWIN)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), MINGW32)
		MK_DIR = mkdir -p
	endif

	ifeq ($(shell uname | cut -d _ -f 1), MINGW64)
		MK_DIR = mkdir -p
	endif
endif

# List the subdirectories for creating object files
SUB_DIRS +=  \
 \
hpl/pm \
gcc \
hpl/osc32kctrl \
hpl/ramecc \
hpl/dmac \
usb/class/dfu/device \
hal/src \
hpl/mclk \
usb \
hal/utils/src \
hpl/gclk \
usb/device \
hpl/oscctrl \
gcc/gcc \
hpl/nvmctrl \
hpl/usb \
hpl/core \
hpl/cmcc

# List the object files
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
hal/src/hal_atomic.o

DEPS := $(OBJS:%.o=%.d)
OBJS_AS_ARGS = $(patsubst %,"%",$(OBJS))
DEPS_AS_ARGS = $(patsubst %,"%",$(DEPS))

BOARD_LC := $(shell echo $(BOARD) | tr A-Z a-z)
OUTPUT_FILE_NAME := bootloader-$(BOARD_LC)-$(GIT_VERSION)
QUOTE := "
# OUTPUT_FILE_PATH +=$(OUTPUT_FILE_NAME).elf
#OUTPUT_FILE_PATH_AS_ARGS +=$(OUTPUT_FILE_NAME).elf
BLSIZE_DEF := -D$(QUOTE)$(shell sed 's/;//g' gcc/blsize.ld | tr -d ' ')$(QUOTE)

# .PHONY MAIN_dfu MAIN_flash
# MAIN_%:
# 	= usb_dfu_main.o

vpath %.c ../
vpath %.s ../
vpath %.S ../

.PHONY: clean-dfu clean-flash

all: $(SUB_DIRS) dfu-merge

dfu-merge: $(OUTPUT_FILE_NAME)-dfu

$(OUTPUT_FILE_NAME)-dfu: $(OUTPUT_FILE_NAME)-flash

# Linker target

$(OUTPUT_FILE_NAME)-%: $(OBJS) usb_%_main.o
	@echo Building target: $@
	@echo Invoking: ARM/GNU Linker
	$(QUOTE)arm-none-eabi-gcc$(QUOTE) -o $(OUTPUT_FILE_NAME)-$*.elf $(OBJS_AS_ARGS) "usb_$*_main.o" \
	-Wl,--no-undefined -Wl,--print-memory-usage -Wl,--strip-discarded -Wl,--start-group -lm -Wl,--end-group -mthumb \
	-Wl,-Map="$(OUTPUT_FILE_NAME)-$*.map" --specs=nano.specs -Wl,--gc-sections -mcpu=cortex-m4 \
	-T"../gcc/gcc/same54p20a_$*.ld" \
	-L"../gcc/gcc"
	@echo Finished building target: $@

	"arm-none-eabi-objcopy" -O binary "$(OUTPUT_FILE_NAME)-$*.elf" "$(OUTPUT_FILE_NAME)-$*.bin"
#	"arm-none-eabi-objcopy" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "$(OUTPUT_FILE_NAME)-$*.elf" "$(OUTPUT_FILE_NAME)-$*.hex"
#	"arm-none-eabi-objcopy" -j .eeprom --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0 --no-change-warnings -O binary "$(OUTPUT_FILE_NAME)-$*.elf" "$(OUTPUT_FILE_NAME)-$*.eep" || exit 0
	"arm-none-eabi-objdump" -h -S "$(OUTPUT_FILE_NAME)-$*.elf" > "$(OUTPUT_FILE_NAME)-$*.lss"
	"arm-none-eabi-size" "$(OUTPUT_FILE_NAME)-$*.elf"
	ln -sf $(OUTPUT_FILE_NAME)-$*.bin bootloader-$(BOARD_LC)-$*.bin
	ln -sf $(OUTPUT_FILE_NAME)-$*.elf bootloader-$(BOARD_LC)-$*.elf

.PHONY: dfu-merge
dfu-merge:
	$(info updating updater section with padded bootloader file..)
	$(SILENT)dd if=/dev/zero bs=16384 count=1 of=dfu-flash-padded.bin
	$(SILENT)dd if=$(OUTPUT_FILE_NAME)-flash.bin conv=notrunc of=dfu-flash-padded.bin
	$(SILENT)"arm-none-eabi-objcopy" --update-section .blupdate=dfu-flash-padded.bin $(OUTPUT_FILE_NAME)-dfu.elf
	$(SILENT)"arm-none-eabi-objcopy" -O binary $(OUTPUT_FILE_NAME)-dfu.elf $(OUTPUT_FILE_NAME)-dfu.bin
	rm dfu-flash-padded.bin

# Compiler targets




%.o: %.c
	@echo Building file: $<
	@echo ARM/GNU C Compiler
	$(QUOTE)arm-none-eabi-gcc$(QUOTE) -x c -mthumb -DDEBUG -Os -ffunction-sections -mlong-calls -g3 -Wall -c -std=gnu99 \
$(BLSIZE_DEF) -D__SAME54P19A__ -D$(BOARD) -mcpu=cortex-m4 -mfloat-abi=softfp -mfpu=fpv4-sp-d16 \
-I"../" -I"../config" -I"../hal/include" -I"../hal/utils/include" -I"../hpl/cmcc" -I"../hpl/core" -I"../hpl/dmac" -I"../hpl/gclk" -I"../hpl/mclk" -I"../hpl/nvmctrl" -I"../hpl/osc32kctrl" -I"../hpl/oscctrl" -I"../hpl/pm" -I"../hpl/port" -I"../hpl/ramecc" -I"../hpl/usb" -I"../hri" -I"../" -I"../config" -I"../usb" -I"../usb/class/dfu" -I"../usb/class/dfu/device" -I"../usb/device" -I"../" -I"../CMSIS/Include" -I"../include"  \
-MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -o "$@" "$<"
	@echo Finished building: $<

%.o: %.s
	@echo Building file: $<
	@echo ARM/GNU Assembler
	$(QUOTE)arm-none-eabi-as$(QUOTE) -x c -mthumb -DDEBUG -Os -ffunction-sections -mlong-calls -g3 -Wall -c -std=gnu99 \
$(BLSIZE_DEF) -D__SAME54P19A__ -D$(BOARD) -mcpu=cortex-m4 -mfloat-abi=softfp -mfpu=fpv4-sp-d16 \
-I"../" -I"../config" -I"../hal/include" -I"../hal/utils/include" -I"../hpl/cmcc" -I"../hpl/core" -I"../hpl/dmac" -I"../hpl/gclk" -I"../hpl/mclk" -I"../hpl/nvmctrl" -I"../hpl/osc32kctrl" -I"../hpl/oscctrl" -I"../hpl/pm" -I"../hpl/port" -I"../hpl/ramecc" -I"../hpl/usb" -I"../hri" -I"../" -I"../config" -I"../usb" -I"../usb/class/dfu" -I"../usb/class/dfu/device" -I"../usb/device" -I"../" -I"../CMSIS/Include" -I"../include"  \
-MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -o "$@" "$<"
	@echo Finished building: $<

%.o: %.S
	@echo Building file: $<
	@echo ARM/GNU Preprocessing Assembler
	$(QUOTE)arm-none-eabi-gcc$(QUOTE) -x c -mthumb -DDEBUG -Os -ffunction-sections -mlong-calls -g3 -Wall -c -std=gnu99 \
$(BLSIZE_DEF) -D__SAME54P19A__ -D$(BOARD) -mcpu=cortex-m4 -mfloat-abi=softfp -mfpu=fpv4-sp-d16 \
-I"../" -I"../config" -I"../hal/include" -I"../hal/utils/include" -I"../hpl/cmcc" -I"../hpl/core" -I"../hpl/dmac" -I"../hpl/gclk" -I"../hpl/mclk" -I"../hpl/nvmctrl" -I"../hpl/osc32kctrl" -I"../hpl/oscctrl" -I"../hpl/pm" -I"../hpl/port" -I"../hpl/ramecc" -I"../hpl/usb" -I"../hri" -I"../" -I"../config" -I"../usb" -I"../usb/class/dfu" -I"../usb/class/dfu/device" -I"../usb/device" -I"../" -I"../CMSIS/Include" -I"../include"  \
-MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"  -o "$@" "$<"
	@echo Finished building: $<

# Detect changes in the dependent files and recompile the respective object files.
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(DEPS)),)
-include $(DEPS)
-include usb_flash_main.d usb_dfu_main.d
endif
endif

$(SUB_DIRS):
	$(MK_DIR) "$@"

clean-%:
	rm -f $(OBJS_AS_ARGS)
	# rm -f $(OUTPUT_FILE_PATH)
	rm -f $(DEPS_AS_ARGS)
	rm -f $(OUTPUT_FILE_NAME)-$*.a $(OUTPUT_FILE_NAME)-$*.hex $(OUTPUT_FILE_NAME)-$*.bin \
		$(OUTPUT_FILE_NAME)-$*.lss $(OUTPUT_FILE_NAME)-$*.eep $(OUTPUT_FILE_NAME)-$*.map \
		$(OUTPUT_FILE_NAME)-$*.srec bootloader-$(BOARD_LC)-$*.bin bootloader-$(BOARD_LC)-$*.elf

clean: clean-dfu clean-flash
	rm -f $(OBJS_AS_ARGS)
	# rm -f $(OUTPUT_FILE_PATH)
	rm -f $(DEPS_AS_ARGS)
	# rm -f $(OUTPUT_FILE_NAME).a $(OUTPUT_FILE_NAME).hex $(OUTPUT_FILE_NAME).bin \
	# 	$(OUTPUT_FILE_NAME).lss $(OUTPUT_FILE_NAME).eep $(OUTPUT_FILE_NAME).map \
	# 	$(OUTPUT_FILE_NAME).srec bootloader-$(BOARD_LC).bin bootloader-$(BOARD_LC).elf

mrproper: clean
	rm -f *.o *.d *.a *.elf *.bin *.hex *.ihex *.eep *.lss *.map *.srec
