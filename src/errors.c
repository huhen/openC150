#include "errors.h"
#include "lcd.h"

static const char msg_iv[]="INPUT VOLTAGE";
static const char msg_rp[]="REVERSE POLARITY";
static const char msg_bc[]="BATTERY CHECK";
static const char msg_ot[]="OVER Ext.TEMP";
static const char msg_oc[]="OVER CAPACITY";
static const char msg_st[]="SAFETY TIME OUT";

static const char msg_cutoff[]="CUTOFF";

void err_iv_too_high()
{
  lcd_clear();
  
  lcd_gotoxy(0,0);
  lcd_putsf(msg_iv);
  
  lcd_gotoxy(8,1);
  lcd_putsf("TOO HIGH");
}

void err_iv_too_low()
{
  lcd_clear();
  
  lcd_gotoxy(0,0);
  lcd_putsf(msg_iv);
  
  lcd_gotoxy(9,1);
  lcd_putsf("TOO LOW");
}

void err_rp_check()
{
  lcd_clear();
  
  lcd_gotoxy(0,0);
  lcd_putsf(msg_rp);
  
  lcd_gotoxy(11,1);
  lcd_putsf("CHECK");
}

void err_bc_disconn()
{
  lcd_clear();
  
  lcd_gotoxy(0,0);
  lcd_putsf(msg_bc);
  
  lcd_gotoxy(6,1);
  lcd_putsf("DISCONNECT");
}

void err_bc_over_volt()
{
  lcd_clear();
  
  lcd_gotoxy(0,0);
  lcd_putsf(msg_bc);
  
  lcd_gotoxy(4,1);
  lcd_putsf("OVER VOLTAGE");
}

void err_bc_lower_volt()
{
  lcd_clear();
  
  lcd_gotoxy(0,0);
  lcd_putsf(msg_bc);
  
  lcd_gotoxy(3,1);
  lcd_putsf("LOWER VOLTAGE");
}

void err_bc_cell_count()
{
  lcd_clear();
  
  lcd_gotoxy(0,0);
  lcd_putsf(msg_bc);
  
  lcd_gotoxy(0,1);
  lcd_putsf("CELL COUNT ERROR");
}

void err_bc_over_cell_volt()
{
  lcd_clear();
  
  lcd_gotoxy(0,0);
  lcd_putsf(msg_bc);
  
  lcd_gotoxy(2,1);
  lcd_putsf("OVER CELL VOLT");
}

void err_bc_lower_cell_volt()
{
  lcd_clear();
  
  lcd_gotoxy(0,0);
  lcd_putsf(msg_bc);
  
  lcd_gotoxy(1,1);
  lcd_putsf("LOWER CELL VOLT");
}

void err_bc_full_bat()
{
  lcd_clear();
  
  lcd_gotoxy(0,0);
  lcd_putsf(msg_bc);
  
  lcd_gotoxy(4,1);
  lcd_putsf("FULL BATTERY");
}

void err_over_ext_temp()
{
  lcd_clear();
  
  lcd_gotoxy(0,0);
  lcd_putsf(msg_ot);
  
  lcd_gotoxy(10,1);
  lcd_putsf(msg_cutoff);
}

void err_over_cap()
{
  lcd_clear();
  
  lcd_gotoxy(0,0);
  lcd_putsf(msg_oc);
  
  lcd_gotoxy(10,1);
  lcd_putsf(msg_cutoff);
}

void err_safety_time_out()
{
  lcd_clear();
  
  lcd_gotoxy(0,0);
  lcd_putsf(msg_st);
  
  lcd_gotoxy(10,1);
  lcd_putsf(msg_cutoff);
}
