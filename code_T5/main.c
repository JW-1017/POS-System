#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "input_touch_control.h"
#include "clock_settlement.h"
#include "file_manage.h"

int power_on(void);
int power_off(void);
int identify_power_button(void);
int stock_check_availabilty(void);		// 재고확인 가능여부 확인
void identify_touch_area(int area, sale* sale_info, sale* stock_info);

int main(void)
{
	int mode = 0, sel = 0;
	time_t cur;
	char stock_info_name[17];
	struct tm* date;

	pthread_t clock_thread;

	// 여기부터 꾸미기printf() 부분
	printf("POST System \n\n");

	init_sale_info(&sale_info);
	init_stock_info(&stock_info);
	init_arr(s_r);
	
	FILE* save_data = fopen("save_data.txt", "rt");

	if (save_data == NULL) {
		// 날짜 시간 출력을 위함 date->tm_year + 1900 년도, date->tm_mon+1 월, date->tm_mday 일 tm_hour, tm_min 시분
		cur = time(NULL);
		date = localtime(&cur);

		min = date->tm_min;
		hour = date->tm_hour;
		day = date->tm_mday;
		month = date->tm_mon + 1;
		year = date->tm_year + 1900;
		check_leap();
	}
	else {
		fscanf(save_data, "%d %d %d %d %d", &year, &month, &day, &hour, &min);
		fscanf(save_data, "%d %d %d %d %d %d %d", &(stock_info.s_arr[0].quantity), &(stock_info.s_arr[1].quantity), &(stock_info.s_arr[2].quantity),
			&(stock_info.s_arr[3].quantity), &(stock_info.s_arr[4].quantity), &(stock_info.s_arr[5].quantity), &(stock_info.s_arr[6].quantity));
		fscanf(save_data, "%d %d %d %d %d %d %d %d\n", &s_r[0][0], &s_r[0][1], &s_r[0][2], &s_r[0][3], &s_r[0][4], &s_r[0][5], &s_r[0][6], &s_r[0][7]);
		fscanf(save_data, "%d %d %d %d %d %d %d %d\n", &s_r[1][0], &s_r[1][1], &s_r[1][2], &s_r[1][3], &s_r[1][4], &s_r[1][5], &s_r[1][6], &s_r[1][7]);
		check_leap();
	}
	fclose(save_data);
	
	pthread_create(&clock_thread, NULL, time_to_settlement, NULL);		// clock thread start
	make_stock_file(&stock_info, year, month, day);
	sale_list_file(&sale_info, year, month, day, hour, min);

	while(1)			
	{	
		printf("\n\n1. Sale Mode\n2. Quantity Changed Mode\n3. Money Received Mode\n");
		printf("4. Refund Mode\n5. Stock Check Mode\n");
		printf("(-1 : Power Mode)\n");			// identify power button 따로 두지 않음
		scanf("%d", &mode);

		if (mode == -1)
		{
			sel = identify_power_button();
			if(sel == 0) {
				pthread_mutex_lock(&mutex1);
				save_file(&stock_info);
				pthread_mutex_unlock(&mutex1);
				settle_flag = 2;					// clock 종료를 위해 flag 이용
				pthread_join(clock_thread, NULL);
				return -1;
			}
		}
		else if(mode >= 1 && mode <= 5){
			identify_touch_area(mode, &sale_info, &stock_info);
		}
		else {
			printf("Wrong Input!\n");
			continue;
		}
	}
	return 0;
}
int power_on(void)
{	
	printf("POS ON\n\n");
	return 1;
}
int power_off(void)
{	
	printf("POS OFF\n\n");
	return 0;
}
int identify_power_button()
{
	int temp;

	while (1)
	{
		printf("\n1. Power On  2. Power Off : ");
		scanf("%d", &temp);

		if (temp == 1) {
			return power_on();
		}
		else if (temp == 2) {
			return power_off();
		}
		else {
			printf("Wrong Input!\n\n");
		}
	}
}
int stock_check_availabilty(void)
{
	if (settle_flag == 0) {
		return 0;
	}
	else {
		return 1;
	}
}
void identify_touch_area(int mode, sale* sale_info, sale* stock_info)
{
	if(mode == 1)
	{
		settle_flag = 0;
		sale_list_manage(sale_info, stock_info);
	}
	else if (mode == 2)
	{
		quantity_change(sale_info, stock_info);
	}
	else if (mode == 3)
	{
		money_received(sale_info, stock_info);
		init_sale_info(sale_info);
		settle_flag = 1;
	}
	else if(mode == 4)
	{
		char barcode[100] = { 0 };
		settle_flag = 0;
		printf("Refund Mode\n");
		printf("Refund Receipt Barcode : ");
		scanf("%s", barcode);

		if (refund_availability(barcode) == 1){				// 환불 가능
			pthread_mutex_lock(&mutex1);
			refund_manage(stock_info , barcode);
			pthread_mutex_unlock(&mutex1);
		}
		settle_flag = 1;
	}
	else if(mode == 5){
		if (stock_check_availabilty() == 0) {			// 재고확인 불가 상태
			printf("Can not check Stock\n\n");
			return;
		}
		pthread_mutex_lock(&mutex1);
		stock_manage(stock_info);
		pthread_mutex_unlock(&mutex1);
		settle_flag = 1;
	}
	else {
		printf("Reselect mode\n");
		return;
	}
}