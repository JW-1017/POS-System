#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "clock_settlement.h"
#include "input_touch_control.h"
#include "file_manage.h"
#include "screen_display.h"

sale sale_info;
sale stock_info;

void init_arr(int arr[][8])
{
	int i;
	for (i = 0; i < 2; i++) {
		memset(arr[i], 0, 4 * 8);
	}
}
void delete_comma(char* str) {
	int i, len = strlen(str);

	for (i = 0; i < len; i++) {
		if (str[i] == ',') {
			str[i] = ' ';
		}
	}
}
int barcode_to_index(char* barcode)
{
	int i, set_num = 1, result = 0;
	int length = strlen(barcode);

	if (length != 3) {
		return 0;		// 잘못된 입력을 위해
	}
	for (i = 2; i >= 0; i--) {		
		if (barcode[i] > '1' || barcode[i] < '0') {
			return 0;
		}
		result += (set_num * (barcode[i] - '0'));
		set_num *= 2;
	}
	return result;
}
void init_item_arr(item arr[], int n)
{
	strncpy(arr[0].name, "snack", 5);
	arr[0].name[5] = 0;
	arr[0].price = 1000;
	arr[0].quantity = n;

	strncpy(arr[1].name, "ice_cream", 9);
	arr[1].name[9] = 0;
	arr[1].price = 1500;
	arr[1].quantity = n;

	strncpy(arr[2].name, "fruit", 5);
	arr[2].name[5] = 0;
	arr[2].price = 3000;
	arr[2].quantity = n;

	strncpy(arr[3].name, "water", 5);
	arr[3].name[5] = 0;
	arr[3].price = 500;
	arr[3].quantity = n;

	strncpy(arr[4].name, "ramen", 5);
	arr[4].name[5] = 0;
	arr[4].price = 800;
	arr[4].quantity = n;

	strncpy(arr[5].name, "beverage", 8);
	arr[5].name[8] = 0;
	arr[5].price = 1200;
	arr[5].quantity = n;

	strncpy(arr[6].name, "coffee", 6);
	arr[6].name[6] = 0;
	arr[6].price = 2000;
	arr[6].quantity = n;
}
void init_sale_info(sale* sale_info)
{
	init_item_arr(sale_info->s_arr, 0);
	sale_info->recv_money = 0;
	sale_info->snd_money = 0;
	sale_info->total_price = 0;
}
void init_stock_info(sale* stock_info)
{
	init_item_arr(stock_info->s_arr, 100);
	stock_info->recv_money = 0;
	stock_info->snd_money = 0;
	stock_info->total_price = 0;
}
void sale_list_manage(sale* sale_info, sale* stock_info)
{
	int index;
	char barcode[BARCODE_LENGTH] = { 0 };

	printf("\nSale Mode\n");

	while (1)
	{
		printf("Barcode Input\n-> Barcode : ");
		scanf("%s", barcode);
		index = barcode_to_index(barcode);
		
		if (index >= 1 && index <= 7) {			// 제대로된 입력
			break;
		}
		else {
			printf("Wrong Input!\n\n");
			continue;
		}
	}
	pthread_mutex_lock(&mutex1);
	if (stock_info->s_arr[index - 1].quantity == 0) {
		// message passing 부분
		printf("\n\nCasher Screen\nDisplay\n");
		printf("%s is sold out\n\n", stock_info->s_arr[index - 1].name);
		pthread_mutex_unlock(&mutex1);
		return;
	}

	sale_info->s_arr[index-1].quantity++;
	stock_info->s_arr[index-1].quantity--;
	sale_info->total_price += (sale_info->s_arr[index-1].price);			

	sale_display(sale_info);
	pthread_mutex_unlock(&mutex1);
}
void quantity_change(sale* sale_info, sale* stock_info)
{
	int index, sel;
	printf("\nQuantity Change Mode\n");

	while (1) {
		printf("1. snack 2.ice cream 3.fruit 4.water 5.ramen 6.beverage 7.coffee\n-> ");
		scanf("%d", &index);
		if (index >= 1 && index <= 7) {			// 제대로된 입력
			break;
		}
		else {
			printf("Wrong Input!\n\n");
			continue;
		}
	}
	while (1) {
		printf("1. (+) 2. (-) 3. Delete : ");
		scanf("%d", &sel);
	
		if(sel < 1 || sel > 3) {
			printf("Wrong Input!\n\n");
			continue;
		}
		else {
			break;
		}
	}
	pthread_mutex_lock(&mutex1);
	if (sel == 1) {
		if (stock_info->s_arr[index - 1].quantity == 0) {		
			// message passing 부분
			printf("\n\nCasher Screen\nDisplay\n");
			printf("%s is sold out\n\n", stock_info->s_arr[index - 1].name);
		}
		else {
			sale_info->s_arr[index - 1].quantity++;
			stock_info->s_arr[index - 1].quantity--;
			sale_info->total_price += (sale_info->s_arr[index - 1].price);
			// message passing으로 바뀔부분
			sale_display(sale_info);
		}
	}
	else if(sel == 2){
		if (sale_info->s_arr[index - 1].quantity == 0) {
			// message passing 부분
			printf("Wrong Input!\n\n");
		}
		else {
			sale_info->s_arr[index - 1].quantity--;
			stock_info->s_arr[index - 1].quantity++;
			sale_info->total_price -= (sale_info->s_arr[index - 1].price);
			// message passing으로 바뀔부분
			sale_display(sale_info);
		}
	}
	else {
		if (sale_info->s_arr[index - 1].quantity == 0) {
			// message passing 부분
			printf("Wrong Input!\n\n");
			pthread_mutex_unlock(&mutex1);
			return;
		}
		int p_num = 0;
		p_num = sale_info->s_arr[index - 1].quantity;
		sale_info->s_arr[index - 1].quantity = 0;
		sale_info->total_price -= (sale_info->s_arr[index - 1].price * p_num);
		stock_info->s_arr[index - 1].quantity += p_num;
		sale_display(sale_info);
	}
	pthread_mutex_unlock(&mutex1);
}
void money_received(sale* sale_info, sale* stock_info)
{
	// 계속 변경되니 미리 저장
	if (sale_info->total_price == 0) {
		printf("Empty Sale List!!\n");
		return;
	}
	int recv_money;
	printf("\nMoney Received Mode\n\n");

	do {				// 부족하면 계속 돈 받음
		printf("Received Money : ");
		scanf("%d", &recv_money);
	} while (recv_money < sale_info->total_price);

	pthread_mutex_lock(&mutex1);
	sale_info->recv_money = recv_money;
	sale_info->snd_money = recv_money - sale_info->total_price;
	
	calculate_sale_info(sale_info, stock_info);
	pthread_mutex_unlock(&mutex1);
}
void calculate_sale_info(sale* sale_info, sale* stock_info) 
{
	int index;
	get_time_flag = 1;
	printf("\n\n");

	for (index = 0; index < 7; index++) {
		if (sale_info->s_arr[index].quantity != 0) {
			s_r[0][index] += sale_info->s_arr[index].quantity;
		}
	}
	s_r[0][7] += sale_info->total_price;

	get_time_flag = 1;
	int day1 = day;
	int month1 = month;
	int year1 = year;
	int hour1 = hour;
	int min1 = min;

	sale_display(sale_info);
	receipt_list_file(year1, month1, day1, hour1, min1);
	sale_receipt_print(sale_info, year1, month1, day1, hour1, min1);
	database_update(sale_info, stock_info, year1, month1, day1, hour1, min1);
}
void stock_manage(sale* stock_info)
{	
	int i;
	// 계속 변경되니 미리 저장
	get_time_flag = 1;

	printf("Stock Check Mode\n");
	get_time_flag = 1;
	int day1 = day;
	int month1 = month;
	int year1 = year;
	int hour1 = hour;
	int min1 = min;
	
	stock_display(stock_info);

	stock_receipt_print(stock_info, year1, month1, day1, hour1, min1);
}
int refund_availability(char* barcode)
{
	int mode = 0;
	int check = 0;
	char t_barcode[20];
	FILE* list_data = fopen("sale_receipt_list.txt", "rt+");				// 읽기 쓰기 모드로 연다, 사용한 영수증을 무효화(끝에 0)

	if (list_data == NULL) {										// 판매자체가 이루어지지 않았을때
		printf("\n\nCasher Screen\nDisplay\n");
		printf("Can not Refund this Receipt\n");
		return 0;
	}
	
	while (fscanf(list_data, "%s%d", &t_barcode, &check) != EOF)
	{	
		if (check == 0) {				
			if (strncmp(t_barcode, barcode, strlen(t_barcode)) == 0) {		// 체크가 0 이면 사용한 영수증, 영수증 번호까지 같으면 바로 불가능
				printf("\n\nCasher Screen\nDisplay\n");
				printf("Can not Refund this Receipt\n");
				fclose(list_data);
				return 0;
			}
			continue;														// 체크가 0 이면 사용한 영수증
		}
		if (strncmp(t_barcode, barcode, strlen(t_barcode)) == 0) {
			while (1) {
				printf("1. Refund Ok 2. Refund Cancel : ");
				scanf("%d", &mode);
				if (mode == 2) {
					return 0;
				}
				else if (mode == 1) {
					break;
				}
				else {
					printf("Wrong Input!\n\n");
					continue;
				}
			}
			fseek(list_data, -1, SEEK_CUR);
			fprintf(list_data, "%d", 0);									// 사용된 상태로 변경
			fclose(list_data);
			return 1;
		}
	}
	printf("\n\nCasher Screen\nDisplay\n");
	printf("Can not Refund this Receipt\n");
	fclose(list_data);
	return 0;
}
void refund_manage(sale* stock_info, char* barcode)
{
	int i = 0, j = 0, k = 0;
	int len = 0;
	item refund_info[7];
	char temp_name[20];			
	int sum_price;		// 바로 버림
	int total_price = 0;
	char f_name[50];
	char f_str[101];

	init_item_arr(refund_info, 0);
	sprintf(f_name, "sale_%s.txt", barcode);

	get_time_flag = 1;
	FILE* sale_data = fopen(f_name, "rt");
	
	while (fgets(f_str, 100, sale_data) != NULL) {
		if (islower(f_str[0])) {
			delete_comma(f_str);
			sscanf(f_str, "%s%d%d%d", &refund_info[j].name, &refund_info[j].price, &refund_info[j].quantity, &sum_price);
			total_price += sum_price;
			j++;
		}
	}
	fclose(sale_data);

	s_r[1][7] += total_price;
	get_time_flag = 1;
	int day1 = day;
	int month1 = month;
	int year1 = year;
	int hour1 = hour;
	int min1 = min;

	for (i = 0; i < j; i++) {										
		if (refund_info[i].quantity != 0) {
			for ( ; k < 7; k++) {
				if (strncmp(stock_info->s_arr[k].name, refund_info[i].name, strlen(stock_info->s_arr[k].name)) == 0) {			// 이름 같은 거 찾아서 재고 증가
					stock_info->s_arr[k].quantity += refund_info[i].quantity;
					s_r[1][k] += refund_info[i].quantity;
					break;
				}
			}
		}
	}
	refund_display(refund_info, total_price, year1, month1, day1, hour1, min1);
	refund_receipt_print(refund_info, j-1, year1, month1, day1, hour1, min1);
	database_update(NULL, stock_info, year1, month1, day1, hour1, min1);
}