/**
  ******************************************************************************
  * @file    usbd_conf.h
  * @author  MCD Application Team
  * @version V2.4.2
  * @date    11-December-2015
  * @brief   Header file for the usbd_conf_template.c file
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
#ifndef __USBD_CONF_H
#define __USBD_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */
  
/** @defgroup USBD_CONF
  * @brief USB device low level driver configuration file
  * @{
  */ 

/** @defgroup USBD_CONF_Exported_Defines
  * @{
  */ 

#define USB_VENDOR_CLASS                  0xFF
#define USB_VENDOR_SUBCLASS               0xFF
#define USB_VENDOR_PROTOCOL               0xFF

#define USBD_MAX_NUM_INTERFACES               1
#define USBD_MAX_NUM_CONFIGURATION            1
#define USBD_MAX_STR_DESC_SIZ                 0x100
#define USBD_SUPPORT_USER_STRING              0 
#define USBD_SELF_POWERED                     1
#define USBD_DEBUG_LEVEL                      2

#define USBD_VENDOR_MAX_ITF_NUM      		1
#define USBD_VENDOR_XFER_SIZE           	1024

#define VENDOR_EP_BULK_IN	0x81
#define VENDOR_EP_BULK_OUT	0x02

#define FS_MAX_PACKET_SIZE                 64  /* Endpoint IN & OUT Packet size */

/****************************************/
/* #define for FS and HS identification */
#define DEVICE_FS 		0

/**
 * @}
 */

/* MSC Class Config */
#define MSC_MEDIA_PACKET                       8192   

/* CDC Class Config */
#define USBD_CDC_INTERVAL                      2000  

 /* DFU Class Config */
#define USBD_DFU_MAX_ITF_NUM                   1
#define USBD_DFU_XFERS_IZE                     1024

 /* AUDIO Class Config */
#define USBD_AUDIO_FREQ                       22100 

/** @defgroup USBD_Exported_Macros
  * @{
  */ 

 /* Memory management macros */   
 /* Memory management macros */

 /** Alias for memory allocation. */
 #define USBD_malloc         (uint32_t *)USBD_static_malloc

 /** Alias for memory release. */
 #define USBD_free           USBD_static_free

 /** Alias for memory set. */
 #define USBD_memset         /* Not used */

 /** Alias for memory copy. */
 #define USBD_memcpy         /* Not used */

 /** Alias for delay. */
 #define USBD_Delay          HAL_Delay

 /* For footprint reasons and since only one allocation is handled in the HID class
    driver, the malloc/free is changed into a static allocation method */
 void *USBD_static_malloc(uint32_t size);
 void USBD_static_free(void *p);

 /* DEBUG macros */  

  
#if (USBD_DEBUG_LEVEL > 0)
#define  USBD_UsrLog(...)   printf(__VA_ARGS__);\
                            printf("\n");
#else
#define USBD_UsrLog(...)   
#endif 
                            
                            
#if (USBD_DEBUG_LEVEL > 1)

#define  USBD_ErrLog(...)   printf("ERROR: ") ;\
                            printf(__VA_ARGS__);\
                            printf("\n");
#else
#define USBD_ErrLog(...)   
#endif 
                            
                            
#if (USBD_DEBUG_LEVEL > 2)                         
#define  USBD_DbgLog(...)   printf("DEBUG : ") ;\
                            printf(__VA_ARGS__);\
                            printf("\n");
#else
#define USBD_DbgLog(...)                         
#endif
                            
/**
  * @}
  */ 
 
    
    
/**
  * @}
  */ 


/** @defgroup USBD_CONF_Exported_Types
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USBD_CONF_Exported_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USBD_CONF_Exported_Variables
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup USBD_CONF_Exported_FunctionsPrototype
  * @{
  */ 
/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif

#endif /* __USBD_CONF_TEMPLATE_H */


/**
  * @}
  */ 

/**
  * @}
  */ 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

