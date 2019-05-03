/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HARDWARE_H
#define __HARDWARE_H

#define FIX_BROKEN_PIN

#include "pin.h"

extern bool RstByIwdg;

typedef Pin<'D', 0> TPinBeep;
extern TPinBeep PinBeep;

typedef Pin<'G', 5> TPinCooler;
extern TPinCooler PinCooler;

typedef Pin<'C', 7> TPinBatKey;
extern TPinBatKey PinBatKey;

typedef Pin<'C', 5> TPinInputDC;
extern TPinInputDC PinInputDC;

typedef Pin<'A', 4> TPinUartRx;
typedef Pin<'A', 5> TPinUartTx;
extern TPinUartRx PinUartRx;
extern TPinUartTx PinUartTx;

typedef Pin<'C', 1> TPinPwmChargeDown;
typedef Pin<'C', 2> TPinPwmChargeUp;
typedef Pin<'C', 3> TPinPwmDischarge;
extern TPinPwmChargeDown PinPwmChargeDown;
extern TPinPwmChargeUp PinPwmChargeUp;
extern TPinPwmDischarge PinPwmDischarge;

typedef Pin<'F', 7> TPinChargeDisable;
extern TPinChargeDisable PinChargeDisable;

typedef Pin<'E', 0> TPinLcdDb4;
typedef Pin<'E', 1> TPinLcdDb5;
typedef Pin<'E', 2> TPinLcdDb6;
typedef Pin<'E', 3> TPinLcdDb7;
typedef Pin<'E', 4> TPinLcdRs;
typedef Pin<'E', 5> TPinLcdE;
extern TPinLcdDb4 PinLcdDb4;
extern TPinLcdDb5 PinLcdDb5;
extern TPinLcdDb6 PinLcdDb6;
extern TPinLcdDb7 PinLcdDb7;
extern TPinLcdRs PinLcdRs;
extern TPinLcdE PinLcdE;

typedef Pin<'I', 0> TPinBalCell1;
typedef Pin<'G', 4> TPinBalCell2;
typedef Pin<'G', 0> TPinBalCell3;
typedef Pin<'G', 1> TPinBalCell4;
typedef Pin<'G', 2> TPinBalCell5;
typedef Pin<'G', 3> TPinBalCell6;
extern TPinBalCell1 PinBalCell1;
extern TPinBalCell2 PinBalCell2;
extern TPinBalCell3 PinBalCell3;
extern TPinBalCell4 PinBalCell4;
extern TPinBalCell5 PinBalCell5;
extern TPinBalCell6 PinBalCell6;

typedef Pin<'B', 0> TPinAdcVcc;
typedef Pin<'B', 1> TPinAdcIn1;
typedef Pin<'B', 2> TPinAdcVbat;
typedef Pin<'B', 3> TPinAdcCell1;
typedef Pin<'B', 4> TPinAdcCell2;
typedef Pin<'B', 5> TPinAdcCell3;
typedef Pin<'B', 6> TPinAdcCell4;
typedef Pin<'B', 7> TPinAdcCell5;
//typedef Pin<'E', 7> TPinAdc8;
//typedef Pin<'E', 6> TPinAdc9;
typedef Pin<'F', 0> TPinAdcCell6;
typedef Pin<'F', 3> TPinAdcCurrDischage;
typedef Pin<'F', 4> TPinAdcCurrCharge;
typedef Pin<'F', 5> TPinAdcTempInt;
typedef Pin<'F', 6> TPinAdcTempExt;
//typedef Pin<'F', 7> TPinAdc15;
extern TPinAdcVcc PinAdcVcc;
extern TPinAdcIn1 PinAdcIn1;
extern TPinAdcVbat PinAdcVbat;
extern TPinAdcCell1 PinAdcCell1;
extern TPinAdcCell2 PinAdcCell2;
extern TPinAdcCell3 PinAdcCell3;
extern TPinAdcCell4 PinAdcCell4;
extern TPinAdcCell5 PinAdcCell5;
//extern TPinAdc8 PinAdc8;
//extern TPinAdc9 PinAdc9;
extern TPinAdcCell6 PinAdcCell6;
extern TPinAdcCurrDischage PinAdcCurrDischage;
extern TPinAdcCurrCharge PinAdcCurrCharge;
extern TPinAdcTempInt PinAdcTempInt;
extern TPinAdcTempExt PinAdcTempExt;
//extern TPinAdc15 PinAdc15;

typedef Pin<'D', 7, 'L'> TPinStart;
typedef Pin<'D', 6, 'L'> TPinPlus;
typedef Pin<'D', 5, 'L'> TPinMinus;
#ifdef FIX_BROKEN_PIN
typedef Pin<'D', 3, 'L'> TPinStop;
#else
typedef Pin<'D', 4, 'L'> TPinStop;
#endif
extern TPinStart PinStart;
extern TPinPlus PinPlus;
extern TPinMinus PinMinus;
extern TPinStop PinStop;

typedef Pin<'A', 1> TPinNC1;
typedef Pin<'A', 2> TPinNC2;
typedef Pin<'A', 3> TPinNC3;
typedef Pin<'A', 6> TPinNC4;
typedef Pin<'C', 4> TPinNC5;
typedef Pin<'C', 6> TPinNC6;
typedef Pin<'D', 2> TPinNC7;
#ifdef FIX_BROKEN_PIN
typedef Pin<'D', 4> TPinNC8;
#else
typedef Pin<'D', 3> TPinNC8;
#endif
typedef Pin<'E', 6> TPinNC9;
typedef Pin<'E', 7> TPinNC10;
typedef Pin<'G', 6> TPinNC11;
typedef Pin<'G', 7> TPinNC12;
extern TPinNC1 PinNC1;
extern TPinNC2 PinNC2;
extern TPinNC3 PinNC3;
extern TPinNC4 PinNC4;
extern TPinNC5 PinNC5;
extern TPinNC6 PinNC6;
extern TPinNC7 PinNC7;
extern TPinNC8 PinNC8;
extern TPinNC9 PinNC9;
extern TPinNC10 PinNC10;
extern TPinNC11 PinNC11;
extern TPinNC12 PinNC12;

/* Exported macro ------------------------------------------------------------*/
#define ADC_VCC_IN_CHAN          (0)
#define ADC_IN1_CHAN             (1)
#define ADC_VBAT_CHAN            (2)
#define ADC_CELL_1_CHAN          (3)
#define ADC_CELL_2_CHAN          (4)
#define ADC_CELL_3_CHAN          (5)
#define ADC_CELL_4_CHAN          (6)
#define ADC_CELL_5_CHAN          (7)
//#define ADC_IN8_CHAN             (8)
//#define ADC_IN9_CHAN             (9)
#define ADC_CELL_6_CHAN          (10)
#define ADC_CURRENT_DISCHARGE_CHAN       (11)
#define ADC_CURRENT_CHARGE_CHAN  (12)
#define ADC_TEMP_INT_CHAN        (13)
#define ADC_TEMP_EXT_CHAN        (14)
//#define ADC_IN15_CHAN             (15)


#define TIM1_PERIOD             (500)
#define CYCLE_TIME              (20)
#define CYCLE_PER_1S            (1000/CYCLE_TIME)
#define I_HYSTER                (50)
#define CELL_BALL_HYSTER        (10)

struct EPROM
{
  int16_t   VCC_CAL1; //587 - 11000mv
  int16_t   VCC_CAL2; //907 - 17000mv
  
  int16_t   VBAT_CAL1; //28 - 800mv
  int16_t   VBAT_CAL2; //928 - 25200mv
  
  int16_t   VCELL_1_CAL1; //162 - 800mv
  int16_t   VCELL_1_CAL2; //859 - 4200mv
  int16_t   VCELL_2_CAL1; //162 - 800mv
  int16_t   VCELL_2_CAL2; //859 - 4200mv
  int16_t   VCELL_3_CAL1; //162 - 800mv
  int16_t   VCELL_3_CAL2; //859 - 4200mv
  int16_t   VCELL_4_CAL1; //162 - 800mv
  int16_t   VCELL_4_CAL2; //859 - 4200mv
  int16_t   VCELL_5_CAL1; //162 - 800mv
  int16_t   VCELL_5_CAL2; //859 - 4200mv
  int16_t   VCELL_6_CAL1; //162 - 800mv
  int16_t   VCELL_6_CAL2; //859 - 4200mv
  
  int16_t   DISCHARGE_CAL1; //64 - 200ma
  int16_t   DISCHARGE_CAL2; //639 - 2000ma
  int16_t   CHARGE_CAL1; //10 - 200ma
  int16_t   CHARGE_CAL2; //
  
  uint8_t   LiType;
  uint8_t   LiMode;
  uint16_t  LiCurrent;
  uint8_t   LiNumS;
  
  uint16_t   LiPoTargetV;
  uint16_t   LiIoTargetV;
  uint16_t   LiFeTargetV;
  uint16_t   LiHvTargetV;
};

extern struct EPROM *config;

#endif /* __HARDWARE_H */
