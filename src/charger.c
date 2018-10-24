#include "charger.h"
#include "hardware.h"
#include "lcd.h"

extern uint8_t RST_BY_WDG;

void delay40us()
{
  TIM4->CNTR=215;
  while (TIM4->CNTR);
}

void charger_start(void)
{
  lcd_init();

  lcd_gotoxy(0,0);
  lcd_putsf("OC150 github.com");
  lcd_gotoxy(0,1);
  lcd_putsf("/huhen/openC150");

  COOL_OFF;
  
  char d=0;
  for(;;)
  {
    delay40us();
    lcd_gotoxy(0,0);
    lcd_putchar(d++);
    
    for(uint16_t t=0;t<5000;t++)delay40us();
    //GPIOE->ODR ^= (uint8_t)GPIO_PIN_5;
    
    /* Reload IWDG counter */
    //IWDG->KR = IWDG_KEY_REFRESH;
  }
}
