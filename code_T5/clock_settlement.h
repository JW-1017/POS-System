#ifndef __CLOCK_SETTLEMENT_H__
#define __CLOCK_SETTLEMENT_H__

#include <pthread.h>
#include <unistd.h>

#define CLOCK_PER_VIRTUAL_MIN 125			// ������ 1000�� 1���ε� �װ� 8������ ����Ǵ� 1/8�ʰ� 1��
#define CLOCK_PER_MIN 60000.0

extern int min;
extern int hour;
extern int day;
extern int month;
extern int year;
extern int leap_day;
extern int get_time_flag;		// 1�� �� �ð� ����ϰ� ����� ���� ���ο� ���� 00 00 
extern int settle_flag;			// 2 �� ����, 0 �̸� ��� ������(�Ǹ� ȯ�� ������), 1�̸� ���� ����
extern pthread_mutex_t mutex1;

void check_leap(void);			// ���� ���
void day_plus(void);			// ��¥ ���
void* time_to_settlement(void* data);
void settlement_of_sales(void);

#endif
