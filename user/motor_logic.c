//================================
//电机业务层
//调用bsp层接口实现电机运动
//因为一个时间段只允许一个执行一条指令，所以所有的方法都做成一个死循环
//================================
#include "user_config.h"
#include "stdint.h"
#include "bsp_motor.h"
#include "bsp_timer.h"
#include "sys.h"
#include "motor_logic.h"
#include "delay.h" 
#include "bsp_output.h"


enum{INIT, OVER_POSITION};//电机初始态；过卡位


#if DRIVERBOARD_TYPE != 3 
/*
电机检测的时间很短，所以不需要检测过载，只要检测过流
*/
uint8_t  logic_motor_check(uint8_t num,uint8_t *motorID)
{
	uint8_t i;
	uint16_t current;
	uint32_t runTime;
	uint32_t check_count = CHECK_SIGNAL_FILTER_VAL;
	
	if(num > MOTOR_TOGETHER || num == 0)//不允许同时检测超过2个电机
	{
		return DONE_RESULT_NOSUPPORT;//不可执行
	}
	
	bsp_motor_start(motorID[0]);	//启动电机
	if(num > 1)//双电机
	{
		bsp_motor_start(motorID[1]);
	}
	
	bsp_midTimer_start(MID_TIME_COUNT);//启动定时器
	
	while(1)
	{
		for(i = 0; i < num; i++ )
		{
			current = bsp_current_get();
			if(current > SHORT_CURRENT)//短路
			{
				bsp_midTimer_stop();	//停止定时器
				bsp_motor_stop();		//停止电机
				mo_set_state(motorID[i],MOTOR_STATE_SHORT);//更新电机状态，短路
				return DONE_RESULT_COMPLETE;	//过流退出
			}
			else
			{
				runTime = bsp_midTimer_ticks();//读取计时
				if(current > START_CURRENT)//超过启动电流
				{
					while(1)
					{
						runTime = bsp_midTimer_ticks();//读取计时
						if(bsp_motor_check(motorID[i]))//电机在卡位
						{
							check_count++;
						}
						else//不在卡位
						{
							if(check_count > 0)
							check_count--;
						}
						delay_us(CHECK_SIGNAL_INTERVAL);//两次电机位置检测间隔时间
						if(check_count >=  (CHECK_SIGNAL_COUNT + CHECK_SIGNAL_FILTER_VAL) )
						{
							mo_set_state(motorID[i], MOTOR_STATE_NORMAL);//电机正常
							bsp_midTimer_stop();	//停止定时器
							bsp_motor_stop();		//停止电机
							return DONE_RESULT_COMPLETE;				
						}
						else
						if(check_count <= CHECK_SIGNAL_FILTER_VAL - CHECK_SIGNAL_COUNT )
						{
							mo_set_state(motorID[i], MOTOR_STATE_OUT);//电机不在位
							bsp_midTimer_stop();	//停止定时器
							bsp_motor_stop();		//停止电机
							return DONE_RESULT_COMPLETE;						
						}
						
						if(runTime > NO_INSTALL_TIMEOUT)//超过1ms未检测到有效电平
						{
							mo_set_state(motorID[i], MOTOR_STATE_UNKNOWN);//电机位置不确定
							bsp_midTimer_stop();	//停止定时器
							bsp_motor_stop();		//停止电机
							return DONE_RESULT_COMPLETE;
						}
					}
				}
				else
				{
					if(runTime > NO_INSTALL_TIMEOUT)//超过1ms未检测到电流
					{
						bsp_midTimer_stop();	//停止定时器
						bsp_motor_stop();		//停止电机
						mo_set_state(motorID[i],MOTOR_STATE_NOINSTALL);//更新电机状态
						return DONE_RESULT_COMPLETE;
					}
				}
			}
		}
	}
}
#else
/****************************************
@说明：中吉和兴元机器只能测试电机是否存在
****************************************/
/*
电机检测的时间很短，所以不需要检测过载，只要检测过流
*/
uint8_t  logic_motor_check(uint8_t num,uint8_t *motorID)
{
	uint8_t i;
	uint16_t current;
	uint32_t runTime;
	uint32_t check_count = CHECK_SIGNAL_FILTER_VAL;
	
	if(num > MOTOR_TOGETHER || num == 0)//不允许同时检测超过2个电机
	{
		return DONE_RESULT_NOSUPPORT;//不可执行
	}
	
	bsp_motor_start(motorID[0]);	//启动电机
	if(num > 1)//双电机
	{
		bsp_motor_start(motorID[1]);
	}
	
	bsp_midTimer_start(MID_TIME_COUNT);//启动定时器
	
	while(1)
	{
		for(i = 0; i < num; i++ )
		{
			current = bsp_current_get();
			if(current > SHORT_CURRENT)//短路
			{
				bsp_midTimer_stop();	//停止定时器
				bsp_motor_stop();		//停止电机
				mo_set_state(motorID[i],MOTOR_STATE_SHORT);//更新电机状态，短路
				return DONE_RESULT_COMPLETE;	//过流退出
			}
			else
			{
				runTime = bsp_midTimer_ticks();//读取计时
				if(current > START_CURRENT)//超过启动电流
				{
					bsp_midTimer_stop();	//停止定时器
					bsp_motor_stop();		//停止电机
					mo_set_state(motorID[i],MOTOR_STATE_NORMAL);//电机正常
					return DONE_RESULT_COMPLETE;
				}
				else
				{
					if(runTime > NO_INSTALL_TIMEOUT)//超过1ms未检测到电流
					{
						bsp_midTimer_stop();	//停止定时器
						bsp_motor_stop();		//停止电机
						mo_set_state(motorID[i],MOTOR_STATE_NOINSTALL);//更新电机状态
						return DONE_RESULT_COMPLETE;
					}
				}
			}
		}
	}
}
#endif
//=======================
//整机电机查询
//=======================
uint8_t  logic_motor_checkAll(void)
{
	uint8_t i;
	for(i = 0;i < MOTOR_SIZE ;i++)
	{
		delay_ms(MOTOR_INTEVAL);	//这里需要一个延时，拉长检测时间，提供引脚恢复时间
		logic_motor_check(1,&i);
	}
	return DONE_RESULT_COMPLETE;
}



/*
出货逻辑
*/
#if DRIVERBOARD_TYPE != 3
/*
@功能：电机出货
@参数：num，出货电机数；motorID，电机序号指针
@说明：以下适用于带有电机信号线的电机出货
*/
uint8_t  logic_motor_sale(uint8_t num,uint8_t *motorID)
{
	uint8_t i, state;
	uint8_t status = INIT;//电机状态
	uint16_t current;//电机电流
	uint32_t runTime;//运行时间
	uint32_t overLoadTime = 0;//过载时间
	uint32_t checkCount = SALE_SIGNAL_FILTER_VAL_1ST;//电机达到位置计数	
	
	logic_motor_check(num,motorID);//检测是否有电机
	for(i = 0; i < num; i++)
	{
		state = get_motor_state(motorID[i]);
		//只有电机不在位或者电机没按装，退出。
		//其他异常都是过程量，不是状态量
		if( state != MOTOR_STATE_NORMAL)
		{
			return DONE_RESULT_FAIL;
		}
	}
	
	bsp_motor_start(motorID[0]);	//启动电机
	if(num > 1)//双电机
	{
		bsp_motor_start(motorID[1]);
	}
	bsp_roughTimer_start(ROUGH_TIME_COUNT);		//启动粗略硬件定时器
	
	while(1)
	{
		for(i = 0; i < num; i++)
		{
			current = bsp_current_get();//记录电机电流
			runTime = bsp_roughTimer_ticks();//记录运行时间

			switch(status)
			{
				case INIT://电机启动
					if(runTime < START_TIMEOUT || current > LOAD_CURRENT)//超时未启动或电机有些堵转
					{
						if(!bsp_motor_check(motorID[i]))//电机过卡位（电机不在位）
						{
							checkCount++;
							if(checkCount < (SALE_SIGNAL_COUNT_1ST + SALE_SIGNAL_FILTER_VAL_1ST) )
							{
								delay_us(SALE_SIGNAL_INTERVAL_1ST);//延时，等待下次检测
							}
							else
							{
								status = OVER_POSITION;//切换状态
								checkCount = SALE_SIGNAL_FILTER_VAL_2ND;
							}
						}
						else
						{
							if(checkCount > 0)
							checkCount--;
						}
						
						if(current > OVERLOAD_CURRENT)//过载
						{
							if(current > SHORT_CURRENT)//过流
							{
								bsp_roughTimer_stop();	//停止定时器
								bsp_motor_stop();		//停止电机
								mo_set_state(motorID[i], MOTOR_STATE_SHORT);//更新电机状态“短路”
								return DONE_RESULT_SHORT;//短路退出
							}
							
							if(overLoadTime == 0)
								overLoadTime = runTime;
							if(overLoadTime != 0)
							{
								if(runTime - overLoadTime > OVERLOAD_TIMEOUT)//过载时间超过允许时间
								{
									bsp_roughTimer_stop(); 	//停止定时器
									bsp_motor_stop();		//停止电机
									mo_set_state(motorID[i],MOTOR_STATE_OVERLOAD);//电机状态“过载”
									return DONE_RESULT_OVER;//过载
								}
							}
						}
						else
						{
							overLoadTime = 0;//清除过载计时
						}
					}
					else//超时未过卡位
					{
						bsp_roughTimer_stop();	//定时器停止
						bsp_motor_stop();		//电机停止
						mo_set_state(motorID[i], MOTOR_STATE_NOSTART);//电机状态更新，超时未启动
						return DONE_RESULT_NOSTART;//超时未启动
					}
					
					if(runTime > LOAD_TIMEOUT_1ST)//因为载货太重，导致电机转动过慢，这里做一个修正，一般不会到这里
					{
						bsp_roughTimer_stop();	//定时器停止
						bsp_motor_stop();		//电机停止
						mo_set_state(motorID[i], MOTOR_STATE_NOSTART);//电机状态更新“超时未启动”
						return DONE_RESULT_NOSTART;//超时未启动
					}
				break;
					
				case OVER_POSITION://电机经过卡位
					if(runTime > STOP_TIMEOUT)//超时未停止
					{
						bsp_roughTimer_stop();	//定时器停止
						bsp_motor_stop();		//电机停止
						mo_set_state(motorID[i], MOTOR_STATE_NOSTOP);//更新电机状态”超时未停止“
						return DONE_RESULT_NOSTOP;
					}

					if(bsp_motor_check(motorID[i]))//电机到卡位（电机在位）
					{
						checkCount++;
						if(checkCount < (SALE_SIGNAL_COUNT_2ND + SALE_SIGNAL_FILTER_VAL_2ND) )
						{
							delay_us(SALE_SIGNAL_INTERVAL_2ND);//等待一个检测时间
						}
						else
						{
							bsp_roughTimer_stop();		//定时器停止
							delay_ms(STOP_DELAY_TIME);	//电机延时停止
							bsp_motor_stop();			//电机停止
							if(runTime < RUN_MIN_TIME)//出货时间小于电机出货的最小花费时间
							{
								mo_set_state(motorID[i], MOTOR_STATE_UNKNOWN);//电机状态更新为“位置未知”
								return DONE_RESULT_STOPLESS;
							}
							else
							{
								mo_set_state(motorID[i], MOTOR_STATE_NORMAL);//电机状态“正常”
								return DONE_RESULT_COMPLETE;
							}
						}
						
						//防止检测过程中的短路（因为时间很短，不需要检测过载）
						if(current > SHORT_CURRENT)//短路
						{
							bsp_roughTimer_stop();	//停止定时器
							bsp_motor_stop();		//停止电机
							mo_set_state(motorID[i],MOTOR_STATE_SHORT);//更新电机状态，短路
							return DONE_RESULT_SHORT;//短路退出
						}
					}
					else
					{
						if(checkCount > 0)
							checkCount--;
						
						if(runTime < 2000)//如果小于2s，那这个值就直接归零
							checkCount = 0;
						
						if(current > OVERLOAD_CURRENT)//过载
						{
							if(current > SHORT_CURRENT)//短路
							{
								bsp_roughTimer_stop();	//停止定时器
								bsp_motor_stop();		//停止电机
								mo_set_state(motorID[i], MOTOR_STATE_SHORT);//更新电机状态，短路
								return DONE_RESULT_SHORT;//短路退出
							}
							if(overLoadTime == 0)
								overLoadTime = runTime;
							if(overLoadTime != 0)
							{
								if(runTime - overLoadTime > OVERLOAD_TIMEOUT)//过载超时
								{
									bsp_roughTimer_stop(); 	//停止定时器
									bsp_motor_stop();		//停止电机
									mo_set_state(motorID[i],MOTOR_STATE_OVERLOAD);//更新电机状态，过载
									return DONE_RESULT_OVER;//过载退出
								}
							}
						}
						else
						{
							overLoadTime = 0;//清除过载计时
						}						
					}
				break;
			}
		}
	}
}
#else
/************************
中吉电机出货逻辑
************************/
uint16_t adc_value[2048];
uint16_t adc,scount;

uint8_t  logic_motor_sale(uint8_t num,uint8_t *motorID)
{
	uint8_t i, state;
	uint16_t cur;//当次检测电流
	uint16_t current;//电机电流
	uint32_t runTime;//运行时间
	uint32_t overLoadTime = 0, st_time = 0;//过载时间
	uint32_t checkCount = 0, current_up = 0;//电机达到位置计数	
	uint8_t status = 0, change = 0;
	adc = 0;scount=0;
	
	logic_motor_check(num,motorID);//检测是否有电机
	for(i = 0; i < num; i++)
	{
		state = get_motor_state(motorID[i]);
		//只有电机不在位或者电机没按装，退出。
		//其他异常都是过程量，不是状态量
		if( state != MOTOR_STATE_NORMAL)
		{
			return DONE_RESULT_FAIL;
		}
	}
	
	
	bsp_motor_start(motorID[0]);	//启动电机
	if(num > 1)//双电机
	{
		bsp_motor_start(motorID[1]);
	}
	bsp_roughTimer_start(ROUGH_TIME_COUNT);	//启动定时器
	delay_ms(2);
	checkCount = 0;
	while(1)
	{
		for(i = 0; i < num; i++)
		{
			cur = bsp_current_get();	//检测电流
			runTime = bsp_roughTimer_ticks();//记录当前时间
			adc_value[adc%2048] = cur;
			adc = (adc+1)%2048;
			if(cur > 120)//电流大于100mA
			{
				if(status == 1)//经过一次卡位
				{
					current_up++;//连续大于100mA次数
					if(current_up > 50)//经过1次卡位并且电流恢复正常
					{
						change = 1;//记录状态，此时重新计算一次过卡位
					}
				}
				else
				{
					if(current_up > 0)
					current_up--;
				}
			}
			if(cur != 0 && cur < 120)//中吉 < 100 ；新源 < 160
//			if( cur < current && current != 0 && cur < 100)//中吉 < 100 ；新源 < 160
			{
//				bsp_roughTimer_stop();
//				bsp_roughTimer_start(ROUGH_TIME_COUNT);
//				runTime = bsp_roughTimer_ticks();//记录当前时间
				checkCount++;
			}
			else
			{
				if(checkCount > 0)
				{
					if(cur != current )//相等不减
					{
//						if(status == 1 && runTime > 1400)
						if(current > 150)
						{
							checkCount--;
						}
//						else
//						{
//							checkCount = 0;
//						}
					}
				}
				current = cur;
			}
			if(status == 1)
			{
				scount++;
				if(scount == 60)
				{
					scount = scount;
				}
			}
			if(adc == 2000)
			{
				adc = adc;
			}
			
			if(checkCount >= 3)
			{
				if(change == 1 || status == 0)
				{
					if(st_time == 0)
						st_time = runTime;
					status++;
				}
				if(status == 2)//转动超过1.5s	//中吉900000;新源500000
				{
					if(runTime - st_time < 1000)
					{
						bsp_motor_stop();
						bsp_roughTimer_stop();
						checkCount = checkCount;
						mo_set_state(motorID[i], MOTOR_STATE_OUT);//更新电机状态
						return DONE_RESULT_NOSTOP;
						
					}
					if(runTime > 1000)
					{
						delay_ms(STOP_DELAY_TIME);
						bsp_motor_stop();
						bsp_roughTimer_stop();
						checkCount = checkCount;
						mo_set_state(motorID[i], MOTOR_STATE_NORMAL);//更新电机状态
						return DONE_RESULT_COMPLETE;
					}
				}
				else
				{
					checkCount = 0;
				}
			}
			if(runTime > STOP_TIMEOUT)//转动超过6s，超时未停止
			{
				bsp_roughTimer_stop();
				bsp_motor_stop();
				mo_set_state(motorID[i], MOTOR_STATE_NOSTOP);//更新电机状态
				return DONE_RESULT_NOSTOP;
			}
//			delay_us(5);//50us检测一次			
		}
	}
}
#endif





#if DRIVERBOARD_TYPE != 3
/*
功能：电机复位
说明：与电机出货区别是这个不需要判断电机位置
*/
uint8_t  logic_motor_reset(uint8_t num,uint8_t *motorID)
{
	uint8_t i, state;
	uint8_t status = INIT;//电机状态
	uint16_t current;//电机电流
	uint32_t runTime;//运行时间
	uint32_t overLoadTime = 0;//过载时间
	uint32_t checkCount = SALE_SIGNAL_FILTER_VAL_1ST;//电机达到位置计数	

	logic_motor_check(num,motorID);//检测是否有电机
	for(i = 0; i < num ;i++)
	{
		state = get_motor_state(motorID[i]);
		if(state == MOTOR_STATE_NOINSTALL || state ==MOTOR_STATE_OVERLOAD || state ==MOTOR_STATE_SHORT)
		{
			return DONE_RESULT_FAIL;
		}
	}
	
	bsp_motor_start(motorID[0]);	//启动电机
	if(num > 1)//双电机
	{
		bsp_motor_start(motorID[1]);
	}
	bsp_roughTimer_start(ROUGH_TIME_COUNT);		//启动粗略硬件定时器
	
	while(1)
	{
		for(i = 0; i < num; i++)
		{
			current = bsp_current_get();		//读电流
			runTime = bsp_roughTimer_ticks();	//读计时

			switch(status)
			{
				case INIT://电机启动
					if(state != MOTOR_STATE_OUT)//电机在正常位置或者有些堵转
					{
						if(runTime < START_TIMEOUT || current > LOAD_CURRENT)//超时启动时间内
						{
							if(!bsp_motor_check(motorID[i]))//电机过卡位（电机不在位）
							{
								//以下为滤波过程
								checkCount++;
								if(checkCount < SALE_SIGNAL_COUNT_1ST + SALE_SIGNAL_FILTER_VAL_1ST)
								{
									delay_us(SALE_SIGNAL_INTERVAL_1ST);//延时开始下次检测
								}
								else
								{
									checkCount = SALE_SIGNAL_FILTER_VAL_2ND;
									status = OVER_POSITION;//切换状态
								}
							}
							else
							{
								if(checkCount > 0)//这个其实没有用
								checkCount--;
							}
							
							if(current > OVERLOAD_CURRENT)//过载
							{
								if(current > SHORT_CURRENT)//短路
								{
									bsp_roughTimer_stop();	//停止定时器
									bsp_motor_stop();		//停止电机
									mo_set_state(motorID[i],MOTOR_STATE_SHORT);//更新电机状态，短路
									return DONE_RESULT_SHORT;//短路退出
								}
								
								if(overLoadTime == 0)
									overLoadTime = runTime;
								if(overLoadTime != 0)
								{
									if(runTime - overLoadTime > OVERLOAD_TIMEOUT)//过载超时
									{
										bsp_roughTimer_stop(); 	//停止定时器
										bsp_motor_stop();		//停止电机
										mo_set_state(motorID[i],MOTOR_STATE_OVERLOAD);//更新电机状态，过载
										return DONE_RESULT_OVER;//过载退出
									}
								}
							}
							else
							{
								overLoadTime = 0;//清除过载计时
							}
						}
						else//超时未过卡位
						{
							bsp_roughTimer_stop();	//定时器停止
							bsp_motor_stop();		//电机停止
							mo_set_state(motorID[i], MOTOR_STATE_NOSTART);//电机状态更新，超时未启动
							return DONE_RESULT_NOSTART;//超时未启动
						}
						
						if(runTime > LOAD_TIMEOUT_1ST)//因为载货太重，导致电机转动过慢，这里做一个修正，一般不会到这里
						{
							bsp_roughTimer_stop();	//定时器停止
							bsp_motor_stop();		//电机停止
							mo_set_state(motorID[i], MOTOR_STATE_NOSTART);//电机状态更新，超时未启动
							return DONE_RESULT_NOSTART;//超时未启动
						}
					}
					else//初始为不在位，则直接进入下个流程
					{
						status = OVER_POSITION;
						checkCount = SALE_SIGNAL_FILTER_VAL_2ND;
					}
				break;
					
				case OVER_POSITION://电机过卡位，第二个流程检测
					if(runTime > STOP_TIMEOUT)//超时未停止
					{
						bsp_roughTimer_stop();	//定时器停止
						bsp_motor_stop();		//电机停止
						mo_set_state(motorID[i], MOTOR_STATE_NOSTOP);//更新电机状态，超时未停
						return DONE_RESULT_NOSTOP;
					}

					if(bsp_motor_check(motorID[i]))//电机转到位（电机在位）
					{
						checkCount++;
						if(checkCount < (SALE_SIGNAL_FILTER_VAL_2ND +  SALE_SIGNAL_COUNT_2ND) )
						{
							delay_us(SALE_SIGNAL_INTERVAL_2ND);//延时，等待下次检测
						}
						else
						{
							bsp_roughTimer_stop();		//定时器停止
							delay_ms(STOP_DELAY_TIME);	//电机延时停止
							bsp_motor_stop();			//电机停止
							mo_set_state(motorID[i],MOTOR_STATE_NORMAL);//更新电机状态,正常
							return DONE_RESULT_COMPLETE; 
						}
						
						//防止检测过程中的短路（因为时间很短，不需要检测过载）
						if(current > SHORT_CURRENT)//短路
						{
							bsp_roughTimer_stop();	//停止定时器
							bsp_motor_stop();		//停止电机
							mo_set_state(motorID[i],MOTOR_STATE_SHORT);//更新电机状态，短路
							return DONE_RESULT_SHORT;//短路退出
						}
					}
					else
					{
						if(checkCount > 0)
						checkCount--;
						if(current > OVERLOAD_CURRENT)//过载
						{
							if(current > SHORT_CURRENT)//过流
							{
								bsp_roughTimer_stop();	//停止定时器
								bsp_motor_stop();		//停止电机
								mo_set_state(motorID[i],MOTOR_STATE_SHORT);//更新电机状态，短路
								return DONE_RESULT_SHORT;//短路退出
							}
							if(overLoadTime == 0)
								overLoadTime = runTime;
							if(overLoadTime != 0)
							{
								if(runTime - overLoadTime > OVERLOAD_TIMEOUT)//过载超时
								{
									bsp_roughTimer_stop(); 	//停止定时器
									bsp_motor_stop();		//停止电机
									mo_set_state(motorID[i],MOTOR_STATE_OVERLOAD);//更新电机状态，过载
									return DONE_RESULT_OVER;//过载退出
								}
							}
						}
						else
						{
							overLoadTime = 0;	//清除过载计时
						}		
					}							
				break;
			}
		}
	}
}
#else	//中吉新源机型
//uint16_t adc_value[2048],adc;
uint8_t  logic_motor_reset(uint8_t num,uint8_t *motorID)
{
	uint8_t i, state;
	uint16_t cur;//当次检测电流
	uint16_t current;//电机电流
	uint32_t runTime;//运行时间
	uint32_t overLoadTime = 0;//过载时间
	uint32_t checkCount = 0;//电机达到位置计数	
	uint8_t change = 0;

	logic_motor_check(num,motorID);//检测是否有电机
	for(i = 0; i < num ;i++)
	{
		state = get_motor_state(motorID[i]);
		if(state == MOTOR_STATE_NOINSTALL || state ==MOTOR_STATE_OVERLOAD || state ==MOTOR_STATE_SHORT)
		{
			return DONE_RESULT_FAIL;
		}
	}

	bsp_motor_start(motorID[0]);	//启动电机
	if(num > 1)//双电机
	{
		bsp_motor_start(motorID[1]);
	}
	bsp_roughTimer_start(ROUGH_TIME_COUNT);	//启动定时器
	while(1)
	{
		for(i = 0; i < num; i++)
		{
			cur = bsp_current_get();	//检测电流
			runTime = bsp_roughTimer_ticks();//记录当前时间
//			adc_value[adc%2048] = cur;
//			adc++;
			if( cur < current && cur < 100)//中吉 < 100 ；新源 < 160
			{
				checkCount++;
			}
			else
			{
				if(checkCount > 0)
				{
					if(current != cur)
					checkCount--;
				}
				current = cur;
			}
			
			if(checkCount >= 4)
			{
				if(runTime > 1500)//转动超过1s	//中吉900000;新源500000
				{
					delay_ms(STOP_DELAY_TIME);
					bsp_motor_stop();
					bsp_roughTimer_stop();
					checkCount = checkCount;
					mo_set_state(motorID[i], MOTOR_STATE_NORMAL);//电机正常
					return DONE_RESULT_COMPLETE;
				}
				else
				{
					if(change == 0)//到这里说明转的不是完整的一圈
					{
						bsp_roughTimer_stop();
						bsp_roughTimer_start(ROUGH_TIME_COUNT);	//重新计时
						change =1;
					}
					checkCount = 0;
				}
			}
			if(runTime > 6000)//转动超过6s，超时未停止
			{
				bsp_roughTimer_stop();
				bsp_motor_stop();
				mo_set_state(motorID[i],MOTOR_STATE_UNKNOWN);//电机正常
				return DONE_RESULT_NOSTOP;
			}
			delay_us(20);//50us检测一次			
		}
	}
}
#endif
//=======================
//整柜电机复位
//=======================
uint8_t  logic_motor_resetAll(void)
{
	uint8_t i;
	for(i = 0;i < MOTOR_SIZE ;i++)
	{
		delay_ms(MOTOR_INTEVAL);	//这里需要一个延时，拉长检测时间，提供引脚恢复时间
		logic_motor_reset(1,&i);
	}
	return DONE_RESULT_COMPLETE;
}

//=======================
//输出回路开
//=======================
uint8_t logic_ex_open(uint8_t  id)
{
	bsp_output_open(id);
	ex_set_state(id,1);//设置通道状态
	return DONE_RESULT_COMPLETE;
}
//=======================
//输出回路关
//=======================
uint8_t logic_ex_close(uint8_t id)
{
	bsp_output_close(id);
	ex_set_state(id,0);//设置通道状态
	return DONE_RESULT_COMPLETE;
}
//=======================
//关闭全部
//=======================
uint8_t logic_ex_reset(void)
{
	uint8_t i;
	bsp_output_reset();
	for(i=0;i<6;i++) ex_set_state(i,0);//设置通道状态
	return DONE_RESULT_COMPLETE;
}

/*
控制回路引脚查询
*/
uint8_t logic_ex_check(uint8_t id)
{
	ex_set_state(id, bsp_ex_check(id));//更新通道状态
	return DONE_RESULT_COMPLETE;
}

