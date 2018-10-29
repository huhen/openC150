#include "lcd.h"
#include "hardware.h"
#include "charger.h"

/**
* @brief  None
* @param  None
* @retval None
*/
static inline void lcd_strob()
{
  delay_us(10);
  WriteHigh(LCD_E_PORT, LCD_E_PIN);
  delay_us(10);
  WriteLow(LCD_E_PORT, LCD_E_PIN);
  delay_us(10);
}

static void lcd_write_high(uint8_t d)
{
  delay_us(10);
  
  WriteLow(LCD_RS_PORT, LCD_RS_PIN);
  
  delay_us(10);
  
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
  
  delay_us(10);
  
  WriteHigh(LCD_RS_PORT, LCD_RS_PIN);
  
  delay_us(10);
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
  delay_us(10);
  
  WriteLow(LCD_RS_PORT, LCD_RS_PIN);
  
  delay_us(10);
  
  lcd_putchar(d);
  
  delay_us(10);
  
  WriteHigh(LCD_RS_PORT, LCD_RS_PIN);
  
  delay_us(10);
}

/**
* @brief  Clear lcd
* @param  None
* @retval None
*/
void lcd_clear(void)
{
  lcd_com(0x01);
  
  delay_ms(2);
}

/**
* @brief  Init lcd
* @param  None
* @retval None
*/
void lcd_init(void)
{
  delay_ms(40);
  
  lcd_write_high(0x30);
  delay_ms(10);
  lcd_write_high(0x30);
  delay_ms(1);
  lcd_write_high(0x30);
  delay_us(40);
  lcd_write_high(0x20);
  delay_us(40);
  
  
  lcd_com(0x28); //0b00101000 4-bit,2 - line mode, 5*8 dots
  delay_us(40);
  
  lcd_com(0x08);
  delay_us(40);
  
  lcd_com(0x17);
  delay_us(40);
  
  lcd_clear();
  
  lcd_com(0x06);//0b00000110 increment mode,entrir shift off
  delay_us(40);
  
  lcd_com(0x0C); //0b00001100 display on,cursor off,blink off
  delay_us(40);
}

/**
* @brief  Put string to lcd
* @param  None
* @retval None
*/
void lcd_putsf(const char *_str)
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
