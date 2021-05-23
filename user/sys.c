//========================================
//全局相关，系统级
//========================================
#include "user_config.h"
#include "stdint.h"
#include "sys.h"

//========================================
//全局变量
//========================================
static volatile uint8_t address;			//驱动板地址
volatile DRV_STATE drvInfo ;		//售货机状态

//========================================
//封装函数
//说明：内存操作，搬运数据；为各个层提供一个状态存取的接口
//设置类向驱动层，bsp层提供
//获取类向业务层提供
//========================================

//================================================================================
//执行类
//================================================================================

//设置地址
void sys_set_addr(uint8_t rs){
	drvInfo.addr = rs;
}
//获取地址
uint8_t  sys_get_addr(void){
	return drvInfo.addr;
}

//================================================================================
//命令封装函数
//================================================================================
//设置命令类型
void msg_set_class(uint8_t rs){
	drvInfo.cmd.cclass  = rs;
}
//获取命令类型
uint8_t  msg_get_class(void){
	return drvInfo.cmd.cclass;
}

//========================================
//设置子命令类型
void msg_set_type(uint8_t rs){
	drvInfo.cmd.type = rs;
}
//获取子命令类型
uint8_t  msg_get_type(void){
	return drvInfo.cmd.type;
}


//========================================
//设置状态
void msg_set_state(uint8_t rs){
	drvInfo.cmd.state = rs;
	
}
//获取状态
uint8_t  msg_get_state(void){
	return drvInfo.cmd.state;
}


//========================================
//设置结果
void msg_set_result(uint8_t rs){
	drvInfo.cmd.result = rs;
}
//获取结果
uint8_t  msg_get_result(void){
	return drvInfo.cmd.result;
}

//========================================
void msg_set_cur(uint8_t rs){
	drvInfo.cmd.cur = rs;
}
uint8_t msg_get_cur(void){
	return drvInfo.cmd.cur ;
}


//================================================================================
//温度类
//================================================================================
//设置温度值
void temp_set_value(uint8_t id ,uint8_t rs){
	drvInfo.temperature.value[id] = rs;
}
//获取温度值
int  temp_get_value(uint8_t id){
	return drvInfo.temperature.value[id];
}

//========================================
//设置温度探头状态
void temp_set_state(uint8_t id,uint8_t rs){
	if(id < 2)
	drvInfo.temperature.state[id] = rs;
}
//获取温度探头状态
uint8_t  temp_get_state(uint8_t id){ 
	if(id < 2)
	return 	drvInfo.temperature.state[id];
	return FALSE; 
} 
//========================================
//设置温度探头id
void temp_set_id(uint8_t id){
	drvInfo.temperature.id = id;
}
//获取温度探头id
uint8_t  temp_get_id(void){ 
	return 	drvInfo.temperature.id;
}


//================================================================================
//电机类
//================================================================================
//设置电机状态
void mo_set_state(uint8_t id,uint8_t rs){
	drvInfo.motor.state[id]  = rs;
}
//获取电机状态
uint8_t  get_motor_state(uint8_t id){
	return drvInfo.motor.state[id];
}

//========================================
//设置当前正操作的电机数
void mo_set_num(uint8_t rs){
	drvInfo.motor.curNum  = rs;
}
//获取当前正操作的电机数
uint8_t  mo_get_num(void){
	return drvInfo.motor.curNum;
}

//========================================
//设置当前正操作电机的编号
void mo_set_id(uint8_t id,uint8_t rs){
	drvInfo.motor.curID[id]  = rs;
}
//获取当前正操作电机的编号
uint8_t *sys_get_motorid(void){
	return  (uint8_t *)&(drvInfo.motor.curID[0]);
}


//================================================================================
//输出回路类
//================================================================================
//设置输出回路状态
void ex_set_state(uint8_t id,uint8_t rs){
	drvInfo.exChannel.state[id]  = rs;
}
//获取命令类型
uint8_t  ex_get_state(uint8_t id){
	return drvInfo.exChannel.state[id];
}

uint8_t  ex_get_id(void){
	return drvInfo.exChannel.id ;
}
void ex_set_id(uint8_t id){
	 drvInfo.exChannel.id = id ;
}

