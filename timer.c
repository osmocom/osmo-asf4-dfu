/*
 * (C) 2026 sysmocom -s.f.m.c. GmbH
 * Author: Alexander Couzens <lynxis@fe80.eu>
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

#include "atmel_start.h"

/* configure tc0 for 0.5s, oneshot mode */
void tc0_setup(void)
{
	/* set TC0 to global clock 3 / 32 khz*/
	hri_gclk_write_PCHCTRL_reg(GCLK, TC0_GCLK_ID, 3 | GCLK_PCHCTRL_CHEN);

	/* enable ABPA access for TC0 */
	hri_mclk_set_APBAMASK_TC0_bit(MCLK);

	hri_tc_wait_for_sync(TC0, TC_SYNCBUSY_SWRST);

	/* disable the TC, to enable access to ctrl registers */
	hri_tc_clear_CTRLA_ENABLE_bit(TC0);

	/* 32 kHz / 64 with an 8 bit counter gives 0.5 seconds, use one shot */
	hri_tc_write_CTRLA_reg(TC0, TC_CTRLA_MODE_COUNT8 | TC_CTRLA_PRESCALER_DIV64);

	hri_tc_set_CTRLB_ONESHOT_bit(TC0);

	/* normal frequency, count between 0 and TOP */
	hri_tc_set_WAVE_WAVEGEN_bf(TC0, 0);

	/* set PERIOD/TOP value to 0xff */
	hri_tccount8_set_PER_PER_bf(TC0, 0xff);

	/* Start the TC0 */
	hri_tc_set_CTRLA_ENABLE_bit(TC0);
}

/* returns true if the tc0 has been re-armed */
bool tc0_finished_rearm(void)
{
	/* still running, do nothing */
	if (!hri_tc_get_STATUS_STOP_bit(TC0)) {
		return false;
	}

	hri_tc_set_CTRLB_CMD_bf(TC0, TC_CTRLBSET_CMD_RETRIGGER_Val);
	return true;
}
