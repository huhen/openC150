#include "stm8s.h"
#include "hardware.h"
#include "charger.h"

uint8_t RST_BY_WDG = 0;

static void CLK_Config(void);
static void GPIO_Config(void);
static void IWDG_Config(void);
static void UART_Config(void);
static void TIM2_Config(void);
static void TIM3_Config(void);
static void TIM4_Config(void);
static void ADC_Config(void);
static uint32_t LSIMeasurment(void);

/**
* @brief  Main program.
* @param  None
* @retval None
*/
void main(void)
{
  /* Clock Configuration */
  CLK_Config(); 
  
  /* GPIO Configuration */
  GPIO_Config();
  
  /* Check if the system has resumed from IWDG reset */
  if (RST_GetFlagStatus(RST_FLAG_IWDGF) != RESET)
  {
    /* IWDGF flag set */
    RST_BY_WDG++;
    
    /* Clear IWDGF Flag */
    RST_ClearFlag(RST_FLAG_IWDGF);
  }  
  
  /* IWDG Configuration */
  IWDG_Config();
  
  /* TIM2 configuration */
  TIM2_Config();
  
  /* TIM3 configuration */
  TIM3_Config();
  
  /* TIM4 configuration */
  TIM4_Config();  
  
  /* UART configuration */
  UART_Config();
  
  /* ADC configuration */
  ADC_Config();
  
  /* enable interrupts */
  enableInterrupts();
  
  charger_start();
  
  for(;;);
}

/**
* @brief  Configure ADC2 Continuous Conversion with End Of Conversion interrupt 
*         enabled .
* @param  None
* @retval None
*/
static void ADC_Config()
{
  /* De-Init ADC peripheral*/
  ADC2_DeInit();
  
  /* Init ADC2 peripheral */
  ADC2_Init(ADC2_CONVERSIONMODE_CONTINUOUS, ADC2_CHANNEL_0, ADC2_PRESSEL_FCPU_D18, \
    ADC2_EXTTRIG_TIM, DISABLE, ADC2_ALIGN_RIGHT, ADC2_SCHMITTTRIG_ALL,\
      DISABLE);
  
  /* Enable EOC interrupt */
  ADC2->CSR |= (uint8_t)ADC2_CSR_EOCIE;
            
  /*Start Conversion */
  ADC2->CR1 |= ADC2_CR1_ADON;
}

/**
* @brief  Configure TIM4
* @param  None
* @retval None
*/
static void TIM4_Config(void)
{
  TIM4_DeInit();
  
  TIM4_TimeBaseInit(TIM4_PRESCALER_16, 255);
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  TIM4_Cmd(ENABLE);   
}

/**
* @brief  Configure TIM3
* @param  None
* @retval None
*/
static void TIM3_Config(void)
{
  TIM3_DeInit();
  
  TIM3_TimeBaseInit(TIM3_PRESCALER_16, 500);
  
  // PWM1 Mode configuration: Channel2
  TIM3_OC2Init(TIM3_OCMODE_PWM1, TIM3_OUTPUTSTATE_ENABLE, 0, TIM3_OCPOLARITY_HIGH);
  TIM3_OC2PreloadConfig(ENABLE);
  
  TIM3_ARRPreloadConfig(ENABLE);
  
  TIM3_Cmd(ENABLE); 
}

/**
* @brief  Configure Output Compare InActive Mode for TIM2 Channel1 1ms tick
* @param  None
* @retval None
*/
static void TIM2_Config(void)
{
  TIM2_DeInit();
  
  /* Time base configuration */
  TIM2_TimeBaseInit(TIM2_PRESCALER_16, 1001);
  
  /* Prescaler configuration */
  TIM2_PrescalerConfig(TIM2_PRESCALER_16, TIM2_PSCRELOADMODE_IMMEDIATE);
  
  /* Output Compare InActive Mode configuration: Channel1 */
  TIM2_OC1Init(TIM2_OCMODE_INACTIVE, TIM2_OUTPUTSTATE_ENABLE, 1001, TIM2_OCPOLARITY_HIGH); 
  TIM2_OC1PreloadConfig(DISABLE);
  
  /* Output Compare InActive Mode configuration: Channel2 */
  //TIM2_OC2Init(TIM2_OCMODE_INACTIVE, TIM2_OUTPUTSTATE_ENABLE, 10000, TIM2_OCPOLARITY_HIGH); 
  //TIM2_OC2PreloadConfig(DISABLE);
  
  /* Output Compare InActive Mode configuration: Channel3 */
  //TIM2_OC3Init(TIM2_OCMODE_INACTIVE, TIM2_OUTPUTSTATE_ENABLE, 65535, TIM2_OCPOLARITY_HIGH); 
  //TIM2_OC3PreloadConfig(DISABLE);
  
  TIM2_ARRPreloadConfig(ENABLE);
  
  /* TIM IT enable */
  TIM2_ITConfig(TIM2_IT_CC1, ENABLE);
  //TIM2_ITConfig(TIM2_IT_CC2, ENABLE);
  //TIM2_ITConfig(TIM2_IT_CC3, ENABLE);
  
  /* TIM2 enable counter */
  TIM2_Cmd(ENABLE);
}

/**
* @brief  Initialize GPIO
* @param  None
* @retval None
*/
static void GPIO_Config(void) 
{
  /* Not using pins as input with pullup */
  GPIO_Init(GPIOA, (GPIO_Pin_TypeDef)(GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_6), GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(GPIOC, (GPIO_Pin_TypeDef)(GPIO_PIN_4 | GPIO_PIN_6), GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(GPIOD, (GPIO_Pin_TypeDef)(GPIO_PIN_2 | GPIO_PIN_3), GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(GPIOE, (GPIO_Pin_TypeDef)(GPIO_PIN_6 | GPIO_PIN_7), GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(GPIOG, (GPIO_Pin_TypeDef)(GPIO_PIN_6 | GPIO_PIN_7), GPIO_MODE_IN_PU_NO_IT);
  
  /* Init pins */
  GPIO_Init(LCD_RS_PORT, LCD_RS_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  WriteLow(LCD_RS_PORT, LCD_RS_PIN);
  GPIO_Init(LCD_E_PORT, LCD_E_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  WriteLow(LCD_E_PORT, LCD_E_PIN);
  GPIO_Init(LCD_DB4_PORT, LCD_DB4_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  WriteLow(LCD_DB4_PORT, LCD_DB4_PIN);
  GPIO_Init(LCD_DB5_PORT, LCD_DB5_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  WriteLow(LCD_DB5_PORT, LCD_DB5_PIN);
  GPIO_Init(LCD_DB6_PORT, LCD_DB6_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  WriteLow(LCD_DB6_PORT, LCD_DB6_PIN);
  GPIO_Init(LCD_DB7_PORT, LCD_DB7_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  WriteLow(LCD_DB7_PORT, LCD_DB7_PIN);
  
  GPIO_Init(COOLER_PORT, COOLER_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  COOL_ON;
  
  GPIO_Init(BEEP_PORT, BEEP_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  WriteLow(BEEP_PORT, BEEP_PIN);
  
  GPIO_Init(BAT_KEY_PORT, BAT_KEY_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  BAT_KEY_OFF;
  
  GPIO_Init(INPUT_DC_KEY_PORT, INPUT_DC_KEY_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  INPUT_DC_KEY_OFF;
  
  GPIO_Init(BAL_CELL_1_PORT, BAL_CELL_1_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  BAL_CELL_1_OFF;
  GPIO_Init(BAL_CELL_2_PORT, BAL_CELL_2_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  BAL_CELL_2_OFF;
  GPIO_Init(BAL_CELL_3_PORT, BAL_CELL_3_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  BAL_CELL_3_OFF;
  GPIO_Init(BAL_CELL_4_PORT, BAL_CELL_4_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  BAL_CELL_4_OFF;
  GPIO_Init(BAL_CELL_5_PORT, BAL_CELL_5_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  BAL_CELL_5_OFF;
  GPIO_Init(BAL_CELL_6_PORT, BAL_CELL_6_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  BAL_CELL_6_OFF;
  
  GPIO_Init(PWM_CHARGE_DOWN_PORT, PWM_CHARGE_DOWN_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  WriteLow(PWM_CHARGE_DOWN_PORT, PWM_CHARGE_DOWN_PIN);
  GPIO_Init(PWM_CHARGE_UP_PORT, PWM_CHARGE_UP_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  WriteHigh(PWM_CHARGE_UP_PORT, PWM_CHARGE_UP_PIN);
  GPIO_Init(PWM_DISCHARGE_PORT, PWM_DISCHARGE_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  WriteLow(PWM_DISCHARGE_PORT, PWM_DISCHARGE_PIN);
  
  GPIO_Init(BT_START_PORT, BT_START_PIN, GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(BT_PLUS_PORT, BT_PLUS_PIN, GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(BT_MINUS_PORT, BT_MINUS_PIN, GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(BT_STOP_PORT, BT_STOP_PIN, GPIO_MODE_IN_FL_NO_IT);
  
  GPIO_Init(ADC_VCC_IN_PORT, ADC_VCC_IN_PIN, GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(ADC_IN1_PORT, ADC_IN1_PIN, GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(ADC_VBAT_PORT, ADC_VBAT_PIN, GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(ADC_CELL_1_PORT, ADC_CELL_1_PIN, GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(ADC_CELL_2_PORT, ADC_CELL_2_PIN, GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(ADC_CELL_3_PORT, ADC_CELL_3_PIN, GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(ADC_CELL_4_PORT, ADC_CELL_4_PIN, GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(ADC_CELL_5_PORT, ADC_CELL_5_PIN, GPIO_MODE_IN_FL_NO_IT);
  //GPIO_Init(ADC_IN8_PORT, ADC_IN8_PIN, GPIO_MODE_IN_FL_NO_IT);
  //GPIO_Init(ADC_IN9_PORT, ADC_IN9_PIN, GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(ADC_CELL_6_PORT, ADC_CELL_6_PIN, GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(ADC_CURRENT_DISCHARGE_PORT, ADC_CURRENT_DISCHARGE_PIN, GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(ADC_CURRENT_CHARGE_PORT, ADC_CURRENT_CHARGE_PIN, GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(ADC_TEMP_INT_PORT, ADC_TEMP_INT_PIN, GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(ADC_TEMP_EXT_PORT, ADC_TEMP_EXT_PIN, GPIO_MODE_IN_FL_NO_IT);
  GPIO_Init(ADC_5V_PORT, ADC_5V_PIN, GPIO_MODE_IN_FL_NO_IT);
}

/**
* @brief  UART1 Configuration for interrupt communication
* @param  None
* @retval None
*/
static void UART_Config(void)
{
  /* Deinitializes the UART1 peripheral */
  UART1_DeInit();
  /* UART1 configuration -------------------------------------------------*/
  /* UART1 configured as follow:
  - BaudRate = 9600 baud  
  - Word Length = 8 Bits
  - One Stop Bit
  - No parity
  - Receive and transmit enabled
  - UART1 Clock disabled
  */
  /* Configure the UART1 */
  UART1_Init((uint32_t)9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO,
             UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
  
  ///* Enable UART1 Transmit interrupt*/
  //UART1_ITConfig(UART1_IT_TXE, ENABLE); 
  /* Enable UART1 Receive interrupt */
  UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
}

/**
* @brief  Configure system clock to run at Maximum clock speed
* @param  None
* @retval None
*/
static void CLK_Config(void)
{
  CLK_DeInit();
  
  /* Configure the Fcpu to DIV1*/
  CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
  
  /* Configure the HSI prescaler to the optimal value */
  CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
  
  /* Configure the system clock to use HSI clock source and to run at 16Mhz */
  CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);
}

/**
* @brief  Configures the IWDG to generate a Reset if it is not refreshed at the
*         correct time. 
* @param  None
* @retval None
*/
static void IWDG_Config(void)
{
  /* Get measured LSI frequency */
  uint32_t LsiFreq = LSIMeasurment();
  
#ifndef DEBUG
  /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
  IWDG_Enable();
  
  /* IWDG timeout equal to 250 ms (the timeout may varies due to LSI frequency
  dispersion) */
  /* Enable write access to IWDG_PR and IWDG_RLR registers */
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  
  /* IWDG counter clock: LSI/128 */
  IWDG_SetPrescaler(IWDG_Prescaler_128);
  
  /* Set counter reload value to obtain 250ms IWDG Timeout.
  Counter Reload Value = 250ms/IWDG counter clock period
  = 250ms / (LSI/128)
  = 0.25s / (LsiFreq/128)
  = LsiFreq/(128 * 4)
  = LsiFreq/512
  */
  IWDG_SetReload((uint8_t)(LsiFreq/512));
  
  /* Reload IWDG counter */
  IWDG->KR = IWDG_KEY_REFRESH;
#endif /* !DEBUG */
}

/**
* @brief  Measure the LSI frequency using timer IC1 and update the calibration registers.
* @note   It is recommended to use a timer clock frequency of at least 10MHz in order 
*         to obtain a better in the LSI frequency measurement.
* @param  None
* @retval None
*/
static uint32_t LSIMeasurment(void)
{
  uint32_t lsi_freq_hz = 0x0;
  uint32_t fmaster = 0x0;
  uint16_t ICValue1 = 0x0;
  uint16_t ICValue2 = 0x0;
  
  /* Get master frequency */
  fmaster = CLK_GetClockFreq();
  
  /* Enable the LSI measurement: LSI clock connected to timer Input Capture 1 */
  AWU->CSR |= AWU_CSR_MSR;
  
  /* Measure the LSI frequency with TIMER Input Capture 1 */
  
  /* Capture only every 8 events!!! */
  /* Enable capture of TI1 */
  TIM3_ICInit(TIM3_CHANNEL_1, TIM3_ICPOLARITY_RISING, TIM3_ICSELECTION_DIRECTTI,
              TIM3_ICPSC_DIV8, 0);
  
  /* Enable TIM3 */
  TIM3_Cmd(ENABLE);
  
  /* wait a capture on cc1 */
  while ((TIM3->SR1 & TIM3_FLAG_CC1) != TIM3_FLAG_CC1);
  /* Get CCR1 value*/
  ICValue1 = TIM3_GetCapture1();
  TIM3_ClearFlag(TIM3_FLAG_CC1);
  
  /* wait a capture on cc1 */
  while ((TIM3->SR1 & TIM3_FLAG_CC1) != TIM3_FLAG_CC1);
  /* Get CCR1 value*/
  ICValue2 = TIM3_GetCapture1();
  TIM3_ClearFlag(TIM3_FLAG_CC1);
  
  /* Disable IC1 input capture */
  TIM3->CCER1 &= (uint8_t)(~TIM3_CCER1_CC1E);
  /* Disable timer3 */
  TIM3_Cmd(DISABLE);
  
  /* Compute LSI clock frequency */
  lsi_freq_hz = (8 * fmaster) / (ICValue2 - ICValue1);
  
  /* Disable the LSI measurement: LSI clock disconnected from timer Input Capture 1 */
  AWU->CSR &= (uint8_t)(~AWU_CSR_MSR);
  
  return (lsi_freq_hz);
}

#ifdef USE_FULL_ASSERT
/**
* @brief  Reports the name of the source file and the source line number
*   where the assert_param error has occurred.
* @param file: pointer to the source file name
* @param line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  
  /* Infinite loop */
  while (1)
  {
  }
}
#endif
