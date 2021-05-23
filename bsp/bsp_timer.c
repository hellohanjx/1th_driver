//========================================
//这里的时钟3，4不用的时候是关闭的，有利于提高cpu速度
//rtc 时钟没有用中断，也是为了加快cpu速度

//========================================

#include "rtc.h"
#include "timer2.h"
#include "timer3.h"
#include "timer4.h"
#include "stdint.h"
#include "bsp_timer.h"

void bsp_timer_init(void)//systick时钟与led呼吸灯相关联，所以放在led初始化中
{
	rtc_config();			//RTC 时钟配置
	timer2_config();
	timer3_config();
	timer4_config();
}

//========================================
//获取当前rtc时间节拍，单位 (s)
//========================================
uint32_t bsp_rtcTime_get(void)
{
	return get_rtc_count();
}



//========================================
//启动/停止 精细计时器 10us基准
//========================================
void bsp_midTimer_start(uint16_t value)
{
	timer3_start(value);
}
void bsp_midTimer_stop(void)
{
	timer3_stop();
}
//获取定时器时钟节拍
uint32_t bsp_midTimer_ticks(void)
{
	return get_timer3_ticks();
}



//========================================
//启动/停止 粗略计时器 500us 基准
//========================================
void bsp_roughTimer_start(uint16_t value)
{
	timer4_start(value);
}
void bsp_roughTimer_stop(void)
{
	timer4_stop();
}
//获取定时器时钟节拍
uint32_t bsp_roughTimer_ticks(void)
{
	return get_timer4_ticks();
}


