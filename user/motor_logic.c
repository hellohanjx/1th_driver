//================================
//���ҵ���
//����bsp��ӿ�ʵ�ֵ���˶�
//��Ϊһ��ʱ���ֻ����һ��ִ��һ��ָ��������еķ���������һ����ѭ��
//================================
#include "user_config.h"
#include "stdint.h"
#include "bsp_motor.h"
#include "bsp_timer.h"
#include "sys.h"
#include "motor_logic.h"
#include "delay.h" 
#include "bsp_output.h"


enum{INIT, OVER_POSITION};//�����ʼ̬������λ


#if DRIVERBOARD_TYPE != 3 
/*
�������ʱ��̣ܶ����Բ���Ҫ�����أ�ֻҪ������
*/
uint8_t  logic_motor_check(uint8_t num,uint8_t *motorID)
{
	uint8_t i;
	uint16_t current;
	uint32_t runTime;
	uint32_t check_count = CHECK_SIGNAL_FILTER_VAL;
	
	if(num > MOTOR_TOGETHER || num == 0)//������ͬʱ��ⳬ��2�����
	{
		return DONE_RESULT_NOSUPPORT;//����ִ��
	}
	
	bsp_motor_start(motorID[0]);	//�������
	if(num > 1)//˫���
	{
		bsp_motor_start(motorID[1]);
	}
	
	bsp_midTimer_start(MID_TIME_COUNT);//������ʱ��
	
	while(1)
	{
		for(i = 0; i < num; i++ )
		{
			current = bsp_current_get();
			if(current > SHORT_CURRENT)//��·
			{
				bsp_midTimer_stop();	//ֹͣ��ʱ��
				bsp_motor_stop();		//ֹͣ���
				mo_set_state(motorID[i],MOTOR_STATE_SHORT);//���µ��״̬����·
				return DONE_RESULT_COMPLETE;	//�����˳�
			}
			else
			{
				runTime = bsp_midTimer_ticks();//��ȡ��ʱ
				if(current > START_CURRENT)//������������
				{
					while(1)
					{
						runTime = bsp_midTimer_ticks();//��ȡ��ʱ
						if(bsp_motor_check(motorID[i]))//����ڿ�λ
						{
							check_count++;
						}
						else//���ڿ�λ
						{
							if(check_count > 0)
							check_count--;
						}
						delay_us(CHECK_SIGNAL_INTERVAL);//���ε��λ�ü����ʱ��
						if(check_count >=  (CHECK_SIGNAL_COUNT + CHECK_SIGNAL_FILTER_VAL) )
						{
							mo_set_state(motorID[i], MOTOR_STATE_NORMAL);//�������
							bsp_midTimer_stop();	//ֹͣ��ʱ��
							bsp_motor_stop();		//ֹͣ���
							return DONE_RESULT_COMPLETE;				
						}
						else
						if(check_count <= CHECK_SIGNAL_FILTER_VAL - CHECK_SIGNAL_COUNT )
						{
							mo_set_state(motorID[i], MOTOR_STATE_OUT);//�������λ
							bsp_midTimer_stop();	//ֹͣ��ʱ��
							bsp_motor_stop();		//ֹͣ���
							return DONE_RESULT_COMPLETE;						
						}
						
						if(runTime > NO_INSTALL_TIMEOUT)//����1msδ��⵽��Ч��ƽ
						{
							mo_set_state(motorID[i], MOTOR_STATE_UNKNOWN);//���λ�ò�ȷ��
							bsp_midTimer_stop();	//ֹͣ��ʱ��
							bsp_motor_stop();		//ֹͣ���
							return DONE_RESULT_COMPLETE;
						}
					}
				}
				else
				{
					if(runTime > NO_INSTALL_TIMEOUT)//����1msδ��⵽����
					{
						bsp_midTimer_stop();	//ֹͣ��ʱ��
						bsp_motor_stop();		//ֹͣ���
						mo_set_state(motorID[i],MOTOR_STATE_NOINSTALL);//���µ��״̬
						return DONE_RESULT_COMPLETE;
					}
				}
			}
		}
	}
}
#else
/****************************************
@˵�����м�����Ԫ����ֻ�ܲ��Ե���Ƿ����
****************************************/
/*
�������ʱ��̣ܶ����Բ���Ҫ�����أ�ֻҪ������
*/
uint8_t  logic_motor_check(uint8_t num,uint8_t *motorID)
{
	uint8_t i;
	uint16_t current;
	uint32_t runTime;
	uint32_t check_count = CHECK_SIGNAL_FILTER_VAL;
	
	if(num > MOTOR_TOGETHER || num == 0)//������ͬʱ��ⳬ��2�����
	{
		return DONE_RESULT_NOSUPPORT;//����ִ��
	}
	
	bsp_motor_start(motorID[0]);	//�������
	if(num > 1)//˫���
	{
		bsp_motor_start(motorID[1]);
	}
	
	bsp_midTimer_start(MID_TIME_COUNT);//������ʱ��
	
	while(1)
	{
		for(i = 0; i < num; i++ )
		{
			current = bsp_current_get();
			if(current > SHORT_CURRENT)//��·
			{
				bsp_midTimer_stop();	//ֹͣ��ʱ��
				bsp_motor_stop();		//ֹͣ���
				mo_set_state(motorID[i],MOTOR_STATE_SHORT);//���µ��״̬����·
				return DONE_RESULT_COMPLETE;	//�����˳�
			}
			else
			{
				runTime = bsp_midTimer_ticks();//��ȡ��ʱ
				if(current > START_CURRENT)//������������
				{
					bsp_midTimer_stop();	//ֹͣ��ʱ��
					bsp_motor_stop();		//ֹͣ���
					mo_set_state(motorID[i],MOTOR_STATE_NORMAL);//�������
					return DONE_RESULT_COMPLETE;
				}
				else
				{
					if(runTime > NO_INSTALL_TIMEOUT)//����1msδ��⵽����
					{
						bsp_midTimer_stop();	//ֹͣ��ʱ��
						bsp_motor_stop();		//ֹͣ���
						mo_set_state(motorID[i],MOTOR_STATE_NOINSTALL);//���µ��״̬
						return DONE_RESULT_COMPLETE;
					}
				}
			}
		}
	}
}
#endif
//=======================
//���������ѯ
//=======================
uint8_t  logic_motor_checkAll(void)
{
	uint8_t i;
	for(i = 0;i < MOTOR_SIZE ;i++)
	{
		delay_ms(MOTOR_INTEVAL);	//������Ҫһ����ʱ���������ʱ�䣬�ṩ���Żָ�ʱ��
		logic_motor_check(1,&i);
	}
	return DONE_RESULT_COMPLETE;
}



/*
�����߼�
*/
#if DRIVERBOARD_TYPE != 3
/*
@���ܣ��������
@������num�������������motorID��������ָ��
@˵�������������ڴ��е���ź��ߵĵ������
*/
uint8_t  logic_motor_sale(uint8_t num,uint8_t *motorID)
{
	uint8_t i, state;
	uint8_t status = INIT;//���״̬
	uint16_t current;//�������
	uint32_t runTime;//����ʱ��
	uint32_t overLoadTime = 0;//����ʱ��
	uint32_t checkCount = SALE_SIGNAL_FILTER_VAL_1ST;//����ﵽλ�ü���	
	
	logic_motor_check(num,motorID);//����Ƿ��е��
	for(i = 0; i < num; i++)
	{
		state = get_motor_state(motorID[i]);
		//ֻ�е������λ���ߵ��û��װ���˳���
		//�����쳣���ǹ�����������״̬��
		if( state != MOTOR_STATE_NORMAL)
		{
			return DONE_RESULT_FAIL;
		}
	}
	
	bsp_motor_start(motorID[0]);	//�������
	if(num > 1)//˫���
	{
		bsp_motor_start(motorID[1]);
	}
	bsp_roughTimer_start(ROUGH_TIME_COUNT);		//��������Ӳ����ʱ��
	
	while(1)
	{
		for(i = 0; i < num; i++)
		{
			current = bsp_current_get();//��¼�������
			runTime = bsp_roughTimer_ticks();//��¼����ʱ��

			switch(status)
			{
				case INIT://�������
					if(runTime < START_TIMEOUT || current > LOAD_CURRENT)//��ʱδ����������Щ��ת
					{
						if(!bsp_motor_check(motorID[i]))//�������λ���������λ��
						{
							checkCount++;
							if(checkCount < (SALE_SIGNAL_COUNT_1ST + SALE_SIGNAL_FILTER_VAL_1ST) )
							{
								delay_us(SALE_SIGNAL_INTERVAL_1ST);//��ʱ���ȴ��´μ��
							}
							else
							{
								status = OVER_POSITION;//�л�״̬
								checkCount = SALE_SIGNAL_FILTER_VAL_2ND;
							}
						}
						else
						{
							if(checkCount > 0)
							checkCount--;
						}
						
						if(current > OVERLOAD_CURRENT)//����
						{
							if(current > SHORT_CURRENT)//����
							{
								bsp_roughTimer_stop();	//ֹͣ��ʱ��
								bsp_motor_stop();		//ֹͣ���
								mo_set_state(motorID[i], MOTOR_STATE_SHORT);//���µ��״̬����·��
								return DONE_RESULT_SHORT;//��·�˳�
							}
							
							if(overLoadTime == 0)
								overLoadTime = runTime;
							if(overLoadTime != 0)
							{
								if(runTime - overLoadTime > OVERLOAD_TIMEOUT)//����ʱ�䳬������ʱ��
								{
									bsp_roughTimer_stop(); 	//ֹͣ��ʱ��
									bsp_motor_stop();		//ֹͣ���
									mo_set_state(motorID[i],MOTOR_STATE_OVERLOAD);//���״̬�����ء�
									return DONE_RESULT_OVER;//����
								}
							}
						}
						else
						{
							overLoadTime = 0;//������ؼ�ʱ
						}
					}
					else//��ʱδ����λ
					{
						bsp_roughTimer_stop();	//��ʱ��ֹͣ
						bsp_motor_stop();		//���ֹͣ
						mo_set_state(motorID[i], MOTOR_STATE_NOSTART);//���״̬���£���ʱδ����
						return DONE_RESULT_NOSTART;//��ʱδ����
					}
					
					if(runTime > LOAD_TIMEOUT_1ST)//��Ϊ�ػ�̫�أ����µ��ת��������������һ��������һ�㲻�ᵽ����
					{
						bsp_roughTimer_stop();	//��ʱ��ֹͣ
						bsp_motor_stop();		//���ֹͣ
						mo_set_state(motorID[i], MOTOR_STATE_NOSTART);//���״̬���¡���ʱδ������
						return DONE_RESULT_NOSTART;//��ʱδ����
					}
				break;
					
				case OVER_POSITION://���������λ
					if(runTime > STOP_TIMEOUT)//��ʱδֹͣ
					{
						bsp_roughTimer_stop();	//��ʱ��ֹͣ
						bsp_motor_stop();		//���ֹͣ
						mo_set_state(motorID[i], MOTOR_STATE_NOSTOP);//���µ��״̬����ʱδֹͣ��
						return DONE_RESULT_NOSTOP;
					}

					if(bsp_motor_check(motorID[i]))//�������λ�������λ��
					{
						checkCount++;
						if(checkCount < (SALE_SIGNAL_COUNT_2ND + SALE_SIGNAL_FILTER_VAL_2ND) )
						{
							delay_us(SALE_SIGNAL_INTERVAL_2ND);//�ȴ�һ�����ʱ��
						}
						else
						{
							bsp_roughTimer_stop();		//��ʱ��ֹͣ
							delay_ms(STOP_DELAY_TIME);	//�����ʱֹͣ
							bsp_motor_stop();			//���ֹͣ
							if(runTime < RUN_MIN_TIME)//����ʱ��С�ڵ����������С����ʱ��
							{
								mo_set_state(motorID[i], MOTOR_STATE_UNKNOWN);//���״̬����Ϊ��λ��δ֪��
								return DONE_RESULT_STOPLESS;
							}
							else
							{
								mo_set_state(motorID[i], MOTOR_STATE_NORMAL);//���״̬��������
								return DONE_RESULT_COMPLETE;
							}
						}
						
						//��ֹ�������еĶ�·����Ϊʱ��̣ܶ�����Ҫ�����أ�
						if(current > SHORT_CURRENT)//��·
						{
							bsp_roughTimer_stop();	//ֹͣ��ʱ��
							bsp_motor_stop();		//ֹͣ���
							mo_set_state(motorID[i],MOTOR_STATE_SHORT);//���µ��״̬����·
							return DONE_RESULT_SHORT;//��·�˳�
						}
					}
					else
					{
						if(checkCount > 0)
							checkCount--;
						
						if(runTime < 2000)//���С��2s�������ֵ��ֱ�ӹ���
							checkCount = 0;
						
						if(current > OVERLOAD_CURRENT)//����
						{
							if(current > SHORT_CURRENT)//��·
							{
								bsp_roughTimer_stop();	//ֹͣ��ʱ��
								bsp_motor_stop();		//ֹͣ���
								mo_set_state(motorID[i], MOTOR_STATE_SHORT);//���µ��״̬����·
								return DONE_RESULT_SHORT;//��·�˳�
							}
							if(overLoadTime == 0)
								overLoadTime = runTime;
							if(overLoadTime != 0)
							{
								if(runTime - overLoadTime > OVERLOAD_TIMEOUT)//���س�ʱ
								{
									bsp_roughTimer_stop(); 	//ֹͣ��ʱ��
									bsp_motor_stop();		//ֹͣ���
									mo_set_state(motorID[i],MOTOR_STATE_OVERLOAD);//���µ��״̬������
									return DONE_RESULT_OVER;//�����˳�
								}
							}
						}
						else
						{
							overLoadTime = 0;//������ؼ�ʱ
						}						
					}
				break;
			}
		}
	}
}
#else
/************************
�м���������߼�
************************/
uint16_t adc_value[2048];
uint16_t adc,scount;

uint8_t  logic_motor_sale(uint8_t num,uint8_t *motorID)
{
	uint8_t i, state;
	uint16_t cur;//���μ�����
	uint16_t current;//�������
	uint32_t runTime;//����ʱ��
	uint32_t overLoadTime = 0, st_time = 0;//����ʱ��
	uint32_t checkCount = 0, current_up = 0;//����ﵽλ�ü���	
	uint8_t status = 0, change = 0;
	adc = 0;scount=0;
	
	logic_motor_check(num,motorID);//����Ƿ��е��
	for(i = 0; i < num; i++)
	{
		state = get_motor_state(motorID[i]);
		//ֻ�е������λ���ߵ��û��װ���˳���
		//�����쳣���ǹ�����������״̬��
		if( state != MOTOR_STATE_NORMAL)
		{
			return DONE_RESULT_FAIL;
		}
	}
	
	
	bsp_motor_start(motorID[0]);	//�������
	if(num > 1)//˫���
	{
		bsp_motor_start(motorID[1]);
	}
	bsp_roughTimer_start(ROUGH_TIME_COUNT);	//������ʱ��
	delay_ms(2);
	checkCount = 0;
	while(1)
	{
		for(i = 0; i < num; i++)
		{
			cur = bsp_current_get();	//������
			runTime = bsp_roughTimer_ticks();//��¼��ǰʱ��
			adc_value[adc%2048] = cur;
			adc = (adc+1)%2048;
			if(cur > 120)//��������100mA
			{
				if(status == 1)//����һ�ο�λ
				{
					current_up++;//��������100mA����
					if(current_up > 50)//����1�ο�λ���ҵ����ָ�����
					{
						change = 1;//��¼״̬����ʱ���¼���һ�ι���λ
					}
				}
				else
				{
					if(current_up > 0)
					current_up--;
				}
			}
			if(cur != 0 && cur < 120)//�м� < 100 ����Դ < 160
//			if( cur < current && current != 0 && cur < 100)//�м� < 100 ����Դ < 160
			{
//				bsp_roughTimer_stop();
//				bsp_roughTimer_start(ROUGH_TIME_COUNT);
//				runTime = bsp_roughTimer_ticks();//��¼��ǰʱ��
				checkCount++;
			}
			else
			{
				if(checkCount > 0)
				{
					if(cur != current )//��Ȳ���
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
				if(status == 2)//ת������1.5s	//�м�900000;��Դ500000
				{
					if(runTime - st_time < 1000)
					{
						bsp_motor_stop();
						bsp_roughTimer_stop();
						checkCount = checkCount;
						mo_set_state(motorID[i], MOTOR_STATE_OUT);//���µ��״̬
						return DONE_RESULT_NOSTOP;
						
					}
					if(runTime > 1000)
					{
						delay_ms(STOP_DELAY_TIME);
						bsp_motor_stop();
						bsp_roughTimer_stop();
						checkCount = checkCount;
						mo_set_state(motorID[i], MOTOR_STATE_NORMAL);//���µ��״̬
						return DONE_RESULT_COMPLETE;
					}
				}
				else
				{
					checkCount = 0;
				}
			}
			if(runTime > STOP_TIMEOUT)//ת������6s����ʱδֹͣ
			{
				bsp_roughTimer_stop();
				bsp_motor_stop();
				mo_set_state(motorID[i], MOTOR_STATE_NOSTOP);//���µ��״̬
				return DONE_RESULT_NOSTOP;
			}
//			delay_us(5);//50us���һ��			
		}
	}
}
#endif





#if DRIVERBOARD_TYPE != 3
/*
���ܣ������λ
˵�������������������������Ҫ�жϵ��λ��
*/
uint8_t  logic_motor_reset(uint8_t num,uint8_t *motorID)
{
	uint8_t i, state;
	uint8_t status = INIT;//���״̬
	uint16_t current;//�������
	uint32_t runTime;//����ʱ��
	uint32_t overLoadTime = 0;//����ʱ��
	uint32_t checkCount = SALE_SIGNAL_FILTER_VAL_1ST;//����ﵽλ�ü���	

	logic_motor_check(num,motorID);//����Ƿ��е��
	for(i = 0; i < num ;i++)
	{
		state = get_motor_state(motorID[i]);
		if(state == MOTOR_STATE_NOINSTALL || state ==MOTOR_STATE_OVERLOAD || state ==MOTOR_STATE_SHORT)
		{
			return DONE_RESULT_FAIL;
		}
	}
	
	bsp_motor_start(motorID[0]);	//�������
	if(num > 1)//˫���
	{
		bsp_motor_start(motorID[1]);
	}
	bsp_roughTimer_start(ROUGH_TIME_COUNT);		//��������Ӳ����ʱ��
	
	while(1)
	{
		for(i = 0; i < num; i++)
		{
			current = bsp_current_get();		//������
			runTime = bsp_roughTimer_ticks();	//����ʱ

			switch(status)
			{
				case INIT://�������
					if(state != MOTOR_STATE_OUT)//���������λ�û�����Щ��ת
					{
						if(runTime < START_TIMEOUT || current > LOAD_CURRENT)//��ʱ����ʱ����
						{
							if(!bsp_motor_check(motorID[i]))//�������λ���������λ��
							{
								//����Ϊ�˲�����
								checkCount++;
								if(checkCount < SALE_SIGNAL_COUNT_1ST + SALE_SIGNAL_FILTER_VAL_1ST)
								{
									delay_us(SALE_SIGNAL_INTERVAL_1ST);//��ʱ��ʼ�´μ��
								}
								else
								{
									checkCount = SALE_SIGNAL_FILTER_VAL_2ND;
									status = OVER_POSITION;//�л�״̬
								}
							}
							else
							{
								if(checkCount > 0)//�����ʵû����
								checkCount--;
							}
							
							if(current > OVERLOAD_CURRENT)//����
							{
								if(current > SHORT_CURRENT)//��·
								{
									bsp_roughTimer_stop();	//ֹͣ��ʱ��
									bsp_motor_stop();		//ֹͣ���
									mo_set_state(motorID[i],MOTOR_STATE_SHORT);//���µ��״̬����·
									return DONE_RESULT_SHORT;//��·�˳�
								}
								
								if(overLoadTime == 0)
									overLoadTime = runTime;
								if(overLoadTime != 0)
								{
									if(runTime - overLoadTime > OVERLOAD_TIMEOUT)//���س�ʱ
									{
										bsp_roughTimer_stop(); 	//ֹͣ��ʱ��
										bsp_motor_stop();		//ֹͣ���
										mo_set_state(motorID[i],MOTOR_STATE_OVERLOAD);//���µ��״̬������
										return DONE_RESULT_OVER;//�����˳�
									}
								}
							}
							else
							{
								overLoadTime = 0;//������ؼ�ʱ
							}
						}
						else//��ʱδ����λ
						{
							bsp_roughTimer_stop();	//��ʱ��ֹͣ
							bsp_motor_stop();		//���ֹͣ
							mo_set_state(motorID[i], MOTOR_STATE_NOSTART);//���״̬���£���ʱδ����
							return DONE_RESULT_NOSTART;//��ʱδ����
						}
						
						if(runTime > LOAD_TIMEOUT_1ST)//��Ϊ�ػ�̫�أ����µ��ת��������������һ��������һ�㲻�ᵽ����
						{
							bsp_roughTimer_stop();	//��ʱ��ֹͣ
							bsp_motor_stop();		//���ֹͣ
							mo_set_state(motorID[i], MOTOR_STATE_NOSTART);//���״̬���£���ʱδ����
							return DONE_RESULT_NOSTART;//��ʱδ����
						}
					}
					else//��ʼΪ����λ����ֱ�ӽ����¸�����
					{
						status = OVER_POSITION;
						checkCount = SALE_SIGNAL_FILTER_VAL_2ND;
					}
				break;
					
				case OVER_POSITION://�������λ���ڶ������̼��
					if(runTime > STOP_TIMEOUT)//��ʱδֹͣ
					{
						bsp_roughTimer_stop();	//��ʱ��ֹͣ
						bsp_motor_stop();		//���ֹͣ
						mo_set_state(motorID[i], MOTOR_STATE_NOSTOP);//���µ��״̬����ʱδͣ
						return DONE_RESULT_NOSTOP;
					}

					if(bsp_motor_check(motorID[i]))//���ת��λ�������λ��
					{
						checkCount++;
						if(checkCount < (SALE_SIGNAL_FILTER_VAL_2ND +  SALE_SIGNAL_COUNT_2ND) )
						{
							delay_us(SALE_SIGNAL_INTERVAL_2ND);//��ʱ���ȴ��´μ��
						}
						else
						{
							bsp_roughTimer_stop();		//��ʱ��ֹͣ
							delay_ms(STOP_DELAY_TIME);	//�����ʱֹͣ
							bsp_motor_stop();			//���ֹͣ
							mo_set_state(motorID[i],MOTOR_STATE_NORMAL);//���µ��״̬,����
							return DONE_RESULT_COMPLETE; 
						}
						
						//��ֹ�������еĶ�·����Ϊʱ��̣ܶ�����Ҫ�����أ�
						if(current > SHORT_CURRENT)//��·
						{
							bsp_roughTimer_stop();	//ֹͣ��ʱ��
							bsp_motor_stop();		//ֹͣ���
							mo_set_state(motorID[i],MOTOR_STATE_SHORT);//���µ��״̬����·
							return DONE_RESULT_SHORT;//��·�˳�
						}
					}
					else
					{
						if(checkCount > 0)
						checkCount--;
						if(current > OVERLOAD_CURRENT)//����
						{
							if(current > SHORT_CURRENT)//����
							{
								bsp_roughTimer_stop();	//ֹͣ��ʱ��
								bsp_motor_stop();		//ֹͣ���
								mo_set_state(motorID[i],MOTOR_STATE_SHORT);//���µ��״̬����·
								return DONE_RESULT_SHORT;//��·�˳�
							}
							if(overLoadTime == 0)
								overLoadTime = runTime;
							if(overLoadTime != 0)
							{
								if(runTime - overLoadTime > OVERLOAD_TIMEOUT)//���س�ʱ
								{
									bsp_roughTimer_stop(); 	//ֹͣ��ʱ��
									bsp_motor_stop();		//ֹͣ���
									mo_set_state(motorID[i],MOTOR_STATE_OVERLOAD);//���µ��״̬������
									return DONE_RESULT_OVER;//�����˳�
								}
							}
						}
						else
						{
							overLoadTime = 0;	//������ؼ�ʱ
						}		
					}							
				break;
			}
		}
	}
}
#else	//�м���Դ����
//uint16_t adc_value[2048],adc;
uint8_t  logic_motor_reset(uint8_t num,uint8_t *motorID)
{
	uint8_t i, state;
	uint16_t cur;//���μ�����
	uint16_t current;//�������
	uint32_t runTime;//����ʱ��
	uint32_t overLoadTime = 0;//����ʱ��
	uint32_t checkCount = 0;//����ﵽλ�ü���	
	uint8_t change = 0;

	logic_motor_check(num,motorID);//����Ƿ��е��
	for(i = 0; i < num ;i++)
	{
		state = get_motor_state(motorID[i]);
		if(state == MOTOR_STATE_NOINSTALL || state ==MOTOR_STATE_OVERLOAD || state ==MOTOR_STATE_SHORT)
		{
			return DONE_RESULT_FAIL;
		}
	}

	bsp_motor_start(motorID[0]);	//�������
	if(num > 1)//˫���
	{
		bsp_motor_start(motorID[1]);
	}
	bsp_roughTimer_start(ROUGH_TIME_COUNT);	//������ʱ��
	while(1)
	{
		for(i = 0; i < num; i++)
		{
			cur = bsp_current_get();	//������
			runTime = bsp_roughTimer_ticks();//��¼��ǰʱ��
//			adc_value[adc%2048] = cur;
//			adc++;
			if( cur < current && cur < 100)//�м� < 100 ����Դ < 160
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
				if(runTime > 1500)//ת������1s	//�м�900000;��Դ500000
				{
					delay_ms(STOP_DELAY_TIME);
					bsp_motor_stop();
					bsp_roughTimer_stop();
					checkCount = checkCount;
					mo_set_state(motorID[i], MOTOR_STATE_NORMAL);//�������
					return DONE_RESULT_COMPLETE;
				}
				else
				{
					if(change == 0)//������˵��ת�Ĳ���������һȦ
					{
						bsp_roughTimer_stop();
						bsp_roughTimer_start(ROUGH_TIME_COUNT);	//���¼�ʱ
						change =1;
					}
					checkCount = 0;
				}
			}
			if(runTime > 6000)//ת������6s����ʱδֹͣ
			{
				bsp_roughTimer_stop();
				bsp_motor_stop();
				mo_set_state(motorID[i],MOTOR_STATE_UNKNOWN);//�������
				return DONE_RESULT_NOSTOP;
			}
			delay_us(20);//50us���һ��			
		}
	}
}
#endif
//=======================
//��������λ
//=======================
uint8_t  logic_motor_resetAll(void)
{
	uint8_t i;
	for(i = 0;i < MOTOR_SIZE ;i++)
	{
		delay_ms(MOTOR_INTEVAL);	//������Ҫһ����ʱ���������ʱ�䣬�ṩ���Żָ�ʱ��
		logic_motor_reset(1,&i);
	}
	return DONE_RESULT_COMPLETE;
}

//=======================
//�����·��
//=======================
uint8_t logic_ex_open(uint8_t  id)
{
	bsp_output_open(id);
	ex_set_state(id,1);//����ͨ��״̬
	return DONE_RESULT_COMPLETE;
}
//=======================
//�����·��
//=======================
uint8_t logic_ex_close(uint8_t id)
{
	bsp_output_close(id);
	ex_set_state(id,0);//����ͨ��״̬
	return DONE_RESULT_COMPLETE;
}
//=======================
//�ر�ȫ��
//=======================
uint8_t logic_ex_reset(void)
{
	uint8_t i;
	bsp_output_reset();
	for(i=0;i<6;i++) ex_set_state(i,0);//����ͨ��״̬
	return DONE_RESULT_COMPLETE;
}

/*
���ƻ�·���Ų�ѯ
*/
uint8_t logic_ex_check(uint8_t id)
{
	ex_set_state(id, bsp_ex_check(id));//����ͨ��״̬
	return DONE_RESULT_COMPLETE;
}

