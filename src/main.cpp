//******************************************************************************
//*
//*     FULLNAME:  Single-Chip Microcontroller Real-Time Operating System
//*
//*     NICKNAME:  scmRTOS
//*
//*     PROCESSOR: STM8 (STMicroelectronics)
//*
//*     TOOLKIT:   EWSTM8 (IAR Systems)
//*
//*     PURPOSE:   STM8/IAR Port Test File
//*
//*     Version: v5.1.0
//*
//*
//*     Copyright (c) 2003-2016, scmRTOS Team
//*
//*     Permission is hereby granted, free of charge, to any person
//*     obtaining  a copy of this software and associated documentation
//*     files (the "Software"), to deal in the Software without restriction,
//*     including without limitation the rights to use, copy, modify, merge,
//*     publish, distribute, sublicense, and/or sell copies of the Software,
//*     and to permit persons to whom the Software is furnished to do so,
//*     subject to the following conditions:
//*
//*     The above copyright notice and this permission notice shall be included
//*     in all copies or substantial portions of the Software.
//*
//*     THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//*     EXPRESS  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//*     MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//*     IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//*     CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//*     TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH
//*     THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//*
//*     =================================================================
//*     Project sources: https://github.com/scmrtos/scmrtos
//*     Documentation:   https://github.com/scmrtos/scmrtos/wiki/Documentation
//*     Wiki:            https://github.com/scmrtos/scmrtos/wiki
//*     Sample projects: https://github.com/scmrtos/scmrtos-sample-projects
//*     =================================================================
//*
//******************************************************************************
//*     STM8/IAR port by Oleksandr O. Redchuk, Copyright (c) 2011-2016

//---------------------------------------------------------------------------
#include <scmRTOS.h>

#include "hardware.h"


//---------------------------------------------------------------------------
//  "Hello, scope!" pins
Pin<'B', 0> IDLE_HOOK;
Pin<'B', 1> PROC1;
Pin<'B', 2> PROC2;
Pin<'B', 3> PROC3;
Pin<'B', 4> TIMER_HOOK;
Pin<'B', 5> TIMER3_ISR;
Pin<'B', 6> TIMER3_TO_PROC1;

//---------------------------------------------------------------------------
//
//      Process types
//
// demonstrate process switching from Proc2 to Proc3 in sleep() or ef.signal() call

typedef OS::process<OS::pr0, 200> TProcMain;
typedef OS::process<OS::pr1, 200> TProc2;
typedef OS::process<OS::pr2, 200> TProc3;

template<> void TProcMain::exec();
template<> void TProc2::exec();
template<> void TProc3::exec();

//---------------------------------------------------------------------------
//
//      Process objects
//
TProcMain ProcMain;
TProc2 Proc2;
TProc3 Proc3;

//---------------------------------------------------------------------------
// System timer setup parameters
static const uint32_t systick_freq = 1000;
static const uint8_t  tim4_prescaler_power = 6;
static const uint32_t tim4_divider = (1UL << tim4_prescaler_power) * systick_freq;

//---------------------------------------------------------------------------
tick_count_t tick_count;        // global variable for OS::GetTickCount testing

OS::TEventFlag Timer3_Ovf;      // set in Timer3_period_ISR(), waited in Proc1
OS::TEventFlag ef;              // set in Proc3, waited in Proc2

/**
* @brief  Configure system clock to run at Maximum clock speed
* @param  None
* @retval None
*/
INLINE static void CLK_Config(void)
{
    /* CLK DeInit */
    CLK->ICKR = CLK_ICKR_RESET_VALUE;
    CLK->ECKR = CLK_ECKR_RESET_VALUE;
    CLK->SWR  = CLK_SWR_RESET_VALUE;
    CLK->SWCR = CLK_SWCR_RESET_VALUE;
    CLK->CKDIVR = CLK_CKDIVR_RESET_VALUE;
    CLK->PCKENR1 = CLK_PCKENR1_RESET_VALUE;
    CLK->PCKENR2 = CLK_PCKENR2_RESET_VALUE;
    CLK->CSSR = CLK_CSSR_RESET_VALUE;
    CLK->CCOR = CLK_CCOR_RESET_VALUE;
    while ((CLK->CCOR & CLK_CCOR_CCOEN)!= 0)
    {}
    CLK->CCOR = CLK_CCOR_RESET_VALUE;
    CLK->HSITRIMR = CLK_HSITRIMR_RESET_VALUE;
    CLK->SWIMCCR = CLK_SWIMCCR_RESET_VALUE;
    
    /* Configure the Fcpu to DIV1*/
    CLK->CKDIVR &= (uint8_t)(~CLK_CKDIVR_CPUDIV);
    CLK->CKDIVR |= (uint8_t)((uint8_t)CLK_PRESCALER_CPUDIV1 & (uint8_t)CLK_CKDIVR_CPUDIV);
    
    /* Configure the HSI prescaler to the optimal value */
    CLK->CKDIVR &= (uint8_t)(~CLK_CKDIVR_HSIDIV);
    CLK->CKDIVR |= (uint8_t)((uint8_t)CLK_PRESCALER_HSIDIV1 & (uint8_t)CLK_CKDIVR_HSIDIV);
    
    /* Configure the system clock to use HSI clock source and to run at 16Mhz */
    /* Current clock master saving */
    CLK_Source_TypeDef clock_master = (CLK_Source_TypeDef)CLK->CMSR;
    
    /* Automatic switch mode management */
    /* Enables Clock switch */
    CLK->SWCR |= CLK_SWCR_SWEN;
    
    /* Disable Switch interrupt */
    CLK->SWCR &= (uint8_t)(~CLK_SWCR_SWIEN);
    
    /* Selection of the target clock source */
    CLK->SWR = (uint8_t)CLK_SOURCE_HSI;
    
    /* Wait until the target clock source is ready */
    uint16_t downCounter = CLK_TIMEOUT;
    while((((CLK->SWCR & CLK_SWCR_SWBSY) != 0 )&& (downCounter != 0)))
    {
        downCounter--;
    }
    
    if(downCounter != 0)
    {
        /* Switch OFF current clock if required */
        if( clock_master == CLK_SOURCE_HSI)
        {
            CLK->ICKR &= (uint8_t)(~CLK_ICKR_HSIEN);
        }
        else if( clock_master == CLK_SOURCE_LSI)
        {
            CLK->ICKR &= (uint8_t)(~CLK_ICKR_LSIEN);
        }
        else if ( clock_master == CLK_SOURCE_HSE)
        {
            CLK->ECKR &= (uint8_t)(~CLK_ECKR_HSEEN);
        }
    }
}

/**
* @brief  Configures the IWDG to generate a Reset if it is not refreshed at the
*         correct time. 
* @param  None
* @retval None
*/
INLINE static void IWDG_Config(void)
{
    /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
    IWDG->KR = IWDG_KEY_ENABLE;
    
    /* IWDG timeout equal to 250 ms (the timeout may varies due to LSI frequency
    dispersion) */
    /* Enable write access to IWDG_PR and IWDG_RLR registers */
    IWDG->KR = (uint8_t)IWDG_WriteAccess_Enable;
    
    /* IWDG counter clock: LSI/128 */
    IWDG->PR = (uint8_t)IWDG_Prescaler_128;
    
    /* Set counter reload value to obtain 250ms IWDG Timeout.
    Counter Reload Value = 250ms/IWDG counter clock period
    */
    IWDG->RLR = (uint8_t)250;
    
    /* Disable write access to IWDG_PR and IWDG_RLR registers */
    IWDG->KR = (uint8_t)IWDG_WriteAccess_Disable;
    
    /* Reload IWDG counter */
    IWDG->KR = IWDG_KEY_REFRESH;
}

/**
* @brief  Initialize GPIO
* @param  None
* @retval None
*/
INLINE static void GPIO_Config(void) 
{
    /* Init pins */
    PinCooler.Mode(GPIO_MODE_OUT_PP_LOW_FAST);
    PinCooler.On();
    
    PinBeep.Mode(GPIO_MODE_OUT_PP_LOW_FAST);
    PinBeep.Off();
    
    PinBatKey.Mode(GPIO_MODE_OUT_PP_LOW_FAST);
    PinBatKey.Off();
    
    PinInputDC.Mode(GPIO_MODE_OUT_PP_LOW_FAST);
    PinInputDC.Off();
    
    PinChargeDisable.Mode(GPIO_MODE_OUT_PP_LOW_FAST);
    PinChargeDisable.On();
    
    PinPwmChargeDown.Mode(GPIO_MODE_OUT_PP_LOW_FAST);
    PinPwmChargeUp.Mode(GPIO_MODE_OUT_PP_LOW_FAST);
    PinPwmDischarge.Mode(GPIO_MODE_OUT_PP_LOW_FAST);
    PinPwmChargeDown.Off();
    PinPwmChargeUp.On();
    PinPwmDischarge.Off();
    
    PinLcdDb4.Mode(GPIO_MODE_OUT_PP_LOW_FAST);
    PinLcdDb5.Mode(GPIO_MODE_OUT_PP_LOW_FAST);
    PinLcdDb6.Mode(GPIO_MODE_OUT_PP_LOW_FAST);
    PinLcdDb7.Mode(GPIO_MODE_OUT_PP_LOW_FAST);
    PinLcdRs.Mode(GPIO_MODE_OUT_PP_LOW_FAST);
    PinLcdE.Mode(GPIO_MODE_OUT_PP_LOW_FAST);
    PinLcdDb4.Off();
    PinLcdDb5.Off();
    PinLcdDb6.Off();
    PinLcdDb7.Off();
    PinLcdRs.Off();
    PinLcdE.Off();
    
    PinBalCell1.Mode(GPIO_MODE_OUT_PP_LOW_FAST);
    PinBalCell2.Mode(GPIO_MODE_OUT_PP_LOW_FAST);
    PinBalCell3.Mode(GPIO_MODE_OUT_PP_LOW_FAST);
    PinBalCell4.Mode(GPIO_MODE_OUT_PP_LOW_FAST);
    PinBalCell5.Mode(GPIO_MODE_OUT_PP_LOW_FAST);
    PinBalCell6.Mode(GPIO_MODE_OUT_PP_LOW_FAST);
    PinBalCell1.Off();
    PinBalCell2.Off();
    PinBalCell3.Off();
    PinBalCell4.Off();
    PinBalCell5.Off();
    PinBalCell6.Off();
    
    PinStart.Mode(GPIO_MODE_IN_FL_NO_IT);
    PinPlus.Mode(GPIO_MODE_IN_FL_NO_IT);
    PinMinus.Mode(GPIO_MODE_IN_FL_NO_IT);
    PinStop.Mode(GPIO_MODE_IN_FL_NO_IT);
    
    PinAdcVcc.Mode(GPIO_MODE_IN_FL_NO_IT);
    PinAdcIn1.Mode(GPIO_MODE_IN_FL_NO_IT);
    PinAdcVbat.Mode(GPIO_MODE_IN_FL_NO_IT);
    PinAdcCell1.Mode(GPIO_MODE_IN_FL_NO_IT);
    PinAdcCell2.Mode(GPIO_MODE_IN_FL_NO_IT);
    PinAdcCell3.Mode(GPIO_MODE_IN_FL_NO_IT);
    PinAdcCell4.Mode(GPIO_MODE_IN_FL_NO_IT);
    PinAdcCell5.Mode(GPIO_MODE_IN_FL_NO_IT);
    //PinAdc8.Mode(GPIO_MODE_IN_FL_NO_IT);
    //PinAdc9.Mode(GPIO_MODE_IN_FL_NO_IT);
    PinAdcCell6.Mode(GPIO_MODE_IN_FL_NO_IT);
    PinAdcCurrDischage.Mode(GPIO_MODE_IN_FL_NO_IT);
    PinAdcCurrCharge.Mode(GPIO_MODE_IN_FL_NO_IT);
    PinAdcTempInt.Mode(GPIO_MODE_IN_FL_NO_IT);
    PinAdcTempExt.Mode(GPIO_MODE_IN_FL_NO_IT);
    //PinAdc15.Mode(GPIO_MODE_IN_FL_NO_IT);
    
    /* Not using pins as input with pullup */
    PinNC1.Mode(GPIO_MODE_IN_PU_NO_IT);
    PinNC2.Mode(GPIO_MODE_IN_PU_NO_IT);
    PinNC3.Mode(GPIO_MODE_IN_PU_NO_IT);
    PinNC4.Mode(GPIO_MODE_IN_PU_NO_IT);
    PinNC5.Mode(GPIO_MODE_IN_PU_NO_IT);
    PinNC6.Mode(GPIO_MODE_IN_PU_NO_IT);
    PinNC7.Mode(GPIO_MODE_IN_PU_NO_IT);
    PinNC8.Mode(GPIO_MODE_IN_PU_NO_IT);
    PinNC9.Mode(GPIO_MODE_IN_PU_NO_IT);
    PinNC10.Mode(GPIO_MODE_IN_PU_NO_IT);
    PinNC11.Mode(GPIO_MODE_IN_PU_NO_IT);
    PinNC12.Mode(GPIO_MODE_IN_PU_NO_IT);
}

INLINE static void SystemTimer_Config(void) 
{
    /* TIM4 DeInit */
    TIM4->CR1 = TIM4_CR1_RESET_VALUE;
    TIM4->IER = TIM4_IER_RESET_VALUE;
    TIM4->CNTR = TIM4_CNTR_RESET_VALUE;
    TIM4->PSCR = TIM4_PSCR_RESET_VALUE;
    TIM4->ARR = TIM4_ARR_RESET_VALUE;
    TIM4->SR1 = TIM4_SR1_RESET_VALUE;
    
    /* Set the Prescaler value */
    TIM4->PSCR = (uint8_t)(tim4_prescaler_power);
    /* Set the Autoreload value */
    TIM4->ARR = (uint8_t)((F_MASTER + tim4_divider/2) / tim4_divider  - 1);
    
    TIM4->SR1 = (uint8_t)(~TIM4_FLAG_UPDATE);
    
    //TIM4->EGR = 0;
    TIM4->IER |= (uint8_t)TIM4_IER_UIE;
    TIM4->CR1 |= TIM4_CR1_CEN;
}

//---------------------------------------------------------------------------
int main()
{
    /* IWDG Configuration */
    IWDG_Config();
    
    /* Check if the system has resumed from IWDG reset */
    RstByIwdg=((uint8_t)(RST->SR & RST_FLAG_IWDGF) != (uint8_t)0x00);
    if(RstByIwdg)
    {
        /* Clear IWDGF Flag */
        RST->SR = (uint8_t)RST_FLAG_IWDGF;
    }
    
    /* Clock Configuration */
    CLK_Config();
    
    /* GPIO Configuration */
    GPIO_Config();
    
    /* System Timer Configuration */
    SystemTimer_Config();
    
    OS::run();
}

//---------------------------------------------------------------------------
template<> void TProcMain::exec()
{
    for(;;) {
        sleep(1);
        
        if(PinStart.Signalled())
            PinBeep.Cpl();
        
        //if(PinStart.Signalled())
        //{
            //PinBeep.On();
            //sleep(1);
            //PinBeep.Off();
            //sleep(1000);
        //}
  
        //PROC1.Off();
        //Timer3_Ovf.wait();
        //PROC1.On();
        //TIMER3_TO_PROC1.Off();
    }
}

//---------------------------------------------------------------------------
template<> void TProc2::exec()
{
    for(;;) {
        if(PinStart.Signalled())
        {
        PinCooler.On();
        }
        else
        {
            PinCooler.Off();
        }
        sleep(10);
        //sleep(9);
        //PROC2.On();
        //sleep(1);
        //ef.signal();
        //PROC2.Off();
    }
}

//---------------------------------------------------------------------------
template<> void TProc3::exec()
{
    for(;;) {
        IWDG->KR = IWDG_KEY_REFRESH;
        sleep(10);
        //ef.wait();
        //tick_count = OS::get_tick_count();
        //PROC3.On();
        //sleep( static_cast<uint16_t>(tick_count) & 0x400  ?  2  :  3  );
        //PROC3.Off();
    }
}

//---------------------------------------------------------------------------
INTERRUPT_HANDLER(Timer3_period_ISR, ITC_IRQ_TIM3_OVF)
{
    TIMER3_ISR.On();
    TIM3->SR1 &= ~TIM3_SR1_UIF;
    
    // TODO: Compare with AVR in identical conditions (context switch scheme, ISR wrapper, F_CPU, ...)
    OS::TISRW ISRW;
    
    TIMER3_TO_PROC1.On();
    
    Timer3_Ovf.signal_isr();
    TIMER3_ISR.Off();
}

//---------------------------------------------------------------------------
#ifdef  USE_FULL_ASSERT
#ifdef __cplusplus
extern "C" {
#endif
    
    void assert_failed(uint8_t* file, uint32_t line)
    {
        for(;;);
    }
    
#ifdef __cplusplus
}
#endif
#endif /* USE_FULL_ASSERT */
