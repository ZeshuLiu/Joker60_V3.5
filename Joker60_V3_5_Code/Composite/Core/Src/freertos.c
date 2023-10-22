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
#include <string.h>
#include "main.h"

#include "usbd_hid_custom.h"
#include "usb_device.h"
#include "usbd_cdc_acm_if.h"

#include "lcd_init.h"
#include "lcd.h"
#include "pic.h"
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
extern USBD_HandleTypeDef hUsbDevice;

static uint8_t ComposedHidReport[ComposedHidReportLen] = {0};


osThreadId_t LED_TaskHandle;
const osThreadAttr_t LED_Task_attributes = {
  .name = "Led_Task",
  .stack_size = 256*4,
  .priority = (osPriority_t) osPriorityLow,
};

osThreadId_t KeyScan_TaskHandle;
const osThreadAttr_t KeyScan_Task_attributes = {
  .name = "KeyScan_Task",
  .stack_size = 128*4,
  .priority = (osPriority_t) osPriorityAboveNormal1,
};


/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

void Start_KeyScan_Task(void *argument);
void Start_LED_Task(void *argument);
void Start_MediaControl_Task(void *argument);

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

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
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

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
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
	static TickType_t lasttick;
	// static uint8_t KeyHidReportFull[KeyHidReportLen+1] = {0};
  static uint8_t ConHidReportFull[ConHidReportLen+1] = {0};
  static uint8_t LastReport = 0;
  volatile uint8_t i = 0;
  
	MX_USB_DEVICE_Init();
  ComposedHidReport[0] = KeyHidReportID;
  ConHidReportFull[0] = ConHidReportID;
  
  lasttick = xTaskGetTickCount();
  /* Infinite loop */
  for(;;)
  { 
    vTaskDelayUntil(&lasttick, 0x01U); // 每隔 1ms 扫描�???�???
    
    if (ComposedHidReport[ConHidReportOffset+1] != LastReport)
    {
      memcpy(&ConHidReportFull[1], &ComposedHidReport[ConHidReportOffset+1], ConHidReportLen);
      while (USBD_CUSTOM_HID_SendReport(&hUsbDevice, ConHidReportFull, ConHidReportLen+1)!=USBD_OK);
      LastReport = ConHidReportFull[1];
      continue;
    }

    USBD_CUSTOM_HID_SendReport(&hUsbDevice, ComposedHidReport, KeyHidReportLen+1);
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
  static uint8_t dat[4] = {'a', 'b', 'c', 'd'};
  uint8_t i, j;
  double t=0;
  HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin, GPIO_PIN_RESET);
  /* Infinite loop */
  for(;;)
  {
    HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
    CDC_Transmit(0,dat,4);
    //HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin);
    osDelay((uint32_t)LED2_Blink_Int);

    
		t += 0.11;

    LCD_ShowPicture(0,31,240,91,gImage_DOG);

    LCD_ShowFloatNum1(128,200,t,4,RED,WHITE,16);

    /*

    LCD_ShowString(0,40,"LCD_W:",RED,0x8081,16,0);
		LCD_ShowIntNum(48,40,LCD_W,3,RED,WHITE,16);
		LCD_ShowString(80,40,"LCD_H:",RED,WHITE,16,0);
		LCD_ShowIntNum(128,40,LCD_H,3,RED,WHITE,16);
		LCD_ShowString(80,40,"LCD_H:",RED,WHITE,16,0);
		LCD_ShowString(0,70,"Increaseing Nun:",RED,WHITE,16,0);
		LCD_ShowFloatNum1(128,70,t,4,RED,WHITE,16);

    LCD_DrawLine(0,20,300,20,MAGENTA);
		LCD_DrawLine(0,20,0,299,MAGENTA);
		LCD_DrawLine(0,299,300,299,MAGENTA);
		LCD_DrawLine(239,20,239,299,MAGENTA);

    for(int i = 0; i <20; i++){
      LCD_ShowIntNum(48,16*i,16*i,3,BLUE,WHITE,16);
      LCD_DrawLine(100,16*i,200,16*i,YELLOW);
      LCD_DrawLine(100,16*i-2,200,16*i-2,BLUE);
			LCD_DrawLine(100,16*i+2,200,16*i+2,BLUE);
			LCD_DrawLine(100,16*i+4,200,16*i+4,RED);
			LCD_DrawLine(100,16*i-4,200,16*i-4,RED);
      osDelay(10);
    }
    */
    
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

  /* Infinite loop */
  for(;;)
  {
    vTaskDelayUntil(&lasttick, 0x01U); // 每隔 1ms 扫描�???�???

    /* 每轮扫描扫描 ScanCountPerms �??? */
    for ( ScanCount = 0; ScanCount < ScanCountPerms; ScanCount++)
    {
      SingleScan(&ComposedHidReport[1]);// 每轮扫描大概需要 1/10 ms
    } 
  }
  /* USER CODE END Start_KeyScan_Task */
}


/* USER CODE END Application */

