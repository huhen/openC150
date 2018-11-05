#include "charger.h"
#include "hardware.h"
#include "lcd.h"
#include "stm8s_it.h"
#include <stdio.h>
#include "errors.h"

#include "discharge.h"
#include "charge.h"

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

uint16_t vcc_in = 0;
uint16_t v_bat = 0;
uint16_t v_cell_1 = 0;
uint16_t v_cell_2 = 0;
uint16_t v_cell_3 = 0;
uint16_t v_cell_4 = 0;
uint16_t v_cell_5 = 0;
uint16_t v_cell_6 = 0;
uint16_t i_discharge = 0;
uint16_t i_charge = 0;

char buff[32];

struct EPROM *config=(struct EPROM*)0x4000;

extern uint8_t RST_BY_WDG;

static uint8_t bt_stop = 0;
static uint8_t bt_start = 0;
static uint8_t bt_minus = 0;
static uint8_t bt_plus = 0;

static uint8_t state = 0;

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
#define PROG_MAX        (PROG_LM)

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

static void scan_adc()
{
  adc_complete = 0;
  
  /*Start conversion */
  ADC2->CR1 |= ADC2_CR1_ADON;
  
  /*Wait stop conversion */
  while(!adc_complete)
  {
    wfi();
  }
#define FILTER_NUM       (16)
#define FILTER_SHIFT     (4)
  
  static uint8_t s_i=0;
  static uint16_t s_v_bat[FILTER_NUM]={0};
  static uint16_t s_v_cell_1[FILTER_NUM]={0};
  static uint16_t s_v_cell_2[FILTER_NUM]={0};
  static uint16_t s_v_cell_3[FILTER_NUM]={0};
  static uint16_t s_v_cell_4[FILTER_NUM]={0};
  static uint16_t s_v_cell_5[FILTER_NUM]={0};
  static uint16_t s_v_cell_6[FILTER_NUM]={0};
  static uint16_t s_i_discharge[FILTER_NUM]={0};
  static uint16_t s_i_charge[FILTER_NUM]={0};
  
  vcc_in = (((int32_t)AdcBuffer[ADC_VCC_IN_CHAN] - config->VCC_CAL1) * (17000 - 11000)) / (config->VCC_CAL2 - config->VCC_CAL1) + 11000;
  
  s_v_bat[s_i] = (((int32_t)AdcBuffer[ADC_VBAT_CHAN] - config->VBAT_CAL1) * (25200 - 800)) / (config->VBAT_CAL2 - config->VBAT_CAL1) + 800;
  s_v_cell_1[s_i] = (((int32_t)AdcBuffer[ADC_CELL_1_CHAN] - config->VCELL_1_CAL1) * (4200 - 800)) / (config->VCELL_1_CAL2 - config->VCELL_1_CAL1) + 800;
  s_v_cell_2[s_i] = (((int32_t)AdcBuffer[ADC_CELL_2_CHAN] - config->VCELL_2_CAL1) * (4200 - 800)) / (config->VCELL_2_CAL2 - config->VCELL_2_CAL1) + 800;
  s_v_cell_3[s_i] = (((int32_t)AdcBuffer[ADC_CELL_3_CHAN] - config->VCELL_3_CAL1) * (4200 - 800)) / (config->VCELL_3_CAL2 - config->VCELL_3_CAL1) + 800;
  s_v_cell_4[s_i] = (((int32_t)AdcBuffer[ADC_CELL_4_CHAN] - config->VCELL_4_CAL1) * (4200 - 800)) / (config->VCELL_4_CAL2 - config->VCELL_4_CAL1) + 800;
  s_v_cell_5[s_i] = (((int32_t)AdcBuffer[ADC_CELL_5_CHAN] - config->VCELL_5_CAL1) * (4200 - 800)) / (config->VCELL_5_CAL2 - config->VCELL_5_CAL1) + 800;
  s_v_cell_6[s_i] = (((int32_t)AdcBuffer[ADC_CELL_6_CHAN] - config->VCELL_6_CAL1) * (4200 - 800)) / (config->VCELL_6_CAL2 - config->VCELL_6_CAL1) + 800;
  s_i_discharge[s_i] = (((int32_t)AdcBuffer[ADC_CURRENT_DISCHARGE_CHAN] - config->DISCHARGE_CAL1) * (2000 - 200)) / (config->DISCHARGE_CAL2 - config->DISCHARGE_CAL1) + 200;
  s_i_charge[s_i] = (((int32_t)AdcBuffer[ADC_CURRENT_CHARGE_CHAN] - config->CHARGE_CAL1) * (1932 - 200)) / (config->CHARGE_CAL2 - config->CHARGE_CAL1) + 200;
  
  
  s_i++;
  if(s_i==FILTER_NUM)s_i=0;
  
  v_bat=0;
  v_cell_1=0;
  v_cell_2=0;
  v_cell_3=0;
  v_cell_4=0;
  v_cell_5=0;
  v_cell_6=0;
  i_discharge=0;
  i_charge=0;
  
  uint8_t i;
  uint32_t tmp=0;
  for(i=0;i<FILTER_NUM;i++) tmp+=s_v_bat[i];
  v_bat=tmp>>FILTER_SHIFT;
  tmp=0;
  for(i=0;i<FILTER_NUM;i++) tmp+=s_v_cell_1[i];
  v_cell_1=tmp>>FILTER_SHIFT;
  tmp=0;
  for(i=0;i<FILTER_NUM;i++) tmp+=s_v_cell_2[i];
  v_cell_2=tmp>>FILTER_SHIFT;
  tmp=0;
  for(i=0;i<FILTER_NUM;i++) tmp+=s_v_cell_3[i];
  v_cell_3=tmp>>FILTER_SHIFT;
  tmp=0;
  for(i=0;i<FILTER_NUM;i++) tmp+=s_v_cell_4[i];
  v_cell_4=tmp>>FILTER_SHIFT;
  tmp=0;
  for(i=0;i<FILTER_NUM;i++) tmp+=s_v_cell_5[i];
  v_cell_5=tmp>>FILTER_SHIFT;
  tmp=0;
  for(i=0;i<FILTER_NUM;i++) tmp+=s_v_cell_6[i];
  v_cell_6=tmp>>FILTER_SHIFT;
  tmp=0;
  for(i=0;i<FILTER_NUM;i++) tmp+=s_i_discharge[i];
  i_discharge=tmp>>FILTER_SHIFT;
  tmp=0;
  for(i=0;i<FILTER_NUM;i++) tmp+=s_i_charge[i];
  i_charge=tmp>>FILTER_SHIFT;
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
  
  if(RST_BY_WDG)
  {
    warn_reset_by_wdg();
    
    beep_on(BEPP_HZ, BEEP_LONG);
    
    while(!(event_bt_stop||event_bt_start||event_bt_minus||event_bt_plus))
    {
      delay_ms(CYCLE_TIME);
      IWDG->KR = IWDG_KEY_REFRESH;
      scan_key();
    }
    
    event_bt_stop=0;
    event_bt_start=0;
    event_bt_minus=0;
    event_bt_plus=0;
  }
  
  //FLASH_Unlock(FLASH_MEMTYPE_DATA);
  //config->LiPoTargetV=4200;
  //config->LiIoTargetV=4100;
  //config->LiFeTargetV=3600;
  //config->LiHvTargetV=4350;
  //config->VBAT_CAL2=925;
  //FLASH_Lock(FLASH_MEMTYPE_DATA);
  //uint8_t ss=0;
  
  lcd_clear();
  
  lcd_gotoxy(0,0);
  lcd_putsf("PROGRAM SELECT");
  
  uint16_t start = tick_ms;
  for(;;)
  {
    if((tick_ms - start) < CYCLE_TIME)
    {
      wfi();
      continue;
    }
    start+= CYCLE_TIME;
    
    uint16_t sw_start = tick_ms;
    
    scan_adc();
    
    scan_key();
    
    int8_t r = 0;
    switch(state)
    {
    case PROG_LI:
      r=prog_li();
      break;
      
    case PROG_NI:
      r=prog_ni();
      break;
      
    case PROG_PB:
      r=prog_pb();
      break;
      
    case PROG_US:
      r=prog_us();
      break;
      
    case PROG_EF:
      r=prog_ef();
      break;
      
    case PROG_LM:
      r=prog_lm();
      break;
    }
    
    if(r>0)
    {
      if(state==PROG_MAX)
      {
        state=0;
      }
      else
      {
        state++;
      }
    }else if(r<0)
    {
      if(state)
      {
        state--;
      }
      else
      {
        state=PROG_MAX;
      }
    }
    
    event_bt_stop=0;
    event_bt_start=0;
    event_bt_minus=0;
    event_bt_plus=0;
    
    uint16_t sw_elapsed = tick_ms - sw_start;
    //lcd_gotoxy(0,1);
    //char buff[16];
    //sprintf(buff,"%u   ", i_charge);
    //lcd_gotoxy(0,0);
    //lcd_putsf(buff);
    //sprintf(buff,"%hu   ", sw_elapsed);
    //lcd_gotoxy(0,1);
    //lcd_putsf(buff);
    
    /* Reload IWDG counter */
    IWDG->KR = IWDG_KEY_REFRESH;
  }
}
