#ifndef __TIMER4_H__
#define __TIMER4_H__

#include "stdint.h"
/*
��ʱ��ʱ�Ӿ�����һ����Ƶ������Ƶֵ�ܵ�APBxʱ�������AHB��Ƶϵ��Ӱ�죬��Ƶϵ��Ϊ1��X1������X2
RCC_PCLK1Config(RCC_HCLK_Div2)�����õ��� APB1�� APB1 = HCLK/2 
�����2��Ƶ������apb2��36M����ʱ�������˱�Ƶ��ʱ��Ƶ��Ϊ72M
*/

 
/*
�ж�ʱ�乫ʽ��T = (TIM_Period + 1) * (TIM_Prescaler + 1) / TIM2
ע�⣺TIM_Period��Ԥװ��ֵ��TIM_Prescaler����Ƶֵ��TIM2����ʱ��ʱ��(72M)
*/

#define TIMER4_PRE_SCALER		35999	//Ԥ��Ƶֵ��TIM_Prescaler��
#define TIMER4_AUTO_LOAD 		199		//��װ��ֵ��TIM_Period��(�ﵽ���ֵ�жϣ�����100us��С�ж�ʱ϶������200us�ж�һ�Σ�**���ﲻ�ԣ�Ӧ����100us��)


#define TIME4_TIME_BASE	((TIMER4_AUTO_LOAD+1)*(TIMER4_PRE_SCALER+1)/72000000)	//ÿ���жϵ�ʱ����



void timer4_config(void) ;
void timer4_start(uint16_t count);
void timer4_stop(void);	
uint32_t get_timer4_ticks(void);

#endif
