/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "KeyScan.h"
#include "KeyValue.h"
#include "Layout.h"
#include "usbd_customhid.h"
#include "usb_device.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern USBD_HandleTypeDef hUsbDeviceFS;

static uint8_t KeyHidReport[KeyHidReportLen] = {0};


osThreadId_t LED_TaskHandle;
const osThreadAttr_t LED_Task_attributes = {
  .name = "Led_Task",
  .stack_size = 64*4,
  .priority = (osPriority_t) osPriorityLow7,
};

osThreadId_t KeyScan_TaskHandle;
const osThreadAttr_t KeyScan_Task_attributes = {
  .name = "KeyScan_Task",
  //! .stack_size = 768*4,
  .stack_size = 128*4,
  .priority = (osPriority_t) osPriorityAboveNormal1,
};

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t USBHidTaskHandle;
const osThreadAttr_t USBHidTask_attributes = {
  .name = "USBHidTaskTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

void Start_KeyScan_Task(void *argument);
void Start_LED_Task(void *argument);

/* USER CODE END FunctionPrototypes */

void StartUSBHidTask(void *argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  USBHidTaskHandle = osThreadNew(StartUSBHidTask, NULL, &USBHidTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  LED_TaskHandle = osThreadNew(Start_LED_Task, NULL, &LED_Task_attributes);
  KeyScan_TaskHandle = osThreadNew(Start_KeyScan_Task, NULL, &KeyScan_Task_attributes);
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartUSBHidTask(void *argument)
{
  static uint8_t KeyHidReportFull[17] = {0};
  static TickType_t lasttick;
  
  lasttick = xTaskGetTickCount();
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  KeyHidReportFull[0] = 0x01;
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    // SingleScan(KeyHidReport);
    vTaskDelayUntil(&lasttick, 0x01U); // 每 1ms 发送一次数据
    memcpy(&KeyHidReportFull[1], KeyHidReport, 16);
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS,KeyHidReportFull,17);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/**
  * @brief  Function implementing the LED_Task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void Start_LED_Task(void *argument)
{
  /* USER CODE BEGIN Start_LED_Task */
  HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin, GPIO_PIN_RESET);
  /* Infinite loop */
  for(;;)
  {
    HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
    //HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin);
    osDelay((uint32_t)LED2_Blink_Int);
    //! osDelay(LED2_Blink_Idle);
    //! osDelay(1000);
  }
  /* USER CODE END Start_LED_Task */
}

/**
  * @brief  Function implementing the LED_Task thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void Start_KeyScan_Task(void *argument)
{ 
  /* USER CODE BEGIN Start_KeyScan_Task */
  static TickType_t lasttick;
  static uint8_t ScanCount = 0;

  lasttick = xTaskGetTickCount();
  //HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin, GPIO_PIN_RESET);
  /* Infinite loop */
  for(;;)
  {
    vTaskDelayUntil(&lasttick, 0x01U); // 每 1ms 扫描n次

    /* 重复扫描 ScanCountPerms 次 */
    for ( ScanCount = 0; ScanCount < ScanCountPerms; ScanCount++)
    {
      SingleScan(KeyHidReport);// 扫描一次大约 1/10 ms
    }
    // SingleScan(KeyHidReport);       
  }
  /* USER CODE END Start_KeyScan_Task */
}

/* USER CODE END Application */

