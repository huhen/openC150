#include "charger.h"
#include "hardware.h"
#include "lcd.h"

extern uint8_t RST_BY_WDG;

void charger_start(void)
{
  lcd_init();

  lcd_gotoxy(0,0);
  lcd_putsf("OC150 github.com");
  lcd_gotoxy(0,1);
  lcd_putsf("/huhen/openC150");

  COOL_OFF;
  
  while (1)
  {
    /* Reload IWDG counter */
    IWDG->KR = IWDG_KEY_REFRESH;
  }
}
