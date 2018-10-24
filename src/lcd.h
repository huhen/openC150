/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_H
#define __LCD_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

void lcd_init(void);
void lcd_gotoxy(char x, char y);
void lcd_putsf(char *_str);
void lcd_putchar(uint8_t d);

#endif /* __LCD_H */