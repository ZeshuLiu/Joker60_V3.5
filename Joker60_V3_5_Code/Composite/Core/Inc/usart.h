/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "usbd_cdc_acm_if.h"
/* USER CODE END Includes */

extern UART_HandleTypeDef huart6;

/* USER CODE BEGIN Private defines */
#define UART6_MAX_RX_LEN 64

extern uint8_t UART6_RX_BUFFER[UART6_MAX_RX_LEN];
/* USER CODE END Private defines */

void MX_USART6_UART_Init(void);

/* USER CODE BEGIN Prototypes */
HAL_StatusTypeDef UpDateUart(UART_HandleTypeDef * uart, uint32_t baudrate);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

