/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CHARGER_H
#define __CHARGER_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

#define EVENT_SHORT     (1)
#define EVENT_LONG      (2)

extern uint16_t vcc_in;
extern uint16_t v_bat;
extern uint16_t v_cell_1;
extern uint16_t v_cell_2;
extern uint16_t v_cell_3;
extern uint16_t v_cell_4;
extern uint16_t v_cell_5;
extern uint16_t v_cell_6;
extern uint16_t i_discharge;
extern uint16_t i_charge;

extern uint8_t event_bt_stop;
extern uint8_t event_bt_start;
extern uint8_t event_bt_minus;
extern uint8_t event_bt_plus;

extern char buff[32];

void delay_us(uint8_t us);
void delay_ms(uint16_t ms);
void delay_s(uint8_t s);

void charger_start(void);

#endif /* __CHARGER_H */