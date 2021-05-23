//====================================
//���ļ�������������λ���
//ע�⣺�������ʱ�������λ��΢������
//ѹסʱ,���IO���24v���ң���ʱcpu����
//Ϊ�͵�ƽ����΢�������ɿ�������ʱ��
//cpu ����Ϊ�ߵ�ƽ
//���ԣ�
//��΢������ѹ�µ�������һ��������
//��΢�����ص���ѹ�£���һ���½���
//΢������ѹ�£��͵�λ
//΢�����ص��𣬸ߵ�λ
//====================================
#include "stm32f10x.h"
#include "misc.h"
#include "stm32f10x_exti.h"
#include "delay.h"

//====================================
//�����λ��� ��ʼ��
//====================================
void io_position_init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;                                   

	//�� GPIO ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB | 
						   RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
	//IO ���ų�ʼ��
	//PB9 -> IO1
	//PB8 -> IO2
	//PB7 -> IO3
	//PB6 -> IO4
	//PB5 -> IO5
	//PD2 -> IO6
	//PC12 -> IO7
	//PC11 -> IO8
	//PC10 -> IO9
	//PA12 -> IO10��������ź�PB12�п��ܳ�ͻ��

	//PB9��PB8��PB7��PB6��PB5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8 | GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//����ģʽ����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//PD2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//����ģʽ����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStructure);

	//PC12��PC11��PC10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_11 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//����ģʽ����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	//PA12
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//����ģʽ����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	//==================================================
	//�ⲿ�ж�����
	//==================================================
	//�ⲿ�ж���5~9��10~15��2
	//NVIC_InitStructure.NVIC_IRQChannel =   EXTI2_IRQn | EXTI9_5_IRQn;    
	NVIC_InitStructure.NVIC_IRQChannel =   EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;            //1����ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                   //0����ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel =   EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;            //1����ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                   //0����ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel =   EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;            //1����ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                   //0����ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//==================================================
	//�ж������ж���������
	//==================================================
	//ѡ��GPIO ������Ϊ�ⲿ�ж���
	//PB5,PB6,PB7,PB8,PB9
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5 | GPIO_PinSource6 | GPIO_PinSource7 | GPIO_PinSource8 | GPIO_PinSource9);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource2);		//PD2 �ⲿ�ж���
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource10 | GPIO_PinSource11 | GPIO_PinSource10);//PC10��PC11��PC12 �ⲿ�ж���
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource12);	//PA12 �ⲿ�ж���

	EXTI_InitStructure.EXTI_Line = EXTI_Line5 | EXTI_Line6 | EXTI_Line7 | EXTI_Line8 | EXTI_Line9 | EXTI_Line2 | EXTI_Line10 | EXTI_Line11 | EXTI_Line12 ;
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
//	EXTI_InitStructure.EXTI_Line = EXTI_IMR_MR5 | EXTI_IMR_MR6 | EXTI_IMR_MR7 | EXTI_IMR_MR8 | EXTI_IMR_MR9 | EXTI_IMR_MR2 | EXTI_IMR_MR10 | EXTI_IMR_MR11 | EXTI_IMR_MR12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                    //����EXTI��·��Ӧ�ж�����
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;                //�½�����Ӧ�ж�����
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;			//ѡ�е��ж���״̬
	EXTI_Init(&EXTI_InitStructure);

}

//=================================
//5~9�ⲿ�ж�
//=================================
void EXTI9_5_IRQHandler1(void)
{    
//	uint8_t  rs;
//	rs = 0;
	
	if(EXTI_GetITStatus(EXTI_IMR_MR5) != RESET)		
	{	
		EXTI_ClearITPendingBit(EXTI_IMR_MR5);                  //�����������λ	
	}
	if(EXTI_GetITStatus(EXTI_IMR_MR6) != RESET)		
	{	
		EXTI_ClearITPendingBit(EXTI_IMR_MR6);                  //�����������λ	
	}
	if(EXTI_GetITStatus(EXTI_IMR_MR7) != RESET)		
	{	
		EXTI_ClearITPendingBit(EXTI_IMR_MR7);                  //�����������λ	
	}
	if(EXTI_GetITStatus(EXTI_IMR_MR8) != RESET)		
	{	
		EXTI_ClearITPendingBit(EXTI_IMR_MR8);                  //�����������λ	
	}
	if(EXTI_GetITStatus(EXTI_IMR_MR9) != RESET)		
	{	
		EXTI_ClearITPendingBit(EXTI_IMR_MR9);                  //�����������λ	
	}

}

//=================================
//2�ⲿ�ж�
//=================================
void EXTI2_IRQHandler(void)
{    
	delay_ms(2);//�������ȴ���ʹ��ƽ�ȶ�
	if(EXTI_GetITStatus(EXTI_Line2) != RESET)
	{	
		//�����е����⣬����Ϊ�����ش���������ʵ���������غ��½��ض������ж�
		if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2))//�����ŵ�ƽ���ߵ�ƽ��ʾ����λ
		//���ﲻҪֱ�Ӳ����������־λ������
		//motor_stop();
		EXTI_ClearITPendingBit(EXTI_Line2);                  //�������λ	
	}
}
//=================================
//10~15�ⲿ�ж�
//=================================

void EXTI15_10_IRQHandler1(void)
{    

	if(EXTI_GetITStatus(EXTI_IMR_MR12) != RESET)
	{	
		EXTI_ClearITPendingBit(EXTI_IMR_MR12);                  //�����������λ	
	}
}

