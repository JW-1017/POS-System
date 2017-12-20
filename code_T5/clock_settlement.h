#ifndef __CLOCK_SETTLEMENT_H__
#define __CLOCK_SETTLEMENT_H__

#include <pthread.h>
#include <unistd.h>

#define CLOCK_PER_VIRTUAL_MIN 125			// 나누기 1000이 1초인데 그게 8분으로 적용되니 1/8초가 1분
#define CLOCK_PER_MIN 60000.0

extern int min;
extern int hour;
extern int day;
extern int month;
extern int year;
extern int leap_day;
extern int get_time_flag;		// 1일 때 시간 계산하고 갖고옴 매일 새로운 시작 00 00 
extern int settle_flag;			// 2 면 종료, 0 이면 모드 선택중(판매 환불 진행중), 1이면 정산 가능
extern pthread_mutex_t mutex1;

void check_leap(void);			// 윤년 계산
void day_plus(void);			// 날짜 계산
void* time_to_settlement(void* data);
void settlement_of_sales(void);

#endif
