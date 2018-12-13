/*
 * (C) 2018, sysmocom -s.f.m.c. GmbH, Author: Kevin Redon <kredon@sysmocom.de>
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
#ifndef USB_DEVICE_MAIN_H
#define USB_DEVICE_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "dfudf.h"
#include "dfudf_desc.h"

void usb_dfu(void);
void usb_dfu_init(void);

/**
 * \berif Initialize USB
 */
void usb_init(void);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // USB_DEVICE_MAIN_H
