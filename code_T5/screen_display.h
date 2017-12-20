#ifndef __SCREEN_DISPLAY_H__
#define __SCREEN_DISPLAY_H__

#include <stdio.h>
#include "input_touch_control.h"

void sale_display(sale* sale_info);
void refund_display(item refund_info[], int t_price, int year1, int month1, int day1, int hour1, int min1);
void stock_display(sale* stock_info);

#endif