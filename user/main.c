//=========================================================================
//������
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

#define FLASH_APP_ADDR		(0x08008000)  	//�û������ַ

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
extern volatile DRV_STATE drvInfo ;		//�ۻ���״̬

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
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, FLASH_APP_ADDR); 	//�����������ַ
	__set_PRIMASK(0); 			//bootloader �ص����жϣ������

	bsp_system_init();	
	bsp_timer_init();
	bsp_output_init();
	bsp_motor_init(); 
	bsp_led_init();
	bsp_master_init();
	curTime = bsp_rtcTime_get();//��ʼ��ʱ
	bsp_addr_check();			//���ַ
	bsp_temp_check();			//���¶�
	
	//�������ź��ߵ��
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
//			curr[curr_c] = bsp_current_get();	//������
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
			if(msg_get_class() == MASTER_CLASS)//������������
			{
				msg_set_state (CMD_STATE_BUSY);//����ռ��
				switch(msg_get_type())//��ǰҪִ�е���������
				{
					case MOTOR_CMD_CHECK :	//�����ѯ
						msg_set_result ( logic_motor_check(mo_get_num (),sys_get_motorid())); 
						msg_link_fun(send_motor_msg);//���ͽ�� 
					break;
					case MOTOR_CMD_SALE :	//����
						msg_set_result ( logic_motor_sale(mo_get_num (),sys_get_motorid()));
						msg_link_fun(send_motor_msg);//���ͽ��
					break;
					case MOTOR_CMD_RESET :	//�����λ
//						msg_set_result ( logic_motor_sale(mo_get_num (),sys_get_motorid()));

						msg_set_result ( logic_motor_reset(mo_get_num (),sys_get_motorid()));
						msg_link_fun(send_motor_msg);//���ͽ��
					break;
					case MOTOR_CMD_CHECKALL ://��������ѯ
						msg_set_result ( logic_motor_checkAll());
						msg_link_fun(send_motorAll_msg);
					break;
					case MOTOR_CMD_RESETALL ://��������λ
						msg_set_result ( logic_motor_resetAll());
						msg_link_fun(send_motorAll_msg);
					break;
					case TEMP_CMD_CHECK :	//�¶Ȳ�ѯ
						//ֱ��ȡ�ڴ�����	
						//ֱ����֯���ݷ��;ͺ���
						msg_set_result(DONE_RESULT_COMPLETE);
						msg_link_fun(send_temp_msg);
					break;
					case OUTPUT_CMD_CHECK :	//�����·��ѯ
						//����Ҫ��ѯ���ŵ�λ����ֹ��λ����λ����λ����֪�������
						msg_set_result(logic_ex_check(ex_get_id()));
						msg_link_fun(send_output_msg);

					break;
					case OUTPUT_CMD_SET :	//�����·��
						msg_set_result(logic_ex_open(ex_get_id()));
						msg_link_fun(send_output_msg);
					break;
					case OUTPUT_CMD_RESET :	//�����·��
						msg_set_result(logic_ex_close(ex_get_id()));
						msg_link_fun(send_output_msg);
					break;

					case OUTPUT_CMD_RESETALL :	//�����·��λ
						msg_set_result (logic_ex_reset());
						msg_link_fun(send_outputAll_msg);
					break;
					case VERSION_CMD_CHECK :	//�汾�Ų�ѯ
						msg_set_result(DONE_RESULT_COMPLETE);
						msg_link_fun(send_version_msg);
					break;
					
//					case PARAM_CMD_SET :	//��������
//						msg_set_result(set_flash());
//						msg_link_fun(send_paramSet_msg);
//					break;
//					case PARAM_CMD_GET :	//������ȡ
//						msg_set_result(DONE_RESULT_COMPLETE);
//						msg_link_fun(send_paramGet_msg);
//					break;
//					case PARAM_CMD_RESET :	//������λ
//						msg_set_result(reset_flash());
//						msg_link_fun(send_paramReset_msg);
//					break;
					
//					case DROP_CMD_CHECK :	//��������ѯ
//						//
//					break;
				}
				msg_set_state(CMD_STATE_DONE);
				cmdClearTimer = bsp_rtcTime_get();//�ӵ�ǰ������ɿ�ʼ��ʱ������ָ��ʱ��δ���Ͳ�ѯָ��������״̬������ִ����ָ��
			}
			else	//������������
			{
				//
			}
		}
		//������ִ�����
		//���߷�����һ��ָ����
		else
		if(msg_get_state() == CMD_STATE_DONE)
		{

			if(msg_get_class() == MASTER_CLASS)//������������
			{
				
				if(bsp_rtcTime_get() - cmdClearTimer > 3)//��ʱδȡ������������״̬
				{
					msg_set_state (CMD_STATE_IDLE);
				}
							
				if(msg_get_type() == EXECUTE_CMD_STATE)//��ѯִ�н��
				{
					send_fun(); 
					msg_set_state(CMD_STATE_IDLE);//���ݻ����ţ��������²�ѯ
				}
			}
		}
			
		else//���У�û�¸�
		//if(msg_get_state () == CMD_STATE_IDLE)//���Ƕ���ִ�з�ʽ��������Բ���
		{
			if(bsp_rtcTime_get() - curTime > 5) //5���һ���¶�
			{
				bsp_addr_check();	//���ַ
				bsp_temp_check();	//���¶�
				curTime = bsp_rtcTime_get();	//���¼�ʱ��
			}
		}
	}
}




