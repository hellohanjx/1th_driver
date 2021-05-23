//=========================
//用主机通讯使用的协议
//=========================
#include "stdint.h"
#include "uart1.h"
#include "user_config.h"
#include "sys.h"
#include "bsp_master.h"
#include "master_protocol.h" 
#include "stm32f10x.h"
#include "timer4.h"
#include "timer3.h"


static const char softVersion[] = {"NO.-1803252"};

/*
功能：打包数据，并发送
参数：data：数据指针；len：数据长度
*/
void package_msg_send(uint8_t *data,uint16_t len)
{
	uint8_t str[100],i,j,chk;
	
	i = 0;
	str[i++] = S_STX;			//包头
	str[i++] = MASTER_ADDR ;	//主机地址
	str[i++] = sys_get_addr();	//驱动板地址
	str[i++] = ' ';
	str[i++] = ' ';
	for(j = 0; j < len; j++)//补零特殊字符
	{
		str[i++] = data[j];
	}
	str[3] = (i + 1) >> 8;		//包长
	str[4] = (i + 1);
	
	//添加校验
	for(j = 0, chk = 0; j < i; j++)
	{
		chk +=  str[j];
	}
	str[i++] = chk;
//	str[i++] = ETX;		//填充包尾

	bsp_master_sendString(str,i);
}

/*
功能：ack应答
说明：收到不是立即执行的指令，回复ack
*/
void send_ack_msg(void)//收到主机命令的回复
{
	uint16_t i;
	uint8_t str[20];
	i = 0;
	str[i++] = msg_get_type();	//收到的命令
	str[i++] = 1;//msg_get_result();//只用来表示收到指令
	
	package_msg_send(str,i);

}
/*
功能：回复驱动板忙
*/
void send_busy_msg(void)//主机忙
{
	uint16_t i;
	uint8_t str[20];
	i = 0;
	str[i++] = 222;		//收到的命令，对222的回复
	str[i++] = msg_get_result();//执行结果
	str[i++] = msg_get_cur();	//当前执行的命令
	
	package_msg_send(str,i);
}

/*
功能：通知主机，驱动板收到的包错误
*/
void send_err_msg(uint8_t type)//收到错误包
{
	uint16_t i;
	uint8_t str[20];
	i = 0;
	str[i++] = 0;//因为是错误包，所以这里统一用0
	str[i++] = type;//执行结果
	
	package_msg_send(str,i);
}

/*
功能：通知主机，收到的命令，驱动板不支持
*/
void send_nosupport_msg(void)//不支持的指令
{
	uint16_t i;
	uint8_t str[20];
	i = 0;
	str[i++] = msg_get_type();	//收到的命令
	str[i++] = DONE_RESULT_NOSUPPORT;//不支持的指令
	
	package_msg_send(str,i);

}


/*=========================================================================================================
下面都是对主机查询命令的应答
说明：主机发送查询指令，则回复包为下面的
=========================================================================================================*/
void send_motor_msg(void)//电机类
{
	uint16_t i;
	uint8_t str[20];
	i = 0;
	str[i++] = msg_get_type();	//命令类型
	str[i++] = msg_get_result();//执行结果
	str[i++] = msg_get_cur();	//命令类型
	str[i++] = mo_get_num ();//电机数量
	str[i++] = get_motor_state ( sys_get_motorid ()[0]);//电机状态
	if(mo_get_num() != 1)
		str[i++] = sys_get_motorid ()[1];
	
	package_msg_send(str,i);
}

void send_motorAll_msg(void)//发送所有电机
{
	uint16_t i,j;
	uint8_t str[100];
	i = 0;
	str[i++] = msg_get_type();	//命令类型
	str[i++] = msg_get_result();//执行结果
	str[i++] = msg_get_cur();	//命令类型
	str[i++] = MOTOR_SIZE ; 	//电机数量
	for(j = 0;j < MOTOR_SIZE;j++)
	str[i++] = get_motor_state (j);
	
	package_msg_send(str,i);
}

void send_temp_msg(void)//温度信息回应
{
	uint16_t i;
	uint8_t str[20];
	i = 0;
	str[i++] = msg_get_type();	//命令类型
	str[i++] = msg_get_result();//执行结果
	str[i++] = msg_get_cur();	//命令类型（当前执行的命令类型）
	str[i++] = temp_get_id();//回路编号
	str[i++] = temp_get_state (temp_get_id());	//温度状态
	str[i++] = temp_get_value(temp_get_id()); 	//温度值
	
	package_msg_send(str,i);
}

void send_output_msg(void)//控制回路
{
	uint16_t i;
	uint8_t str[20];
	i = 0;
	str[i++] = msg_get_type();	//命令类型
	str[i++] = msg_get_result();//执行结果
	str[i++] = msg_get_cur();	//命令类型（当前执行的命令类型）
	str[i++] = ex_get_id();//回路编号
	str[i++] = ex_get_state (ex_get_id());	//回路状态
	
	package_msg_send(str,i);
}

void send_outputAll_msg(void)//控制回路
{
	uint16_t i;
	uint8_t str[20];
	i = 0;
	str[i++] = msg_get_type();	//命令类型
	str[i++] = msg_get_result();//执行结果
	str[i++] = msg_get_cur();	//命令类型（当前执行的命令类型）
	
	package_msg_send(str,i);
}

void send_version_msg(void)//版本号查询
{
	uint16_t i,j;
	uint8_t str[20];
	i = 0;
	str[i++] = msg_get_type();	//命令类型
	str[i++] = msg_get_result();//执行结果
	str[i++] = msg_get_cur();	//命令类型（当前执行的命令类型）
	for(j = 0;j < (sizeof(softVersion) -1 );j++)
	{
		if(j == 1)
			str[i++] = DRIVERBOARD_TYPE + '0';
		else
			str[i++] = softVersion[j];
	}
	
	package_msg_send(str,i);
}


/*
发送参数设置应答
*/
void send_paramSet_msg(void)
{
	uint16_t i;
	uint8_t str[20];
	i = 0;
	str[i++] = msg_get_type();	//命令类型
	str[i++] = msg_get_result();//执行结果
	str[i++] = msg_get_cur();	//命令类型（当前执行的命令类型）
	
	package_msg_send(str,i);
}

/*
发送参数读取应答
*/
//void send_paramGet_msg(void)
//{
//	uint16_t i;
//	uint8_t str[100];
//	i = 0;
//	str[i++] = msg_get_type();	//命令类型
//	str[i++] = msg_get_result();//执行结果
//	str[i++] = msg_get_cur();	//命令类型（当前执行的命令类型）
//	
//	//驱动板类型
//	str[i++] = (DRIVERBOARD_TYPE >> 8)&0xff;
//	str[i++] = (DRIVERBOARD_TYPE)&0xff;
//	
//	//温度探头类型
//	str[i++] = (TEMPERATURE_TYPE >> 8)&0xff;	
//	str[i++] = TEMPERATURE_TYPE & 0xff;	
//	
//	//启动电流(mA)
//	str[i++] = (g_motorStartCurrent >> 8) & 0xff 	;	
//	str[i++] = g_motorStartCurrent & 0xff;	
//	
//	//短路电流(mA)
//	str[i++] = (g_motorShortCurrent >> 8) & 0xff	;
//	str[i++] = g_motorShortCurrent & 0xff	;

//	//电机过载电流(mA)
//	str[i++] = (g_motorOverloadCurrent >> 8) & 0xff	;	
//	str[i++] = g_motorOverloadCurrent & 0xff	;	

//	//过载允许时间(s)
//	str[i++] = ((g_motorOverloadTimeout/(TIME_MS_US/TIME4_BASE_INTERRUPT)) >> 8) & 0xff;	
//	str[i++] = (g_motorOverloadTimeout/(TIME_MS_US/TIME4_BASE_INTERRUPT)) & 0xff;	

//	//超时未启动时间(s)
//	str[i++] = ((g_motorStartTimeout/(TIME_S_US/TIME4_BASE_INTERRUPT)) >> 8) & 0xff	;	
//	str[i++] = (g_motorStartTimeout/(TIME_S_US/TIME4_BASE_INTERRUPT)) & 0xff	;	

//	//超时未停止时间(s)
//	str[i++] = ((g_motorStopTimeout/(TIME_S_US/TIME4_BASE_INTERRUPT)) >> 8) & 0xff ;	
//	str[i++] = (g_motorStopTimeout/(TIME_S_US/TIME4_BASE_INTERRUPT)) & 0xff	;	
//	
//	//电机停止延时(ms)
//	str[i++] = (g_motorStopDelay	>> 8) & 0xff	;	
//	str[i++] = g_motorStopDelay & 0xff	;	

//	//出货时信号检测次数
//	str[i++] = (g_signal_saleCount >> 8) & 0xff ;	
//	str[i++] = g_signal_saleCount ;
//	
//	//出货时信号检测间隔(us)
//	str[i++] = (g_signal_saleInterval >> 8) & 0xff;	
//	str[i++] = g_signal_saleInterval ;	

//	//查询时信号检测次数
//	str[i++] = (g_signal_checkCount >> 8) & 0xff ;	
//	str[i++] = g_signal_checkCount ;
//	
//	//查询信号检测间隔(us)
//	str[i++] = (g_signal_checkInterval >> 8) & 0xff;	
//	str[i++] = g_signal_checkInterval ;	
//	package_msg_send(str,i);
//}

///*
//发送参数复位应答
//*/
//void send_paramReset_msg(void)
//{
//	uint16_t i;
//	uint8_t str[20];
//	i = 0;
//	str[i++] = msg_get_type();	//命令类型
//	str[i++] = msg_get_result();//执行结果
//	str[i++] = msg_get_cur();	//命令类型（当前执行的命令类型）
//	
//	package_msg_send(str,i);
//}


/*
功能：与主板通信中断处理回调函数
*/
void cmd_dealwith(volatile uint8_t *data,volatile uint16_t len)
{
	uint32_t i;
	uint8_t chk;
	if(data[2] == sys_get_addr())//驱动板地址
	{
		if(data[0] == M_STX)//包头包尾正确
		{
			//计算校验和
			for(i = 0, chk = 0; i < len - 1; i++)
			{
				chk += data[i];
			}
			
			if(chk == data[len - 1])//校验和正确
			{
				//判断长度
				if((data[3]<< 8 | data[4]) == len)
				{
					msg_set_class(MASTER_CLASS);
					if(data[5] < 16)//执行命令
					{
						//包长度正确
						msg_set_type(data[5]);//收到控制类命令
						if(msg_get_state() == CMD_STATE_IDLE)//空闲
						{
							//发消息
							msg_set_state (CMD_STATE_NEED);	//有命令要执行
							msg_set_result(DONE_RESULT_BUSY);
							msg_set_cur(data[5]);			//要执行的命令
							//其他数据丢给main总线去解析，不要在这里解析
							
							switch(data[5])
							{
								//=================
								//把相同长度的指令放在一起
								//=================
								//指定电机类指令
								//=================
								case MOTOR_CMD_CHECK:
								case MOTOR_CMD_SALE:
								case MOTOR_CMD_RESET:
									if(data[6] == 1)//单货道
									{
										mo_set_num (1);
										mo_set_id (0,data[7]);
									}
									else//双货道
									{
										mo_set_num (data[6]);
										mo_set_id (0,data[7]);
										mo_set_id (1,data[8]);
									}
								break;
								case TEMP_CMD_CHECK:
									temp_set_id(data[6]);
								break;
								//=================
								//控制类指令
								//=================
								case OUTPUT_CMD_CHECK:
								case OUTPUT_CMD_SET:
								case OUTPUT_CMD_RESET:
									ex_set_id (data[6]);//要操作的通道
								break;
								
								case DRIVERBOARD_RESET:
									RESTART();//软复位芯片
								break;
								//=================
								//单字节指令
								//啥也不用作
								//=================
//								case  	MOTOR_CMD_CHECKALL:
//								case	MOTOR_CMD_RESETALL:	
//								case	VERSION_CMD_CHECK:
//								case	OUTPUT_CMD_RESETALL:
//								case	EXECUTE_CMD_STATE:

//								break;
							}	
						}
						send_ack_msg();//收到命令
					}
					
					else//查询执行状态命令
					if(data[5] == EXECUTE_CMD_STATE)
					{
						if(msg_get_state() == CMD_STATE_BUSY || msg_get_state() == CMD_STATE_NEED)//总线忙
						{
							send_busy_msg();
						}
						else
						if(msg_get_state() == CMD_STATE_DONE)//结果待上报
						{
							msg_set_type(data[5]);//收到的命令
						}
						else
						if(msg_get_state() == CMD_STATE_IDLE)//收到的命令
						{
							msg_set_type(data[5]);//收到的命令
							msg_set_state(CMD_STATE_DONE);
						}
						
					}
					else//不支持的命令
					{
						send_err_msg(DONE_RESULT_NOSUPPORT);
					}
				}
				else
				{
					//通知包长不对
					send_err_msg(DONE_RESULT_LENERR);
				}
				
			}
			else
			{
				//通知校验和错误
				send_err_msg(DONE_RESULT_CHKERR);
				
			}
		}
		else
		{
			//通知包头包尾错误
			send_err_msg(DONE_RESULT_FORMATEERR);
		}
	}
	else
	{
		//只有驱动板地址是正确的，才能进行回复,否则会造成485总线冲突
		//这里什么都不做
		uint16_t t;
		t = t;
	}
}

