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
#include "atmel_start.h"
#include "usb_start.h"
#include "config/usbd_config.h"

#if CONF_USBD_HS_SP
static uint8_t single_desc_bytes[] = {
    /* Device descriptors and Configuration descriptors list. */
    DFUD_HS_DESCES_LS_FS};
static uint8_t single_desc_bytes_hs[] = {
    /* Device descriptors and Configuration descriptors list. */
    DFUD_HS_DESCES_HS};
#else
static uint8_t single_desc_bytes[] = {
    /* Device descriptors and Configuration descriptors list. */
    DFUD_DESCES_LS_FS};
#endif

static struct usbd_descriptors single_desc[]
    = {{single_desc_bytes, single_desc_bytes + sizeof(single_desc_bytes)}
#if CONF_USBD_HS_SP
       ,
       {single_desc_bytes_hs, single_desc_bytes_hs + sizeof(single_desc_bytes_hs)}
#endif
};

/** USB DFU functional descriptor (with DFU attributes) */
static const uint8_t usb_dfu_func_desc_bytes[] = {DFUD_IFACE_DESCB};
static const usb_dfu_func_desc_t* usb_dfu_func_desc = (usb_dfu_func_desc_t*)&usb_dfu_func_desc_bytes;
/** Ctrl endpoint buffer */
static uint8_t ctrl_buffer[64];



/* transmit given string descriptor */
static bool send_str_desc(uint8_t ep, const struct usb_req *req, enum usb_ctrl_stage stage,
			  const uint8_t *desc)
{
	uint16_t len_req = LE16(req->wLength);
	uint16_t len_desc = desc[0];
	uint16_t len_tx;
	bool need_zlp = !(len_req & (CONF_USB_DFUD_BMAXPKSZ0 - 1));

	if (len_req <= len_desc) {
		need_zlp = false;
		len_tx = len_req;
	} else {
		len_tx = len_desc;
	}

	if (ERR_NONE != usbdc_xfer(ep, (uint8_t *)desc, len_tx, need_zlp)) {
		return true;
	}

	return false;
}

extern uint8_t sernr_buf_descr[];
/* call-back for every control EP request */
static int32_t string_req_cb(uint8_t ep, struct usb_req *req, enum usb_ctrl_stage stage)
{
	uint8_t index, type;

	if (stage != USB_SETUP_STAGE)
		return ERR_NOT_FOUND;

	if ((req->bmRequestType & (USB_REQT_TYPE_MASK | USB_REQT_DIR_IN)) !=
	    (USB_REQT_TYPE_STANDARD | USB_REQT_DIR_IN))
		return ERR_NOT_FOUND;

	/* abort if it's not a GET DESCRIPTOR request */
	if (req->bRequest != USB_REQ_GET_DESC)
		return ERR_NOT_FOUND;

	/* abort if it's not about a string descriptor */
	type = req->wValue >> 8;
	if (type != USB_DT_STRING)
		return ERR_NOT_FOUND;
#if 0
	printf("ep=%02x, bmReqT=%04x, bReq=%02x, wValue=%04x, stage=%d\r\n",
		ep, req->bmRequestType, req->bRequest, req->wValue, stage);
#endif
	/* abort if it's not a standard GET request */
	index = req->wValue & 0x00FF;
	switch (index) {
	case CONF_USB_DFUD_ISERIALNUM:
		return send_str_desc(ep, req, stage, sernr_buf_descr);
	default:
		return ERR_NOT_FOUND;
	}
}


static struct usbdc_handler string_req_h = {NULL, (FUNC_PTR)string_req_cb};



/**
 * \brief USB DFU Init
 */
void usb_dfu_init(void)
{
	usbdc_init(ctrl_buffer);
	dfudf_init();
#if defined(SYSMOOCTSIM)
	usbdc_register_handler(USBDC_HDL_REQ, &string_req_h);
#endif

	usbdc_start(single_desc);
	usbdc_attach();
}

/**
 * \brief reset device
 */
static void usb_dfu_reset(const enum usb_event ev, const uint32_t param)
{
	(void)param; // not used
	switch (ev) {
	case USB_EV_RESET:
		usbdc_detach(); // make sure we are detached
		NVIC_SystemReset(); // initiate a system reset
		break;
	default:
		break;
	}
	
}

/**
 * \brief Enter USB DFU runtime
 */
void usb_dfu(void)
{
	while (!dfudf_is_enabled()); // wait for DFU to be installed
	LED_SYSTEM_on(); // switch LED on to indicate USB DFU stack is ready

	ASSERT(hri_nvmctrl_read_STATUS_BOOTPROT_bf(FLASH_0.dev.hw) <= 15);
	uint32_t application_start_address = (15 - hri_nvmctrl_read_STATUS_BOOTPROT_bf(FLASH_0.dev.hw)) * 8192; // calculate bootloader size to know where we should write the application firmware
	ASSERT(application_start_address > 0);

	while (true) { // main DFU infinite loop
		// run the second part of the USB DFU state machine handling non-USB aspects
		if (USB_DFU_STATE_DFU_DNLOAD_SYNC == dfu_state || USB_DFU_STATE_DFU_DNBUSY == dfu_state) { // there is some data to be flashed
			LED_SYSTEM_off(); // switch LED off to indicate we are flashing
			if (dfu_download_length > 0) { // there is some data to be flashed
				int32_t rc = flash_write(&FLASH_0, application_start_address + dfu_download_offset, dfu_download_data, dfu_download_length); // write downloaded data chunk to flash
				if (ERR_NONE == rc) {
					dfu_state = USB_DFU_STATE_DFU_DNLOAD_IDLE; // indicate flashing this block has been completed
				} else { // there has been a programming error
					dfu_state = USB_DFU_STATE_DFU_ERROR;
					if (ERR_BAD_ADDRESS == rc) {
						dfu_status = USB_DFU_STATUS_ERR_ADDRESS;
					} else if (ERR_DENIED == rc) {
						dfu_status = USB_DFU_STATUS_ERR_WRITE;
					} else {
						dfu_status = USB_DFU_STATUS_ERR_PROG;
					}
				}
			} else { // there was no data to flash
				// this case should not happen, but it's not a critical error
				dfu_state = USB_DFU_STATE_DFU_DNLOAD_IDLE; // indicate flashing can continue
			}
			LED_SYSTEM_on(); // switch LED on to indicate USB DFU can resume
		}
		if (USB_DFU_STATE_DFU_MANIFEST == dfu_state) { // we can start manifestation (finish flashing)
			// in theory every DFU files should have a suffix to with a CRC to check the data
			// in practice most downloaded files are just the raw binary with DFU suffix
			dfu_manifestation_complete = true; // we completed flashing and all checks
			if (usb_dfu_func_desc->bmAttributes & USB_DFU_ATTRIBUTES_MANIFEST_TOLERANT) {
				dfu_state = USB_DFU_STATE_DFU_MANIFEST_SYNC;
			} else {
				dfu_state = USB_DFU_STATE_DFU_MANIFEST_WAIT_RESET;
			}
		}
		if (USB_DFU_STATE_DFU_MANIFEST_WAIT_RESET == dfu_state) {
			if (usb_dfu_func_desc->bmAttributes & USB_DFU_ATTRIBUTES_WILL_DETACH) {
				usb_dfu_reset(USB_EV_RESET, 0); // immediately reset
			} else { // wait for USB reset
				usb_d_register_callback(USB_D_CB_EVENT, (FUNC_PTR)usb_dfu_reset); // register new USB reset event handler
			}
		}
	}
}

void usb_init(void)
{
	usb_dfu_init();
}
