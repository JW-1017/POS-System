#include "screen_display.h"
#include "input_touch_control.h"

void sale_display(sale* sale_info)
{
	int index;

	printf("\n\nCasher Screen\n");
	for (index = 0; index < 7; index++) {
		if (sale_info->s_arr[index].quantity != 0) {
			printf("%10s      quantity : %7d | price : %7d | sum_price : %7d\n", sale_info->s_arr[index].name, sale_info->s_arr[index].quantity
				, sale_info->s_arr[index].price, sale_info->s_arr[index].price * sale_info->s_arr[index].quantity);
		}
	}
	printf("Total Sale Money%58d\nReceive Money%61d\nSend Money%64d\n", sale_info->total_price, sale_info->recv_money, sale_info->snd_money);
	printf("\n\nClient Screen\n");
	printf("Total Sale Money%58d\nReceive Money%61d\nSend Money%64d\n", sale_info->total_price, sale_info->recv_money, sale_info->snd_money);
}
void refund_display(item refund_info[], int t_price, int year1, int month1, int day1, int hour1, int min1)
{
	int i;
	printf("\n\nCasher Screen\n");
	printf("Date                                                      %d.%02d.%02d %02d:%02d\n", year1, month1, day1, hour1, min1);
	for (i = 0; i < 7; i++) {
		if (refund_info[i].quantity != 0) {
			printf("%10s      quantity : %7d | price : %7d | sum_price : %7d\n", refund_info[i].name, refund_info[i].price,
				refund_info[i].quantity, refund_info[i].price * refund_info[i].quantity);
		}
	}
	printf("Total Refund Money%56d\n", t_price);

	printf("\n\nClient Screen\n");
	printf("Date\t \t%d.%02d.%02d %02d:%02d\n", year1, month1, day1, hour1, min1);
	printf("Total Refund Money%56d\n", t_price);
}
void stock_display(sale* stock_info)
{
	int index;
	printf("\n\nCasher Screen\n");
	
	for (index = 0; index < 7; index++) {
		if (stock_info->s_arr[index].quantity != 0) {
			printf("%10s      quantity : %7d | price : %7d\n", stock_info->s_arr[index].name, stock_info->s_arr[index].quantity, stock_info->s_arr[index].price);
		}
	}
}