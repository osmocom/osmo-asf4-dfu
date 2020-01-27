/**
 * \file
 * \brief USB DFU bootloader implementation (DFU mode)
 *
 * Copyright (c) 2018-2019 sysmocom -s.f.m.c. GmbH, Author: Kevin Redon <kredon@sysmocom.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <errno.h>
#include "atmel_start.h"
#include "atmel_start_pins.h"

/** Start address of the application in flash
 *  \remark must be initialized by check_bootloader
 */
static uint32_t* application_start_address;

/** Location of the DFU magic value to force starting DFU */
static volatile uint32_t* dfu_magic = (uint32_t*)HSRAM_ADDR; // magic value should be written at start of RAM

/** Check if the bootloader is valid
 *  \return true if the bootloader is valid and can be run
 *  \remark initializes application_start_address
 */
static bool check_bootloader(void)
{
	if (hri_nvmctrl_read_STATUS_BOOTPROT_bf(FLASH_0.dev.hw) > 15) { // ensure BOOTPROT setting is valid
		return false;
	}
	application_start_address = (uint32_t*)((15 - hri_nvmctrl_read_STATUS_BOOTPROT_bf(FLASH_0.dev.hw)) * 8192); // calculate bootloader size to know start address of the application (e.g. after the bootloader)
	if (0 == application_start_address) { // no space has been reserved for the bootloader
		return false;
	}
	return true;
}

/** Check if starting the bootloader is forced
 *  \return true of the DFU bootloader should be started
 */
static bool check_force_dfu(void)
{
	if (0x44465521 == *dfu_magic) { // check for the magic value which can be set by the main application
		*dfu_magic = 0; // erase value so we don't stay in the DFU bootloader upon reset
		return true;
	}
	if (0 == gpio_get_pin_level(BUTTON_FORCE_DFU)) { // signal is low when button is pressed
		return true;
	}
	return false;
}

/** Check if the application is valid
 *  \return true if the application is valid and can be started
 *  \warning application_start_address must be initialized
 */
static bool check_application(void)
{
	/* the application starts with the vector table
	 * the first entry in the vector table is the initial stack pointer (SP) address
	 * the stack will be placed in RAM which begins at 0x2000 0000, and there is up to 256 KB of RAM (0x40000).
	 * if the SP is not in this range (e.g. flash has been erased) there is no valid application
	 * the second entry in the vector table is the reset address, corresponding to the application start
	 */
	return (HSRAM_ADDR == ((*application_start_address) & 0xFFF80000));
}

/** Start the application
 *  \warning application_start_address must be initialized
 */
static void start_application(void)
{
	__set_MSP(*application_start_address); // re-base the Stack Pointer
	SCB->VTOR = ((uint32_t) application_start_address & SCB_VTOR_TBLOFF_Msk); // re-base the vector table base address
	asm("bx %0"::"r"(*(application_start_address + 1))); // jump to application Reset Handler in the application */
}
#if defined(SYSMOOCTSIM)
/* Section 9.6 of SAMD5x/E5x Family Data Sheet */
static int get_chip_unique_serial(uint8_t *out, size_t len)
{
	uint32_t *out32 = (uint32_t *)out;
	if (len < 16)
		return -EINVAL;

	out32[0] = *(uint32_t *)0x008061fc;
	out32[1] = *(uint32_t *)0x00806010;
	out32[2] = *(uint32_t *)0x00806014;
	out32[3] = *(uint32_t *)0x00806018;

	return 0;
}

/* same as get_chip_unique_serial but in hex-string format */
static int get_chip_unique_serial_str(char *out, size_t len)
{
	uint8_t buf[16];
	int rc;

	if (len < 16*2 + 1)
		return -EINVAL;

	rc = get_chip_unique_serial(buf, sizeof(buf));
	if (rc < 0)
		return rc;
	for (int i = 0; i < sizeof(buf); i++)
		sprintf(&out[i*2], "%02x", buf[i]);
	return 0;
}

static int str_to_usb_desc(char* in, uint8_t in_sz, uint8_t* out, uint8_t out_sz){
	if (2+in_sz*2 < out_sz)
		return -1;

	memset(out, 0, out_sz);
	out[0] = out_sz;
	out[1] = 0x3;
	for (int i= 2; i < out_sz; i+=2)
		out[i] = in[(i >> 1) - 1];
	return 0;
}

char sernr_buf[16*2+1];
//unicode for descriptor
uint8_t sernr_buf_descr[1+1+16*2*2];
#endif

int main(void)
{
	atmel_start_init(); // initialise system
#if defined(SYSMOOCTSIM)
	get_chip_unique_serial_str(sernr_buf, sizeof(sernr_buf));
	str_to_usb_desc(sernr_buf, sizeof(sernr_buf), sernr_buf_descr, sizeof(sernr_buf_descr));
#endif
	if (!check_bootloader()) { // check bootloader
		// blink the LED to tell the user we don't know where the application starts
		while (true) {
			gpio_set_pin_level(LED_SYSTEM, false);
			delay_ms(500);
			gpio_set_pin_level(LED_SYSTEM, true);
			delay_ms(500);
		}
	}
	if (!check_force_dfu() && check_application()) { // application is valid
		start_application(); // start application
	} else {
		if (!check_application()) { // if the application is corrupted the start DFU start should be dfuERROR
			dfu_state = USB_DFU_STATE_DFU_ERROR;
		}
		usb_dfu(); // start DFU bootloader
	}
}
