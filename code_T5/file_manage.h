#ifndef __FILE_MANGAE_H__
#define __FILE_MANGAE_H__

#include <stdio.h>
#include"input_touch_control.h"

extern int s_r[2][8];

void save_file(sale* stock_info);
void receipt_list_file(int year1, int month1, int day1, int hour1, int min1);		// 판매 영수증 관리 파일
void make_stock_file(sale* stock_info, int year1, int month1, int day1);
void sale_list_file(sale* sale_info, int year1, int month1, int day1, int hour1, int min1);
void database_update(sale* sale_info, sale* stock_info, int year1, int month1, int day1, int hour1, int min1);
void sale_receipt_print(sale* sale_info, int year1, int month1, int day1, int hour1, int min1);
void refund_receipt_print(item refund_info[], int index, int year1, int month1, int day1, int hour1, int min1);
void stock_receipt_print(sale* stock_info, int year1, int month1, int day1, int hour1, int min1);
void settlement_report_print();			// 시작한 순간 해당 시간을 가져옴

#endif