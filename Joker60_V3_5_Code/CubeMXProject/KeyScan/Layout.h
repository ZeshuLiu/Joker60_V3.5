#ifndef __LAYOUT_H__
#define __LAYOUT_H__

#include <stdint.h>
#include <main.h>

#define CHG_LCTRL_CPSLOCK

#define DebonuceTime 5 //防抖使用前五次数据
#define ScanCountPerms 3 //每一ms内扫描3次

#define KeyBoardColCount 14
#define KeyBoardRowCount 5

#define FN_COL 2
#define FN_ROW 4

#define PN_COL 9
#define PN_ROW 4

#define KeyHidReportLen 16

extern GPIO_TypeDef *KeyboardColListPort[KeyBoardColCount];
extern uint16_t  KeyboardColListPin[KeyBoardColCount];
extern uint16_t  KeyboardRowListPin[KeyBoardRowCount];
extern GPIO_TypeDef *KeyboardRowListPort[KeyBoardRowCount];


extern uint8_t Normal_Value[KeyBoardRowCount][KeyBoardColCount];        // Normal KeyValue 
extern uint8_t FN_Press_Value[KeyBoardRowCount][KeyBoardColCount];      // KeyValue When Fn_Pressed

enum {
  LED2_Blink_Idle = 1000,
  LED2_Blink_FN = 100
};

extern uint16_t LED2_Blink_Int;
#endif
