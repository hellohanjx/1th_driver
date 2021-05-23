#ifndef __TIMER3_H__
#define __TIMER3_H__

#include "stdint.h"

/*
�ж�ʱ�乫ʽ��T = (TIM_Period + 1) * (TIM_Prescaler + 1) / TIM2
ע�⣺TIM_Period��Ԥװ��ֵ��TIM_Prescaler����Ƶֵ��TIM2����ʱ��ʱ��(72M)
*/

#define AUTO_LOAD 		0		//��װ��ֵ(�ﵽ���ֵ�ж�)
#define PRE_SCALER		3599	//Ԥ��Ƶֵ��50us��һ����)

#define TIME3_TIME_BASE	((AUTO_LOAD+1)*(PRE_SCALER+1)/72000000)	//ÿ���жϵ�ʱ����

void timer3_config(void) ;
void timer3_start(uint16_t count);
void timer3_stop(void);	
uint32_t get_timer3_ticks(void);

#endif

