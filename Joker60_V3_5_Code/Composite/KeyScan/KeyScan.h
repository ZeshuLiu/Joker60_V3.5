#ifndef __KEYSCAN_H__
#define __KEYSCAN_H__

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"


void KeyReportConstructFunc(uint8_t *KeyReport, uint8_t Row, uint8_t Col, _Bool ifPressed);
_Bool SingleScan(uint8_t *KeyReport);
void LOCK_KEYBOARD(void);
void HexDispSwitch(void);
#endif
