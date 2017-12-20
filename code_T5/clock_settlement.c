#include <time.h>
#include <stdio.h>
#include "clock_settlement.h"
#include "input_touch_control.h"
#include "file_manage.h"

int min;
int hour;
int day;
int month;
int year;
int leap_day;
int settle_flag = 1;
int get_time_flag = 0;
pthread_mutex_t mutex1;

void check_leap(void) {						
	if ((!(year % 4) && year % 100) || !(year % 400)) {
		leap_day = 29;
	}
	else {
		leap_day = 28;
	}
}
void day_plus(void)						
{
	day++;

	if (month == 2 && day > leap_day) {
		day = 1;
		month++;
	}
	else if ((month == 4 || month == 6 || month == 7 || month == 9 || month == 11) && day > 30) {
		day = 1;
		month++;
	}
	else if (day > 31) {
		if (month == 12) {
			day = 1;
			month = 1;
			year++;
			check_leap();
			return;
		}
		day = 1;
		month++;
	}
}

void* time_to_settlement(void* data)					// �ð� ��� ����
{
	clock_t start, end;
	clock_t t_start, t_end;		// �ð� üũ���� ���ε�
	int check = 0;
	int temp_hour = hour;					// �ѹ��� ���̰� ������
	int temp_min = min;						// �ѹ��� ���̰� ������
	double set_num = (hour * 60 + min) / 480.0;			// ���� �ð��� 24�ð����� ���� ���� �ð��� ����� ����
	t_start = clock();
	start = clock();
	pthread_mutex_init(&mutex1, NULL);

    while(1)
	{
		if(settle_flag == 2){
			break;
		}
        end = clock();
		if ((double)((end - start) / CLOCK_PER_MIN) + set_num >= 3)			// ���� 3
		{
			t_start = clock();
			day_plus();
			set_num = 0;		
			temp_min = 0;
			temp_hour = 0;
			hour = 0;
			min = 0;
			get_time_flag = 1;

			if (settle_flag == 0) {
				check = 1;
			}
			else if (settle_flag == 1)
			{
				pthread_mutex_lock(&mutex1);
				settlement_of_sales();
				pthread_mutex_unlock(&mutex1);
				check = 0;
			}
			start = end;
			
			pthread_mutex_lock(&mutex1);
			init_arr(s_r);
			init_stock_info(&stock_info);
			make_stock_file(&stock_info, year, month, day);
			sale_list_file(&sale_info, year, month, day, hour, min);
			pthread_mutex_unlock(&mutex1);
		}
		if (get_time_flag == 1) {
			t_end = clock();
			min = ((t_end - t_start) / CLOCK_PER_VIRTUAL_MIN) + temp_min;
			if (min >= 60) {
				hour = (min / 60) + temp_hour;
				min = min % 60;
			}
			get_time_flag = 0;
		}
		if (check == 1 && settle_flag == 1) {				// �ð� �����µ� ��� ���� ���� �ƴ� ��(���� ������ ��)
			pthread_mutex_lock(&mutex1);
			settlement_of_sales();
			pthread_mutex_unlock(&mutex1);
			check = 0;
		}
    }
}

void settlement_of_sales(void)
{
	settlement_report_print();
}