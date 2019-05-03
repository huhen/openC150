#include <scmRTOS.h>
#include "hardware.h"

bool RstByIwdg;

TPinBeep PinBeep;

TPinCooler PinCooler;

TPinBatKey PinBatKey;

TPinInputDC PinInputDC;

TPinUartRx PinUartRx;
TPinUartTx PinUartTx;

TPinPwmChargeDown PinPwmChargeDown;
TPinPwmChargeUp PinPwmChargeUp;
TPinPwmDischarge PinPwmDischarge;

TPinChargeDisable PinChargeDisable;

TPinLcdDb4 PinLcdDb4;
TPinLcdDb5 PinLcdDb5;
TPinLcdDb6 PinLcdDb6;
TPinLcdDb7 PinLcdDb7;
TPinLcdRs PinLcdRs;
TPinLcdE PinLcdE;

TPinBalCell1 PinBalCell1;
TPinBalCell2 PinBalCell2;
TPinBalCell3 PinBalCell3;
TPinBalCell4 PinBalCell4;
TPinBalCell5 PinBalCell5;
TPinBalCell6 PinBalCell6;

TPinAdcVcc PinAdcVcc;
TPinAdcIn1 PinAdcIn1;
TPinAdcVbat PinAdcVbat;
TPinAdcCell1 PinAdcCell1;
TPinAdcCell2 PinAdcCell2;
TPinAdcCell3 PinAdcCell3;
TPinAdcCell4 PinAdcCell4;
TPinAdcCell5 PinAdcCell5;
//TPinAdc8 PinAdc8;
//TPinAdc9 PinAdc9;
TPinAdcCell6 PinAdcCell6;
TPinAdcCurrDischage PinAdcCurrDischage;
TPinAdcCurrCharge PinAdcCurrCharge;
TPinAdcTempInt PinAdcTempInt;
TPinAdcTempExt PinAdcTempExt;
//TPinAdc15 PinAdc15;

TPinStart PinStart;
TPinPlus PinPlus;
TPinMinus PinMinus;
TPinStop PinStop;

TPinNC1 PinNC1;
TPinNC2 PinNC2;
TPinNC3 PinNC3;
TPinNC4 PinNC4;
TPinNC5 PinNC5;
TPinNC6 PinNC6;
TPinNC7 PinNC7;
TPinNC8 PinNC8;
TPinNC9 PinNC9;
TPinNC10 PinNC10;
TPinNC11 PinNC11;
TPinNC12 PinNC12;
