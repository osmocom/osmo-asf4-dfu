This is an implementation of the DFU mode of the [USB DFU Device Class Specification](https://usb.org/document-library/device-firmware-upgrade-11-new-version-31-aug-2004) for the Microchip SAM D5x/E5x micro-controller.
It is meant to be used as bootloader to allow flashing the main application over USB.

This branch is specific to the proprietary sysmocom sysmoOCTSIM hardware.

The code uses the [Atmel START](https://start.atmel.com/) ASFv4 library.

USB DFU
=======

This implementation support the following USB DFU capabilities:

* can download: allowing to download the code over USB on the device (enabled per default)
* manifestation tolerant: allowing to download after a previous download (disabled per default)
* will detach: forcing the device the reset after a download, else it wit for a USB reset (enable per default)

Set the corresponding attributes in the 'DFUD_IFACE_DESCB' macro definition in the 'usb/class/dfu/device/dfudf_desc.h' file.

Compiling
=========

Use the 'Makefile' script to compile the source code using the ARM none EABI GCC cross-cimpilig toolchain:
```
cd gcc
make
```

The resulting firmware binary is `AtmelStart.bin`.

Flashing
========

To flash the bootloader you can use OpenOCD with any SWJ adapter.

The USB DFU bootloader should be flashed in a protected area of the flash memory to prevent for erasing it, as specified in data sheet section 25.6.2 Memory Organization.
The bootloader size is configured in the NVM user configuration BOOTPROT field, as specified in data sheet section 25.6.9 NVM User Configuration.
The bit position of the BOOTPROT field is documented in data sheet section 9.4 NVM User Page Mapping.
We need to reserve as least 2 pages of 8192 bytes for the bootloader since it is a bit larger than 10 KB (e.g. over 8 KB).
This setting will also tell the bootloader where to flash the application firmware to (e.g. after the bootloader reserved space).
The LED will blink once per second if this size is not set.

ST-LINK/V2
----------

To flash the USB DFU bootloader, we will use OpenOCD (with the [SAM E54 patch](http://openocd.zylin.com/#/c/4272/) and a ST-LINK/V2 SWD adapter.
The command will perform the following actions:
* remove reserved bootloader space so we can erase it (and reset MCU for change to be effective)
* erase the whole flash
* program the bootloader
* reserve bootloader space

`openocd --file interface/stlink.cfg --command "transport select hla_swd" --command "set CHIPNAME same54" --command "set CPUTAPID 0x2ba01477" --file target/atsame5x.cfg --command "init" --command "reset halt" --command "atsame5 bootloader 0" --command "flash erase_sector 0 0 last" --command "reset halt" --command "program ./AtmelStart.bin" --command "atsame5 bootloader 16384"  --command "reset run" --command "shutdown"`
