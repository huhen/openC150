#include "charge.h"
#include "hardware.h"

static uint16_t pulse_charge_down = 0;
static uint16_t pulse_charge_up = 0;

static void charge_set_down()
{
  /* Set the Pulse value */
  TIM1->CCR1H = (uint8_t)(pulse_charge_down >> 8);
  TIM1->CCR1L = (uint8_t)(pulse_charge_down);
}

static void charge_set_up()
{
  /* Set the Pulse value */
  TIM1->CCR2H = (uint8_t)(pulse_charge_up >> 8);
  TIM1->CCR2L = (uint8_t)(pulse_charge_up);
}

void charge_off()
{
  pulse_charge_up = 0;
  charge_set_up();
  
  pulse_charge_down = 0;
  charge_set_down();
}

void charge_up()
{
  if(pulse_charge_down<=TIM1_PERIOD)
  {
    pulse_charge_down++;
    charge_set_down();
  }
  else if(pulse_charge_up<=TIM1_PERIOD)
  {
    pulse_charge_up++;
    charge_set_up();
  }
}

void charge_down()
{
  if(pulse_charge_up)
  {
    pulse_charge_up--;
    charge_set_up();
  }
  else if(pulse_charge_down) 
  {
    pulse_charge_down--;
    charge_set_down();
  }
}
