This is an implementation of the DFU mode of the [USB DFU Device Class Specification](https://usb.org/document-library/device-firmware-upgrade-11-new-version-31-aug-2004) for the Microchip SAM D5x/E5x micro-controller.
It is meant to be used as bootloader to allow flashing the main application over USB.

The code has been developed using a [SAM E54](https://www.microchip.com/wwwproducts/en/ATSAME54P20A) micro-controller.
It should work on any chip of the SAM D5x/E5x device family by replacing the corresponding device-specific definitions (usually including the chip name in the file name).

The code uses the [Atmel START](https://start.atmel.com/) ASFv4 library.

Board
=====

The code can be compiled for specific boards:

- Microchip [SAM E54 Xplained Pro](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNo/ATSAME54-XPRO) development board, named *SAME54_XPLAINED_PRO*
- sysmocom sysmoOCTSIM, name *SYSMOOCTSIM*

The board name can be set in 'gcc/Makefile' *BOARD* variable, or provided while compiling (e.g. `make BOARD=SAME54_XPLAINED_PRO`).
*SAME54_XPLAINED_PRO* is the default value.

USB DFU
=======

This implementation support the following USB DFU capabilities:

* can download: allowing to download the code over USB on the device (enabled per default)
* manifestation tolerant: allowing to download after a previous download (disabled per default)
* will detach: forcing the device the reset after a download, else it wit for a USB reset (enable per default)

Set the corresponding attributes in the 'DFUD_IFACE_DESCB' macro definition in the 'usb/class/dfu/device/dfudf_desc.h' file.

To force the DFU bootloader to start there are several possibilities:

* if the application following the bootloader is invalid (e.g. MSP is not in RAM)
* if a button is pressed (the button defined in *BUTTON_FORCE_DFU*)
* if the magic value "DFU!" (e.g. 0x44465521) is set at the start of the RAM (e.g. by the main application when performing a USB detach) 

Compiling
=========

Use the 'Makefile' script to compile the source code using the ARM none EABI GCC cross-compiling toolchain:
```
cd gcc
make
```

The board name can be set in 'gcc/Makefile' *BOARD* variable, or provided while compiling (e.g. `make BOARD=SAME54_XPLAINED_PRO`).
*SAME54_XPLAINED_PRO* is the default value.

The resulting firmware binary is `bootloader-$(BOARD)-$(GIT_VERSION).bin`.

Flashing
========

The USB DFU bootloader should be flashed in a protected area of the flash memory to prevent for erasing it, as specified in data sheet section 25.6.2 Memory Organization.
The bootloader size is configured in the NVM user configuration BOOTPROT field, as specified in data sheet section 25.6.9 NVM User Configuration.
The bit position of the BOOTPROT field is documented in data sheet section 9.4 NVM User Page Mapping.
We need to reserve as least 2 pages of 8192 bytes for the bootloader since it is a bit larger than 10 KB (e.g. over 8 KB).
This setting will also tell the bootloader where to flash the application firmware to (e.g. after the bootloader reserved space).
The LED will blink once per second if this size is not set.

EDBG
----

To flash the bootloader using the [edbg tool](https://github.com/ataradov/edbg) over the EDBG interface of the SAM E54 Xplained Pro development board.

To flash the USB DFU bootloader, perform the following actions:
* remove reserved bootloader space so we can erase it: `edbg --target atmel_cm4v2 --fuse wv,29:26,15`
* erase the whole flash: `edbg --target atmel_cm4v2 --fuse v,29:26,15 --erase`
* program the bootloader: `edbg --target atmel_cm4v2 --fuse v,29:26,15 --program --verify --file bootloader-BOARD-XXXX.bin`
* reserve bootloader space: `edbg --target atmel_cm4v2 --fuse wv,29:26,13`

SWJ
---

The bootloader can also be flashed over SWJ (e.g. JTAG or SWD).
You need to perform the following actions:

* remove reserved bootloader space so we can erase it (and reset MCU for change to be effective)
* erase the whole flash
* program the bootloader
* reserve bootloader space

Using OpenOCD (with the [SAM E54 patch](http://openocd.zylin.com/#/c/4272/)) and a ST-LINK/V2 SWD adapter, run the following command:

`openocd --file interface/stlink.cfg --command "transport select hla_swd" --command "set CHIPNAME same54" --command "set CPUTAPID 0x2ba01477" --file target/atsame5x.cfg --command "init" --command "reset halt" --command "atsame5 bootloader 0" --command "flash erase_sector 0 0 last" --command "reset halt" --command "program ./bootloader.bin" --command "atsame5 bootloader 16384"  --command "reset run" --command "shutdown"`

