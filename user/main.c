//=========================================================================
//主程序
//=========================================================================
#include "stdint.h"
#include "bsp_others.h"
#include "bsp_output.h"
#include "bsp_led.h"
#include "bsp_motor.h"
#include "bsp_master.h"
#include "sys.h"
#include "motor_logic.h"
#include "delay.h"
#include "bsp_timer.h"
#include "master_protocol.h"
#include "param.h"
#include "misc.h"


static MSG msg_send_fun = 0;
static volatile uint32_t cmdClearTimer ; 

#define FLASH_APP_ADDR		(0x08008000)  	//用户程序地址

/*
*/
uint8_t  msg_link_fun(MSG fun)
{
	if(fun == 0)
		return FALSE ;
	msg_send_fun = fun;
	return TRUE ; 
}
/*
*/
void send_fun(void)
{
	if(msg_send_fun != 0)
		msg_send_fun();
}
	

static volatile uint32_t curTime;
extern volatile DRV_STATE drvInfo ;		//售货机状态

//static	uint16_t check[4][100], cur,last,timeC[4],cc[2048];
//static	uint8_t count = 0, status = 0, curState = 0;
//static uint32_t ccCount,tt;


//static  uint16_t current,scount_1;
//static  uint32_t timeT; 

//uint16_t adc_value[2048],adc,status;
uint16_t curr[4096];
uint32_t curr_c = 0;
uint8_t state =0,state_c = 0;

int main(void)
{
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, FLASH_APP_ADDR); 	//重置向量表地址
	__set_PRIMASK(0); 			//bootloader 关掉了中断，这里打开

	bsp_system_init();	
	bsp_timer_init();
	bsp_output_init();
	bsp_motor_init(); 
	bsp_led_init();
	bsp_master_init();
	curTime = bsp_rtcTime_get();//开始计时
	bsp_addr_check();			//查地址
	bsp_temp_check();			//查温度
	
	//测试无信号线电机
//	while(1)
//	{
//		uint8_t id[2] = {0, 0};
////		logic_motor_check(1, id);
////		delay_ms (2000);
////		logic_motor_reset(1,id);
////		delay_ms (2000);
////		logic_motor_sale(1,id);
////		delay_ms (2000);
//		bsp_motor_start(0);
//		delay_ms (300);
//		while(1)
//		{
//			curr[curr_c] = bsp_current_get();	//检测电流
//			if(curr[curr_c] < 30)
//			{
//				state = 1;
//				
//				bsp_motor_stop();
//				delay_ms (3000);
//				break;
//			}
//			if(curr_c == 4095)
//			{
//				curr_c = curr_c;
//			}
//			curr_c = (curr_c+1)%4096;
////			if(state == 1)
////				state_c++;
////			if(state == 1 && state_c > 200){
////				bsp_motor_stop();
////				delay_ms (3000);
////				break;
////			}

//		}
//		

//	}
	
	while (1)
	{
		if(msg_get_state() == CMD_STATE_NEED)
		{
			if(msg_get_class() == MASTER_CLASS)//主机命令类型
			{
				msg_set_state (CMD_STATE_BUSY);//总线占用
				switch(msg_get_type())//当前要执行的命令类型
				{
					case MOTOR_CMD_CHECK :	//电机查询
						msg_set_result ( logic_motor_check(mo_get_num (),sys_get_motorid())); 
						msg_link_fun(send_motor_msg);//发送结果 
					break;
					case MOTOR_CMD_SALE :	//出货
						msg_set_result ( logic_motor_sale(mo_get_num (),sys_get_motorid()));
						msg_link_fun(send_motor_msg);//发送结果
					break;
					case MOTOR_CMD_RESET :	//电机复位
//						msg_set_result ( logic_motor_sale(mo_get_num (),sys_get_motorid()));

						msg_set_result ( logic_motor_reset(mo_get_num (),sys_get_motorid()));
						msg_link_fun(send_motor_msg);//发送结果
					break;
					case MOTOR_CMD_CHECKALL ://整柜电机查询
						msg_set_result ( logic_motor_checkAll());
						msg_link_fun(send_motorAll_msg);
					break;
					case MOTOR_CMD_RESETALL ://整柜电机复位
						msg_set_result ( logic_motor_resetAll());
						msg_link_fun(send_motorAll_msg);
					break;
					case TEMP_CMD_CHECK :	//温度查询
						//直接取内存数据	
						//直接组织数据发送就好了
						msg_set_result(DONE_RESULT_COMPLETE);
						msg_link_fun(send_temp_msg);
					break;
					case OUTPUT_CMD_CHECK :	//输出回路查询
						//这里要查询引脚电位，防止下位机复位，上位机不知道的情况
						msg_set_result(logic_ex_check(ex_get_id()));
						msg_link_fun(send_output_msg);

					break;
					case OUTPUT_CMD_SET :	//输出回路开
						msg_set_result(logic_ex_open(ex_get_id()));
						msg_link_fun(send_output_msg);
					break;
					case OUTPUT_CMD_RESET :	//输出回路关
						msg_set_result(logic_ex_close(ex_get_id()));
						msg_link_fun(send_output_msg);
					break;

					case OUTPUT_CMD_RESETALL :	//输出回路复位
						msg_set_result (logic_ex_reset());
						msg_link_fun(send_outputAll_msg);
					break;
					case VERSION_CMD_CHECK :	//版本号查询
						msg_set_result(DONE_RESULT_COMPLETE);
						msg_link_fun(send_version_msg);
					break;
					
//					case PARAM_CMD_SET :	//参数设置
//						msg_set_result(set_flash());
//						msg_link_fun(send_paramSet_msg);
//					break;
//					case PARAM_CMD_GET :	//参数读取
//						msg_set_result(DONE_RESULT_COMPLETE);
//						msg_link_fun(send_paramGet_msg);
//					break;
//					case PARAM_CMD_RESET :	//参数复位
//						msg_set_result(reset_flash());
//						msg_link_fun(send_paramReset_msg);
//					break;
					
//					case DROP_CMD_CHECK :	//调货检测查询
//						//
//					break;
				}
				msg_set_state(CMD_STATE_DONE);
				cmdClearTimer = bsp_rtcTime_get();//从当前命令完成开始计时，超过指定时间未发送查询指令，清除命令状态，可以执行新指令
			}
			else	//其他类型命令
			{
				//
			}
		}
		//有命令执行完成
		//或者返回上一条指令结果
		else
		if(msg_get_state() == CMD_STATE_DONE)
		{

			if(msg_get_class() == MASTER_CLASS)//主机命令类型
			{
				
				if(bsp_rtcTime_get() - cmdClearTimer > 3)//超时未取结果，进入空闲状态
				{
					msg_set_state (CMD_STATE_IDLE);
				}
							
				if(msg_get_type() == EXECUTE_CMD_STATE)//查询执行结果
				{
					send_fun(); 
					msg_set_state(CMD_STATE_IDLE);//数据还存着，可以重新查询
				}
			}
		}
			
		else//空闲，没事干
		//if(msg_get_state () == CMD_STATE_IDLE)//都是堵塞执行方式，这里可以不用
		{
			if(bsp_rtcTime_get() - curTime > 5) //5秒查一次温度
			{
				bsp_addr_check();	//查地址
				bsp_temp_check();	//查温度
				curTime = bsp_rtcTime_get();	//更新计时器
			}
		}
	}
}




