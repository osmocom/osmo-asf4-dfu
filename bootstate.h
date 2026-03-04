/*
 * Copyright (C) 2026 sysmocom -s.f.m.c. GmbH, Author: Alexander Couzens <lynxis@fe80.eu>
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

#ifndef OCTSIM_BOOTSTATE_H
#define OCTSIM_BOOTSTATE_H

#include <stdint.h>

/* boot counter might be used for other things as well */
#define BOOTSTATE_MAGIC 0xbff1c0de
#define BOOTSTATE_VERSION 1

/* allow extension of the struct, but limit it to a reasonable size */
#define BOOTSTATE_MAX_SIZE 256

struct bootstate {
	uint32_t crc32;
	uint32_t magic;
	uint32_t version;
	uint32_t length; /* length of the whole struct */

	uint8_t counter;
	uint8_t rfu1;
	uint16_t rfu2;
} __attribute__((__packed__));

/** brief Set the boot counter in the boot state */
int set_bootcounter(void *data, uint8_t bootcounter);

/** brief Get the boot counter
 * *value has been set, when ret == 0
 */
int get_bootcounter(const void *data, uint8_t *value);

/** brief Check if the boot state is valid
 * Returns 0 if state is valid or < 0 if invalid.
 */
int check_bootstate(const void *data);

/** brief Increase the bootcounter
 * If the bootstate isn't initialized, it will initialized the bootstate and set bootcount to 1.
 * @return the increased value of the counter.
 */
uint8_t increase_bootcounter(void *data);

#endif /* #ifndef OCTSIM_BOOTSTATE_H */
