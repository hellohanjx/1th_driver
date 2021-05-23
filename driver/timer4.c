//=========================
//ͨ�ö�ʱ��4 ������
//˵���������� bsp���ṩ�ӿ�
//=========================

#include "stm32f10x_tim.h"
#include "misc.h"
#include "timer4.h" 

static volatile uint32_t tim4Ticks ;
//============================
//�������ݺ����Ƽ�ʱ��
//˵��������led������
//���ܣ���ʼ����ʱ��
//============================
void timer4_config(void) 
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure; 
	NVIC_InitTypeDef 			NVIC_InitStructure; 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 	//����ʱ��4ʱ�ӣ�APB1���36 M�����Ƕ�ʱ����x2��
	
	TIM_TimeBaseStructure.TIM_Prescaler = TIMER4_PRE_SCALER;   	//Ԥ��Ƶ����������ʱ�ӷ�Ƶ��
	TIM_TimeBaseStructure.TIM_Period = 0;      	//�Զ���װ������ֵ
	//TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			//�ظ����� ��
	
	//ʱ�ӷָ� 4��Ƶ��@@@@@���ֻ����Ϊ��������ã�
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4; 	
	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_IT_Update; 	//������ʽ (���ϼ���)
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);         //��ʼ�� time4 ����������
	
	//���з��� TIM_TimeBaseInit ���棬���⿪���ͽ����ж�
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);	
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE );                   //��������ж�
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;             //�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  	//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;          //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;        		//ʹ��ͨ���ж�
	NVIC_Init(&NVIC_InitStructure);                            	//��ʼ���ж�
	TIM_Cmd(TIM4, DISABLE);                                     //timer4 ����
}
//============================
//��ʱ��4 ����
//����: count ��ʱʱ��
//============================
void timer4_start(uint16_t count)
{
	tim4Ticks = 0;
	TIM_SetAutoreload(TIM4, count);	//�ı���װֵ
	TIM_SetCounter(TIM4, 0);		//���������,���ϼ���
	TIM_Cmd(TIM4, ENABLE);
}
//============================
//��ʱ��4 ֹͣ
//����: count ��ʱʱ����us��
//============================
void timer4_stop(void)
{
	TIM_SetAutoreload(TIM4, 0);	//�ı���װֵ
	TIM_SetCounter(TIM4, 0);		//���������,���ϼ���
	TIM_Cmd(TIM4, DISABLE);
	tim4Ticks = 0;
}
//===============================
//TIM4 �ж�
//===============================
void TIM4_IRQHandler(void)
{ 
  if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
  { 
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		tim4Ticks++;
  } 
} 
//===============================
//��ȡ��ʱ��4���ģ���Ҫ���ṩ��ҵ����ã�����systick����Ϊsystick���ò���
//===============================
uint32_t get_timer4_ticks(void)
{
	return tim4Ticks*100;//����*����ʱ�� = �ж�һ�ε�ʱ�� ʱ��100ms��һ������ = 100ms
//	return (TIME4_TIME_BASE * tim4Ticks);//����*����ʱ�� = �ж�һ�ε�ʱ��
}


