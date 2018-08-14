/**
  ******************************************************************************
  * @file    usbd_vendor.c
  * @author  MCD Application Team
  * @version V2.4.2
  * @date    11-December-2015
  * @brief   This file provides the HID core functions.
  *
  * @verbatim
  *      
  *          ===================================================================      
  *                                VENDOR Class  Description
  *          ===================================================================
  *          
  *
  *
  *
  *           
  *      
  * @note     In HS mode and when the DMA is used, all variables and data structures
  *           dealing with the DMA during the transaction process should be 32-bit aligned.
  *           
  *      
  *  @endverbatim
  *
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

/* Includes ------------------------------------------------------------------*/
#include "usbd_desc.h"
#include "usbd_ctlreq.h"
#include "usbd_custom.h"


/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */


/** @defgroup USBD_VENDOR
  * @brief usbd core module
  * @{
  */ 

/** @defgroup USBD_VENDOR_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USBD_VENDOR_Private_Defines
  * @{
  */ 

/**
  * @}
  */ 


/** @defgroup USBD_VENDOR_Private_Macros
  * @{
  */ 
                                         
/**
  * @}
  */ 




/** @defgroup USBD_VENDOR_Private_FunctionPrototypes
  * @{
  */


static uint8_t  USBD_VENDOR_Init (USBD_HandleTypeDef *pdev, 
                               uint8_t cfgidx);

static uint8_t  USBD_VENDOR_DeInit (USBD_HandleTypeDef *pdev, 
                                 uint8_t cfgidx);

static uint8_t  USBD_VENDOR_Setup (USBD_HandleTypeDef *pdev, 
                                USBD_SetupReqTypedef *req);

static uint8_t  *USBD_VENDOR_GetCfgDesc (uint16_t *length);

static uint8_t  *USBD_VENDOR_GetDeviceQualifierDesc (uint16_t *length);

static uint8_t  USBD_VENDOR_DataIn (USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t  USBD_VENDOR_DataOut (USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t  USBD_VENDOR_EP0_RxReady (USBD_HandleTypeDef *pdev);

static uint8_t  USBD_VENDOR_EP0_TxReady (USBD_HandleTypeDef *pdev);

static uint8_t  USBD_VENDOR_SOF (USBD_HandleTypeDef *pdev);

static uint8_t  USBD_VENDOR_IsoINIncomplete (USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t  USBD_VENDOR_IsoOutIncomplete (USBD_HandleTypeDef *pdev, uint8_t epnum);

/**
  * @}
  */ 

/** @defgroup USBD_VENDOR_Private_Variables
  * @{
  */ 

USBD_ClassTypeDef  USBD_CLASS_VENDOR = 
{
  USBD_VENDOR_Init,
  USBD_VENDOR_DeInit,
  USBD_VENDOR_Setup,
  USBD_VENDOR_EP0_TxReady,  
  USBD_VENDOR_EP0_RxReady,
  USBD_VENDOR_DataIn,
  USBD_VENDOR_DataOut,
  USBD_VENDOR_SOF,
  USBD_VENDOR_IsoINIncomplete,
  USBD_VENDOR_IsoOutIncomplete,
  USBD_VENDOR_GetCfgDesc,
  USBD_VENDOR_GetCfgDesc, 
  USBD_VENDOR_GetCfgDesc,
  USBD_VENDOR_GetDeviceQualifierDesc,
};

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=4   
#endif
/* USB VENDOR device Configuration Descriptor */
static uint8_t USBD_VENDOR_CfgDesc[USB_VENDOR_CONFIG_DESC_SIZ] =
{
  USB_LEN_CFG_DESC, /* bLength: Configuration Descriptor size */
  USB_DESC_TYPE_CONFIGURATION, /* bDescriptorType: Configuration */
  USB_VENDOR_CONFIG_DESC_SIZ,
  /* wTotalLength: Bytes returned */
  0x00,
  0x01,         /*bNumInterfaces: 1 interface*/
  0x01,         /*bConfigurationValue: Configuration value*/
  0x02,         /*iConfiguration: Index of string descriptor describing the configuration*/
  0xC0,         /*bmAttributes: bus powered and Supports Remote Wakeup */
  0x32,         /*MaxPower 100 mA: this current is used for detecting Vbus*/
  /* 09 */

#if USBD_VENDOR_EXT_DESC_SIZ
  USBD_VENDOR_EXT_DESC
#endif

#if USBD_VENDOR_FUNC_DESC_SIZ
  USBD_VENDOR_FUNC_DESC
#endif
  /***********************************************************/
};
  
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
  #pragma data_alignment=4   
#endif
/* USB Standard Device Descriptor */
__ALIGN_BEGIN static uint8_t USBD_VENDOR_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END =
{
  USB_LEN_DEV_QUALIFIER_DESC,
  USB_DESC_TYPE_DEVICE_QUALIFIER,
  0x00,
  0x02,
  0x00,
  0x00,
  0x00,
  0x40,
  0x01,
  0x00,
};

/**
  * @}
  */ 

/** @defgroup USBD_VENDOR_Private_Functions
  * @{
  */ 

/**
  * @brief  USBD_VENDOR_Init
  *         Initialize the VENDOR interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_VENDOR_Init (USBD_HandleTypeDef *pdev, 
                               uint8_t cfgidx)
{
	uint8_t ret = 0;
	USBD_VENDOR_HandleTypeDef *hVendor;

	if (pdev->dev_speed == USBD_SPEED_HIGH) {
		/* Open EP IN */
		USBD_LL_OpenEP(pdev, VENDOR_IN_EP,
		USBD_EP_TYPE_BULK, VENDOR_DATA_HS_IN_PACKET_SIZE);

		/* Open EP OUT */
		USBD_LL_OpenEP(pdev, VENDOR_OUT_EP,
		USBD_EP_TYPE_BULK, VENDOR_DATA_HS_OUT_PACKET_SIZE);

	} else {
		/* Open EP IN */
		USBD_LL_OpenEP(pdev, VENDOR_IN_EP,
		USBD_EP_TYPE_BULK, VENDOR_DATA_FS_IN_PACKET_SIZE);

		/* Open EP OUT */
		USBD_LL_OpenEP(pdev, VENDOR_OUT_EP,
		USBD_EP_TYPE_BULK, VENDOR_DATA_FS_OUT_PACKET_SIZE);
	}

	pdev->pClassData = USBD_malloc(sizeof(USBD_VENDOR_HandleTypeDef));

	if (pdev->pClassData == NULL) {
		ret = 1;
	} else {
		hVendor = (USBD_VENDOR_HandleTypeDef*) pdev->pClassData;

		/* Init  physical Interface components */
		//((USBD_VENDOR_ItfTypeDef *) pdev->pUserData)->Init();

		/* Init Xfer states */
		hVendor->TxState = 0;
		hVendor->RxState = 0;

		if (pdev->dev_speed == USBD_SPEED_HIGH) {
			/* Prepare Out endpoint to receive next packet */
			USBD_LL_PrepareReceive(pdev, VENDOR_OUT_EP, hVendor->RxBuffer,
			VENDOR_DATA_HS_OUT_PACKET_SIZE);
		} else {
			/* Prepare Out endpoint to receive next packet */
			USBD_LL_PrepareReceive(pdev, VENDOR_OUT_EP, hVendor->RxBuffer,
			VENDOR_DATA_FS_OUT_PACKET_SIZE);
		}
	}

	return ret;
}

/**
  * @brief  USBD_VENDOR_Init
  *         DeInitialize the VENDOR layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_VENDOR_DeInit (USBD_HandleTypeDef *pdev, 
                                 uint8_t cfgidx)
{

	return USBD_OK;
}

/**
  * @brief  USBD_VENDOR_Setup
  *         Handle the VENDOR specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
static uint8_t  USBD_VENDOR_Setup (USBD_HandleTypeDef *pdev, 
                                USBD_SetupReqTypedef *req)
{
	uint8_t *pbuf = 0;
	uint16_t len = 0;
	uint8_t ret = USBD_OK;

	//USBD_VENDOR_HandleTypeDef *hdfu = (USBD_VENDOR_HandleTypeDef*) pdev->pClassData;
	static uint8_t ifalt = 0;

	switch (req->bmRequest & USB_REQ_TYPE_MASK) {
	case USB_REQ_TYPE_CLASS:
		switch (req->bRequest) {

		default:
			USBD_CtlError(pdev, req);
			return USBD_FAIL;
		}
		break;

	case USB_REQ_TYPE_STANDARD:
		switch (req->bRequest) {
		case USB_REQ_GET_DESCRIPTOR:
			if ((req->wValue >> 8) == VENDOR_DESCRIPTOR_TYPE) {
				pbuf = USBD_VENDOR_CfgDesc
						+ (9 * (USBD_VENDOR_MAX_ITF_NUM + 1));
				len = MIN(USB_VENDOR_DESC_SIZ, req->wLength);
			}

			USBD_CtlSendData(pdev, pbuf, len);
			break;

		case USB_REQ_GET_INTERFACE:
			//USBD_CtlSendData(pdev, (uint8_t *) &hdfu->alt_setting, 1);
			USBD_CtlSendData (pdev, &ifalt, 1);
			break;

		case USB_REQ_SET_INTERFACE:
			if ((uint8_t) (req->wValue) < USBD_VENDOR_MAX_ITF_NUM) {
				//hdfu->alt_setting = (uint8_t) (req->wValue);
			} else {
				/* Call the error management function (command will be nacked */
				USBD_CtlError(pdev, req);
				ret = USBD_FAIL;
			}
			break;

		default:
			USBD_CtlError(pdev, req);
			return USBD_FAIL;
		}
	}
	return ret;
}


/**
  * @brief  USBD_VENDOR_GetCfgDesc
  *         return configuration descriptor
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t  *USBD_VENDOR_GetCfgDesc (uint16_t *length)
{
  *length = sizeof (USBD_VENDOR_CfgDesc);
  return USBD_VENDOR_CfgDesc;
}

/**
* @brief  DeviceQualifierDescriptor 
*         return Device Qualifier descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
uint8_t  *USBD_VENDOR_DeviceQualifierDescriptor (uint16_t *length)
{
  *length = sizeof (USBD_VENDOR_DeviceQualifierDesc);
  return USBD_VENDOR_DeviceQualifierDesc;
}


/**
  * @brief  USBD_VENDOR_DataIn
  *         handle data IN Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_VENDOR_DataIn (USBD_HandleTypeDef *pdev, 
                              uint8_t epnum)
{

  return USBD_OK;
}

/**
  * @brief  USBD_VENDOR_EP0_RxReady
  *         handle EP0 Rx Ready event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t  USBD_VENDOR_EP0_RxReady (USBD_HandleTypeDef *pdev)
{

  return USBD_OK;
}
/**
  * @brief  USBD_VENDOR_EP0_TxReady
  *         handle EP0 TRx Ready event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t  USBD_VENDOR_EP0_TxReady (USBD_HandleTypeDef *pdev)
{

  return USBD_OK;
}
/**
  * @brief  USBD_VENDOR_SOF
  *         handle SOF event
  * @param  pdev: device instance
  * @retval status
  */
static uint8_t  USBD_VENDOR_SOF (USBD_HandleTypeDef *pdev)
{

  return USBD_OK;
}
/**
  * @brief  USBD_VENDOR_IsoINIncomplete
  *         handle data ISO IN Incomplete event
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_VENDOR_IsoINIncomplete (USBD_HandleTypeDef *pdev, uint8_t epnum)
{

  return USBD_OK;
}
/**
  * @brief  USBD_VENDOR_IsoOutIncomplete
  *         handle data ISO OUT Incomplete event
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_VENDOR_IsoOutIncomplete (USBD_HandleTypeDef *pdev, uint8_t epnum)
{

  return USBD_OK;
}
/**
  * @brief  USBD_VENDOR_DataOut
  *         handle data OUT Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_VENDOR_DataOut (USBD_HandleTypeDef *pdev, 
                              uint8_t epnum)
{
	if (pdev->pClassData != NULL) {
		USBD_VENDOR_HandleTypeDef *hVendor =
				(USBD_VENDOR_HandleTypeDef*) pdev->pClassData;

		/* Get the received data length */
		hVendor->RxLength = USBD_LL_GetRxDataSize(pdev, epnum);

		/* USB data will be immediately processed, this allow next USB traffic being
		 NAKed till the end of the application Xfer */
		USBD_LL_PrepareReceive(pdev, VENDOR_OUT_EP, hVendor->RxBuffer,
				VENDOR_DATA_FS_OUT_PACKET_SIZE);

		return USBD_OK;
	} else {
		return USBD_FAIL;
	}
}

/**
* @brief  DeviceQualifierDescriptor 
*         return Device Qualifier descriptor
* @param  length : pointer data length
* @retval pointer to descriptor buffer
*/
uint8_t  *USBD_VENDOR_GetDeviceQualifierDesc (uint16_t *length)
{
  *length = sizeof (USBD_VENDOR_DeviceQualifierDesc);
  return USBD_VENDOR_DeviceQualifierDesc;
}

/**
  * @}
  */ 


/**
  * @}
  */ 


/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
