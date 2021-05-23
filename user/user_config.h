//================================
//功能：全局配置文件
//以后头文件和配置项信息直接加到这里来
//================================

#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

#include "stdint.h"

/*******************************
驱动板类型
0.普通机型
1.金码机型，需要转接板
2.均益机型
3.中吉机型
*******************************/
#define DRIVERBOARD_TYPE 	3	//驱动板类型



//========================================
//宏定义
//========================================
#define MOTOR_SIZE		80	//支持电机数量
#define OUTPUT_SIZE		6	//ac 控制通道数
#define TEMP_SIZE		2	//温度通道数
#define MOTOR_TOGETHER	2	//同时启动的电机数
#define MASTER_ADDR 	1	//主机地址


#define ADDR_MAX		15	//最大地址


#define ADC_INTEVAL		100//电机检测间隔时间 ms
#define MOTOR_INTEVAL	50//电机重新启动与上次启动间隔时间 ms

#define RX_LENGTH	100	//uart1 接收缓冲区长度
#define TX_LENGTH	100	//uart1 发送缓冲区长度

/*
驱动板配置参数
*/
#if DRIVERBOARD_TYPE == 0	//易触机型
/*
经测试，间隔30us检测一次，正常300us即可检测到5次平稳电平（电机卡位）
经测试，大概最长600ms，从卡位在位到不在位
*/
	#define TEMPERATURE_TYPE 1	//温度传感器类型	0: 3470/2.5k 金码机型；1: 3470/5k 易触机型；2: 3950/10k; 自用机型
	#define START_CURRENT		30		//电机启动电流 mA
	#define SHORT_CURRENT		5000	//电机短路电流 mA 
	#define OVERLOAD_CURRENT	3000 	//电机过载电流 mA
	#define OVERLOAD_TIMEOUT	1000 	//电机过载超时时间 ms
	#define START_TIMEOUT		1000	//电机超时未启动 ms 
	#define STOP_TIMEOUT		4500	//电机超时未停止 ms	
	
	#define LOAD_CURRENT	300	//负载电流
	#define LOAD_TIMEOUT_1ST	1500 //负载第一步超时时间 MS(此时间应大于 “超时未启动时间”)
	#define LOAD_TIMEOUT_2ST	4500 //负载第二步超时时间 MS
	
	#define SALE_SIGNAL_FILTER_VAL_1ST	20		//出货滤波初始值
	#define SALE_SIGNAL_COUNT_1ST		(SALE_SIGNAL_FILTER_VAL_1ST)//电机出货时电机转过卡位信号达到次数	（达到此次数，认为电平信号正确）
	#define SALE_SIGNAL_INTERVAL_1ST	3000	//电机出货时电机转过卡位前两次信号检测间隔 us 
	
	#define SALE_SIGNAL_FILTER_VAL_2ND	20		//出货滤波初始值
	#define SALE_SIGNAL_COUNT_2ND		(SALE_SIGNAL_FILTER_VAL_2ND)//电机出货时电机转到卡位信号达到次数	（达到此次数，认为电平信号正确）
	#define SALE_SIGNAL_INTERVAL_2ND	1000	//电机出货时电机转到卡位两次信号检测间隔 us 
	
	#define CHECK_SIGNAL_FILTER_VAL	15//检测滤波初始值
	#define CHECK_SIGNAL_COUNT		(CHECK_SIGNAL_FILTER_VAL)	//检测时信号达到次数	（达到此次数，认为信号电平正确）
	#define CHECK_SIGNAL_INTERVAL	25		//检测时两次信号检测间隔 us
	
	#define STOP_DELAY_TIME		80	//电机停止时的延时 ms
	#define RUN_MIN_TIME		1500//电机运行最小时间 ms（短于此时间出现到位信号，认为电机未启动）
	
	#define NO_INSTALL_TIMEOUT	1000	//电机检测过程最长时间 us（超过此时间未检测到启动电流，认为没有电机）
	
	#define ROUGH_TIME_COUNT	199	//定时器4计数次数，每次500us
	#define MID_TIME_COUNT		1	//定时器3计数次数，每次50us
#endif

#if DRIVERBOARD_TYPE == 1	//金码机型
	#define TEMPERATURE_TYPE 0	//温度传感器类型	0: 3470/2.5k	金码机型；1: 3470/5k 易触机型；2: 3950/10k; 自用机型
	#define START_CURRENT	50	//电机启动电流 mA
	#define SHORT_CURRENT	5000//电机短路电流 mA 
	#define OVERLOAD_CURRENT	3000 //电机过载电流 mA
	#define OVERLOAD_TIMEOUT	1000 //电机过载超时时间 ms
	#define START_TIMEOUT		2000	//电机超时未启动 ms 
	#define STOP_TIMEOUT		8000	//电机超时未停止 ms	
	
	#define LOAD_CURRENT	300	//负载电流
	#define LOAD_TIMEOUT_1ST	2500 //负载第一步超时时间 MS
	#define LOAD_TIMEOUT_2ST	6000 //负载第二步超时时间 MS

	#define SALE_SIGNAL_COUNT_1ST		5		//电机出货时电机转过卡位信号达到次数	（达到此次数，认为电平信号正确）
	#define SALE_SIGNAL_INTERVAL_1ST	3000	//电机出货时电机转过卡位前两次信号检测间隔 us 
	#define SALE_SIGNAL_FILTER_VAL_1ST	10	//出货滤波初始值
	
	#define SALE_SIGNAL_COUNT_2ND		45		//电机出货时电机转到卡位信号达到次数	（达到此次数，认为电平信号正确）
	#define SALE_SIGNAL_INTERVAL_2ND	1000	//电机出货时电机转到卡位两次信号检测间隔 us 
	#define SALE_SIGNAL_FILTER_VAL_2ND	10	//出货滤波初始值
	
	#define CHECK_SIGNAL_COUNT	5			//检测时信号达到次数	（达到此次数，认为信号电平正确）
	#define CHECK_SIGNAL_INTERVAL	20		//检测时两次信号检测间隔 us
	#define CHECK_SIGNAL_FILTER_VAL	10		//检测滤波初始值
	
	#define STOP_DELAY_TIME		100	//电机停止时的延时 ms
	#define RUN_MIN_TIME		2000//电机运行最小时间 ms（短于此时间出现到位信号，认为电机未启动）

	#define ROUGH_TIME_COUNT	199	//计数次数，每次500us
	#define MID_TIME_COUNT		1	//计数次数，每次500us
	#define NO_INSTALL_TIMEOUT	2000//检测电机最长时间us（超过此时间未检测到启动电流，认为没有电机）
#endif


#if DRIVERBOARD_TYPE == 2	//均益配置
	#define TEMPERATURE_TYPE 2	//温度传感器类型	0: 3470/2.5k	金码机型；1: 3470/5k 易触机型；2: 3950/10k; 自用机型
	#define START_CURRENT	30	//电机启动电流 mA
	#define SHORT_CURRENT	5000//电机短路电流 mA 
	#define OVERLOAD_CURRENT	3000 //电机过载电流 mA
	#define OVERLOAD_TIMEOUT	1000 //电机过载超时时间 ms
	#define START_TIMEOUT		2000	//电机超时未启动 ms 
	#define STOP_TIMEOUT		6000	//电机超时未停止 ms	
	
	#define LOAD_CURRENT	300	//负载电流
	#define LOAD_TIMEOUT_1ST	2500 //负载第一步超时时间 MS
	#define LOAD_TIMEOUT_2ST	600 //负载第二步超时时间 MS
	
	#define SALE_SIGNAL_COUNT_1ST		5		//电机出货时电机转过卡位信号达到次数	（达到此次数，认为电平信号正确）
	#define SALE_SIGNAL_INTERVAL_1ST	3000	//电机出货时电机转过卡位前两次信号检测间隔 us 
	#define SALE_SIGNAL_FILTER_VAL_1ST	10	//出货滤波初始值
	
	#define SALE_SIGNAL_COUNT_2ND		5		//电机出货时电机转到卡位信号达到次数	（达到此次数，认为电平信号正确）
	#define SALE_SIGNAL_INTERVAL_2ND	1000		//电机出货时电机转到卡位两次信号检测间隔 us 
	#define SALE_SIGNAL_FILTER_VAL_2ND	10	//出货滤波初始值
	
	#define CHECK_SIGNAL_COUNT	5			//检测时信号达到次数	（达到此次数，认为信号电平正确）
	#define CHECK_SIGNAL_INTERVAL	20		//检测时两次信号检测间隔 us
	#define CHECK_SIGNAL_FILTER_VAL	10		//检测滤波初始值
	
	#define STOP_DELAY_TIME		30	//电机停止时的延时 ms
	#define RUN_MIN_TIME		1500//电机运行最小时间 ms（短于此时间出现到位信号，认为电机未启动）

	#define ROUGH_TIME_COUNT	199	//计数次数，每次500us
	#define MID_TIME_COUNT		1	//计数次数，每次500us
	#define NO_INSTALL_TIMEOUT	2000	//检测电机最长时间us（超过此时间未检测到启动电流，认为没有电机）
#endif

#if DRIVERBOARD_TYPE == 3	//中吉机型
/*
经测试，间隔30us检测一次，正常300us即可检测到5次平稳电平（电机卡位）
经测试，大概最长600ms，从卡位在位到不在位
*/
	#define TEMPERATURE_TYPE 2	//温度传感器类型	0: 3470/2.5k 金码机型；1: 3470/5k 易触机型；2: 3950/10k; 自用机型
	#define START_CURRENT		30		//电机启动电流 mA
	#define SHORT_CURRENT		5000	//电机短路电流 mA 
	#define OVERLOAD_CURRENT	3000 	//电机过载电流 mA
	#define OVERLOAD_TIMEOUT	1000 	//电机过载超时时间 ms
	#define START_TIMEOUT		1000	//电机超时未启动 ms 
	#define STOP_TIMEOUT		4500	//电机超时未停止 ms	
	
	#define LOAD_CURRENT	300	//负载电流
	#define LOAD_TIMEOUT_1ST	1500 //负载第一步超时时间 MS(此时间应大于 “超时未启动时间”)
	#define LOAD_TIMEOUT_2ST	4000 //负载第二步超时时间 MS
	
	
	#define SALE_SIGNAL_COUNT_1ST		10		//电机出货时电机转过卡位信号达到次数	（达到此次数，认为电平信号正确）
	#define SALE_SIGNAL_INTERVAL_1ST	3000	//电机出货时电机转过卡位前两次信号检测间隔 us 
	#define SALE_SIGNAL_FILTER_VAL_1ST	20		//出货滤波初始值
	
	#define SALE_SIGNAL_COUNT_2ND		5		//电机出货时电机转到卡位信号达到次数	（达到此次数，认为电平信号正确）
	#define SALE_SIGNAL_INTERVAL_2ND	1000	//电机出货时电机转到卡位两次信号检测间隔 us 
	#define SALE_SIGNAL_FILTER_VAL_2ND	10	//出货滤波初始值
	
	#define CHECK_SIGNAL_COUNT		5		//检测时信号达到次数	（达到此次数，认为信号电平正确）
	#define CHECK_SIGNAL_INTERVAL	30		//检测时两次信号检测间隔 us
	#define CHECK_SIGNAL_FILTER_VAL	10		//检测滤波初始值
	
	#define STOP_DELAY_TIME		100	//电机停止时的延时 ms
	#define RUN_MIN_TIME		1500//电机运行最小时间 ms（短于此时间出现到位信号，认为电机未启动）
	
	#define NO_INSTALL_TIMEOUT	1000	//电机检测过程最长时间 us（超过此时间未检测到启动电流，认为没有电机）
	
	#define ROUGH_TIME_COUNT	199	//定时器4计数次数，每次500us
	#define MID_TIME_COUNT		1	//定时器3计数次数，每次50us
#endif
//========================================
//日志记录命令
//========================================




#endif
