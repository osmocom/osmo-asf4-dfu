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

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "bootstate.h"

static uint32_t calculate_crc(const uint8_t *data, uint32_t length)
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

int check_bootstate(const void *data)
{
	const struct bootstate *state = data;

	if (state->magic != BOOTSTATE_MAGIC)
		return -1;

	if (state->length < sizeof(struct bootstate))
		return -2;

	if (state->length > BOOTSTATE_MAX_SIZE)
		return -3;

	if (state->version != BOOTSTATE_VERSION)
		return -4;

	if (state->crc32 != calculate_crc((const void *)&state->magic, state->length - 4))
		return -5;

	return 0;
}

int get_bootcounter(const void *data, uint8_t *value)
{
	if (check_bootstate(data))
		return -1;

	const struct bootstate *state = data;
	*value = state->counter;

	return 0;
}

uint8_t increase_bootcounter(void *data)
{
	struct bootstate *state = data;
	uint8_t counter;

	if (check_bootstate(data)) {
		set_bootcounter(data, 1);
	} else if (state->counter < 0) {
		set_bootcounter(data, 1);
	} else {
		counter = state->counter;
		if (counter < 127)
			counter++;

		set_bootcounter(data, counter);
	}

	return state->counter;
}

int set_bootcounter(void *data, uint8_t bootcounter)
{
	struct bootstate *state = data;

	if (check_bootstate(state)) {
		memset(state, 0, sizeof(*state));
		state->magic = BOOTSTATE_MAGIC;
		state->version = BOOTSTATE_VERSION;
		state->length = sizeof(*state);
	}

	state->counter = bootcounter;
	state->crc32 = calculate_crc((uint8_t *)&state->magic, state->length - 4);

	return 0;
}

