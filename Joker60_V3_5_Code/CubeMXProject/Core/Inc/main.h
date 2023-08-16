/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ROW5_Pin GPIO_PIN_0
#define ROW5_GPIO_Port GPIOA
#define COL5_Pin GPIO_PIN_1
#define COL5_GPIO_Port GPIOA
#define COL4_Pin GPIO_PIN_2
#define COL4_GPIO_Port GPIOA
#define COL1_Pin GPIO_PIN_3
#define COL1_GPIO_Port GPIOA
#define COL2_Pin GPIO_PIN_4
#define COL2_GPIO_Port GPIOA
#define COL3_Pin GPIO_PIN_5
#define COL3_GPIO_Port GPIOA
#define LED1_Pin GPIO_PIN_6
#define LED1_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_7
#define LED2_GPIO_Port GPIOA
#define SCK2_Pin GPIO_PIN_13
#define SCK2_GPIO_Port GPIOB
#define MISO2_Pin GPIO_PIN_14
#define MISO2_GPIO_Port GPIOB
#define MOSI2_Pin GPIO_PIN_15
#define MOSI2_GPIO_Port GPIOB
#define TX6_Pin GPIO_PIN_6
#define TX6_GPIO_Port GPIOC
#define RX6_Pin GPIO_PIN_7
#define RX6_GPIO_Port GPIOC
#define SDA3_Pin GPIO_PIN_9
#define SDA3_GPIO_Port GPIOC
#define SCL3_Pin GPIO_PIN_8
#define SCL3_GPIO_Port GPIOA
#define USB_N_Pin GPIO_PIN_11
#define USB_N_GPIO_Port GPIOA
#define USB_P_Pin GPIO_PIN_12
#define USB_P_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWDCLK_Pin GPIO_PIN_14
#define SWDCLK_GPIO_Port GPIOA
#define ROW1_Pin GPIO_PIN_15
#define ROW1_GPIO_Port GPIOA
#define ROW2_Pin GPIO_PIN_10
#define ROW2_GPIO_Port GPIOC
#define COL10_Pin GPIO_PIN_11
#define COL10_GPIO_Port GPIOC
#define COL11_Pin GPIO_PIN_12
#define COL11_GPIO_Port GPIOC
#define COL12_Pin GPIO_PIN_2
#define COL12_GPIO_Port GPIOD
#define COL13_Pin GPIO_PIN_3
#define COL13_GPIO_Port GPIOB
#define COL14_Pin GPIO_PIN_4
#define COL14_GPIO_Port GPIOB
#define ROW3_Pin GPIO_PIN_5
#define ROW3_GPIO_Port GPIOB
#define COL6_Pin GPIO_PIN_6
#define COL6_GPIO_Port GPIOB
#define COL7_Pin GPIO_PIN_7
#define COL7_GPIO_Port GPIOB
#define COL8_Pin GPIO_PIN_8
#define COL8_GPIO_Port GPIOB
#define COL9_Pin GPIO_PIN_9
#define COL9_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
