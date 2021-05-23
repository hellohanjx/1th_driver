//==============================
//timer3 ��ʱ������
//==============================
#include "stdint.h"
#include "timer3.h"
#include "stm32f10x_tim.h"
#include "misc.h"

static volatile uint32_t tim3Ticks ;

//======================================
//��ʱ��3����
//======================================
void timer3_config(void) 
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure; 
	NVIC_InitTypeDef 			NVIC_InitStructure; 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 	//����ʱ��3ʱ�ӣ�APB1���36 M�����Ƕ�ʱ����x2��
	
	//===============================================================
	//��ʽ :  T = (TIM_Period + 1) * (TIM_Prescaler + 1) / TIM2 Clock 
	// TIM_Prescaler = 72000,000/(1/100us)
	//===============================================================
	TIM_TimeBaseStructure.TIM_Prescaler = PRE_SCALER;   	//Ԥ��Ƶ����������ʱ�ӷ�Ƶ��10000HZ 100us
	TIM_TimeBaseStructure.TIM_Period = 0;      		//�Զ���װ������ֵ

	//TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;		//�ظ����� �� ���߼����������У�ͨ���ޣ�
	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_FLAG_Update; 	//������ʽ (���ϼ���)

	//ʱ�ӷָ� 4��Ƶ��@@@@@���ֻ����Ϊ��������ã�
	//TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 	
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);             	//��ʼ������������
	
	//���з��� TIM_TimeBaseInit ���棬���⿪���ͽ����ж�
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);	
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );                   //��������ж�
	
	//��ʱ���ж�����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;             //�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  	//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;          //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;        		//ʹ��ͨ���ж�
	NVIC_Init(&NVIC_InitStructure);                            	//��ʼ���ж�
	TIM_Cmd(TIM3, DISABLE );                                      //timer3 ����
}
//============================
//��ʱ��3 ����
//����: ��ʱʱ�� = count * 10us
//============================
void timer3_start(uint16_t count)
{
	tim3Ticks = 0;
	TIM_SetAutoreload(TIM3, count);	//�ı���װֵ
	TIM_SetCounter(TIM3,0);			//��Ϊ�����ϼ����������������������ֵ
	TIM_Cmd(TIM3, ENABLE);
}
//============================
//��ʱ��3 ����
//����: count ��ʱʱ����us��
//============================
void timer3_stop(void)
{
	TIM_SetAutoreload(TIM3, 0);	//�ı���װֵ
	TIM_SetCounter(TIM3,0);		//��Ϊ�����ϼ����������������������ֵ
	TIM_Cmd(TIM3, DISABLE);
	tim3Ticks = 0;
}
//===============================
//TIM3 �ж�
//===============================
void TIM3_IRQHandler(void)
{ 
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{ 
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		tim3Ticks++;
		//�����淽ʽ���ԣ������ж�ʱ����ʱ����Ȼ�ڹ���
//		tf[0] = TIM_GetCounter(TIM3);
//		tf[1] = TIM_GetCounter(TIM3);
//		tf[2] = TIM_GetCounter(TIM3);
		//TIM_Cmd(TIM3, DISABLE);
	} 
} 
//===============================
//��ȡ��ʱ��3���ģ���Ҫ���ṩ��ҵ����ã�����systick����Ϊsystick���ò���
//===============================
uint32_t get_timer3_ticks(void)
{
	return tim3Ticks*50;//һ������ = 50us
}

