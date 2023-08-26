
/* Includes ------------------------------------------------------------------*/
#include "KeyScan.h"
#include "Layout.h"
#include "KeyValue.h"
#include <string.h>
#include "FreeRTOS.h"


_Bool keyChange = 0;

/*! @author Liu Zeshu
    @brief Scan the keyboard matrix once; There is filter to eliminate switch bounce: #1. When keypressed[i][j] == 0 (Not Think it is Pressed), if the io think 
key is pressed (may be a bounce), KeyScan[i][j] += 1. #2 When (KeyScan[i][j]/DebonuceTime > 0) is true, set keypressed[i][j] = 1(Pressed). #3 If keypressed[i][j] == 1
and the io think the key is released, directly set keypressed[i][j] = 0 (Released). ## DebounceTime is defined in Layout.h
    @param KeyRep[] List contains Hid Report Data. 
    @return If any key has changed
*/
_Bool SingleScan(uint8_t KeyRep[]){
    static _Bool gpio_value, keypressed[KeyBoardRowCount][KeyBoardColCount] = {0};
    static uint8_t KeyScan[KeyBoardRowCount][KeyBoardColCount] = {0};

    // Scan takes about 50us when ther are no debug outputs. With output, it takes 3000us or longer depending how much is printed out!
    #ifdef TimeIt
        uint64_t start_us = to_us_since_boot(get_absolute_time());
    #endif

    keyChange = 0;
    for (int i = 0; i < KeyBoardRowCount; i++){
        if (i > 0){
            //gpio_put(KeyboardRowList[i-1],1);
            HAL_GPIO_WritePin(KeyboardRowListPort[i-1],KeyboardRowListPin[i-1], GPIO_PIN_SET);
        }
        else{
            //gpio_put(KeyboardRowList[KeyBoardRowCount-1],1);
            HAL_GPIO_WritePin(KeyboardRowListPort[KeyBoardRowCount-1],KeyboardRowListPin[KeyBoardRowCount-1], GPIO_PIN_SET);
        }
        //gpio_put(KeyboardRowList[i], 0);
        HAL_GPIO_WritePin(KeyboardRowListPort[i],KeyboardRowListPin[i], GPIO_PIN_RESET);

        //sleep_us(10);
        //? start_time = to_us_since_boot(get_absolute_time());
        //? while (to_us_since_boot(get_absolute_time()) - start_time < 1) tight_loop_contents();
        
        for (int j = 0; j < KeyBoardColCount; j++){
            // Debounce Read
            //gpio_value = gpio_get(KeyboardColList[j]);
            gpio_value = HAL_GPIO_ReadPin(KeyboardColListPort[j],KeyboardColListPin[j]);
            if(gpio_value == 0 && keypressed[i][j] == 0){
                KeyScan[i][j] = (KeyScan[i][j] + 1)%(DebonuceTime+1);
                #ifdef DBG
                    //printf("Keychange %d,%d->%d\n",i,j,gpio_value);
                #endif
            }
            else if (gpio_value == 1 && keypressed[i][j] == 1){
                KeyScan[i][j] = 0;
                #ifdef DBG
                    //printf("Keychange %d,%d->%d\n",i,j,gpio_value);
                #endif
            }

            //获取防抖后的值
            if (KeyScan[i][j]/DebonuceTime > 0 && keypressed[i][j] == 0){
                keypressed[i][j] = 1;
                KeyReportConstructFunc(KeyRep,i,j,1);
                keyChange = 1;
                #ifdef DBG
                    printf("!! Keychange Debonuce %d,%d->%d\n",i,j,keypressed[i][j]);
                #endif
            }
            else if (KeyScan[i][j]/DebonuceTime == 0 && keypressed[i][j] == 1){
                keypressed[i][j] = 0;
                KeyReportConstructFunc(KeyRep,i,j,0);
                keyChange = 1;
                #ifdef DBG
                    printf("!! Keychange Debonuce %d,%d->%d\n",i,j,keypressed[i][j]);
                #endif
            }

        }

    }

    #ifdef TimeIt
        if (keyChange) printf("Scan Used %llu us\n\n",to_us_since_boot(get_absolute_time()) - start_us );
    #endif

#ifdef DBG
    if (keyChange==1 && 0){
        for (int i = 0; i < KeyBoardRowCount; i++){
            printf("Row:%d", i);
            for (int j = 0; j < KeyBoardColCount; j++){
                printf("Col%d-%d, ", j,keypressed[i][j]);
            }
            printf("------------------------------\n\n");
        }
    }
#endif
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

    // First time press FN
    if (Row == FN_ROW && Col == FN_COL && ifFn == 0 && IfPress == 1){ 
        ifFn = 1;
        LED2_Blink_Int = LED2_Blink_FN;
        for (int i = 0; i < KeyHidReportLen; i++){
            KeyReport[i] = 0;
        }
        return ;
    }

    // First time release FN
    if (Row == FN_ROW && Col == FN_COL && ifFn == 1 && IfPress == 0){ 
        ifFn = 0;
        LED2_Blink_Int = LED2_Blink_Idle;
        for (int i = 0; i < KeyHidReportLen; i++){
            KeyReport[i] = 0;
        }
        return ;
    }

    
    // Normal Mode (PN not pressed!)
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

        // Modifier Key
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
        
        // Normal Key
        temp = (keyval)%0x08;
        temp = 0x01<<temp; 
        if (IfPress){
            KeyReport[1 + (keyval)/0x08] = KeyReport[1 + (keyval)/0x08] | temp;
        }
        else{
            KeyReport[1 + (keyval)/0x08] = KeyReport[1 + (keyval)/0x08] & (~temp);
        }
        #ifdef DBG
            printf("Report No.%d = 0x%02X\n", (1 + (keyval)/0x08), KeyReport[1 + (keyval)/0x08]);
        #endif

    }// Normal Mode (PN not pressed!)

    // PN Mode (PN Pressed)
    else{
        //Todo 
    }// PN Mode (PN Pressed)
		
    
}

