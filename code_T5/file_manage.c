#include "input_touch_control.h"
#include "file_manage.h"
#include "clock_settlement.h"

int s_r[2][8];			// 하루 판매 갯수 환불 개수, 각 행 마지막 total 가격

void save_file(sale* stock_info)
{
	get_time_flag = 1;

	FILE* save_data = fopen("save_data.txt", "wt");

	get_time_flag = 1;
	int day1 = day;
	int month1 = month;
	int year1 = year;
	int hour1 = hour;
	int min1 = min;

	fprintf(save_data, "%d %d %d %d %d\n", year1, month1, day1, hour1, min1);
	fprintf(save_data, "%d %d %d %d %d %d %d\n", stock_info->s_arr[0].quantity, stock_info->s_arr[1].quantity, stock_info->s_arr[2].quantity,
		stock_info->s_arr[3].quantity, stock_info->s_arr[4].quantity, stock_info->s_arr[5].quantity, stock_info->s_arr[6].quantity);
	fprintf(save_data, "%d %d %d %d %d %d %d %d\n", s_r[0][0], s_r[0][1], s_r[0][2], s_r[0][3], s_r[0][4], s_r[0][5], s_r[0][6], s_r[0][7]);
	fprintf(save_data, "%d %d %d %d %d %d %d %d\n", s_r[1][0], s_r[1][1], s_r[1][2], s_r[1][3], s_r[1][4], s_r[1][5], s_r[1][6], s_r[1][7]);
	fclose(save_data);
}
void receipt_list_file(int year1, int month1, int day1, int hour1, int min1)
{
	FILE* list_data = fopen("sale_receipt_list.txt", "at");

	fprintf(list_data, "%d%02d%02d%02d%02d 1\n", year1, month1, day1, hour1, min1);

	fclose(list_data);
}
void make_stock_file(sale* stock_info, int year1, int month1, int day1)
{
	int i = 0;
	char f_name[25];
	sprintf(f_name, "%d%02d%02d_product.txt", year1, month1, day1);
	FILE* stock_data = fopen(f_name, "wt");

	fprintf(stock_data, "Date : %d.%02d.%02d (년.월.일)\n", year1, month1, day1);
	fprintf(stock_data, "Sale Product\n");
	for (i = 0; i < 7; i++) {
		fprintf(stock_data, "Name, Price, Quantity\n");
		fprintf(stock_data, "%s, %d, %d\n", stock_info->s_arr[i].name, stock_info->s_arr[i].price, stock_info->s_arr[i].quantity);
	}
	fclose(stock_data);
}
void sale_receipt_print(sale* sale_info, int year1, int month1, int day1, int hour1, int min1)
{
	int i = 0;
	char f_name[50];
	sprintf(f_name, "sale_%d%02d%02d%02d%02d.txt", year1, month1, day1, hour1, min1);

	FILE* sale_data = fopen(f_name, "wt");
	fprintf(sale_data, "Receipt Num : %d.%02d.%02d.%02d.%02d (년.월.일.시.분)\n", year1, month1, day1, hour1, min1);
	fprintf(sale_data, "Date : %d.%02d.%02d (년.월.일)\n", year1, month1, day1);
	fprintf(sale_data, "Sale Product\n");

	for (i = 0; i < 7; i++) {
		if (sale_info->s_arr[i].quantity != 0) {
			fprintf(sale_data, "Name, Price, Quantity, Sum_price\n");
			fprintf(sale_data, "%s, %d, %d, %d\n", sale_info->s_arr[i].name, sale_info->s_arr[i].price, sale_info->s_arr[i].quantity, sale_info->s_arr[i].price * sale_info->s_arr[i].quantity);
		}
	}
	fprintf(sale_data, "Total Sale Price : %d\n", sale_info->total_price);
	fclose(sale_data);
}
void sale_list_file(sale* sale_info, int year1, int month1, int day1, int hour1, int min1)
{
	int i = 0;
	char f_name[50];
	sprintf(f_name, "%d%02d%02d_sale_management.txt", year1, month1, day1);

	FILE* sale_data = fopen(f_name, "at");
	fprintf(sale_data, "Receipt Num : %d.%02d.%02d.%02d.%02d (년.월.일.시.분)\n", year1, month1, day1, hour1, min1);
	fprintf(sale_data, "Date : %d.%02d.%02d (년.월.일)\n", year1, month1, day1);
	fprintf(sale_data, "Sale Product\n");
	for (i = 0; i < 7; i++) {
		if (sale_info->s_arr[i].quantity != 0) {
			fprintf(sale_data, "Name, Price, Quantity, Sum_price\n");
			fprintf(sale_data, "%s, %d, %d, %d\n", sale_info->s_arr[i].name, sale_info->s_arr[i].price, sale_info->s_arr[i].quantity, sale_info->s_arr[i].price * sale_info->s_arr[i].quantity);
		}
	}
	fprintf(sale_data, "Total Sale price : %d\n", sale_info->total_price);
	fclose(sale_data);
}
void database_update(sale* sale_info, sale* stock_info, int year1, int month1, int day1, int hour1, int min1)
{
	make_stock_file(stock_info, year1, month1, day1);
	if (sale_info != NULL)												// 환불에선 이용안함 (NULL 넘김)
	{
		sale_list_file(sale_info, year1, month1, day1, hour1, min1);

	}
}
void refund_receipt_print(item refund_info[], int index, int year1, int month1, int day1, int hour1, int min1)
{
	int i = 0;
	int total = 0;
	char f_name[50];
	sprintf(f_name, "refund_%d%02d%02d%02d%02d.txt", year1, month1, day1, hour1, min1);
	FILE* refund_data = fopen(f_name, "wt");
	fprintf(refund_data, "Receipt Num : %d.%02d.%02d.%02d.%02d (년.월.일.시.분)\n", year1, month1, day1, hour1, min1);
	fprintf(refund_data, "Date : %d.%02d.%02d (년.월.일)\n", year1, month1, day1);
	fprintf(refund_data, "Refund Product\n");
	for (i = 0; i <= index; i++) {
		if (refund_info[i].quantity != 0) {
			fprintf(refund_data, "Name, Price, Quantity, Sum_price\n");
			fprintf(refund_data, "%s, %d, %d, %d\n", refund_info[i].name, refund_info[i].price, refund_info[i].quantity, refund_info[i].price * refund_info[i].quantity);
			total += refund_info[i].price * refund_info[i].quantity;
		}
	}
	fprintf(refund_data, "Total Refund Price : %d\n", total);
	fclose(refund_data);
}
void stock_receipt_print(sale* stock_info, int year1, int month1, int day1, int hour1, int min1)
{
	int i = 0;
	char f_name[50];
	sprintf(f_name, "stock_%d%02d%02d.txt", year1, month1, day1);
	FILE* stock_data = fopen(f_name, "wt");

	fprintf(stock_data, "Date : %d.%02d.%02d.%02d.%02d (년.월.일.시.분)\n", year1, month1, day1, hour1, min1);
	for (i = 0; i < 7; i++) {
		fprintf(stock_data, "Name, Price, Quantity\n");
		fprintf(stock_data, "%s, %d, %d\n", stock_info->s_arr[i].name, stock_info->s_arr[i].price, stock_info->s_arr[i].quantity);
	}
	fclose(stock_data);
}
void settlement_report_print()
{
	int i = 0;
	char f_name[50];
	int day1 = day;
	int month1 = month;
	int year1 = year;
	item temp[7];

	init_item_arr(temp, 0);
	sprintf(f_name, "settle_%d%02d%02d.txt", year1, month1, day1);
	FILE* settle_data = fopen(f_name, "wt");

	fprintf(settle_data, "Date : %d.%02d.%02d (년.월.일)\n", year1, month1, day1);
	fprintf(settle_data, "Sale Product\n");
	for (i = 0; i < 7; i++) {
		if (s_r[0][i] != 0) {
			fprintf(settle_data, "Name, Price, Quantity, Sum_price\n");
			fprintf(settle_data, "%s, %d, %d, %d\n", temp[i].name, temp[i].price, s_r[0][i], temp[i].price * s_r[0][i]);
		}
	}
	fprintf(settle_data, "Total Sale Price : %d\n", s_r[0][7]);

	fprintf(settle_data, "\nRefund Product\n");
	for (i = 0; i < 7; i++) {
		if (s_r[1][i] != 0) {
			fprintf(settle_data, "Name, Price, Quantity, Sum_price\n");
			fprintf(settle_data, "%s, %d, %d, %d\n", temp[i].name, temp[i].price, s_r[1][i], temp[i].price * s_r[1][i]);
		}
	}
	fprintf(settle_data, "Total Refund Price : %d\n", s_r[1][7]);
	fclose(settle_data);
}