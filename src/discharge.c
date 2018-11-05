#include "discharge.h"
#include "hardware.h"

static uint16_t pulse_discharge = 0;

static void discharge_set()
{
  /* Set the Pulse value */
  TIM1->CCR3H = (uint8_t)(pulse_discharge >> 8);
  TIM1->CCR3L = (uint8_t)(pulse_discharge);
}

void discharge_off()
{
  pulse_discharge = 0;
  discharge_set();
}

void discharge_up()
{
  if(pulse_discharge<=TIM1_PERIOD) pulse_discharge++;
  discharge_set();
}

void discharge_down()
{
  if(pulse_discharge) pulse_discharge--;
  discharge_set();
}
