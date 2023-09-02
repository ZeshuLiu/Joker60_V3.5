/**
  ******************************************************************************
  * @file           : usbd_custom_hid_if.c
  * @version        : v1.0_Cube
  * @brief          : USB Device Custom HID interface file.
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbd_hid_custom_if.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint8_t buffer[0x40];
/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device.
  * @{
  */

/** @addtogroup USBD_CUSTOM_HID
  * @{
  */

/** @defgroup USBD_CUSTOM_HID_Private_TypesDefinitions USBD_CUSTOM_HID_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Defines USBD_CUSTOM_HID_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */
#define CapsLockMask 0x02
#define NumLockMask 0x01
#define ScrolLockMask 0x03
/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Macros USBD_CUSTOM_HID_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Variables USBD_CUSTOM_HID_Private_Variables
  * @brief Private variables.
  * @{
  */

/** Usb HID report descriptor. */
__ALIGN_BEGIN static uint8_t CUSTOM_HID_ReportDesc[USBD_CUSTOM_HID_REPORT_DESC_SIZE] __ALIGN_END =
    {
        /* USER CODE BEGIN 0 */

        /* 键盘 */
        0x05, 0x01,         //   Usage Page (Generic Desktop),
        0x09, 0x06,         //   Usage (Keyboard),
        0xA1, 0x01,         //   Collection (Application),
        0x85, 0x01,         //   REPORT_ID (1)
        // bitmap of modifiers
        0x75, 0x01,         //   Report Size (1),
        0x95, 0x08,         //   Report Count (8),
        0x05, 0x07,       //   Usage Page (Key Codes),
        0x19, 0xE0,       //   Usage Minimum (224),
        0x29, 0xE7,       //   Usage Maximum (231),
        0x15, 0x00,       //   Logical Minimum (0),
        0x25, 0x01,       //   Logical Maximum (1),
        0x81, 0x02,       //   Input (Data, Variable, Absolute), ;Modifier byte
        // bitmap of keys
        0x95, 0x78,       //   Report Count (120),
        0x75, 0x01,       //   Report Size (1),
        0x15, 0x00,       //   Logical Minimum (0),
        0x25, 0x01,       //   Logical Maximum(1),
        0x05, 0x07,       //   Usage Page (Key Codes),
        0x19, 0x00,       //   Usage Minimum (0),
        0x29, 0x77,       //   Usage Maximum (),
        0x81, 0x02,       //   Input (Data, Variable, Absolute),
        // LED control
        0x95, 0x03,       // Report Count (3)
        0x75, 0x01,       // Report Size  (1)
        0x05, 0x08,       // USAGE_PAGE (LEDs)
        0x19, 0x01,       // Usage Minimum (Num Lock   1)
        0x29, 0x03,       // Usage Maximum (Scroll Lock   3)
        0x91, 0x02,       // Output (Data,Var,Abs)
        //output凑共1byte(无实际用处)
        0x95, 0x05,       // Report Count (5)
        0x75, 0x01,       // Report Size  (1)
        0x91, 0x01,       // Output (Cnst,Var,Abs)
        0xC0, /*     END_COLLECTION	             */

        /* 音量控制部分 */
        0x05, 0x0c,         // UsagePage(Consumer)
        0x09, 0x01,         // Usage(Volume)
        0xa1, 0x01,         // Collection (Application)
        0x85, 0x03,         // REPORT_ID (3)
        
        0xa1, 0x00, 

        0x09, 0xe9,         // LOCAL_USAGE(Volume Increment)   
        0x09, 0xea,         // LOCAL_USAGE(Volume Decrement)  
        0x09, 0xe2,         // LOCAL_USAGE(Mute) 
        0x09, 0xcd,         // LOCAL_USAGE(Play/Pause)

        0x35, 0x00,  
        0x45, 0x07, 
        0x15, 0x00,
        0x25, 0x01,

        0x75, 0x01,         // Report Size  (1)
        0x95, 0x04,         // Report Count (4)
        0x81, 0x02,         // Input (Data, Variable, Absolute),
  
        0x75, 0x01,
        0x95, 0x04, 
        0x81, 0x01, 
        0xC0, 
        /* USER CODE END 0 */
        0xC0 /*     END_COLLECTION	             */
};

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Exported_Variables USBD_CUSTOM_HID_Exported_Variables
  * @brief Public variables.
  * @{
  */
extern USBD_HandleTypeDef hUsbDevice;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */
/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_FunctionPrototypes USBD_CUSTOM_HID_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t CUSTOM_HID_Init(void);
static int8_t CUSTOM_HID_DeInit(void);
static int8_t CUSTOM_HID_OutEvent(uint8_t event_idx, uint8_t state);

/**
  * @}
  */

USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops = {CUSTOM_HID_ReportDesc,
                                                  CUSTOM_HID_Init,
                                                  CUSTOM_HID_DeInit,
                                                  CUSTOM_HID_OutEvent};

/** @defgroup USBD_CUSTOM_HID_Private_Functions USBD_CUSTOM_HID_Private_Functions
  * @brief Private functions.
  * @{
  */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_Init(void)
{
  /* USER CODE BEGIN 4 */
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  DeInitializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_DeInit(void)
{
  /* USER CODE BEGIN 5 */
  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  Manage the CUSTOM HID class events
  * @param  event_idx: Event index
  * @param  state: Event state
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_OutEvent(uint8_t event_idx, uint8_t state)
{
  /* USER CODE BEGIN 6 */
  volatile static uint8_t a = 0;
  
  if (event_idx == 1) 
  {
    /* Caps Lock */
    if ((state&CapsLockMask))// CapsLock ON
    {
      HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
    }
    else if ((~state)&CapsLockMask)// CapsLock OFF
    {
      HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
    }

    #if 0 // Reserved for other mask
    /* Num Lock */
    if ((state&NumLockMask))// CapsLock ON
    {
      
    }
    else if ((~state)&NumLockMask)// CapsLock OFF
    {
      
    }

    /* Scroll Lock */
    if ((state&ScrolLockMask))// CapsLock ON
    {
      
    }
    else if ((~state)&ScrolLockMask)// CapsLock OFF
    {
      
    }
    #endif
    
    a += 1; 
  }
  
  //UNUSED(event_idx);
  //UNUSED(state);

  /* Start next USB packet transfer once data processing is completed */
  if (USBD_CUSTOM_HID_ReceivePacket(&hUsbDevice) != (uint8_t)USBD_OK)
  {
    return -1;
  }

  return (USBD_OK);
  /* USER CODE END 6 */
}

/* USER CODE BEGIN 7 */
/**
  * @brief  Send the report to the Host
  * @param  report: The report to be sent
  * @param  len: The report length
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
/*
static int8_t USBD_CUSTOM_HID_SendReport(uint8_t *report, uint16_t len)
{
  return USBD_CUSTOM_HID_SendReport(&hUsbDevice, report, len);
}
*/
/* USER CODE END 7 */

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */
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
