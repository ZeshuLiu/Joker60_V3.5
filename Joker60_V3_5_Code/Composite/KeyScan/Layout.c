#include "Layout.h"
#include "KeyValue.h"
#include "KeyScan.h"


uint16_t LED2_Blink_Int = LED2_Blink_Idle;

/* Matrix Connection BEGIN */
GPIO_TypeDef *KeyboardColListPort[KeyBoardColCount] = {
    GPIOA,     GPIOA,      GPIOA,      GPIOA,       //4
    GPIOA,      GPIOB,      GPIOB,      GPIOB,      //8
    GPIOB,      GPIOC,      GPIOC,      GPIOD,      //12
    GPIOB,      GPIOB                               //14
    };                             
uint16_t  KeyboardColListPin[KeyBoardColCount] = {
    GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_2,
    GPIO_PIN_1, GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_8,
    GPIO_PIN_9, GPIO_PIN_11,GPIO_PIN_12,GPIO_PIN_2,
    GPIO_PIN_3, GPIO_PIN_4
    };
GPIO_TypeDef *KeyboardRowListPort[KeyBoardRowCount] = { 
    GPIOA,       GPIOC,       GPIOB,      GPIOC,       GPIOA
    };

uint16_t  KeyboardRowListPin[KeyBoardRowCount] = {
    GPIO_PIN_15, GPIO_PIN_10, GPIO_PIN_5, GPIO_PIN_13, GPIO_PIN_0
    };

/* Matrix Connection END */

/*
Normal Key Map (NO FN&PN)
*/
uint8_t Normal_Value[KeyBoardRowCount][KeyBoardColCount]={
    {KeyESC, Key1, Key2,Key3,Key4,Key5,Key6,Key7,Key8,Key9,Key0,KeyMINUS,KeyEQUAL,KeyBACKSPACE},
    {KeyTAB, KeyQ,KeyW,KeyE,KeyR,KeyT, KeyY, KeyU,KeyI,KeyO,KeyP,KeyLBRACKET,KeyRBRACKET,KeyBSLASH},
    {KeyLCtrl, KeyA,KeyS,KeyD,KeyF,KeyG,KeyH,KeyJ,KeyK,KeyL,KeySEMICOLON, KeyQUOTA, 0,KeyRETURN},
    {KeyLShift, KeyZ,KeyX,KeyC,KeyV,KeyB,KeyN,KeyM,KeyCOMMA,KeyPERIOD,KeyFSLASH, 0, 0, KeyRShift},
    {KeyCAPSLOCK,KeyLAlt,KeyFN,0,0,KeySPACE,0,0,0,KeyPN,KeyRAlt,0,KeyRGui,KeyRCtrl}
};

uint8_t FN_Press_Value[KeyBoardRowCount][KeyBoardColCount]={
    {KeyGRAVE, KeyF1, KeyF2,KeyF3,KeyF4,KeyF5,KeyF6,KeyF7,KeyF8,KeyF9,KeyF10,KeyF11,KeyF12,KeyDELETE},
    {KeyTAB, KeyQ,KeyUARROW,KeyE,KeyR,KeyT, KeyY, KeyU,KeyINSERT,KeyO,KeyPRINTSCREEN,KeySCROLLLOCK,KeyRBRACKET,KeyBSLASH},
    {KeyLCtrl, KeyLARROW,KeyDARROW,KeyRARROW,KeyF,KeyG,KeyH,KeyPlay,KeyK,KeyL,KeyHOME, KeyPAGEUP, 0,KeyRETURN},
    {KeyLShift, KeyZ,KeyX,KeyC,KeyV,KeyB,KeyVolDown,KeyVolUp,KeyMute,KeyEND,KeyPAGEDOWN, 0, 0, KeyRShift},
    {KeyCAPSLOCK,KeyLGui,KeyFN,0,0,KeySPACE,0,0,0,KeyPN,KeyRAlt,0,KeyRGui,KeyRCtrl}
};

void (* PN_Press_FUNC[KeyBoardRowCount][KeyBoardColCount])(void) ={
    {NULL, NULL, NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
    {NULL, NULL, NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
    {NULL, NULL, NULL,NULL,NULL,NULL,HexDispSwitch,NULL,NULL,LOCK_KEYBOARD,NULL,NULL,NULL,NULL},
    {NULL, NULL, NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
    {NULL, NULL, NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL}
};

uint8_t FN_NORMAL_DIF_HID_REP_LIST[KeyHidReportLen] = {
    0xF3,   // Low KeyLCtrl KeyLShift KeyLAlt KeyLGui - KeyRCtrl KeyRShift KeyRAlt KeyRGui High
    0x60,   // Low x x x x - KeyA KeyB KeyC KeyD High
    0x4F,   // Low KeyE KeyF KeyG KeyH - KeyI KeyJ KeyK KeyL High
    0xB4,   // Low KeyM KeyN KeyO KeyP - KeyQ KeyR KeyS KeyT High

    0x3B,   // Low KeyU KeyV KeyW KeyX - KeyY KeyZ Key1 Key2 High
    0x00,   // Low Key3 Key4 Key5 Key6 - Key7 Key8 Key9 Key0 High
    0x19,   // Low KeyRETURN KeyESC KeyBACKSPACE KeyTAB - KeySPACE KeyMINUS KeyEQUAL KeyLBRACKET High
    0x03,   // Low KeyRBRACKET KeyBSLASH X KeySEMICOLON - KeyQUOTA KeyGRAVE KeyCOMMA KeyPERIOD High

    0x02,   // Low KeyFSLASH KeyCAPSLOCK KeyF1 KeyF2 - KeyF3 KeyF4 KeyF5 KeyF6 High
    0x00,   // Low KeyF7 KeyF8 KeyF9 KeyF10 - KeyF11 KeyF12 KeyPRINTSCREEN KeySCROLLLOCK High
    0x00,   // Low KeyPAUSE KeyINSERT KeyHOME KeyPAGEUP - KeyDELETE KeyEND KeyPAGEDOWN KeyRARROW High
    0x00,   // Low KeyLARROW KeyDARROW KeyUARROW KeyNUMLOCK - X X X X High

    0x00,
    0x00,
    0x00,
    0x00,
};


void test_func_pointer(void)
{
    return;
}
