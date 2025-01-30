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

#include <stdint.h>
#ifndef HOST_TOOL
#include "atmel_start.h"

#ifdef __clang__
#define OPTNONE optnone
#else
#define OPTNONE optimize("O0")
#endif

extern void _Reset_Handler(void);

__attribute__((used, section(".crc_code")))
#endif
static uint32_t
calculate_crc(const uint8_t *data, uint32_t length)
{
	uint32_t crc = 0xFFFFFFFF;
	for (uint32_t i = 0; i < length; i++) {
		crc ^= data[i];
		for (uint8_t j = 0; j < 8; j++) {
			crc = (crc & 1) ? (crc >> 1) ^ 0xEDB88320 : crc >> 1;
		}
	}
	return ~crc;
}

#ifndef HOST_TOOL
__attribute__((used, section(".crc_code"), OPTNONE)) void Reset_Handler(void)
{
	extern uint32_t __CRC_Checksum;
	extern uint32_t __CRC_Start;
	extern uint32_t __CRC_End;
	extern int main(void);

	/*
  the old BL will unfortunately enable usb even when booting and not doing dfu..
  we must reset it here, or the host will get really unhappy trying to talk to the "attached"
  dev and reset the bus...
  */
	USB->DEVICE.CTRLA.reg = 0x1;

	uint32_t length = (uint32_t)&__CRC_End - (uint32_t)&__CRC_Start;
	uint32_t calculated_crc = calculate_crc((uint8_t *)&__CRC_Start, length);
	uint32_t expected_crc = *(uint32_t *)&__CRC_Checksum;

	if (calculated_crc != expected_crc) {
		static volatile uint32_t *dfu_magic = (uint32_t *)HSRAM_ADDR;
		*dfu_magic = 0x44465521;

		/* Reset the device if CRC does not match */
		__DSB();
		SCB->AIRCR = (uint32_t)((0x5FAUL << SCB_AIRCR_VECTKEY_Pos) | (SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) |
					SCB_AIRCR_SYSRESETREQ_Msk);
		__DSB();
	}

	/* Proceed to main application */
	// main();
	_Reset_Handler();

	/* Infinite loop if main returns */
	while (1)
		;
}
#endif

#ifdef HOST_TOOL
#include <stdio.h>
#include <stdlib.h>

/* Usage: ./tool <binary_file> <start_offset_hex> <length_hex>
   - start_offset_hex and length_hex are hex values indicating where to calculate the CRC.
*/
int main(int argc, char *argv[])
{
	if (argc != 4) {
		fprintf(stderr, "Usage: %s <binary_file> <start_offset_hex> <length_hex>\n", argv[0]);
		return 1;
	}

	const char *filename = argv[1];
	uint32_t start_offset = strtoul(argv[2], NULL, 10);
	uint32_t length = strtoul(argv[3], NULL, 10);

	FILE *f = fopen(filename, "rb");
	if (!f) {
		perror("fopen");
		return 1;
	}

	// Seek to start_offset
	if (fseek(f, (long)start_offset, SEEK_SET) != 0) {
		perror("fseek");
		fclose(f);
		return 1;
	}

	uint8_t *buf = (uint8_t *)malloc(length);
	if (!buf) {
		fprintf(stderr, "Memory allocation error\n");
		fclose(f);
		return 1;
	}

	size_t read_bytes = fread(buf, 1, length, f);
	fclose(f);

	if (read_bytes != length) {
		fprintf(stderr, "Error reading file or not enough data\n");
		free(buf);
		return 1;
	}

	uint32_t crc = calculate_crc(buf, length);
	free(buf);

	printf("%08X\n", crc);
	return 0;
}
#endif
