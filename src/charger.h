/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CHARGER_H
#define __CHARGER_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

extern uint8_t event_bt_stop;
extern uint8_t event_bt_start;
extern uint8_t event_bt_minus;
extern uint8_t event_bt_plus;

void delay_us(uint8_t us);
void delay_ms(uint16_t ms);
void delay_s(uint8_t s);

void charger_start(void);

#endif /* __CHARGER_H */