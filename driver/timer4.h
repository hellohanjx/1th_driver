#ifndef __TIMER4_H__
#define __TIMER4_H__

#include "stdint.h"
/*
定时器时钟经过了一个倍频器，倍频值受到APBx时钟相对于AHB分频系数影响，分频系数为1则X1，其他X2
RCC_PCLK1Config(RCC_HCLK_Div2)，设置低速 APB1， APB1 = HCLK/2 
这个是2分频，所以apb2是36M，定时器经过了倍频，时钟频率为72M
*/

 
/*
中断时间公式：T = (TIM_Period + 1) * (TIM_Prescaler + 1) / TIM2
注意：TIM_Period：预装载值；TIM_Prescaler：分频值；TIM2：定时器时钟(72M)
*/

#define TIMER4_PRE_SCALER		35999	//预分频值（TIM_Prescaler）
#define TIMER4_AUTO_LOAD 		199		//重装载值（TIM_Period）(达到这个值中断，比如100us最小中断时隙，就是200us中断一次（**这里不对，应该是100us）)


#define TIME4_TIME_BASE	((TIMER4_AUTO_LOAD+1)*(TIMER4_PRE_SCALER+1)/72000000)	//每次中断的时间间隔



void timer4_config(void) ;
void timer4_start(uint16_t count);
void timer4_stop(void);	
uint32_t get_timer4_ticks(void);

#endif
