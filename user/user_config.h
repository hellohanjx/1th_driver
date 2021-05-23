//================================
//���ܣ�ȫ�������ļ�
//�Ժ�ͷ�ļ�����������Ϣֱ�Ӽӵ�������
//================================

#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

#include "stdint.h"

/*******************************
����������
0.��ͨ����
1.������ͣ���Ҫת�Ӱ�
2.�������
3.�м�����
*******************************/
#define DRIVERBOARD_TYPE 	3	//����������



//========================================
//�궨��
//========================================
#define MOTOR_SIZE		80	//֧�ֵ������
#define OUTPUT_SIZE		6	//ac ����ͨ����
#define TEMP_SIZE		2	//�¶�ͨ����
#define MOTOR_TOGETHER	2	//ͬʱ�����ĵ����
#define MASTER_ADDR 	1	//������ַ


#define ADDR_MAX		15	//����ַ


#define ADC_INTEVAL		100//��������ʱ�� ms
#define MOTOR_INTEVAL	50//��������������ϴ��������ʱ�� ms

#define RX_LENGTH	100	//uart1 ���ջ���������
#define TX_LENGTH	100	//uart1 ���ͻ���������

/*
���������ò���
*/
#if DRIVERBOARD_TYPE == 0	//�״�����
/*
�����ԣ����30us���һ�Σ�����300us���ɼ�⵽5��ƽ�ȵ�ƽ�������λ��
�����ԣ�����600ms���ӿ�λ��λ������λ
*/
	#define TEMPERATURE_TYPE 1	//�¶ȴ���������	0: 3470/2.5k ������ͣ�1: 3470/5k �״����ͣ�2: 3950/10k; ���û���
	#define START_CURRENT		30		//����������� mA
	#define SHORT_CURRENT		5000	//�����·���� mA 
	#define OVERLOAD_CURRENT	3000 	//������ص��� mA
	#define OVERLOAD_TIMEOUT	1000 	//������س�ʱʱ�� ms
	#define START_TIMEOUT		1000	//�����ʱδ���� ms 
	#define STOP_TIMEOUT		4500	//�����ʱδֹͣ ms	
	
	#define LOAD_CURRENT	300	//���ص���
	#define LOAD_TIMEOUT_1ST	1500 //���ص�һ����ʱʱ�� MS(��ʱ��Ӧ���� ����ʱδ����ʱ�䡱)
	#define LOAD_TIMEOUT_2ST	4500 //���صڶ�����ʱʱ�� MS
	
	#define SALE_SIGNAL_FILTER_VAL_1ST	20		//�����˲���ʼֵ
	#define SALE_SIGNAL_COUNT_1ST		(SALE_SIGNAL_FILTER_VAL_1ST)//�������ʱ���ת����λ�źŴﵽ����	���ﵽ�˴�������Ϊ��ƽ�ź���ȷ��
	#define SALE_SIGNAL_INTERVAL_1ST	3000	//�������ʱ���ת����λǰ�����źż���� us 
	
	#define SALE_SIGNAL_FILTER_VAL_2ND	20		//�����˲���ʼֵ
	#define SALE_SIGNAL_COUNT_2ND		(SALE_SIGNAL_FILTER_VAL_2ND)//�������ʱ���ת����λ�źŴﵽ����	���ﵽ�˴�������Ϊ��ƽ�ź���ȷ��
	#define SALE_SIGNAL_INTERVAL_2ND	1000	//�������ʱ���ת����λ�����źż���� us 
	
	#define CHECK_SIGNAL_FILTER_VAL	15//����˲���ʼֵ
	#define CHECK_SIGNAL_COUNT		(CHECK_SIGNAL_FILTER_VAL)	//���ʱ�źŴﵽ����	���ﵽ�˴�������Ϊ�źŵ�ƽ��ȷ��
	#define CHECK_SIGNAL_INTERVAL	25		//���ʱ�����źż���� us
	
	#define STOP_DELAY_TIME		80	//���ֹͣʱ����ʱ ms
	#define RUN_MIN_TIME		1500//���������Сʱ�� ms�����ڴ�ʱ����ֵ�λ�źţ���Ϊ���δ������
	
	#define NO_INSTALL_TIMEOUT	1000	//����������ʱ�� us��������ʱ��δ��⵽������������Ϊû�е����
	
	#define ROUGH_TIME_COUNT	199	//��ʱ��4����������ÿ��500us
	#define MID_TIME_COUNT		1	//��ʱ��3����������ÿ��50us
#endif

#if DRIVERBOARD_TYPE == 1	//�������
	#define TEMPERATURE_TYPE 0	//�¶ȴ���������	0: 3470/2.5k	������ͣ�1: 3470/5k �״����ͣ�2: 3950/10k; ���û���
	#define START_CURRENT	50	//����������� mA
	#define SHORT_CURRENT	5000//�����·���� mA 
	#define OVERLOAD_CURRENT	3000 //������ص��� mA
	#define OVERLOAD_TIMEOUT	1000 //������س�ʱʱ�� ms
	#define START_TIMEOUT		2000	//�����ʱδ���� ms 
	#define STOP_TIMEOUT		8000	//�����ʱδֹͣ ms	
	
	#define LOAD_CURRENT	300	//���ص���
	#define LOAD_TIMEOUT_1ST	2500 //���ص�һ����ʱʱ�� MS
	#define LOAD_TIMEOUT_2ST	6000 //���صڶ�����ʱʱ�� MS

	#define SALE_SIGNAL_COUNT_1ST		5		//�������ʱ���ת����λ�źŴﵽ����	���ﵽ�˴�������Ϊ��ƽ�ź���ȷ��
	#define SALE_SIGNAL_INTERVAL_1ST	3000	//�������ʱ���ת����λǰ�����źż���� us 
	#define SALE_SIGNAL_FILTER_VAL_1ST	10	//�����˲���ʼֵ
	
	#define SALE_SIGNAL_COUNT_2ND		45		//�������ʱ���ת����λ�źŴﵽ����	���ﵽ�˴�������Ϊ��ƽ�ź���ȷ��
	#define SALE_SIGNAL_INTERVAL_2ND	1000	//�������ʱ���ת����λ�����źż���� us 
	#define SALE_SIGNAL_FILTER_VAL_2ND	10	//�����˲���ʼֵ
	
	#define CHECK_SIGNAL_COUNT	5			//���ʱ�źŴﵽ����	���ﵽ�˴�������Ϊ�źŵ�ƽ��ȷ��
	#define CHECK_SIGNAL_INTERVAL	20		//���ʱ�����źż���� us
	#define CHECK_SIGNAL_FILTER_VAL	10		//����˲���ʼֵ
	
	#define STOP_DELAY_TIME		100	//���ֹͣʱ����ʱ ms
	#define RUN_MIN_TIME		2000//���������Сʱ�� ms�����ڴ�ʱ����ֵ�λ�źţ���Ϊ���δ������

	#define ROUGH_TIME_COUNT	199	//����������ÿ��500us
	#define MID_TIME_COUNT		1	//����������ÿ��500us
	#define NO_INSTALL_TIMEOUT	2000//������ʱ��us��������ʱ��δ��⵽������������Ϊû�е����
#endif


#if DRIVERBOARD_TYPE == 2	//��������
	#define TEMPERATURE_TYPE 2	//�¶ȴ���������	0: 3470/2.5k	������ͣ�1: 3470/5k �״����ͣ�2: 3950/10k; ���û���
	#define START_CURRENT	30	//����������� mA
	#define SHORT_CURRENT	5000//�����·���� mA 
	#define OVERLOAD_CURRENT	3000 //������ص��� mA
	#define OVERLOAD_TIMEOUT	1000 //������س�ʱʱ�� ms
	#define START_TIMEOUT		2000	//�����ʱδ���� ms 
	#define STOP_TIMEOUT		6000	//�����ʱδֹͣ ms	
	
	#define LOAD_CURRENT	300	//���ص���
	#define LOAD_TIMEOUT_1ST	2500 //���ص�һ����ʱʱ�� MS
	#define LOAD_TIMEOUT_2ST	600 //���صڶ�����ʱʱ�� MS
	
	#define SALE_SIGNAL_COUNT_1ST		5		//�������ʱ���ת����λ�źŴﵽ����	���ﵽ�˴�������Ϊ��ƽ�ź���ȷ��
	#define SALE_SIGNAL_INTERVAL_1ST	3000	//�������ʱ���ת����λǰ�����źż���� us 
	#define SALE_SIGNAL_FILTER_VAL_1ST	10	//�����˲���ʼֵ
	
	#define SALE_SIGNAL_COUNT_2ND		5		//�������ʱ���ת����λ�źŴﵽ����	���ﵽ�˴�������Ϊ��ƽ�ź���ȷ��
	#define SALE_SIGNAL_INTERVAL_2ND	1000		//�������ʱ���ת����λ�����źż���� us 
	#define SALE_SIGNAL_FILTER_VAL_2ND	10	//�����˲���ʼֵ
	
	#define CHECK_SIGNAL_COUNT	5			//���ʱ�źŴﵽ����	���ﵽ�˴�������Ϊ�źŵ�ƽ��ȷ��
	#define CHECK_SIGNAL_INTERVAL	20		//���ʱ�����źż���� us
	#define CHECK_SIGNAL_FILTER_VAL	10		//����˲���ʼֵ
	
	#define STOP_DELAY_TIME		30	//���ֹͣʱ����ʱ ms
	#define RUN_MIN_TIME		1500//���������Сʱ�� ms�����ڴ�ʱ����ֵ�λ�źţ���Ϊ���δ������

	#define ROUGH_TIME_COUNT	199	//����������ÿ��500us
	#define MID_TIME_COUNT		1	//����������ÿ��500us
	#define NO_INSTALL_TIMEOUT	2000	//������ʱ��us��������ʱ��δ��⵽������������Ϊû�е����
#endif

#if DRIVERBOARD_TYPE == 3	//�м�����
/*
�����ԣ����30us���һ�Σ�����300us���ɼ�⵽5��ƽ�ȵ�ƽ�������λ��
�����ԣ�����600ms���ӿ�λ��λ������λ
*/
	#define TEMPERATURE_TYPE 2	//�¶ȴ���������	0: 3470/2.5k ������ͣ�1: 3470/5k �״����ͣ�2: 3950/10k; ���û���
	#define START_CURRENT		30		//����������� mA
	#define SHORT_CURRENT		5000	//�����·���� mA 
	#define OVERLOAD_CURRENT	3000 	//������ص��� mA
	#define OVERLOAD_TIMEOUT	1000 	//������س�ʱʱ�� ms
	#define START_TIMEOUT		1000	//�����ʱδ���� ms 
	#define STOP_TIMEOUT		4500	//�����ʱδֹͣ ms	
	
	#define LOAD_CURRENT	300	//���ص���
	#define LOAD_TIMEOUT_1ST	1500 //���ص�һ����ʱʱ�� MS(��ʱ��Ӧ���� ����ʱδ����ʱ�䡱)
	#define LOAD_TIMEOUT_2ST	4000 //���صڶ�����ʱʱ�� MS
	
	
	#define SALE_SIGNAL_COUNT_1ST		10		//�������ʱ���ת����λ�źŴﵽ����	���ﵽ�˴�������Ϊ��ƽ�ź���ȷ��
	#define SALE_SIGNAL_INTERVAL_1ST	3000	//�������ʱ���ת����λǰ�����źż���� us 
	#define SALE_SIGNAL_FILTER_VAL_1ST	20		//�����˲���ʼֵ
	
	#define SALE_SIGNAL_COUNT_2ND		5		//�������ʱ���ת����λ�źŴﵽ����	���ﵽ�˴�������Ϊ��ƽ�ź���ȷ��
	#define SALE_SIGNAL_INTERVAL_2ND	1000	//�������ʱ���ת����λ�����źż���� us 
	#define SALE_SIGNAL_FILTER_VAL_2ND	10	//�����˲���ʼֵ
	
	#define CHECK_SIGNAL_COUNT		5		//���ʱ�źŴﵽ����	���ﵽ�˴�������Ϊ�źŵ�ƽ��ȷ��
	#define CHECK_SIGNAL_INTERVAL	30		//���ʱ�����źż���� us
	#define CHECK_SIGNAL_FILTER_VAL	10		//����˲���ʼֵ
	
	#define STOP_DELAY_TIME		100	//���ֹͣʱ����ʱ ms
	#define RUN_MIN_TIME		1500//���������Сʱ�� ms�����ڴ�ʱ����ֵ�λ�źţ���Ϊ���δ������
	
	#define NO_INSTALL_TIMEOUT	1000	//����������ʱ�� us��������ʱ��δ��⵽������������Ϊû�е����
	
	#define ROUGH_TIME_COUNT	199	//��ʱ��4����������ÿ��500us
	#define MID_TIME_COUNT		1	//��ʱ��3����������ÿ��50us
#endif
//========================================
//��־��¼����
//========================================




#endif
