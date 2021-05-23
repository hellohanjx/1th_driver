#ifndef __TIMER3_H__
#define __TIMER3_H__

#include "stdint.h"

/*
中断时间公式：T = (TIM_Period + 1) * (TIM_Prescaler + 1) / TIM2
注意：TIM_Period：预装载值；TIM_Prescaler：分频值；TIM2：定时器时钟(72M)
*/

#define AUTO_LOAD 		0		//重装载值(达到这个值中断)
#define PRE_SCALER		3599	//预分频值（50us计一次数)

#define TIME3_TIME_BASE	((AUTO_LOAD+1)*(PRE_SCALER+1)/72000000)	//每次中断的时间间隔

void timer3_config(void) ;
void timer3_start(uint16_t count);
void timer3_stop(void);	
uint32_t get_timer3_ticks(void);

#endif

