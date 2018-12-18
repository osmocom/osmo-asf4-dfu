/**
 * \file
 *
 * \brief USB Device Stack DFU Function Definition.
 *
 * Copyright (c) 2015-2018 Microchip Technology Inc. and its subsidiaries.
 * Copyright (c) 2018 sysmocom -s.f.m.c. GmbH, Author: Kevin Redon <kredon@sysmocom.de>
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 */

#ifndef USBDF_DFU_H_
#define USBDF_DFU_H_

#include "usbdc.h"

/**
 * \brief Initialize the USB DFU Function Driver
 * \return Operation status.
 */
int32_t dfudf_init(void);

/**
 * \brief Deinitialize the USB DFU Function Driver
 * \return Operation status.
 */
void dfudf_deinit(void);

/**
 * \brief Check whether DFU Function is enabled
 * \return Operation status.
 * \return true DFU Function is enabled
 * \return false DFU Function is disabled
 */
bool dfudf_is_enabled(void);

#endif /* USBDF_DFU_H_ */