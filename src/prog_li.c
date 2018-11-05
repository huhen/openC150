#include "prog_li.h"
#include "hardware.h"
#include "charger.h"
#include "charge.h"
#include "lcd.h"
#include "stm8s_it.h"
#include <stdio.h>

#define LI_TYPE_PO      (0)
#define LI_TYPE_IO      (1)
#define LI_TYPE_FE      (2)
#define LI_TYPE_HV      (3)

static const char msg_lipo[]="LiPo";
static const char msg_liio[]="LiIo";
static const char msg_life[]="LiFe";
static const char msg_lihv[]="LiHV";

static const char* msg_types[]={msg_lipo, msg_liio, msg_life, msg_lihv};

#define LI_MODE_BCHARGE         (0)
#define LI_MODE_CHARGE          (1)
#define LI_MODE_FCHARGE         (2)
#define LI_MODE_STORAGE         (3)
#define LI_MODE_DISCHARGE       (4)

static const char msg_balance[]="BALANCE CHG";
static const char msg_charge[]="CHARGE";
static const char msg_fcharge[]="FAST CHARGE";
static const char msg_storage[]="STORAGE";
static const char msg_discharge[]="DISCHARGE";

static const char* msg_modes[]={msg_balance, msg_charge, msg_fcharge, msg_storage, msg_discharge};

static const char msg_balance_simple[]="BAL";
static const char msg_charge_simple[]="CHG";
static const char msg_fcharge_simple[]="FAS";
static const char msg_storage_simple[]="STO";
static const char msg_discharge_simple[]="DSC";

static const char* msg_modes_simple[]={msg_balance_simple, msg_charge_simple, msg_fcharge_simple, msg_storage_simple, msg_discharge_simple};

#define LI_MAX_CURRENT  (10000)
#define LI_MAX_NUM_S    (6)
#define LI_MAX_MODE     (LI_MODE_DISCHARGE)
#define LI_MAX_TYPE     (LI_TYPE_HV)

static uint8_t state = 0;
static uint8_t sub_state = 0;
static uint8_t battery_check_state;

static uint8_t charge_state;
static uint8_t discharge_state;

static uint8_t mode = 0;
static uint8_t type = 0;
static uint16_t current = 0;
static uint8_t num_s = 0;

static uint8_t counter = 0;
static uint8_t chet_nechet = 0;

static uint32_t charged_sum;
static uint32_t charged_sum32;

static uint16_t get_cell_nom_v()
{
  switch(type)
  {
  case LI_TYPE_PO:return 3700;
  case LI_TYPE_IO:return 3600;
  case LI_TYPE_FE:return 3300;
  case LI_TYPE_HV:return 3800;
  default: return 0;
  }
}

static uint16_t get_cell_precharge_v()
{
  switch(type)
  {
  case LI_TYPE_PO:return 3200;
  case LI_TYPE_IO:return 2500;
  case LI_TYPE_FE:return 2500;
  case LI_TYPE_HV:return 3300;
  default: return 0;
  }
}

static uint16_t get_cell_target_v()
{
  switch(type)
  {
  case LI_TYPE_PO:return config->LiPoTargetV;
  case LI_TYPE_IO:return config->LiIoTargetV;
  case LI_TYPE_FE:return config->LiFeTargetV;
  case LI_TYPE_HV:return config->LiHvTargetV;
  default: return 0;
  }
}

static void print_a()
{
  sprintf(buff, "%hhu.%hhuA", (uint8_t)(current / 1000), (uint8_t)((current % 1000) / 100));
  lcd_putsf(buff);
}

static void print_num_s()
{
  uint16_t volt = get_cell_nom_v()*num_s;
  
  uint8_t i = sprintf(buff, "%hhu.%hhuV(%hhuS)", (uint8_t)(volt / 1000), (uint8_t)((volt % 1000) / 100), num_s);
  lcd_gotoxy(16-i,1);
  lcd_putsf(buff);
}

static void print_full_menu()
{
  lcd_clear();
  
  lcd_gotoxy(0,0);
  lcd_putsf(msg_types[type]);
  lcd_put_char();
  lcd_putsf(msg_modes[mode]);
  
  lcd_gotoxy(0,1);
  print_a();
  print_num_s();
}

static void battery_check()
{
  static uint8_t num_s_r;
  
  switch(battery_check_state)
  {
  case 0:
    lcd_clear();
    lcd_gotoxy(0,0);
    lcd_putsf("BATTERY CHECK...");
    battery_check_state++;
    BAT_KEY_ON;
    break;
    
  case 1:
    num_s_r=v_bat/get_cell_nom_v();
    battery_check_state++;
    break;
    
  case 2:
    lcd_clear();
    lcd_gotoxy(0,0);
    sprintf(buff, "R:%hhuSER    S:%hhuSER)", num_s_r, num_s);
    lcd_putsf(buff);
    lcd_gotoxy(0,1);
    lcd_putsf("CONFIRM(ENTER)");
    battery_check_state++;
    BAT_KEY_OFF;
    break;
    
  case 3:
    if(event_bt_start)
    {
      charge_state=0;
      discharge_state=0;
      tick_s = 0;
      state++;
    }
    else if(event_bt_stop)
    {
      print_full_menu();
      state-=2;
    }
    break;
  }
}

static void charge_print_full(uint8_t reset)
{
  static uint8_t last_i_charge1;
  static uint8_t last_i_charge2;
  static uint8_t last_v_bat1;
  static uint8_t last_v_bat2;
  static uint32_t last_tick_s;
  static uint8_t last_counter;
  
  if(reset)
  {
    last_i_charge1=0xff;
    last_i_charge2=0xff;
    last_v_bat1=0xff;
    last_v_bat2=0xff;
    last_tick_s=0xffffffff;
    last_counter=0xff;
    
    lcd_clear();
    lcd_gotoxy(0,1);
    lcd_putsf(msg_modes_simple[mode]);
  }
  
  uint8_t i;
  
  uint16_t tmp=i_charge;
  uint8_t i_charge1=tmp / 1000;
  uint8_t i_charge2;
  if(i_charge1<10)
  {
    i_charge2=(tmp % 1000) / 10;
  }
  else
  {
    i_charge2=(tmp % 1000) / 100;
  }
  tmp=v_bat;
  uint8_t v_bat1=tmp / 1000;
  uint8_t v_bat2;
  if(v_bat1<10)
  {
    v_bat2=(tmp % 1000) / 10;
  }
  else
  {
    v_bat2=(tmp % 1000) / 100;
  }
  
  if(last_i_charge1!=i_charge1 || last_i_charge2!=i_charge2)
  {
    last_i_charge1=i_charge1;
    last_i_charge2=i_charge2;
    if(i_charge1<10)
    {
      i = sprintf(buff, "%hhu.%02hhuA", i_charge1, i_charge2);
    }
    else
    {
      i = sprintf(buff, "%hhu.%hhuA", i_charge1, i_charge2);
    }
    lcd_gotoxy((16-6)-i,0);
    lcd_putsf(buff);
  }
  
  if(last_v_bat1!=v_bat1 || last_v_bat2!=v_bat2)
  {
    last_v_bat1=v_bat1;
    last_v_bat2=v_bat2;
    if(v_bat1<10)
    {
      i = sprintf(buff, "%hhu.%02hhuV", v_bat1, v_bat2);
    }
    else
    {
      i = sprintf(buff, "%hhu.%hhuV", v_bat1, v_bat2);
    }
    lcd_gotoxy(16-i,0);
    lcd_putsf(buff);
  }
  
  if(last_tick_s!=tick_s)
  {
    last_tick_s=tick_s;
    
    uint16_t minutes=last_tick_s / 60;
    uint8_t seconds=last_tick_s % 60;
    
    lcd_gotoxy(0,0);
    
    if(last_tick_s&1)
    {
      lcd_putsf(msg_types[type]);
    }
    else
    {
      sprintf(buff, " %hhuS ", num_s);
      lcd_putsf(buff);
    }
    
    i = sprintf(buff, "%03hu:%02hhu", minutes, seconds);
    last_counter=25;
    
    lcd_gotoxy((16-6)-i,1);
    lcd_putsf(buff);
    
    i = sprintf(buff, "%5lu", charged_sum32/3600);
    lcd_gotoxy(16-i,1);
    lcd_putsf(buff);
  }
  
  if(last_counter==0xff)
  {
  }
  else if(last_counter)
  {
    last_counter--;
  }
  else
  {
    lcd_gotoxy(7,1);
    lcd_putchar(' ');
    last_counter=0xff;
  }
}

static void charge_print_v_cells(uint8_t reset)
{
  static uint16_t last_v_cell_1=0xffff;
  static uint16_t last_v_cell_2=0xffff;
  static uint16_t last_v_cell_3=0xffff;
  static uint16_t last_v_cell_4=0xffff;
  static uint16_t last_v_cell_5=0xffff;
  static uint16_t last_v_cell_6=0xffff;
  
  if(reset)
  {
    last_v_cell_1=0xffff;
    last_v_cell_2=0xffff;
    last_v_cell_3=0xffff;
    last_v_cell_4=0xffff;
    last_v_cell_5=0xffff;
    last_v_cell_6=0xffff;
    
    lcd_clear();
    lcd_gotoxy(14,0);
    lcd_putsf("mV");
    lcd_gotoxy(14,1);
    lcd_putsf("mV");
  }
  
  uint8_t i;
  
  if(last_v_cell_1!=v_cell_1)
  {
    last_v_cell_1=v_cell_1;
    i = sprintf(buff, "%4hu", last_v_cell_1);
    lcd_gotoxy(4-i,0);
    lcd_putsf(buff);
  }
  
  if(last_v_cell_2!=v_cell_2)
  {
    last_v_cell_2=v_cell_2;
    i = sprintf(buff, "%4hu", last_v_cell_2);
    lcd_gotoxy(9-i,0);
    lcd_putsf(buff);
  }
  
  if(last_v_cell_3!=v_cell_3)
  {
    last_v_cell_3=v_cell_3;
    i = sprintf(buff, "%4hu", last_v_cell_3);
    lcd_gotoxy(13-i,0);
    lcd_putsf(buff);
  }
  
  if(last_v_cell_4!=v_cell_4)
  {
    last_v_cell_4=v_cell_4;
    i = sprintf(buff, "%4hu", last_v_cell_4);
    lcd_gotoxy(4-i,1);
    lcd_putsf(buff);
  }
  
  if(last_v_cell_5!=v_cell_5)
  {
    last_v_cell_5=v_cell_5;
    i = sprintf(buff, "%4hu", last_v_cell_5);
    lcd_gotoxy(9-i,1);
    lcd_putsf(buff);
  }
  
  if(last_v_cell_6!=v_cell_6)
  {
    last_v_cell_6=v_cell_6;
    i = sprintf(buff, "%4hu", last_v_cell_6);
    lcd_gotoxy(13-i,1);
    lcd_putsf(buff);
  }
}

static void charge()
{
  static uint16_t taget_v_cell;
  static uint8_t cnt = 0;
  static uint8_t steep;
  static uint16_t stab_cnt;
  
  
  switch(charge_state)
  {
  case 0:
    charged_sum = 0;
    charged_sum32 = 0;
    
    charge_print_full(1);
    charge_state++;
    BAT_KEY_ON;
    CHARGE_DISABLE_OFF;
    COOL_ON;
    
    if(mode==LI_MODE_STORAGE)
    {
      taget_v_cell=get_cell_target_v();
      taget_v_cell=taget_v_cell-(taget_v_cell/11);
      //taget_v=taget_v_cell*num_s;
    }
    else
    {
      taget_v_cell=get_cell_target_v();
      //taget_v=taget_v_cell*num_s;
    }
    //taget_v_adc=find_taget_v_adc(taget_v);
    
    if(current<I_HYSTER)current=I_HYSTER;
    steep = 0;
    stab_cnt = 0;
    
    break;
    
  case 1:
    
    cnt++;
    if(cnt>10)
    {
      charge_print_full(0);
      cnt=0;
    }
    
    if(event_bt_plus)
    {
      charge_print_v_cells(1);
      charge_state++;
    }
    else if(event_bt_minus)
    {
      lcd_clear();
      lcd_gotoxy(0,1);
      charge_state+=2;
    }
    break;
    
  case 2:
    cnt++;
    if(cnt>10)
    {
      charge_print_v_cells(0);
      cnt=0;
    }
    
    if(event_bt_plus)
    {
      charge_state--;
      charge_print_full(1);
    }
    break;
    
  case 3:
    break;
  }
  
  if(i_charge>current && (i_charge-current)>500)
  {
    charge_off();
  }
  
  uint8_t need_up=0;
  uint8_t need_down=0;
  
  uint16_t min_cell_v=0xffff;
  uint16_t max_cell_v=0;
  
  if(num_s>0&&v_cell_1>max_cell_v) max_cell_v=v_cell_1;
  if(num_s>1&&v_cell_2>max_cell_v) max_cell_v=v_cell_2;
  if(num_s>2&&v_cell_3>max_cell_v) max_cell_v=v_cell_3;
  if(num_s>3&&v_cell_4>max_cell_v) max_cell_v=v_cell_4;
  if(num_s>4&&v_cell_5>max_cell_v) max_cell_v=v_cell_5;
  if(num_s>5&&v_cell_6>max_cell_v) max_cell_v=v_cell_6;
  
  if(num_s>0&&min_cell_v>v_cell_1) min_cell_v=v_cell_1;
  if(num_s>1&&min_cell_v>v_cell_2) min_cell_v=v_cell_2;
  if(num_s>2&&min_cell_v>v_cell_3) min_cell_v=v_cell_3;
  if(num_s>3&&min_cell_v>v_cell_4) min_cell_v=v_cell_4;
  if(num_s>4&&min_cell_v>v_cell_5) min_cell_v=v_cell_5;
  if(num_s>5&&min_cell_v>v_cell_6) min_cell_v=v_cell_6;
  
  if(i_charge>current)
  {
    need_down++;
  }
  
  if(steep==2)
  {
    if(max_cell_v>(taget_v_cell+CELL_BALL_HYSTER))
    {
      need_down++;
    }
  }
  else
  {
    if(max_cell_v>taget_v_cell)
    {
      need_down++;
    }
  }
  switch(steep)
  {
  case 0:
    {
      if(min_cell_v<get_cell_precharge_v())
      {
        uint16_t precharge=current/10;
        if(precharge<I_HYSTER)precharge=I_HYSTER;
        
        if(i_charge>precharge)
        {
          need_down++;
        }
        else if(i_charge<(precharge-I_HYSTER))
        {
          need_up++;
        }
      }
      else
      {
        steep++;
      }
    }
    break;
    
  case 1:
    {
      if(min_cell_v<(taget_v_cell - CELL_BALL_HYSTER) || i_charge<(current-I_HYSTER))
      {
        need_up++;
      }
      
      uint16_t endcharge;
      if(mode==LI_MODE_FCHARGE)
      {
        endcharge=current/5;
      }
      else
      {
        endcharge=current/10;
      }
      
      if(i_charge<endcharge)
      {
        if(stab_cnt!=(CYCLE_PER_1S*30))
        {
          stab_cnt++;
        }
        else
        {
          steep++;
        }
      }
      else
      {
        stab_cnt=0;
      }
    }
    break;
    
  case 2://balancing
    {
      if(mode!=LI_MODE_BCHARGE)
      {
        steep++;
        break;
      }
      
      uint16_t taget_v_ball_off=taget_v_cell - CELL_BALL_HYSTER;
      uint16_t taget_v_ball_on=taget_v_cell;
      
      if(min_cell_v>taget_v_ball_off)
      {
        steep++;
        break;
      }
      
      if(v_cell_1>taget_v_ball_on)
      {
        BAL_CELL_1_ON;
      }
      else if(v_cell_1<taget_v_ball_off)
      {
        BAL_CELL_1_OFF;
      }
      
      if(v_cell_2>taget_v_ball_on)
      {
        BAL_CELL_2_ON;
      }
      else if(v_cell_2<taget_v_ball_off)
      {
        BAL_CELL_2_OFF;
      }
      
      if(v_cell_3>taget_v_ball_on)
      {
        BAL_CELL_3_ON;
      }
      else if(v_cell_3<taget_v_ball_off)
      {
        BAL_CELL_3_OFF;
      }
      
      if(v_cell_4>taget_v_ball_on)
      {
        BAL_CELL_4_ON;
      }
      else if(v_cell_4<taget_v_ball_off)
      {
        BAL_CELL_4_OFF;
      }
      
      if(v_cell_5>taget_v_ball_on)
      {
        BAL_CELL_5_ON;
      }
      else if(v_cell_5<taget_v_ball_off)
      {
        BAL_CELL_5_OFF;
      }
      
      if(v_cell_6>taget_v_ball_on)
      {
        BAL_CELL_6_ON;
      }
      else if(v_cell_6<taget_v_ball_off)
      {
        BAL_CELL_6_OFF;
      }
    }
    break;
    
  case 3:
    {
      charge_off();
      
      BAL_CELL_1_OFF;
      BAL_CELL_2_OFF;
      BAL_CELL_3_OFF;
      BAL_CELL_4_OFF;
      BAL_CELL_5_OFF;
      BAL_CELL_6_OFF;
      
      COOL_OFF;
    }
    break;
  }
  
  if(need_down)
  {
    charge_down();
  }
  else if(need_up)
  {
    charge_up();
  }
}

static void discharge()
{
  switch(discharge_state)
  {
  case 0:
    break;
  }
}

int8_t prog_li(void)
{
  charged_sum+=i_charge;
  
  counter++;
  if(counter>=CYCLE_PER_1S)
  {
    counter=0;
    
    charged_sum32+=(charged_sum / CYCLE_PER_1S);
    charged_sum=0;
    
    chet_nechet++;
    if(chet_nechet>1)chet_nechet=0;
  }
  
  
  switch(state)
  {
  case 0:
    lcd_clear();
    
    lcd_gotoxy(0,0);
    lcd_putsf("PROGRAM SELECT");
    
    lcd_gotoxy(0,1);
    lcd_putsf("Lithium Battery");
    
    state++;
    break;
    
  case 1:
    if(event_bt_start)
    {
      state++;
      
      mode=config->LiMode;
      type=config->LiType;
      current=config->LiCurrent;
      num_s=config->LiNumS;
      
      if(mode>LI_MAX_MODE)mode=LI_MAX_MODE;
      if(type>LI_MAX_TYPE)type=LI_MAX_TYPE;
      if(current>LI_MAX_CURRENT)current=LI_MAX_CURRENT;
      if(num_s>LI_MAX_NUM_S)num_s=LI_MAX_NUM_S;
    }
    else if(event_bt_plus)
    {
      state = 0;
      return 1;
    }
    else if(event_bt_minus)
    {
      state = 0;
      return -1;
    }
    break;
    
  case 2:
    print_full_menu();
    state++;
    break;
    
  case 3:
    if(event_bt_stop)
    {
      state = 0;
    }
    else if(event_bt_minus)
    {
      if(mode)
      {
        mode--;
      }else
      {
        mode=LI_MAX_MODE;
      }
      
      state--;
    }
    else if(event_bt_plus)
    {
      if(mode==LI_MAX_MODE)
      {
        mode=0;
      }
      else
      {
        mode++;
      }
      state--;
    }
    else if(event_bt_start==1)
    {
      state++;
    }
    else if(event_bt_start==2)
    {
      battery_check_state = 0;
      state+=2;
    }
    break;
    
  case 4:
    switch(sub_state)
    {
    case 0:
      if(counter==25)
      {
        lcd_gotoxy(0,0);
        lcd_putsf("    ");
      }
      else if(counter==0)
      {
        lcd_gotoxy(0,0);
        lcd_putsf(msg_types[type]);
      }
      
      if(event_bt_minus)
      {
        if(type)
        {
          type--;
        }
        else
        {
          type=LI_MAX_TYPE;
        }
        print_full_menu();
      }
      else if(event_bt_plus)
      {
        if(type==LI_MAX_TYPE)
        {
          type=0;
        }
        else
        {
          type++;
        }
        print_full_menu();
      }
      break;
      
    case 1:
      if(counter==25)
      {
        lcd_gotoxy(0,1);
        lcd_putsf("     ");
      }
      else if(counter==0)
      {
        lcd_gotoxy(0,1);
        print_a();
      }
      
      if(event_bt_minus==1)
      {
        if(current>=100)
        {
          current-=100;
        }
        else current=0;
        
        print_full_menu();
      }
      else if(event_bt_minus==2)
      {
        if(current>=1000)
        {
          current-=1000;
        }
        else current=0;
        
        print_full_menu();
      }
      else if(event_bt_plus==1)
      {
        current+=100;
        if(current>LI_MAX_CURRENT)current=LI_MAX_CURRENT;
        print_full_menu();
      }
      else if(event_bt_plus==2)
      {
        current+=1000;
        if(current>LI_MAX_CURRENT)current=LI_MAX_CURRENT;
        print_full_menu();
      }
      break;
      
    case 2:
      if(counter==25)
      {
        lcd_gotoxy(5,1);
        lcd_putsf("           ");
      }
      else if(counter==0)
      {
        print_num_s();
      }
      
      if(event_bt_minus)
      {
        if(num_s)
        {
          num_s--;
          print_full_menu();
        }
      }
      else if(event_bt_plus)
      {
        if(num_s<LI_MAX_NUM_S)
        {
          num_s++;
          print_full_menu();
        }
      }
      break;
    }
    if(event_bt_stop)
    {
      state--;
      print_full_menu();
    }
    else if(event_bt_start==1)
    {
      if(sub_state==2)
      {
        sub_state=0;
      }
      else
      {
        sub_state++;
      }
      print_full_menu();
    }
    else if(event_bt_start==2)
    {
      state--;
      print_full_menu();
      
      FLASH_Unlock(FLASH_MEMTYPE_DATA);
      config->LiMode=mode;
      config->LiType=type;
      config->LiCurrent=current;
      config->LiNumS=num_s;
      FLASH_Lock(FLASH_MEMTYPE_DATA);
    }
    break;
    
  case 5://check batt
    battery_check();
    break;
    
  case 6:
    if(mode==LI_MODE_DISCHARGE)
    {
      discharge();
    }
    else
    {
      charge();
    }
    break;
  }
  
  return 0;
}
