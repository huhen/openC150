#include "charger.h"
#include "hardware.h"
#include "lcd.h"
#include "stm8s_it.h"
#include <stdio.h>
#include "errors.h"

#include "prog_ef.h"
#include "prog_li.h"
#include "prog_lm.h"
#include "prog_ni.h"
#include "prog_pb.h"
#include "prog_us.h"

uint8_t event_bt_stop = 0;
uint8_t event_bt_start = 0;
uint8_t event_bt_minus = 0;
uint8_t event_bt_plus = 0;

extern uint8_t RST_BY_WDG;

static uint8_t bt_stop = 0;
static uint8_t bt_start = 0;
static uint8_t bt_minus = 0;
static uint8_t bt_plus = 0;

static uint8_t state = 0;

#define CYCLE_TIME      (20)
#define NUM_TICK_LONG   (50)

#define BEPP_HZ         (2000)
#define BEEP_LONG       (500)
#define BEPP_SHORT      (200)

#define PROG_LI         (0)
#define PROG_NI         (1)
#define PROG_PB         (2)
#define PROG_US         (3)
#define PROG_EF         (4)
#define PROG_LM         (5)

#define EVENT_SHORT     (1)
#define EVENT_LONG      (2)


void delay_us(uint8_t us)
{
  TIM4->CNTR = ((uint8_t)0 - us);
  while (TIM4->CNTR);
}

void delay_ms(uint16_t ms)
{
  uint16_t start = tick_ms;
  if(!ms) ms++;
  while((tick_ms - start) < ms)
  {
    wfi();
  }
}

void delay_s(uint8_t s)
{
  uint16_t ss = s * 10;
  while(ss--)
  {
    delay_ms(100);
    IWDG->KR = IWDG_KEY_REFRESH;
  }
}

void beep_on(uint16_t hz, uint16_t ms)
{
  beep_start_ms = tick_ms;
  beep_time_ms = ms;
  uint16_t period = 1000000/hz;
  TIM3_SetAutoreload(period);
  TIM3_SetCompare2(period/2);
}

static void scan_key()
{
  if(BT_STOP)
  {
    if(bt_stop!=0xff) bt_stop++;
  }
  else
  {
    if(bt_stop)
    {
      if(bt_stop<NUM_TICK_LONG)
      {
        beep_on(BEPP_HZ, BEPP_SHORT);
        event_bt_stop = EVENT_SHORT;
      }
      else
      {
        beep_on(BEPP_HZ, BEEP_LONG);
        event_bt_stop = EVENT_LONG;
      }

      bt_stop=0;
    }
  }
  
  if(BT_MINUS)
  {
    if(bt_minus!=0xff) bt_minus++;
  }
  else
  {
    if(bt_minus)
    {
      if(bt_minus<NUM_TICK_LONG)
      {
        beep_on(BEPP_HZ, BEPP_SHORT);
        event_bt_minus = EVENT_SHORT;
      }
      else
      {
        beep_on(BEPP_HZ, BEEP_LONG);
        event_bt_minus = EVENT_LONG;
      }
      
      bt_minus=0;
    }
  }
  
  if(BT_PLUS)
  {
    if(bt_plus!=0xff) bt_plus++;
  }
  else
  {
    if(bt_plus)
    {
      if(bt_plus<NUM_TICK_LONG)
      {
        beep_on(BEPP_HZ, BEPP_SHORT);
        event_bt_plus = EVENT_SHORT;
      }
      else
      {
        beep_on(BEPP_HZ, BEEP_LONG);
        event_bt_plus = EVENT_LONG;
      }
      
      bt_plus=0;
    }
  }
  
  if(BT_START)
  {
    if(bt_start!=0xff) bt_start++;
  }
  else
  {
    if(bt_start)
    {
      if(bt_start<NUM_TICK_LONG)
      {
        beep_on(BEPP_HZ, BEPP_SHORT);
        event_bt_start = EVENT_SHORT;
      }
      else
      {
        beep_on(BEPP_HZ, BEEP_LONG);
        event_bt_start = EVENT_LONG;
      }
      
      bt_start=0;
    }
  }
}

void charger_start(void)
{
  lcd_init();
  IWDG->KR = IWDG_KEY_REFRESH;
  
  lcd_gotoxy(0,0);
  lcd_putsf("OC150 github.com");
  lcd_gotoxy(0,1);
  lcd_putsf("/huhen/openC150");
  
  beep_on(BEPP_HZ, BEPP_SHORT);
  
  delay_s(1);
  
  COOL_OFF;
  
  delay_s(1);
  
  uint16_t start = tick_ms;
  for(;;)
  {
    if((tick_ms - start) < CYCLE_TIME)
    {
      wfi();
      continue;
    }
    start+= CYCLE_TIME;
    
    scan_key();
    
    switch(state)
    {
    case PROG_LI:
      prog_li();
      break;
      
    case PROG_NI:
      prog_ni();
      break;
      
    case PROG_PB:
      prog_pb();
      break;
      
    case PROG_US:
      prog_us();
      break;
      
    case PROG_EF:
      prog_ef();
      break;
      
    case PROG_LM:
      prog_lm();
      break;
    }
    
    event_bt_stop=0;
    event_bt_start=0;
    event_bt_minus=0;
    event_bt_plus=0;
    
    /* Reload IWDG counter */
    IWDG->KR = IWDG_KEY_REFRESH;
  }
}
