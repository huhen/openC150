/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HARDWARE_H
#define __HARDWARE_H

#define FIX_BROKEN_PIN

/* Exported macro ------------------------------------------------------------*/
#define BT_START_PIN            (GPIO_PIN_7)
#define BT_PLUS_PIN             (GPIO_PIN_6)
#define BT_MINUS_PIN            (GPIO_PIN_5)
#ifdef FIX_BROKEN_PIN
#define BT_STOP_PIN             (GPIO_PIN_3)
#else
#define BT_STOP_PIN             (GPIO_PIN_4)
#endif
#define BT_START_PORT           (GPIOD)
#define BT_PLUS_PORT            (GPIOD)
#define BT_MINUS_PORT           (GPIOD)
#define BT_STOP_PORT            (GPIOD)

#define BEEP_PORT               (GPIOD)
#define BEEP_PIN                (GPIO_PIN_0)

#define COOLER_PORT             (GPIOG)
#define COOLER_PIN              (GPIO_PIN_5)

#define BAT_KEY_PORT            (GPIOC)
#define BAT_KEY_PIN             (GPIO_PIN_7)

#define INPUT_DC_KEY_PORT       (GPIOC)
#define INPUT_DC_KEY_PIN        (GPIO_PIN_5)

#define UART1_RX_PIN            (GPIO_PIN_4)
#define UART1_TX_PIN            (GPIO_PIN_5)
#define UART1_RX_PORT           (GPIOA)
#define UART1_TX_PORT           (GPIOA)

#define PWM_CHARGE_DOWN_PIN     (GPIO_PIN_1)
#define PWM_CHARGE_UP_PIN       (GPIO_PIN_2)
#define PWM_DISCHARGE_PIN       (GPIO_PIN_3)
#define PWM_CHARGE_DOWN_PORT    (GPIOC)
#define PWM_CHARGE_UP_PORT      (GPIOC)
#define PWM_DISCHARGE_PORT      (GPIOC)

#define CHARGE_DISABLE_PIN      (GPIO_PIN_7)
#define CHARGE_DISABLE_PORT     (GPIOF)

#define LCD_RS_PIN              (GPIO_PIN_4)
#define LCD_E_PIN               (GPIO_PIN_5)
#define LCD_DB4_PIN             (GPIO_PIN_0)
#define LCD_DB5_PIN             (GPIO_PIN_1)
#define LCD_DB6_PIN             (GPIO_PIN_2)
#define LCD_DB7_PIN             (GPIO_PIN_3)
#define LCD_RS_PORT             (GPIOE)
#define LCD_E_PORT              (GPIOE)
#define LCD_DB4_PORT            (GPIOE)
#define LCD_DB5_PORT            (GPIOE)
#define LCD_DB6_PORT            (GPIOE)
#define LCD_DB7_PORT            (GPIOE)

#define BAL_CELL_1_PIN          (GPIO_PIN_0)
#define BAL_CELL_2_PIN          (GPIO_PIN_4)
#define BAL_CELL_3_PIN          (GPIO_PIN_0)
#define BAL_CELL_4_PIN          (GPIO_PIN_1)
#define BAL_CELL_5_PIN          (GPIO_PIN_2)
#define BAL_CELL_6_PIN          (GPIO_PIN_3)
#define BAL_CELL_1_PORT         (GPIOI)
#define BAL_CELL_2_PORT         (GPIOG)
#define BAL_CELL_3_PORT         (GPIOG)
#define BAL_CELL_4_PORT         (GPIOG)
#define BAL_CELL_5_PORT         (GPIOG)
#define BAL_CELL_6_PORT         (GPIOG)

#define ADC_VCC_IN_PIN          (GPIO_PIN_0)
#define ADC_IN1_PIN             (GPIO_PIN_1)
#define ADC_VBAT_PIN            (GPIO_PIN_2)
#define ADC_CELL_1_PIN          (GPIO_PIN_3)
#define ADC_CELL_2_PIN          (GPIO_PIN_4)
#define ADC_CELL_3_PIN          (GPIO_PIN_5)
#define ADC_CELL_4_PIN          (GPIO_PIN_6)
#define ADC_CELL_5_PIN          (GPIO_PIN_7)
//#define ADC_IN8_PIN             (GPIO_PIN_7)
//#define ADC_IN9_PIN             (GPIO_PIN_6)
#define ADC_CELL_6_PIN          (GPIO_PIN_0)
#define ADC_CURRENT_DISCHARGE_PIN       (GPIO_PIN_3)
#define ADC_CURRENT_CHARGE_PIN  (GPIO_PIN_4)
#define ADC_TEMP_INT_PIN        (GPIO_PIN_5)
#define ADC_TEMP_EXT_PIN        (GPIO_PIN_6)
//#define ADC_IN15_PIN            (GPIO_PIN_7)
#define ADC_VCC_IN_PORT         (GPIOB)
#define ADC_IN1_PORT            (GPIOB)
#define ADC_VBAT_PORT           (GPIOB)
#define ADC_CELL_1_PORT         (GPIOB)
#define ADC_CELL_2_PORT         (GPIOB)
#define ADC_CELL_3_PORT         (GPIOB)
#define ADC_CELL_4_PORT         (GPIOB)
#define ADC_CELL_5_PORT         (GPIOB)
//#define ADC_IN8_PORT            (GPIOE)
//#define ADC_IN9_PORT            (GPIOE)
#define ADC_CELL_6_PORT         (GPIOF)
#define ADC_CURRENT_DISCHARGE_PORT      (GPIOF)
#define ADC_CURRENT_CHARGE_PORT (GPIOF)
#define ADC_TEMP_INT_PORT       (GPIOF)
#define ADC_TEMP_EXT_PORT       (GPIOF)
//#define ADC_IN15_PORT           (GPIOF)
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

#define WriteHigh(Port, Pins)   Port->ODR |= (uint8_t)(Pins)
#define WriteLow(Port, Pins)    Port->ODR &= (uint8_t)(~(Pins))

#define BAL_CELL_1_ON           WriteHigh(BAL_CELL_1_PORT, BAL_CELL_1_PIN)
#define BAL_CELL_1_OFF          WriteLow(BAL_CELL_1_PORT, BAL_CELL_1_PIN)
#define BAL_CELL_2_ON           WriteHigh(BAL_CELL_2_PORT, BAL_CELL_2_PIN)
#define BAL_CELL_2_OFF          WriteLow(BAL_CELL_2_PORT, BAL_CELL_2_PIN)
#define BAL_CELL_3_ON           WriteHigh(BAL_CELL_3_PORT, BAL_CELL_3_PIN)
#define BAL_CELL_3_OFF          WriteLow(BAL_CELL_3_PORT, BAL_CELL_3_PIN)
#define BAL_CELL_4_ON           WriteHigh(BAL_CELL_4_PORT, BAL_CELL_4_PIN)
#define BAL_CELL_4_OFF          WriteLow(BAL_CELL_4_PORT, BAL_CELL_4_PIN)
#define BAL_CELL_5_ON           WriteHigh(BAL_CELL_5_PORT, BAL_CELL_5_PIN)
#define BAL_CELL_5_OFF          WriteLow(BAL_CELL_5_PORT, BAL_CELL_5_PIN)
#define BAL_CELL_6_ON           WriteHigh(BAL_CELL_6_PORT, BAL_CELL_6_PIN)
#define BAL_CELL_6_OFF          WriteLow(BAL_CELL_6_PORT, BAL_CELL_6_PIN)

#define COOL_ON                 WriteHigh(COOLER_PORT, COOLER_PIN)
#define COOL_OFF                WriteLow(COOLER_PORT, COOLER_PIN)

#define BEEP_ON                 WriteHigh(BEEP_PORT, BEEP_PIN)
#define BEEP_OFF                WriteLow(BEEP_PORT, BEEP_PIN)

#define BAT_KEY_OFF             WriteHigh(BAT_KEY_PORT, BAT_KEY_PIN)
#define BAT_KEY_ON              WriteLow(BAT_KEY_PORT, BAT_KEY_PIN)

#define CHARGE_DISABLE_OFF       WriteLow(CHARGE_DISABLE_PORT, CHARGE_DISABLE_PIN)
#define CHARGE_DISABLE_ON        WriteHigh(CHARGE_DISABLE_PORT, CHARGE_DISABLE_PIN)

#define INPUT_DC_KEY_ON         WriteHigh(INPUT_DC_KEY_PORT, INPUT_DC_KEY_PIN)
#define INPUT_DC_KEY_OFF        WriteLow(INPUT_DC_KEY_PORT, INPUT_DC_KEY_PIN)

#define BT_START                (!(BT_START_PORT->IDR & (uint8_t)BT_START_PIN))
#define BT_PLUS                 (!(BT_PLUS_PORT->IDR & (uint8_t)BT_PLUS_PIN))
#define BT_MINUS                (!(BT_MINUS_PORT->IDR & (uint8_t)BT_MINUS_PIN))
#define BT_STOP                 (!(BT_STOP_PORT->IDR & (uint8_t)BT_STOP_PIN))

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
