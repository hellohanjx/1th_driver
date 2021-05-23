#ifndef __SYS_H__
#define __SYS_H__

#include "stdint.h"
#include "user_config.h"

typedef void (*MSG)( );//定义一个回调函数类型

//======================
//接收数据回调函数
//说明：处理驱动层接收数据
//======================
#define MASTER_CLASS	1	//主机类
#define DROP_CLASS		2	//调货检测类


//====================================
//主机命令类型
//====================================
#define MOTOR_CMD_CHECK		1	
#define MOTOR_CMD_SALE		2
#define MOTOR_CMD_RESET 	3


#define TEMP_CMD_CHECK		6	//双字节指令
#define OUTPUT_CMD_CHECK	7
#define	OUTPUT_CMD_SET		8
#define OUTPUT_CMD_RESET	9

//单字节指令
#define MOTOR_CMD_CHECKALL	4	//所有电机查询
#define MOTOR_CMD_RESETALL	5	//所有电机复位
#define VERSION_CMD_CHECK	11	//版本查询
#define EXECUTE_CMD_STATE	222	//查询指令
#define OUTPUT_CMD_RESETALL	10



#define DRIVERBOARD_RESET	15	//驱动板复位
#define DROP_CMD_CHECK		16	//调货检测
//====================================
//电机状态
//====================================
#define MOTOR_STATE_NORMAL		1	//正常
#define MOTOR_STATE_OUT			2	//不在位
#define MOTOR_STATE_OVERLOAD	3	//过流
#define MOTOR_STATE_NOSTART		4	//超时未启动
#define MOTOR_STATE_NOSTOP		5	//超时未停止
#define MOTOR_STATE_NOINSTALL	6	//未安装电机
#define MOTOR_STATE_SHORT		7	//电机短路
#define MOTOR_STATE_UNKNOWN		8	//电机位置未知

//====================================
//温度探头状态
//====================================
#define TEMP_STATE_NORMAL		1
#define TEMP_STATE_NOINSTALL	2
#define TEMP_STATE_ERR			3
//====================================
//命令执行结果
//====================================
#define DONE_RESULT_COMPLETE	1	//执行成功
#define DONE_RESULT_FAIL		2	//执行失败
#define DONE_RESULT_BUSY		3	//执行忙
#define	DONE_RESULT_CHKERR		4	//校验和错误
#define DONE_RESULT_LENERR		5	//长度不符
#define DONE_RESULT_NOSUPPORT	6	//不支持的指令
#define DONE_RESULT_FORMATEERR	7	//包头包尾错
#define DONE_RESULT_STOPLESS	8	//出货时小于最小运动时间
#define DONE_RESULT_NOSTART		9	//超时未启动
#define DONE_RESULT_NOSTOP		10//超时未停止
#define DONE_RESULT_SHORT			11//短路
#define DONE_RESULT_OVER			12//过载

//====================================
//命令执行状态
//====================================
#define CMD_STATE_IDLE		0	//空闲
#define CMD_STATE_BUSY		1	//正在执行
#define CMD_STATE_DONE		2	//执行完成
#define CMD_STATE_NEED		3	//需要执行


//=================================
//命令类型
//=================================
typedef struct {
	 uint8_t cclass;		//消息类型,(区分设备：主机，调货检测，自己任务)
	 uint8_t type;		//消息子类型
	 uint8_t state;		//执行状态（每次只允许执行1条指令）注意：执行状态不是空闲的时候，下面所有锁定，不允许更改
	 uint8_t result;	//命令执行结果（动作结束后产生结果）		
	 uint8_t cur;		//当前正在执行的指令
}CMD_CLASS;
//=================================
//温度类型
//=================================
typedef struct 
{
	 uint8_t id;					//操作的温度回路id
	 uint8_t state[TEMP_SIZE]; 		//温度回路状态
	 int8_t  value[TEMP_SIZE]; 		//温度值
}TEMP_CLASS;

//=================================
//电机类型
//=================================
typedef struct 
{
	 uint8_t curNum;					//当前要操做的电机数量
	 uint8_t curID[MOTOR_TOGETHER];	//当前要操作的电机编号
	 uint8_t state[MOTOR_SIZE]; 		//电机状态
}MOTOR_CLASS;
//=================================
//输出类型
//=================================
typedef struct 
{
	 uint8_t id; 					//操作的输出回路id
	 uint8_t state[OUTPUT_SIZE]; 	//输出回路状态
}EXPORT_CLASS;

//=================================
//说明：下面这个结构体用来表示驱动板
//状态，售货机的整体情况
//=================================
typedef struct 
{
	volatile uint8_t addr;				//驱动板地址
	volatile CMD_CLASS cmd;
	volatile MOTOR_CLASS motor;			//电机
	volatile TEMP_CLASS temperature;	//温度
	volatile EXPORT_CLASS exChannel;	//输出
//	volatile uint16_t param[G_PARAM_SIZE];	//参数
}DRV_STATE; 


void sys_set_addr(uint8_t rs);
uint8_t  sys_get_addr(void);
void msg_set_class(uint8_t rs);
uint8_t  msg_get_class(void);
void msg_set_type(uint8_t rs);
uint8_t  msg_get_type(void);
void msg_set_state(uint8_t rs);
uint8_t  msg_get_state(void);
void msg_set_result(uint8_t rs);
uint8_t  msg_get_result(void);
void temp_set_value(uint8_t id ,uint8_t rs);
int  temp_get_value(uint8_t id);//注意这个是有符号型
void temp_set_state(uint8_t id,uint8_t rs);
uint8_t  temp_get_state(uint8_t id);
void mo_set_state(uint8_t id,uint8_t rs);
uint8_t  get_motor_state(uint8_t id);
void mo_set_num(uint8_t rs);
uint8_t  mo_get_num(void);
void mo_set_id(uint8_t id,uint8_t rs);

//uint8_t  sys_get_motorid(uint8_t id);

void ex_set_state(uint8_t id,uint8_t rs);
uint8_t  ex_get_state(uint8_t id);
 uint8_t *sys_get_motorid(void);
					
uint8_t  ex_get_id(void);
void ex_set_id(uint8_t id);
uint8_t  sys_get_channeState(void);
void sys_set_channeState(uint8_t rs);

void sys_set_cmd(uint8_t rs);
uint8_t  sys_get_cmd(void);

void msg_set_cur(uint8_t rs);
uint8_t msg_get_cur(void);

void temp_set_id(uint8_t id);
uint8_t  temp_get_id(void);

void param_set(volatile uint8_t *param);
volatile uint16_t *param_get(void);

#endif

