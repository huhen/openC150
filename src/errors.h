/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ERRORS_H
#define __ERRORS_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

void warn_reset_by_wdg();

void err_iv_too_high();
void err_iv_too_low();
void err_rp_check();
void err_bc_disconn();
void err_bc_over_volt();
void err_bc_lower_volt();
void err_bc_cell_count();
void err_bc_over_cell_volt();
void err_bc_lower_cell_volt();
void err_bc_full_bat();
void err_over_ext_temp();
void err_over_cap();
void err_safety_time_out();

#endif /* __ERRORS_H */