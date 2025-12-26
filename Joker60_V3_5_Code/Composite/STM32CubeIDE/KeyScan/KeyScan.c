
/* Includes ------------------------------------------------------------------*/
#include "KeyScan.h"
#include "Layout.h"
#include "KeyValue.h"
#include <string.h>
#include "FreeRTOS.h"


_Bool keyChange = 0;
static _Bool KEYBOARD_LOCK = 0;
_Bool HEX_DISP = 0;

/** @author Liu Zeshu
  *  @brief Scan the keyboard matrix once; There is filter to eliminate switch bounce: #1. When keypressed[i][j] == 0 (Not Think it is Pressed), if the io think 
key is pressed (may be a bounce), KeyScan[i][j] += 1. #2 When (KeyScan[i][j]/DebonuceTime > 0) is true, set keypressed[i][j] = 1(Pressed). #3 If keypressed[i][j] == 1
and the io think the key is released, directly set keypressed[i][j] = 0 (Released). ## DebounceTime is defined in Layout.h
  *  @param KeyRep[] List contains Hid Report Data.
  *  @return If any key has changed
*/
_Bool SingleScan(uint8_t KeyRep[]){
    static _Bool gpio_value, keypressed[KeyBoardRowCount][KeyBoardColCount] = {0};
    static uint8_t KeyScan[KeyBoardRowCount][KeyBoardColCount] = {0};

    keyChange = 0;

    /* 扫描键盘矩阵 CODE START*/
    for (int i = 0; i < KeyBoardRowCount; i++)
    {
        /* 将上一行置高+将当前行置低 CODE START */
        if (i > 0)
        {
            HAL_GPIO_WritePin(KeyboardRowListPort[i-1],KeyboardRowListPin[i-1], GPIO_PIN_SET);
        }
        else
        {
            HAL_GPIO_WritePin(KeyboardRowListPort[KeyBoardRowCount-1],KeyboardRowListPin[KeyBoardRowCount-1], GPIO_PIN_SET);
        }
        HAL_GPIO_WritePin(KeyboardRowListPort[i],KeyboardRowListPin[i], GPIO_PIN_RESET);
        /* 将上一行置高+将当前行置低 CODE END */

        /* 读取每一列的电平 CODE START */
        for (int j = 0; j < KeyBoardColCount; j++)
        {
            /* 读取当前引脚电平 */
            gpio_value = HAL_GPIO_ReadPin(KeyboardColListPort[j],KeyboardColListPin[j]);

            /* 键盘按下需要滞后滤波，但是松开立即执行 */
            /* 如果当前没有认为该键按下，但是引脚电平为低（被按下），则keyScan[i][j]+=1 */
            /* 并且需要保证其值不会太大（<= DebonuceTime) 防止其溢出 */
            if(gpio_value == 0 && keypressed[i][j] == 0)
            {
                KeyScan[i][j] = ( (KeyScan[i][j] + 1) > (DebonuceTime+1) ) ? DebonuceTime :  (KeyScan[i][j] + 1) ;      // 要认为按下需要滤波
            }
            /* 如果当前认为该键按下，但是引脚电平为高（被释放），则keyScan[i][j] = 0 */
            else if (gpio_value == 1 && keypressed[i][j] == 1){
                // KeyScan[i][j] = 0;
                KeyScan[i][j] = ( (KeyScan[i][j] == 0) ) ? 0 :  (KeyScan[i][j] - 1) ;
            }

            /* 获取防抖后的值 CODE START*/

            /* 如果KeyScan[i][j] >= DebonuceTime 并且原先认为其没有按下 */
            /* 认为其按下，并且生成新的HidReport */
            if (KeyScan[i][j] >= DebonuceTime && keypressed[i][j] == 0)
            {
                keypressed[i][j] = 1;
                KeyReportConstructFunc(KeyRep,i,j,1);
                keyChange = 1;
            }

            /*  KeyScan[i][j] < DebonuceRelaseTime 并且原先认为其按下 */
            /* 认为其没有按下，并且生成新的HidReport */
            else if (KeyScan[i][j] < DebonuceRelaseTime && keypressed[i][j] == 1)
            {
                keypressed[i][j] = 0;
                KeyReportConstructFunc(KeyRep,i,j,0);
                keyChange = 1;
            }

            /* 获取防抖后的值 CODE END*/
        }
        /* 读取每一列的电平 CODE END*/
    }
    /* 扫描键盘矩阵 CODE END*/

    return keyChange;
}

/*! @author Liu Zeshu
    @brief Set correct hid report considering which keys are pressed
    @param KeyReport[] List contains Hid Report Data. 
    @param Row Position of the changed key.
    @param Col Position of the changed key.
    @param IfPress True if the key is pressed, False if the key is released.
*/
void KeyReportConstructFunc(uint8_t KeyReport[], uint8_t Row, uint8_t Col, _Bool IfPress){
    static uint8_t keyval, temp;
    static _Bool ifFn=0, ifPn=0;


    /* 第一次按下PN CODE START */
    if (Row == PN_ROW && Col == PN_COL && ifPn == 0 && IfPress == 1){ 
        ifPn = 1;
        LED2_Blink_Int = LED2_Blink_PN;
        for (int i = 0; i < ComposedHidReportLen - 1; i++){
            KeyReport[i] = 0;
        }
        return ;
    }
    /* 第一次按下PN  CODE END */

    /* 第一次松开PN CODE START */
    if (Row == PN_ROW && Col == PN_COL && ifPn == 1 && IfPress == 0){ 
        ifPn = 0;
        if(LED2_Blink_Int != LED2_Blink_OFF) LED2_Blink_Int = LED2_Blink_Idle;
        for (int i = 0; i < ComposedHidReportLen - 1; i++){
            KeyReport[i] = 0;
        }
        return ;
    }
    /* 第一次松开PN CODE END */

    if(KEYBOARD_LOCK && !ifPn) return;

    /* 第一次按下FN CODE START */
    if (Row == FN_ROW && Col == FN_COL && ifFn == 0 && IfPress == 1){ 
        ifFn = 1;
        LED2_Blink_Int = LED2_Blink_FN;
        for (int i = 0; i < ComposedHidReportLen - 1; i++){
            KeyReport[i] &= FN_NORMAL_DIF_HID_REP_LIST[i];
        }
        return ;
    }
    /* 第一次按下FN  CODE END */

    /* 第一次松开FN CODE START */
    if (Row == FN_ROW && Col == FN_COL && ifFn == 1 && IfPress == 0){ 
        ifFn = 0;
        LED2_Blink_Int = LED2_Blink_Idle;
        for (int i = 0; i < ComposedHidReportLen - 1; i++){
            KeyReport[i] &= FN_NORMAL_DIF_HID_REP_LIST[i];
        }
        return ;
    }
    /* 第一次松开FN CODE END */

    /* 正常键值 (没有按下PN) CODE START */
    if (ifPn==0){
        if (ifFn==0){
            keyval = Normal_Value[Row][Col];
        }
        else{
            keyval = FN_Press_Value[Row][Col];
        }
        #ifdef DBG
            printf("Key Value = 0x%02X\n", keyval);
        #endif

        if(keyval == 0xff){
            return;
        }

        /* 按下的是修饰键 CODE START */
        if (keyval > ModifierStart){
            temp = keyval - ModifierStart - 1;
            temp = 0x01<<temp;
            if (IfPress){
                KeyReport[0] = KeyReport[0] | temp;
            }
            else{
                KeyReport[0] = KeyReport[0] & (~temp);
            }
            #ifdef DBG
                printf("Modifiers = 0x%02X\n", KeyReport[0]);
            #endif

            return;
        }
        /* 按下的是修饰键 CODE END */

        /* 按下的是普通按键 CODE START */
        if (keyval < MAX_NORMAL_KEY)
        {
            
            temp = (keyval)%0x08;
            temp = 0x01<<temp; 
            if (IfPress){
                KeyReport[1 + (keyval)/0x08] |= temp;
            }
            else{
                KeyReport[1 + (keyval)/0x08] &= (~temp);
            }
            #ifdef DBG
                printf("Report No.%d = 0x%02X\n", (1 + (keyval)/0x08), KeyReport[1 + (keyval)/0x08]);
            #endif
            return;
        }
        /* 普通按键 END */
        
        /* 按下的是媒体控制 CODE START */
        switch (keyval)
        {
        case KeyVolUp:
            if (IfPress){
                KeyReport[ConHidReportOffset] |=  KeyVolUpReport;
            }
            else{
                KeyReport[ConHidReportOffset] &= (~KeyVolUpReport);
            }
            return;
        case KeyVolDown:
            if (IfPress){
                KeyReport[ConHidReportOffset] |= KeyVolDownReport;
            }
            else{
                KeyReport[ConHidReportOffset] &= (~KeyVolDownReport);
            }
            return;
        case KeyMute:
            if (IfPress){
                KeyReport[ConHidReportOffset] |= KeyMuteReport;
            }
            else{
                KeyReport[ConHidReportOffset] &= (~KeyMuteReport);
            }
            return;
        case KeyPlay:
            if (IfPress){
                KeyReport[ConHidReportOffset] |= KeyPlayReport;
            }
            else{
                KeyReport[ConHidReportOffset] &= (~KeyPlayReport);
            }
            return;
        
        default:
            break;
        }
        /* 媒体控制 END */
        
        /* !Todo 超过了我们限制的位数 0-0x77 （120键）的回报报文 */

    }
    /* 按下的是普通按键 CODE END */
    
    /* PN被按下情况的键值 CODE START */
    else{
        if (PN_Press_FUNC[Row][Col] != NULL){
            PN_Press_FUNC[Row][Col]();
        }
    }
    /* PN被按下情况的键值 CODE END */
	
}

/**
 * @brief 切换键盘锁定状态
 * 
 */
void LOCK_KEYBOARD(void)
{
    static uint32_t L_TIME = 0;
    if(osKernelGetTickCount() - L_TIME < 1000){
        return;
    }
    L_TIME = osKernelGetTickCount();
    KEYBOARD_LOCK = !KEYBOARD_LOCK;
    if (KEYBOARD_LOCK)
    {
        LED2_Blink_Int = LED2_Blink_OFF;
    }
    else{
        LED2_Blink_Int = LED2_Blink_Idle;
    }
    
    
    return;
}

/**
 * @brief 切换HEX或ASCII显示
 * 
 */
void HexDispSwitch(void)
{
    extern _Bool HEX_DISP;
    static uint32_t L_TIME = 0;
    if(osKernelGetTickCount() - L_TIME < 1000){
        return;
    }
    L_TIME = osKernelGetTickCount();
    HEX_DISP = !HEX_DISP;
    return;
}
