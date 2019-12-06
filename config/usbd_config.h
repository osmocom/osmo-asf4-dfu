/* Auto-generated config file usbd_config.h */
#ifndef USBD_CONFIG_H
#define USBD_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

// ---- USB Device Stack Core Options ----

// <q> High Speed Support
// <i> Enable high speed specific descriptors support, e.g., DeviceQualifierDescriptor and OtherSpeedConfiguration Descriptor.
// <i> High speed support require descriptors description array on start, for LS/FS and HS support in first and second place.
// <id> usbd_hs_sp
#ifndef CONF_USBD_HS_SP
#define CONF_USBD_HS_SP 0
#endif

// ---- USB Device Stack DFU Options ----

// <e> Enable String Descriptors
// <ID> USB_DFUD_STR_EN
#ifndef CONF_USB_DFUD_STR_EN
#define CONF_USB_DFUD_STR_EN 0
#endif
// <s> Language IDs
// <i> Language IDs in c format, split by comma (E.g., 0x0409 ...)
// <id> usb_dfud_langid
#ifndef CONF_USB_DFUD_LANGID
#define CONF_USB_DFUD_LANGID "0x0409"
#endif

#ifndef CONF_USB_DFUD_LANGID_DESC
#define CONF_USB_DFUD_LANGID_DESC 4, 0x03, 0x09, 0x04,
#endif
// </e>

// <h> DFU Device Descriptor

// <o> bcdUSB
// <0x0200=> USB 2.0 version
// <0x0210=> USB 2.1 version
// <id> usb_dfud_bcdusb
#ifndef CONF_USB_DFUD_BCDUSB
#define CONF_USB_DFUD_BCDUSB 0x200
#endif

// <o> bDeviceClass
// <0=> unused
// <id> usb_dfud_bdeviceclass
#ifndef CONF_USB_DFUD_BDEVICECLASS
#define CONF_USB_DFUD_BDEVICECLASS 0
#endif

// <o> bDeviceSubClass
// <0=> unused
// <id> usb_dfud_bdevicesubclass
#ifndef CONF_USB_DFUD_BDEVICESUBCLASS
#define CONF_USB_DFUD_BDEVICESUBCLASS 0
#endif

// <o> bDeviceProtocol
// <0=> unused
// <id> usb_dfud_bdeviceprotocol
#ifndef CONF_USB_DFUD_BDEVICEPROTOCOL
#define CONF_USB_DFUD_BDEVICEPROTOCOL 0
#endif

// <o> bMaxPackeSize0
// <0x0008=> 8 bytes
// <0x0010=> 16 bytes
// <0x0020=> 32 bytes
// <0x0040=> 64 bytes
// <id> usb_dfud_bmaxpksz0
#ifndef CONF_USB_DFUD_BMAXPKSZ0
#define CONF_USB_DFUD_BMAXPKSZ0 0x40
#endif

// <o> idVendor <0x0000-0xFFFF>
// <0x1d50=> OpenMoko
// <id> usb_dfud_idvendor
#ifndef CONF_USB_OPENMOKO_IDVENDOR
#define CONF_USB_OPENMOKO_IDVENDOR 0x1d50
#endif

// <o> idProduct <0x0000-0xFFFF>
// <0x6140=> osmo-ASF4-DFU
// <id> usb_dfud_idproduct
#ifndef CONF_USB_OSMOASF4DFU_IDPRODUCT
#if defined(SYSMOOCTSIM)
#define CONF_USB_OSMOASF4DFU_IDPRODUCT 0x6141
#else
#define CONF_USB_OSMOASF4DFU_IDPRODUCT 0x6140
#endif
#endif

// <o> bcdDevice <0x0000-0xFFFF>
// <id> usb_dfud_bcddevice
#ifndef CONF_USB_DFUD_BCDDEVICE
#define CONF_USB_DFUD_BCDDEVICE 0x000
#endif

// <e> Enable string descriptor of iManufact
// <id> usb_dfud_imanufact_en
#ifndef CONF_USB_DFUD_IMANUFACT_EN
#define CONF_USB_DFUD_IMANUFACT_EN 1
#endif

#ifndef CONF_USB_DFUD_IMANUFACT
#define CONF_USB_DFUD_IMANUFACT (CONF_USB_DFUD_IMANUFACT_EN * (CONF_USB_DFUD_IMANUFACT_EN))
#endif

// <s> Unicode string of iManufact
// <id> usb_dfud_imanufact_str
#ifndef CONF_USB_DFUD_IMANUFACT_STR
#if defined(SYSMOOCTSIM)
#define CONF_USB_DFUD_IMANUFACT_STR "sysmocom - s.f.m.c. GmbH"
#else
#define CONF_USB_DFUD_IMANUFACT_STR "Osmocom"
#endif
#endif

#ifndef CONF_USB_DFUD_IMANUFACT_STR_DESC
#if defined(SYSMOOCTSIM)
#define CONF_USB_DFUD_IMANUFACT_STR_DESC 50, 0x03, 's',0, 'y',0, 's',0, 'm',0, 'o',0, 'c',0, 'o',0, 'm',0, ' ',0, '-',0, ' ',0, \
						   's',0, '.',0, 'f',0, '.',0, 'm',0, '.',0, 'c',0, '.',0, ' ',0, 'G',0, 'm',0, 'b',0, 'H',0,
#else
#define CONF_USB_DFUD_IMANUFACT_STR_DESC 16, 0x03, 'O', 0x00, 's', 0x00, 'm', 0x00, 'o', 0x00, 'c', 0x00, 'o', 0x00, 'm', 0x00,
#endif
#endif

// </e>

// <e> Enable string descriptor of iProduct
// <id> usb_dfud_iproduct_en
#ifndef CONF_USB_DFUD_IPRODUCT_EN
#define CONF_USB_DFUD_IPRODUCT_EN 1
#endif

#ifndef CONF_USB_DFUD_IPRODUCT
#define CONF_USB_DFUD_IPRODUCT                                                                                     \
	(CONF_USB_DFUD_IPRODUCT_EN * (CONF_USB_DFUD_IMANUFACT_EN + CONF_USB_DFUD_IPRODUCT_EN))
#endif

// <s> Unicode string of iProduct
// <id> usb_dfud_iproduct_str
#ifndef CONF_USB_DFUD_IPRODUCT_STR
#if defined(SYSMOOCTSIM)
#define CONF_USB_DFUD_IPRODUCT_STR "sysmoOCTSIM (osmo-ASF4-DFU)"
#else
#define CONF_USB_DFUD_IPRODUCT_STR "osmo-ASF4-DFU"
#endif
#endif

#ifndef CONF_USB_DFUD_IPRODUCT_STR_DESC
#if defined(SYSMOOCTSIM)
#define CONF_USB_DFUD_IPRODUCT_STR_DESC 56, 0x03, 's', 0x00, 'y', 0x00, 's', 0x00, 'm', 0x00, 'o', 0x00, 'O', 0x00, 'C', 0x00, 'T', 0x00, 'S', 0x00, 'I', 0x00, 'M', 0x00, ' ', 0x00, '(', 0x00, 'o', 0x00, 's', 0x00, 'm', 0x00, 'o', 0x00, '-', 0x00, 'A', 0x00, 'S', 0x00, 'F', 0x00, '4', 0x00, '-', 0x00, 'D', 0x00, 'F', 0x00, 'U', 0x00, ')', 0x00,
#else
#define CONF_USB_DFUD_IPRODUCT_STR_DESC 28, 0x03, 'o', 0x00, 's', 0x00, 'm', 0x00, 'o', 0x00, '-', 0x00, 'A', 0x00, 'S', 0x00, 'F', 0x00, '4', 0x00, '-', 0x00, 'D', 0x00, 'F', 0x00, 'U', 0x00,
#endif
#endif

// </e>

// <e> Enable string descriptor of iSerialNum
// <id> usb_dfud_iserialnum_en
#ifndef CONF_USB_DFUD_ISERIALNUM_EN
#define CONF_USB_DFUD_ISERIALNUM_EN 0
#endif

#ifndef CONF_USB_DFUD_ISERIALNUM
#define CONF_USB_DFUD_ISERIALNUM                                                                                   \
	(CONF_USB_DFUD_ISERIALNUM_EN                                                                                   \
	 * (CONF_USB_DFUD_IMANUFACT_EN + CONF_USB_DFUD_IPRODUCT_EN + CONF_USB_DFUD_ISERIALNUM_EN))
#endif

// <s> Unicode string of iSerialNum
// <id> usb_dfud_iserialnum_str
#ifndef CONF_USB_DFUD_ISERIALNUM_STR
#define CONF_USB_DFUD_ISERIALNUM_STR "123456789ABCDEF"
#endif

#ifndef CONF_USB_DFUD_ISERIALNUM_STR_DESC
#define CONF_USB_DFUD_ISERIALNUM_STR_DESC 
#endif

// </e>

// <o> bNumConfigurations <0x01-0xFF>
// <id> usb_dfud_bnumconfig
#ifndef CONF_USB_DFUD_BNUMCONFIG
#define CONF_USB_DFUD_BNUMCONFIG 0x1
#endif

// </h>

// <h> DFU Configuration Descriptor

// <o> wTotalLength <0x01-0xFF>
// <id> usb_dfud_wtotallength
#ifndef CONF_USB_DFUD_WTOTALLENGTH
#define CONF_USB_DFUD_WTOTALLENGTH 27
#endif

// <o> bNumInterfaces <0x01-0xFF>
// <id> usb_dfud_bnuminterfaces
#ifndef CONF_USB_DFUD_BNUMINTERFACES
#define CONF_USB_DFUD_BNUMINTERFACES 1
#endif

// <o> bConfigurationValue <0x01-0xFF>
// <id> usb_dfud_bconfigval
#ifndef CONF_USB_DFUD_BCONFIGVAL
#define CONF_USB_DFUD_BCONFIGVAL 0x1
#endif

// <e> Enable string descriptor of iConfig
// <id> usb_dfud_iconfig_en
#ifndef CONF_USB_DFUD_ICONFIG_EN
#define CONF_USB_DFUD_ICONFIG_EN 0
#endif

#ifndef CONF_USB_DFUD_ICONFIG
#define CONF_USB_DFUD_ICONFIG                                                                                      \
	(CONF_USB_DFUD_ICONFIG_EN                                                                                      \
	 * (CONF_USB_DFUD_IMANUFACT_EN + CONF_USB_DFUD_IPRODUCT_EN + CONF_USB_DFUD_ISERIALNUM_EN               \
	    + CONF_USB_DFUD_ICONFIG_EN))
#endif

// <s> Unicode string of iConfig
// <id> usb_dfud_iconfig_str
#ifndef CONF_USB_DFUD_ICONFIG_STR
#define CONF_USB_DFUD_ICONFIG_STR ""
#endif

#ifndef CONF_USB_DFUD_ICONFIG_STR_DESC
#define CONF_USB_DFUD_ICONFIG_STR_DESC
#endif

// </e>

// <o> bmAttributes
// <0x80=> Bus power supply, not support for remote wakeup
// <0xA0=> Bus power supply, support for remote wakeup
// <0xC0=> Self powered, not support for remote wakeup
// <0xE0=> Self powered, support for remote wakeup
// <id> usb_dfud_bmattri
#ifndef CONF_USB_DFUD_BMATTRI
#define CONF_USB_DFUD_BMATTRI 0x80
#endif

// <o> bMaxPower <0x00-0xFF>
// <id> usb_dfud_bmaxpower
#ifndef CONF_USB_DFUD_BMAXPOWER
#define CONF_USB_DFUD_BMAXPOWER 0x32
#endif
// </h>

// <h> DFU Interface Descriptor

// <o> bInterfaceNumber <0x00-0xFF>
// <id> usb_dfud_bifcnum
#ifndef CONF_USB_DFUD_BIFCNUM
#define CONF_USB_DFUD_BIFCNUM 0x0
#endif

// <o> bAlternateSetting <0x00-0xFF>
// <id> usb_dfud_baltset
#ifndef CONF_USB_DFUD_BALTSET
#define CONF_USB_DFUD_BALTSET 0x0
#endif

// <o> bNumEndpoints <0x00-0xFF>
// <id> usb_dfud_bnumep
#ifndef CONF_USB_DFUD_BNUMEP
#define CONF_USB_DFUD_BNUMEP 0x0
#endif

// <e> Enable string descriptor of iInterface
// <id> usb_dfud_iinterface_en
#ifndef CONF_USB_DFUD_IINTERFACE_EN
#define CONF_USB_DFUD_IINTERFACE_EN 1
#endif

#ifndef CONF_USB_DFUD_IINTERFACE
#define CONF_USB_DFUD_IINTERFACE                                                                                      \
	(CONF_USB_DFUD_IINTERFACE_EN                                                                                      \
	 * (CONF_USB_DFUD_IMANUFACT_EN + CONF_USB_DFUD_IPRODUCT_EN + CONF_USB_DFUD_ISERIALNUM_EN               \
	    + CONF_USB_DFUD_ICONFIG_EN + CONF_USB_DFUD_IINTERFACE_EN))
#endif

// <s> Unicode string of iInterface
// <id> usb_dfud_iinterface_str
#ifndef CONF_USB_DFUD_IINTERFAGE_STR
#define CONF_USB_DFUD_IINTERFACE_STR "SAM D5x/E5x DFU bootloader (DFU mode)"
#endif

#ifndef CONF_USB_DFUD_IINTERFACE_STR_DESC
#define CONF_USB_DFUD_IINTERFACE_STR_DESC 76, 0x03, 'S', 0x00, 'A', 0x00, 'M', 0x00, ' ', 0x00, 'D', 0x00, '5', 0x00, 'x', 0x00, '/', 0x00, 'E', 0x00, '5',  0x00, 'x', 0x00, ' ', 0x00, 'D', 0x00, 'F', 0x00, 'U', 0x00, ' ', 0x00, 'b', 0x00, 'o', 0x00, 'o', 0x00, 't', 0x00, 'l', 0x00, 'o', 0x00, 'a', 0x00, 'd', 0x00, 'e', 0x00, 'r', 0x00, ' ', 0x00, '(', 0x00, 'D', 0x00, 'F', 0x00, 'U', 0x00, ' ', 0x00, 'm', 0x00, 'o', 0x00, 'd', 0x00, 'e', 0x00, ')', 0x00,
#endif

// </e>
// </h>

// <<< end of configuration section >>>

#endif // USBD_CONFIG_H
