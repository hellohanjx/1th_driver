//=========================
//通用定时器4 驱动层
//说明：用来向 bsp层提供接口
//=========================

#include "stm32f10x_tim.h"
#include "misc.h"
#include "timer4.h" 

static volatile uint32_t tim4Ticks ;
//============================
//接收数据呼吸灯计时器
//说明：控制led呼吸灯
//功能：初始化定时器
//============================
void timer4_config(void) 
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure; 
	NVIC_InitTypeDef 			NVIC_InitStructure; 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 	//开定时器4时钟（APB1最高36 M，但是定时器会x2）
	
	TIM_TimeBaseStructure.TIM_Prescaler = TIMER4_PRE_SCALER;   	//预分频（将计数器时钟分频）
	TIM_TimeBaseStructure.TIM_Period = 0;      	//自动重装载器的值
	//TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			//重复计数 关
	
	//时钟分割 4分频（@@@@@这个只有作为输出才有用）
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4; 	
	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_IT_Update; 	//计数方式 (向上计数)
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);         //初始化 time4 计数器配置
	
	//这行放在 TIM_TimeBaseInit 后面，避免开机就进入中断
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);	
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE );                   //允许更新中断
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;             //中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  	//主优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;          //从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;        		//使能通道中断
	NVIC_Init(&NVIC_InitStructure);                            	//初始化中断
	TIM_Cmd(TIM4, DISABLE);                                     //timer4 禁能
}
//============================
//定时器4 启动
//参数: count 定时时长
//============================
void timer4_start(uint16_t count)
{
	tim4Ticks = 0;
	TIM_SetAutoreload(TIM4, count);	//改变重装值
	TIM_SetCounter(TIM4, 0);		//清除计数器,向上计数
	TIM_Cmd(TIM4, ENABLE);
}
//============================
//定时器4 停止
//参数: count 定时时长（us）
//============================
void timer4_stop(void)
{
	TIM_SetAutoreload(TIM4, 0);	//改变重装值
	TIM_SetCounter(TIM4, 0);		//清除计数器,向上计数
	TIM_Cmd(TIM4, DISABLE);
	tim4Ticks = 0;
}
//===============================
//TIM4 中断
//===============================
void TIM4_IRQHandler(void)
{ 
  if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
  { 
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		tim4Ticks++;
  } 
} 
//===============================
//获取定时器4节拍，主要是提供给业务层用，代替systick，因为systick不好操作
//===============================
uint32_t get_timer4_ticks(void)
{
	return tim4Ticks*100;//次数*计数时基 = 中断一次的时间 时基100ms；一个节拍 = 100ms
//	return (TIME4_TIME_BASE * tim4Ticks);//次数*计数时基 = 中断一次的时间
}


