/**
  ******************************************************************************
  * @file    usbd_template_core.h
  * @author  MCD Application Team
  * @version V2.4.2
  * @date    11-December-2015
  * @brief   Header file for the usbd_template_core.c file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/ 
#ifndef __USB_VENDOR_CORE_H
#define __USB_VENDOR_CORE_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include  "usbd_ioreq.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */
  
/** @defgroup USBD_TEMPLATE
  * @brief This file is the header file for usbd_template_core.c
  * @{
  */ 


/** @defgroup USBD_TEMPLATE_Exported_Defines
  * @{
  */

#define VENDOR_IN_EP                                0x81  /* EP1 for data IN */
#define VENDOR_OUT_EP                               0x01  /* EP1 for data OUT */
//#define VENDOR_CMD_EP                               0x82  /* EP2 for CDC commands */

/* VENDOR Endpoints parameters: you can fine tune these values depending on the needed baudrates and performance. */

#define VENDOR_DATA_HS_MAX_PACKET_SIZE              512  /* Endpoint IN & OUT Packet size */
#define VENDOR_DATA_FS_MAX_PACKET_SIZE              64  /* Endpoint IN & OUT Packet size */
//#define VENDOR_CMD_PACKET_SIZE                      8  /* Control Endpoint Packet size */

//#define VENDOR_CONFIG_DESC_SIZ                  67
#define VENDOR_DATA_HS_IN_PACKET_SIZE               VENDOR_DATA_HS_MAX_PACKET_SIZE
#define VENDOR_DATA_HS_OUT_PACKET_SIZE              VENDOR_DATA_HS_MAX_PACKET_SIZE

#define VENDOR_DATA_FS_IN_PACKET_SIZE               VENDOR_DATA_FS_MAX_PACKET_SIZE
#define VENDOR_DATA_FS_OUT_PACKET_SIZE              VENDOR_DATA_FS_MAX_PACKET_SIZE

/************************************************************************/

#ifndef USBD_VENDOR_EXT_DESC_SIZ
#define	USBD_VENDOR_EXT_DESC_SIZ        	0
#endif

#ifndef USBD_VENDOR_FUNC_DESC_SIZ
#define	USBD_VENDOR_FUNC_DESC_SIZ        	0
#endif

#define USB_VENDOR_CONFIG_DESC_SIZ       	(9 + USBD_VENDOR_EXT_DESC_SIZ + USBD_VENDOR_FUNC_DESC_SIZ)
#define USB_VENDOR_DESC_SIZ                	9
#define VENDOR_DESCRIPTOR_TYPE            	0x21

/**
  * @}
  */ 

#define TRANSFER_SIZE_BYTES(size)      ((uint8_t)(size)), /* XFERSIZEB0 */\
                                       ((uint8_t)(size >> 8)) /* XFERSIZEB1 */


/** @defgroup USBD_CORE_Exported_TypesDefinitions
  * @{
  */
typedef struct {
	uint32_t data[VENDOR_DATA_HS_MAX_PACKET_SIZE / 4]; /* Force 32bits alignment */
	uint8_t CmdOpCode;
	uint8_t CmdLength;
	uint8_t *RxBuffer;
	uint8_t *TxBuffer;
	uint32_t RxLength;
	uint32_t TxLength;

	__IO uint32_t TxState;
	__IO uint32_t RxState;
} USBD_VENDOR_HandleTypeDef;

/**
 * \brief Standard USB configuration descriptor structure
 */
typedef struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint16_t wTotalLength;
	uint8_t bNumInterfaces;
	uint8_t bConfigurationValue;
	uint8_t iConfiguration;
	uint8_t bmAttributes;
	uint8_t bMaxPower;
} usb_conf_desc_t;

typedef struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bInterfaceNumber;
	uint8_t bAlternateSetting;
	uint8_t bNumEndpoints;
	uint8_t bInterfaceClass;
	uint8_t bInterfaceSubClass;
	uint8_t bInterfaceProtocol;
	uint8_t iInterface;
} usb_iface_desc_t;

typedef struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint8_t bEndpointAddress;
	uint8_t bmAttributes;
	uint16_t wMaxPacketSize;
	uint8_t bInterval;
} usb_ep_desc_t;

//! Interface descriptor structure for vendor Class interface
typedef struct {
	usb_iface_desc_t iface0;
#if USBD_VENDOR_MAX_ITF_NUM>1
	usb_iface_desc_t iface1;
#endif
#if UDI_VENDOR_EPS_SIZE_INT_FS
	usb_ep_desc_t ep_interrupt_in;
	usb_ep_desc_t ep_interrupt_out;
#endif
#if UDI_VENDOR_EPS_SIZE_BULK_FS
	usb_ep_desc_t ep_bulk_in;
	usb_ep_desc_t ep_bulk_out;
#endif
#if UDI_VENDOR_EPS_SIZE_ISO_FS
	usb_ep_desc_t ep_iso_in;
	usb_ep_desc_t ep_iso_out;
#endif
} udi_vendor_desc_t;

/**
  * @}
  */ 



/** @defgroup USBD_CORE_Exported_Macros
  * @{
  */ 

/**
  * @}
  */ 

/** @defgroup USBD_CORE_Exported_Variables
  * @{
  */ 
extern USBD_ClassTypeDef  USBD_CLASS_VENDOR;

/**
  * @}
  */ 

/** @defgroup USB_CORE_Exported_Functions
  * @{
  */ 
/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif

#endif  /* __USB_TEMPLATE_CORE_H */
/**
  * @}
  */ 

/**
  * @}
  */ 
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
