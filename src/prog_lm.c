#include "prog_lm.h"
#include "charger.h"
#include "lcd.h"

static uint8_t state = 0;
int8_t prog_lm(void)
{
  switch(state)
  {
  case 0:
    lcd_clear();
    
    lcd_gotoxy(0,0);
    lcd_putsf("PROGRAM SELECT");
    
    lcd_gotoxy(0,1);
    lcd_putsf("Load Memory");
    
    state++;
    break;
    
  case 1:
    if(event_bt_start)
    {
      state++;
    }
    else if(event_bt_plus)
    {
      state = 0;
      return 1;
    }
    else if(event_bt_minus)
    {
      state = 0;
      return -1;
    }
    break;
    
  case 2:
    lcd_clear();
    
    lcd_gotoxy(0,0);
    lcd_putsf("Load Memory");
    
    lcd_gotoxy(0,1);
    lcd_putsf("Not implemented");
    state++;
    break;
    
  case 3:
    if(event_bt_stop)
    {
      state = 0;
      return 0;
    }
    
    break;
  }
  
  return 0;
}
