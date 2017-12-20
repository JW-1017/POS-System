#ifndef __INPUT_TOUCH_CONTROL_H__
#define __INPUT_TOUCH_CONTROL_H__

#define BARCODE_LENGTH 6	// 바코드 최대 길이 3자리
#define MAX_ITEM 7		// 기본 7가지

typedef struct _item {
	char name[11];
	int quantity;
	int price;
}item;

typedef struct sale {
	item s_arr[MAX_ITEM];		
	int total_price;
	int recv_money;
	int snd_money;
}sale;


extern sale sale_info;
extern sale stock_info;

void init_arr(int arr[][8]);
void delete_comma(char* str);				// 
int barcode_to_index(char* barcode);		// 바코드 10진수로 변환
void init_item_arr(item arr[], int n);		// n이 100이면 재고 초기화 0이면 판매 목록 초기화
void init_sale_info(sale* sale_info);
void init_stock_info(sale* stock_info);
void sale_list_manage(sale* sale_info, sale* stock_info);		// 재고 확인을 할 필요 없음 현재 찍힌거 자체가 재고가 있다는 것
void quantity_change(sale* sale_info, sale* stock_info);
void money_received(sale* sale_info, sale* stock_info);
int refund_availability(char* barcode);				// 영수증 번호 뒤에 숫자가 0이면 이미 환불한 영수증
void refund_manage(sale* stock_info, char* barcode);
void stock_manage(sale* stock_info);				// 하루지나면 바로바로 초기화 해주고 판매나 환불 내역도 바로 stock_info에 반영되기 때문에 파일 부르지 않음
void calculate_sale_info(sale* sale_info, sale* stock_info);

#endif