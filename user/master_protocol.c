//=========================
//������ͨѶʹ�õ�Э��
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
���ܣ�������ݣ�������
������data������ָ�룻len�����ݳ���
*/
void package_msg_send(uint8_t *data,uint16_t len)
{
	uint8_t str[100],i,j,chk;
	
	i = 0;
	str[i++] = S_STX;			//��ͷ
	str[i++] = MASTER_ADDR ;	//������ַ
	str[i++] = sys_get_addr();	//�������ַ
	str[i++] = ' ';
	str[i++] = ' ';
	for(j = 0; j < len; j++)//���������ַ�
	{
		str[i++] = data[j];
	}
	str[3] = (i + 1) >> 8;		//����
	str[4] = (i + 1);
	
	//���У��
	for(j = 0, chk = 0; j < i; j++)
	{
		chk +=  str[j];
	}
	str[i++] = chk;
//	str[i++] = ETX;		//����β

	bsp_master_sendString(str,i);
}

/*
���ܣ�ackӦ��
˵�����յ���������ִ�е�ָ��ظ�ack
*/
void send_ack_msg(void)//�յ���������Ļظ�
{
	uint16_t i;
	uint8_t str[20];
	i = 0;
	str[i++] = msg_get_type();	//�յ�������
	str[i++] = 1;//msg_get_result();//ֻ������ʾ�յ�ָ��
	
	package_msg_send(str,i);

}
/*
���ܣ��ظ�������æ
*/
void send_busy_msg(void)//����æ
{
	uint16_t i;
	uint8_t str[20];
	i = 0;
	str[i++] = 222;		//�յ��������222�Ļظ�
	str[i++] = msg_get_result();//ִ�н��
	str[i++] = msg_get_cur();	//��ǰִ�е�����
	
	package_msg_send(str,i);
}

/*
���ܣ�֪ͨ�������������յ��İ�����
*/
void send_err_msg(uint8_t type)//�յ������
{
	uint16_t i;
	uint8_t str[20];
	i = 0;
	str[i++] = 0;//��Ϊ�Ǵ��������������ͳһ��0
	str[i++] = type;//ִ�н��
	
	package_msg_send(str,i);
}

/*
���ܣ�֪ͨ�������յ�����������岻֧��
*/
void send_nosupport_msg(void)//��֧�ֵ�ָ��
{
	uint16_t i;
	uint8_t str[20];
	i = 0;
	str[i++] = msg_get_type();	//�յ�������
	str[i++] = DONE_RESULT_NOSUPPORT;//��֧�ֵ�ָ��
	
	package_msg_send(str,i);

}


/*=========================================================================================================
���涼�Ƕ�������ѯ�����Ӧ��
˵�����������Ͳ�ѯָ���ظ���Ϊ�����
=========================================================================================================*/
void send_motor_msg(void)//�����
{
	uint16_t i;
	uint8_t str[20];
	i = 0;
	str[i++] = msg_get_type();	//��������
	str[i++] = msg_get_result();//ִ�н��
	str[i++] = msg_get_cur();	//��������
	str[i++] = mo_get_num ();//�������
	str[i++] = get_motor_state ( sys_get_motorid ()[0]);//���״̬
	if(mo_get_num() != 1)
		str[i++] = sys_get_motorid ()[1];
	
	package_msg_send(str,i);
}

void send_motorAll_msg(void)//�������е��
{
	uint16_t i,j;
	uint8_t str[100];
	i = 0;
	str[i++] = msg_get_type();	//��������
	str[i++] = msg_get_result();//ִ�н��
	str[i++] = msg_get_cur();	//��������
	str[i++] = MOTOR_SIZE ; 	//�������
	for(j = 0;j < MOTOR_SIZE;j++)
	str[i++] = get_motor_state (j);
	
	package_msg_send(str,i);
}

void send_temp_msg(void)//�¶���Ϣ��Ӧ
{
	uint16_t i;
	uint8_t str[20];
	i = 0;
	str[i++] = msg_get_type();	//��������
	str[i++] = msg_get_result();//ִ�н��
	str[i++] = msg_get_cur();	//�������ͣ���ǰִ�е��������ͣ�
	str[i++] = temp_get_id();//��·���
	str[i++] = temp_get_state (temp_get_id());	//�¶�״̬
	str[i++] = temp_get_value(temp_get_id()); 	//�¶�ֵ
	
	package_msg_send(str,i);
}

void send_output_msg(void)//���ƻ�·
{
	uint16_t i;
	uint8_t str[20];
	i = 0;
	str[i++] = msg_get_type();	//��������
	str[i++] = msg_get_result();//ִ�н��
	str[i++] = msg_get_cur();	//�������ͣ���ǰִ�е��������ͣ�
	str[i++] = ex_get_id();//��·���
	str[i++] = ex_get_state (ex_get_id());	//��·״̬
	
	package_msg_send(str,i);
}

void send_outputAll_msg(void)//���ƻ�·
{
	uint16_t i;
	uint8_t str[20];
	i = 0;
	str[i++] = msg_get_type();	//��������
	str[i++] = msg_get_result();//ִ�н��
	str[i++] = msg_get_cur();	//�������ͣ���ǰִ�е��������ͣ�
	
	package_msg_send(str,i);
}

void send_version_msg(void)//�汾�Ų�ѯ
{
	uint16_t i,j;
	uint8_t str[20];
	i = 0;
	str[i++] = msg_get_type();	//��������
	str[i++] = msg_get_result();//ִ�н��
	str[i++] = msg_get_cur();	//�������ͣ���ǰִ�е��������ͣ�
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
���Ͳ�������Ӧ��
*/
void send_paramSet_msg(void)
{
	uint16_t i;
	uint8_t str[20];
	i = 0;
	str[i++] = msg_get_type();	//��������
	str[i++] = msg_get_result();//ִ�н��
	str[i++] = msg_get_cur();	//�������ͣ���ǰִ�е��������ͣ�
	
	package_msg_send(str,i);
}

/*
���Ͳ�����ȡӦ��
*/
//void send_paramGet_msg(void)
//{
//	uint16_t i;
//	uint8_t str[100];
//	i = 0;
//	str[i++] = msg_get_type();	//��������
//	str[i++] = msg_get_result();//ִ�н��
//	str[i++] = msg_get_cur();	//�������ͣ���ǰִ�е��������ͣ�
//	
//	//����������
//	str[i++] = (DRIVERBOARD_TYPE >> 8)&0xff;
//	str[i++] = (DRIVERBOARD_TYPE)&0xff;
//	
//	//�¶�̽ͷ����
//	str[i++] = (TEMPERATURE_TYPE >> 8)&0xff;	
//	str[i++] = TEMPERATURE_TYPE & 0xff;	
//	
//	//��������(mA)
//	str[i++] = (g_motorStartCurrent >> 8) & 0xff 	;	
//	str[i++] = g_motorStartCurrent & 0xff;	
//	
//	//��·����(mA)
//	str[i++] = (g_motorShortCurrent >> 8) & 0xff	;
//	str[i++] = g_motorShortCurrent & 0xff	;

//	//������ص���(mA)
//	str[i++] = (g_motorOverloadCurrent >> 8) & 0xff	;	
//	str[i++] = g_motorOverloadCurrent & 0xff	;	

//	//��������ʱ��(s)
//	str[i++] = ((g_motorOverloadTimeout/(TIME_MS_US/TIME4_BASE_INTERRUPT)) >> 8) & 0xff;	
//	str[i++] = (g_motorOverloadTimeout/(TIME_MS_US/TIME4_BASE_INTERRUPT)) & 0xff;	

//	//��ʱδ����ʱ��(s)
//	str[i++] = ((g_motorStartTimeout/(TIME_S_US/TIME4_BASE_INTERRUPT)) >> 8) & 0xff	;	
//	str[i++] = (g_motorStartTimeout/(TIME_S_US/TIME4_BASE_INTERRUPT)) & 0xff	;	

//	//��ʱδֹͣʱ��(s)
//	str[i++] = ((g_motorStopTimeout/(TIME_S_US/TIME4_BASE_INTERRUPT)) >> 8) & 0xff ;	
//	str[i++] = (g_motorStopTimeout/(TIME_S_US/TIME4_BASE_INTERRUPT)) & 0xff	;	
//	
//	//���ֹͣ��ʱ(ms)
//	str[i++] = (g_motorStopDelay	>> 8) & 0xff	;	
//	str[i++] = g_motorStopDelay & 0xff	;	

//	//����ʱ�źż�����
//	str[i++] = (g_signal_saleCount >> 8) & 0xff ;	
//	str[i++] = g_signal_saleCount ;
//	
//	//����ʱ�źż����(us)
//	str[i++] = (g_signal_saleInterval >> 8) & 0xff;	
//	str[i++] = g_signal_saleInterval ;	

//	//��ѯʱ�źż�����
//	str[i++] = (g_signal_checkCount >> 8) & 0xff ;	
//	str[i++] = g_signal_checkCount ;
//	
//	//��ѯ�źż����(us)
//	str[i++] = (g_signal_checkInterval >> 8) & 0xff;	
//	str[i++] = g_signal_checkInterval ;	
//	package_msg_send(str,i);
//}

///*
//���Ͳ�����λӦ��
//*/
//void send_paramReset_msg(void)
//{
//	uint16_t i;
//	uint8_t str[20];
//	i = 0;
//	str[i++] = msg_get_type();	//��������
//	str[i++] = msg_get_result();//ִ�н��
//	str[i++] = msg_get_cur();	//�������ͣ���ǰִ�е��������ͣ�
//	
//	package_msg_send(str,i);
//}


/*
���ܣ�������ͨ���жϴ���ص�����
*/
void cmd_dealwith(volatile uint8_t *data,volatile uint16_t len)
{
	uint32_t i;
	uint8_t chk;
	if(data[2] == sys_get_addr())//�������ַ
	{
		if(data[0] == M_STX)//��ͷ��β��ȷ
		{
			//����У���
			for(i = 0, chk = 0; i < len - 1; i++)
			{
				chk += data[i];
			}
			
			if(chk == data[len - 1])//У�����ȷ
			{
				//�жϳ���
				if((data[3]<< 8 | data[4]) == len)
				{
					msg_set_class(MASTER_CLASS);
					if(data[5] < 16)//ִ������
					{
						//��������ȷ
						msg_set_type(data[5]);//�յ�����������
						if(msg_get_state() == CMD_STATE_IDLE)//����
						{
							//����Ϣ
							msg_set_state (CMD_STATE_NEED);	//������Ҫִ��
							msg_set_result(DONE_RESULT_BUSY);
							msg_set_cur(data[5]);			//Ҫִ�е�����
							//�������ݶ���main����ȥ��������Ҫ���������
							
							switch(data[5])
							{
								//=================
								//����ͬ���ȵ�ָ�����һ��
								//=================
								//ָ�������ָ��
								//=================
								case MOTOR_CMD_CHECK:
								case MOTOR_CMD_SALE:
								case MOTOR_CMD_RESET:
									if(data[6] == 1)//������
									{
										mo_set_num (1);
										mo_set_id (0,data[7]);
									}
									else//˫����
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
								//������ָ��
								//=================
								case OUTPUT_CMD_CHECK:
								case OUTPUT_CMD_SET:
								case OUTPUT_CMD_RESET:
									ex_set_id (data[6]);//Ҫ������ͨ��
								break;
								
								case DRIVERBOARD_RESET:
									RESTART();//��λоƬ
								break;
								//=================
								//���ֽ�ָ��
								//ɶҲ������
								//=================
//								case  	MOTOR_CMD_CHECKALL:
//								case	MOTOR_CMD_RESETALL:	
//								case	VERSION_CMD_CHECK:
//								case	OUTPUT_CMD_RESETALL:
//								case	EXECUTE_CMD_STATE:

//								break;
							}	
						}
						send_ack_msg();//�յ�����
					}
					
					else//��ѯִ��״̬����
					if(data[5] == EXECUTE_CMD_STATE)
					{
						if(msg_get_state() == CMD_STATE_BUSY || msg_get_state() == CMD_STATE_NEED)//����æ
						{
							send_busy_msg();
						}
						else
						if(msg_get_state() == CMD_STATE_DONE)//������ϱ�
						{
							msg_set_type(data[5]);//�յ�������
						}
						else
						if(msg_get_state() == CMD_STATE_IDLE)//�յ�������
						{
							msg_set_type(data[5]);//�յ�������
							msg_set_state(CMD_STATE_DONE);
						}
						
					}
					else//��֧�ֵ�����
					{
						send_err_msg(DONE_RESULT_NOSUPPORT);
					}
				}
				else
				{
					//֪ͨ��������
					send_err_msg(DONE_RESULT_LENERR);
				}
				
			}
			else
			{
				//֪ͨУ��ʹ���
				send_err_msg(DONE_RESULT_CHKERR);
				
			}
		}
		else
		{
			//֪ͨ��ͷ��β����
			send_err_msg(DONE_RESULT_FORMATEERR);
		}
	}
	else
	{
		//ֻ���������ַ����ȷ�ģ����ܽ��лظ�,��������485���߳�ͻ
		//����ʲô������
		uint16_t t;
		t = t;
	}
}

