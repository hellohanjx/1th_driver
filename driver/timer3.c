//==============================
//timer3 定时器驱动
//==============================
#include "stdint.h"
#include "timer3.h"
#include "stm32f10x_tim.h"
#include "misc.h"

static volatile uint32_t tim3Ticks ;

//======================================
//定时器3配置
//======================================
void timer3_config(void) 
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure; 
	NVIC_InitTypeDef 			NVIC_InitStructure; 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 	//开定时器3时钟（APB1最高36 M，但是定时器会x2）
	
	//===============================================================
	//公式 :  T = (TIM_Period + 1) * (TIM_Prescaler + 1) / TIM2 Clock 
	// TIM_Prescaler = 72000,000/(1/100us)
	//===============================================================
	TIM_TimeBaseStructure.TIM_Prescaler = PRE_SCALER;   	//预分频（将计数器时钟分频）10000HZ 100us
	TIM_TimeBaseStructure.TIM_Period = 0;      		//自动重装载器的值

	//TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;		//重复计数 关 （高级计数器才有，通用无）
	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_FLAG_Update; 	//计数方式 (向上计数)

	//时钟分割 4分频（@@@@@这个只有作为输出才有用）
	//TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 	
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);             	//初始化计数器配置
	
	//这行放在 TIM_TimeBaseInit 后面，避免开机就进入中断
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);	
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );                   //允许更新中断
	
	//定时器中断设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;             //中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  	//主优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;          //从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;        		//使能通道中断
	NVIC_Init(&NVIC_InitStructure);                            	//初始化中断
	TIM_Cmd(TIM3, DISABLE );                                      //timer3 禁能
}
//============================
//定时器3 启动
//参数: 定时时长 = count * 10us
//============================
void timer3_start(uint16_t count)
{
	tim3Ticks = 0;
	TIM_SetAutoreload(TIM3, count);	//改变重装值
	TIM_SetCounter(TIM3,0);			//因为是向上计数，所以这里清除计数器值
	TIM_Cmd(TIM3, ENABLE);
}
//============================
//定时器3 启动
//参数: count 定时时长（us）
//============================
void timer3_stop(void)
{
	TIM_SetAutoreload(TIM3, 0);	//改变重装值
	TIM_SetCounter(TIM3,0);		//因为是向上计数，所以这里清除计数器值
	TIM_Cmd(TIM3, DISABLE);
	tim3Ticks = 0;
}
//===============================
//TIM3 中断
//===============================
void TIM3_IRQHandler(void)
{ 
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{ 
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		tim3Ticks++;
		//经下面方式测试，仿真中断时，定时器仍然在工作
//		tf[0] = TIM_GetCounter(TIM3);
//		tf[1] = TIM_GetCounter(TIM3);
//		tf[2] = TIM_GetCounter(TIM3);
		//TIM_Cmd(TIM3, DISABLE);
	} 
} 
//===============================
//获取定时器3节拍，主要是提供给业务层用，代替systick，因为systick不好操作
//===============================
uint32_t get_timer3_ticks(void)
{
	return tim3Ticks*50;//一个节拍 = 50us
}

