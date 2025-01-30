/*
 * Copyright (C) 2024 sysmocom -s.f.m.c. GmbH, Author: Eric Wild <ewild@sysmocom.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <errno.h>
#include <stdint.h>
#include "atmel_start.h"
#include "atmel_start_pins.h"
#include "component/nvmctrl.h"

#ifdef __clang__
#define OPTNONE optnone
#else
#define OPTNONE optimize("O0")
#endif

#define BLSZ_BYTE (BL_SIZE_BYTE)
static volatile const uint32_t bl_update_data[BLSZ_BYTE / sizeof(uint32_t)]
	__attribute__((section(".fwupdate"), used, retain)) = { 0xFF };

_Static_assert(BLSZ_BYTE % NVMCTRL_PAGE_SIZE == 0, "invalid bl size");

__attribute__((section(".ramfunc"), OPTNONE)) void same54_flash_write_page(uint32_t address, uint8_t *data)
{
	// address &= ~(NVMCTRL_PAGE_SIZE - 1);
	while (!NVMCTRL->STATUS.bit.READY) {
	}

	hri_nvmctrl_write_CTRLB_reg(NVMCTRL, NVMCTRL_CTRLB_CMD_PBC | NVMCTRL_CTRLB_CMDEX_KEY);

	while (!NVMCTRL->STATUS.bit.READY) {
	}

	volatile uint32_t *dst = (uint32_t *)address;
	volatile uint32_t *src = (uint32_t *)data;

	for (uint32_t i = 0; i < (NVMCTRL_PAGE_SIZE / sizeof(uint32_t)); i++) {
		dst[i] = src[i];
	}

	NVMCTRL->ADDR.reg = address;
	NVMCTRL->CTRLB.reg = NVMCTRL_CTRLB_CMD_WP | NVMCTRL_CTRLB_CMDEX_KEY;
	while (!NVMCTRL->INTFLAG.bit.DONE) {
	}
	while (!NVMCTRL->STATUS.bit.READY) {
	}
}

#define NVMCTRL_REGIONS_NUM 32
#define REGION_SZ (FLASH_SIZE / NVMCTRL_REGIONS_NUM)
#define BL_IN_PAGESZ (BLSZ_BYTE / NVMCTRL_PAGE_SIZE)
#define BL_IN_REGIONS (BLSZ_BYTE / REGION_SZ)

__attribute__((section(".ramfunc"), OPTNONE)) void do_update()
{
	uint16_t mask = NVMCTRL_CTRLA_AHBNS0 | NVMCTRL_CTRLA_AHBNS1 | NVMCTRL_CTRLA_RWS_Msk;
	uint16_t _nvm = (NVMCTRL_CTRLA_CACHEDIS0 | NVMCTRL_CTRLA_CACHEDIS1 | NVMCTRL_CTRLA_PRM(3) |
			 NVMCTRL_CTRLA_RWS(0x7) | NVMCTRL_CTRLA_WMODE(0) | NVMCTRL_CTRLA_AUTOWS);
	uint32_t ctrla = NVMCTRL->CTRLA.reg;
	ctrla &= ~(mask);
	ctrla |= _nvm;
	NVMCTRL->CTRLA.reg = ctrla;

	NVIC_DisableIRQ(NVMCTRL_0_IRQn);
	NVIC_DisableIRQ(NVMCTRL_1_IRQn);
	NVIC_ClearPendingIRQ(NVMCTRL_0_IRQn);
	NVIC_ClearPendingIRQ(NVMCTRL_1_IRQn);

	const uint32_t bootloader_regions = BLSZ_BYTE / NVMCTRL_BLOCK_SIZE;

	_Static_assert(BL_IN_REGIONS > 0, "bootprot regions must be at least 1");

	// unlock everything
	for (int i = 0; i < NVMCTRL_REGIONS_NUM; i++) {
		while (!hri_nvmctrl_get_STATUS_READY_bit(NVMCTRL)) {
		}
		hri_nvmctrl_write_ADDR_reg(NVMCTRL, REGION_SZ * i);
		hri_nvmctrl_write_CTRLB_reg(NVMCTRL, NVMCTRL_CTRLB_CMD_UR | NVMCTRL_CTRLB_CMDEX_KEY);
	}

	// disable bl protection
	hri_nvmctrl_write_CTRLB_reg(NVMCTRL, NVMCTRL_CTRLB_CMD_SBPDIS | NVMCTRL_CTRLB_CMDEX_KEY);
	while (!hri_nvmctrl_get_STATUS_READY_bit(NVMCTRL)) {
	}

	for (int i = 0; i < bootloader_regions; i++) {
		while (!hri_nvmctrl_get_STATUS_READY_bit(NVMCTRL)) {
		}
		hri_nvmctrl_write_ADDR_reg(NVMCTRL, NVMCTRL_BLOCK_SIZE * i);
		hri_nvmctrl_write_CTRLB_reg(NVMCTRL, NVMCTRL_CTRLB_CMD_EB | NVMCTRL_CTRLB_CMDEX_KEY);
	}

	while (!hri_nvmctrl_get_STATUS_READY_bit(NVMCTRL)) {
	}

	// Write firmware data to flash
	uint32_t address = 0;
	uint8_t *data_ptr = (uint8_t *)bl_update_data;
	uint32_t remaining = BLSZ_BYTE;

	while (remaining >= NVMCTRL_PAGE_SIZE) {
		same54_flash_write_page(address, data_ptr);
		address += NVMCTRL_PAGE_SIZE;
		data_ptr += NVMCTRL_PAGE_SIZE;
		remaining -= NVMCTRL_PAGE_SIZE;
	}

	//erases itself, fns likely not inlined @ -Os.
	for (int i = bootloader_regions; i < bootloader_regions + 1; i++) {
		while (!(((Nvmctrl *)NVMCTRL)->STATUS.reg & NVMCTRL_STATUS_READY) >> NVMCTRL_STATUS_READY_Pos) {
		}
		((Nvmctrl *)NVMCTRL)->ADDR.reg = NVMCTRL_BLOCK_SIZE * i;
		((Nvmctrl *)NVMCTRL)->CTRLB.reg = NVMCTRL_CTRLB_CMD_EB | NVMCTRL_CTRLB_CMDEX_KEY;
	}
	while (!(((Nvmctrl *)NVMCTRL)->STATUS.reg & NVMCTRL_STATUS_READY) >> NVMCTRL_STATUS_READY_Pos) {
	}

	__DSB();
	SCB->AIRCR = (uint32_t)((0x5FAUL << SCB_AIRCR_VECTKEY_Pos) | (SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) |
				SCB_AIRCR_SYSRESETREQ_Msk);
	__DSB();
}
int main(void)
{
	__disable_irq();
	MCLK->AHBMASK.reg |= MCLK_AHBMASK_NVMCTRL;
	do_update();
}
