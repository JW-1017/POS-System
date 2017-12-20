#ifndef __INPUT_TOUCH_CONTROL_H__
#define __INPUT_TOUCH_CONTROL_H__

#define BARCODE_LENGTH 6	// ���ڵ� �ִ� ���� 3�ڸ�
#define MAX_ITEM 7		// �⺻ 7����

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
int barcode_to_index(char* barcode);		// ���ڵ� 10������ ��ȯ
void init_item_arr(item arr[], int n);		// n�� 100�̸� ��� �ʱ�ȭ 0�̸� �Ǹ� ��� �ʱ�ȭ
void init_sale_info(sale* sale_info);
void init_stock_info(sale* stock_info);
void sale_list_manage(sale* sale_info, sale* stock_info);		// ��� Ȯ���� �� �ʿ� ���� ���� ������ ��ü�� ��� �ִٴ� ��
void quantity_change(sale* sale_info, sale* stock_info);
void money_received(sale* sale_info, sale* stock_info);
int refund_availability(char* barcode);				// ������ ��ȣ �ڿ� ���ڰ� 0�̸� �̹� ȯ���� ������
void refund_manage(sale* stock_info, char* barcode);
void stock_manage(sale* stock_info);				// �Ϸ������� �ٷιٷ� �ʱ�ȭ ���ְ� �Ǹų� ȯ�� ������ �ٷ� stock_info�� �ݿ��Ǳ� ������ ���� �θ��� ����
void calculate_sale_info(sale* sale_info, sale* stock_info);

#endif