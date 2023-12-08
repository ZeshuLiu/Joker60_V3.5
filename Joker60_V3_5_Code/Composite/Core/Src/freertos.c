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
#include "usart.h"

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

char CMD_BUFFER[MAX_DISP_ROW][MAX_DISP_LEN+1] = {'\0'};
uint8_t CMD_LEN[MAX_DISP_ROW] = {0};
uint8_t CMD_DIR[MAX_DISP_ROW] = {0};
uint8_t CMD_POINTER = 0;

uint16_t TX_CNT = 0;
uint16_t RX_CNT = 0;

static uint8_t if_show_pic = 1;

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
  .priority = (osPriority_t) osPriorityHigh,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

void DRAW_FRAME(uint8_t mode);
void DRAW_DATA(uint8_t mode);

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
    vTaskDelayUntil(&lasttick, 0x01U); // 每隔 1ms 扫描一次
    
    // 发送媒体键
    if (ComposedHidReport[ConHidReportOffset+1] != LastReport)
    {
      memcpy(&ConHidReportFull[1], &ComposedHidReport[ConHidReportOffset+1], ConHidReportLen);
      while (USBD_CUSTOM_HID_SendReport(&hUsbDevice, ConHidReportFull, ConHidReportLen+1)!=USBD_OK);
      LastReport = ConHidReportFull[1];
      continue;
    }

    // 发送键盘按键
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

  HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin, GPIO_PIN_RESET);
  /* Infinite loop */
  for(;;)
  {
    HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
    // CDC_Transmit(0,dat,4);
    //HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin);
    if(LED2_Blink_Int == LED2_Blink_OFF){
      HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
      osDelay(1000);
      continue;
    }
      
    osDelay((uint32_t)LED2_Blink_Int);


    if (if_show_pic){
        LCD_ShowPicture(0,10,240,91,gImage_DOG);
        if_show_pic = 0;
        DRAW_FRAME(0);
    }
    
    DRAW_DATA(0);
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

#define MODE_0_START_X 255
#define DOG_END 101
#define FRAME_COLOR_0 GRAYBLUE
#define CMD_IN_COLOR (HEX_DISP) ? BLUE:GREEN
#define CMD_OUT_COLOR (HEX_DISP) ? MAGENTA:RED

void DRAW_FRAME(uint8_t mode)
{
  // uint8_t pos;

  switch (mode)
  {
    case 0:
      /* 进出指令框 */
      LCD_DrawLine(0,MODE_0_START_X, 240,MODE_0_START_X, FRAME_COLOR_0);      // 命令行下方横线
      LCD_DrawLine(0,DOG_END+2, 240,DOG_END+2, FRAME_COLOR_0);                // 命令行顶横线
      LCD_DrawLine(0,DOG_END+2, 0,MODE_0_START_X - 1, FRAME_COLOR_0);         // 命令行左侧横线
      LCD_DrawLine(239,DOG_END+2, 239,MODE_0_START_X - 1, FRAME_COLOR_0);     // 命令行右侧横线

      /* 串口参数显示 */
      LCD_ShowString(20,MODE_0_START_X+3,"BaudRate:",WHITE,BLACK,24,0);// 字高24 
      LCD_ShowString(30,MODE_0_START_X+27,"TX:",WHITE,BLACK,16,0);// 字高16
      LCD_ShowString(133,MODE_0_START_X+27,"RX:",WHITE,BLACK,16,0);// 字高16
      break;
    
    default:
      break;
  }
  
}

void DRAW_DATA(uint8_t mode)
{
  uint8_t pos, i;
  uint8_t temp_word[3*MAX_DISP_HEX+4] = {'\0'}; //! 需要保证这个长度至少为 (3 * MAX_DISP_HEX + 4)才能保证正常刷新

  extern _Bool HEX_DISP; // 当前是否按照16进制格式显示

  switch (mode)
  {
    case 0x00:
      LCD_ShowIntNum(129, MODE_0_START_X+3, huart6.Init.BaudRate, 7, WHITE, BLACK, 24);
      LCD_ShowIntNum(54, MODE_0_START_X+27, TX_CNT, 5, WHITE, BLACK, 16);
      LCD_ShowIntNum(157, MODE_0_START_X+27, RX_CNT, 5, WHITE, BLACK, 16);

      // LCD_Fill(1,DOG_END+3,238,MODE_0_START_X-1,BLACK);
      /* 进出指令显示 */
      for ( i = 0; i < MAX_DISP_ROW; i++)
      {
        if (i > CMD_POINTER-1){
          pos = MAX_DISP_ROW - i - 1 + CMD_POINTER;
        }
        else{
          pos = CMD_POINTER - i - 1;
        }

        if (CMD_DIR[pos] != 0)
        {
          if (HEX_DISP)
          {
            memset(temp_word, ' ', sizeof(temp_word)-1);
            temp_word[0] = '\0';
            for (uint8_t j = 0; j < MAX_DISP_HEX + 1; j++)
            {
              if(j+1 > CMD_LEN[pos] ) break;
              sprintf((char *) temp_word, "%s %02X",(char *) temp_word, CMD_BUFFER[pos][j]);
            }
            temp_word[strlen((char *) temp_word)] = ' '; 
          }
          
          LCD_ShowString_CL(2,                                             // x
              DOG_END + 4 + 20*i,                                       // y          
              (HEX_DISP) ? (const uint8_t *) &temp_word[1]:(const uint8_t *) CMD_BUFFER[pos], 
              (CMD_DIR[pos] == 1) ? CMD_IN_COLOR : CMD_OUT_COLOR ,      // 字符颜色
              BLACK,16,0);
        }
        
      }
      break;
    
    default:
      break;
  }
}
/* USER CODE END Application */

