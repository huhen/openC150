/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HARDWARE_H
#define __HARDWARE_H

/* Exported macro ------------------------------------------------------------*/
#define BT_START_PIN            (GPIO_PIN_7)
#define BT_PLUS_PIN             (GPIO_PIN_6)
#define BT_MINUS_PIN            (GPIO_PIN_5)
#define BT_STOP_PIN             (GPIO_PIN_4)
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

#endif /* __HARDWARE_H */
