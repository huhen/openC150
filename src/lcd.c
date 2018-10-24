#include "lcd.h"
#include "hardware.h"

/**
  * @brief  Delay
  * @param  None
  * @retval None
  */
static void delay(uint16_t a)
{
  while(a--)
  {
    nop();
  }
}

/**
  * @brief  None
  * @param  None
  * @retval None
  */
static inline void lcd_strob()
{
  delay(10);
  WriteHigh(LCD_E_PORT, LCD_E_PIN);
  delay(10);
  WriteLow(LCD_E_PORT, LCD_E_PIN);
  delay(10);
}

/**
  * @brief  Write data to lcd
  * @param  None
  * @retval None
  */
void lcd_putchar(uint8_t d)
{
  if(d&0x80)
  {
    WriteHigh(LCD_DB7_PORT, LCD_DB7_PIN);
  }
  else
  {
    WriteLow(LCD_DB7_PORT, LCD_DB7_PIN);
  }
  
  if(d&0x40)
  {
    WriteHigh(LCD_DB6_PORT, LCD_DB6_PIN);
  }
  else
  {
    WriteLow(LCD_DB6_PORT, LCD_DB6_PIN);
  }
  
  if(d&0x20)
  {
    WriteHigh(LCD_DB5_PORT, LCD_DB5_PIN);
  }
  else
  {
    WriteLow(LCD_DB5_PORT, LCD_DB5_PIN);
  }
  
  if(d&0x10)
  {
    WriteHigh(LCD_DB4_PORT, LCD_DB4_PIN);
  }
  else
  {
    WriteLow(LCD_DB4_PORT, LCD_DB4_PIN);
  }
  
  lcd_strob();
   
  if(d&0x08)
  {
    WriteHigh(LCD_DB7_PORT, LCD_DB7_PIN);
  }
  else
  {
    WriteLow(LCD_DB7_PORT, LCD_DB7_PIN);
  }
  
  if(d&0x04)
  {
    WriteHigh(LCD_DB6_PORT, LCD_DB6_PIN);
  }
  else
  {
    WriteLow(LCD_DB6_PORT, LCD_DB6_PIN);
  }
  
  if(d&0x02)
  {
    WriteHigh(LCD_DB5_PORT, LCD_DB5_PIN);
  }
  else
  {
    WriteLow(LCD_DB5_PORT, LCD_DB5_PIN);
  }
  
  if(d&0x01)
  {
    WriteHigh(LCD_DB4_PORT, LCD_DB4_PIN);
  }
  else
  {
    WriteLow(LCD_DB4_PORT, LCD_DB4_PIN);
  }
  
  lcd_strob();
}

/**
  * @brief  Write comand to lcd
  * @param  None
  * @retval None
  */
static void lcd_com(uint8_t d)
{
  WriteLow(LCD_RS_PORT, LCD_RS_PIN);
  
  delay(10);
  
  lcd_putchar(d);
  
  delay(10);
  
  WriteHigh(LCD_RS_PORT, LCD_RS_PIN);
}

/**
  * @brief  Clear lcd
  * @param  None
  * @retval None
  */
void lcd_clear(void)
{
  lcd_com(0x01);
  delay(2600);//Очистка LCD
}

/**
  * @brief  Init lcd
  * @param  None
  * @retval None
  */
void lcd_init(void)
{
  for(uint16_t t=0;t<200;t++)
  {
    IWDG->KR = IWDG_KEY_REFRESH;  
    delay(260); //40ms
  }
  
  lcd_com(0x28); //0b00101000 4-bit,2 - line mode, 5*8 dots
  delay(56);
  
  lcd_com(0x0C); //0b00001100 display on,cursor off,blink off
  delay(56);

  lcd_clear();
  
  lcd_com(0x06);//0b00000110 increment mode,entrir shift off
  delay(56);
}

/**
  * @brief  Put string to lcd
  * @param  None
  * @retval None
  */
void lcd_putsf(char *_str)
{
  while (*_str)
  {
    lcd_putchar(*_str++);
  }
}

/**
  * @brief  Move to position
  * @param  None
  * @retval None
  */
void lcd_gotoxy(char x, char y)
{
  lcd_com((0x80+y*64)+x);
}
