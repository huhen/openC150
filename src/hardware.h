/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HARDWARE_H
#define __HARDWARE_H

//#define FIX_BROKEN_PIN

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
#define ADC_5V_PIN              (GPIO_PIN_7)
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
#define ADC_5V_PORT             (GPIOF)

#define WriteHigh(Port, Pins)   Port->ODR |= (uint8_t)(Pins)
#define WriteLow(Port, Pins)    Port->ODR &= (uint8_t)(~(Pins))

#define BAL_CELL_1_ON           WriteHigh(BAL_CELL_1_PORT, BAL_CELL_1_PIN)
#define BAL_CELL_1_OFF          WriteLow(BAL_CELL_1_PORT, BAL_CELL_1_PIN)
#define BAL_CELL_2_ON           WriteHigh(BAL_CELL_2_POR, BAL_CELL_2_PIN)
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

#define BAT_KEY_ON              WriteHigh(BAT_KEY_PORT, BAT_KEY_PIN)
#define BAT_KEY_OFF             WriteLow(BAT_KEY_PORT, BAT_KEY_PIN)

#define INPUT_DC_KEY_ON         WriteHigh(INPUT_DC_KEY_PORT, INPUT_DC_KEY_PIN)
#define INPUT_DC_KEY_OFF        WriteLow(INPUT_DC_KEY_PORT, INPUT_DC_KEY_PIN)

#define BT_START                (!(BT_START_PORT->IDR & (uint8_t)BT_START_PIN))
#define BT_PLUS                 (!(BT_PLUS_PORT->IDR & (uint8_t)BT_PLUS_PIN))
#define BT_MINUS                (!(BT_MINUS_PORT->IDR & (uint8_t)BT_MINUS_PIN))
#define BT_STOP                 (!(BT_STOP_PORT->IDR & (uint8_t)BT_STOP_PIN))

#endif /* __HARDWARE_H */
