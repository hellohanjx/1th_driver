//============================
//�������������
//���ܣ���bsp�ṩ�����ӿ�
//============================
#include "stm32f10x.h"
#include "misc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_dma.h"
#include "moto.h"
#include "user_config.h"
#include "delay.h"
#include "stm32f10x_exti.h"

//========================================================================================================================
//�����
//========================================================================================================================
//==============================
//�г�ʼ����74hc237��ʼ��
//==============================
void motor_line_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);	

	//���������� PA12 PA11 PA8 ,237�ĵ�ַ��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_11 |  GPIO_Pin_8;	//PA12->PO1,PA11->PO2��PA8->PA3 ����Ϊͨ���������  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			
	GPIO_Init(GPIOA, &GPIO_InitStructure);		
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);								//74HC237 ѡ�񲻴��ڵ�ͨ��
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	
	//ż�������� PC9 PC8 PC7, 237�ĵ�ַ��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 |  GPIO_Pin_8 | GPIO_Pin_9;	//PC7->PE3, PC8->PE2��PC9->PE1 ����Ϊͨ���������  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//���߷�ת�ٶ�Ϊ50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					
	GPIO_ResetBits(GPIOC,GPIO_Pin_9);								//74HC237 ѡ�񲻴��ڵ�ͨ��
	GPIO_ResetBits(GPIOC,GPIO_Pin_8);		
	GPIO_ResetBits(GPIOC,GPIO_Pin_7);
}
//==============================
//�йر�
//==============================
void line_stop(void)
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_9);								//74HC237 ѡ�񲻴��ڵ�ͨ��
	GPIO_ResetBits(GPIOC,GPIO_Pin_8);		
	GPIO_ResetBits(GPIOC,GPIO_Pin_7);
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);								//74HC237 ѡ�񲻴��ڵ�ͨ��
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
}
//==============================
//�н�ͨ
//==============================
void line_start(uint8_t id)
{
	switch(id){
		//(������) 1,3,5,7,9
		case 0://Y1����� HLL
			GPIO_SetBits(GPIOA,GPIO_Pin_12);
			GPIO_ResetBits(GPIOA,GPIO_Pin_11);
			GPIO_ResetBits(GPIOA,GPIO_Pin_8);
		break;
		case 2://Y2����� LHL
			GPIO_ResetBits(GPIOA,GPIO_Pin_12);
			GPIO_SetBits(GPIOA,GPIO_Pin_11);
			GPIO_ResetBits(GPIOA,GPIO_Pin_8);
		break;
		case 4://Y3����� HHL
			GPIO_SetBits(GPIOA,GPIO_Pin_12);
			GPIO_SetBits(GPIOA,GPIO_Pin_11);
			GPIO_ResetBits(GPIOA,GPIO_Pin_8);
		break;
		case 6://Y4����� LLH
			GPIO_ResetBits(GPIOA,GPIO_Pin_12);
			GPIO_ResetBits(GPIOA,GPIO_Pin_11);
			GPIO_SetBits(GPIOA,GPIO_Pin_8);
		break;
		case 8://Y5����� HLH
			GPIO_SetBits(GPIOA,GPIO_Pin_12);
			GPIO_ResetBits(GPIOA,GPIO_Pin_11);
			GPIO_SetBits(GPIOA,GPIO_Pin_8);
		break;
		
		//ż���� 2,4,6,8,10
		case 1://Y1����� HLL
			GPIO_SetBits(GPIOC,GPIO_Pin_9);		
			GPIO_ResetBits(GPIOC,GPIO_Pin_8);		
			GPIO_ResetBits(GPIOC,GPIO_Pin_7);
		break;
		case 3://Y2����� LHL
			GPIO_ResetBits(GPIOC,GPIO_Pin_9);		
			GPIO_SetBits(GPIOC,GPIO_Pin_8);		
			GPIO_ResetBits(GPIOC,GPIO_Pin_7);
		break;
		case 5://Y3����� HHL
			GPIO_SetBits(GPIOC,GPIO_Pin_9);		
			GPIO_SetBits(GPIOC,GPIO_Pin_8);		
			GPIO_ResetBits(GPIOC,GPIO_Pin_7);
		break;
		case 7://Y4����� LLH
			GPIO_ResetBits(GPIOC,GPIO_Pin_9);		
			GPIO_ResetBits(GPIOC,GPIO_Pin_8);		
			GPIO_SetBits(GPIOC,GPIO_Pin_7);
		break;
		case 9://Y5����� HLH
			GPIO_SetBits(GPIOC,GPIO_Pin_9);		
			GPIO_ResetBits(GPIOC,GPIO_Pin_8);		
			GPIO_SetBits(GPIOC,GPIO_Pin_7);
		break;
		default:
			break;
	}
}
//========================================================================================================================
//�����
//========================================================================================================================
//==============================
//�г�ʼ����74hc138��ʼ��
//==============================
void motor_row_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB, ENABLE);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;	//PB0->D6,PB1->D5 ����Ϊͨ���������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;	//PC4->DT2,PC5->D7 ����Ϊͨ���������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_4);						//74HC138 ���ȫ������
}

//==============================
//�йر�
//==============================
void row_stop(void)
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_4);						//74HC138 ���ȫ������
}
//==============================
//�н�ͨ
//==============================
static void open_check_channel(uint8_t id, uint8_t sig);

void row_start(uint8_t id, uint8_t sig)
{
	switch(id)
	{
		case 0:
			//Y0 ����� 	LLL
			GPIO_SetBits(GPIOC,GPIO_Pin_4);//E3����
			GPIO_ResetBits(GPIOB,GPIO_Pin_1);//D5����
			GPIO_ResetBits(GPIOB,GPIO_Pin_0);//D6����
			GPIO_ResetBits(GPIOC,GPIO_Pin_5);//D7����
		
			open_check_channel(id, sig);//�����ź�ͨ��
			break;
		case 1:
			//Y1 �����		HLL
			GPIO_SetBits(GPIOC,GPIO_Pin_4);//E3����
			GPIO_SetBits(GPIOB,GPIO_Pin_1);//D5����
			GPIO_ResetBits(GPIOB,GPIO_Pin_0);//D6����
			GPIO_ResetBits(GPIOC,GPIO_Pin_5);//D7����
		
			open_check_channel(id, sig);//�����ź�ͨ��

			break;
		case 2:
			//Y2 �����		LHL
			GPIO_SetBits(GPIOC,GPIO_Pin_4);//E3����
			GPIO_ResetBits(GPIOB,GPIO_Pin_1);//D5����
			GPIO_SetBits(GPIOB,GPIO_Pin_0);//D6����
			GPIO_ResetBits(GPIOC,GPIO_Pin_5);//D7����
		
			open_check_channel(id, sig);//�����ź�ͨ��

			break;
		case 3:
			//Y3 �����		HHL
			GPIO_SetBits(GPIOC,GPIO_Pin_4);//E3����
			GPIO_SetBits(GPIOB,GPIO_Pin_1);//D5����
			GPIO_SetBits(GPIOB,GPIO_Pin_0);//D6����
			GPIO_ResetBits(GPIOC,GPIO_Pin_5);//D7����
		
			open_check_channel(id, sig);//�����ź�ͨ��

			break;
		case 4:
			//Y4 �����		LLH
			GPIO_SetBits(GPIOC,GPIO_Pin_4);//E3����
			GPIO_ResetBits(GPIOB,GPIO_Pin_1);//D5����
			GPIO_ResetBits(GPIOB,GPIO_Pin_0);//D6����
			GPIO_SetBits(GPIOC,GPIO_Pin_5);//D7����
		
			open_check_channel(id, sig);//�����ź�ͨ��

			break;
		case 5:
			//Y5 �����		HLH
			GPIO_SetBits(GPIOC,GPIO_Pin_4);//E3����
			GPIO_SetBits(GPIOB,GPIO_Pin_1);//D5����
			GPIO_ResetBits(GPIOB,GPIO_Pin_0);//D6����
			GPIO_SetBits(GPIOC,GPIO_Pin_5);//D7����
		
			open_check_channel(id, sig);//�����ź�ͨ��
			break;
		case 6:
			//Y6 �����		LHH
			GPIO_SetBits(GPIOC,GPIO_Pin_4);//E3����
			GPIO_ResetBits(GPIOB,GPIO_Pin_1);//D5����
			GPIO_SetBits(GPIOB,GPIO_Pin_0);//D6����
			GPIO_SetBits(GPIOC,GPIO_Pin_5);//D7����
		
			open_check_channel(id, sig);//�����ź�ͨ��
			break;
		case 7:
			//Y7�����		HHH
			GPIO_SetBits(GPIOC,GPIO_Pin_4);//E3����
			GPIO_SetBits(GPIOB,GPIO_Pin_1);//D5����
			GPIO_SetBits(GPIOB,GPIO_Pin_0);//D6����
			GPIO_SetBits(GPIOC,GPIO_Pin_5);//D7����
		
			open_check_channel(id, sig);//�����ź�ͨ��
			break;
	}
	
}


//========================================================================================================================
//�����λ���
//========================================================================================================================
//==============================
//�����λ������ų�ʼ��
//==============================
void motor_signal_config(void)
{
	//IO ���ų�ʼ��
	GPIO_InitTypeDef GPIO_InitStructure;
	
//	NVIC_InitTypeDef NVIC_InitStructure;
//	EXTI_InitTypeDef EXTI_InitStructure;                                   

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA 
	| RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);	

	//SIG1 -> PB8 ��������
	//SO1- > PB7	�������
	//SO2 -> PB6	�������
	//SO3 -> PB5	�������
	
	//SIG2 -> PC10 //��������
	//SE1- > PC11	�������
	//SE2 -> PC12	�������
	//SE3 -> PD2	�������
	
	//�������ź�
	//����4051����
	//PB7��PB6��PB5 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//����ģʽ���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//�ź�����
	//PB8 -> SIG
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//����ģʽ����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	/*
	�ź������ж�
	*/
//	NVIC_InitStructure.NVIC_IRQChannel =   EXTI9_5_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;            //1����ռ���ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                   //0����ռ���ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	
//	EXTI_ClearITPendingBit(EXTI_Line8);
//	EXTI_ClearFlag(EXTI_Line8);
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource8);				//pb8���ⲿ�ж���8����
//	EXTI_InitStructure.EXTI_Line = EXTI_Line8;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                    	//����EXTI��·��Ӧ�ж�����
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;         	//�½�����Ӧ�ж�����
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;								//ѡ�е��ж���״̬
//	EXTI_Init(&EXTI_InitStructure);


	//ż�����ź�
	//����4051����
	//PD2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//����ģʽ���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	//PC12��PC11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//����ģʽ���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	//�ź�����
	//PC10 -> SIG
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//����ģʽ����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	/*
	�ź������ж�
	*/
//	NVIC_InitStructure.NVIC_IRQChannel =   EXTI15_10_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;            //1����ռ���ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                   //0����ռ���ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

//	EXTI_ClearFlag(EXTI_Line10);
//	EXTI_ClearITPendingBit(EXTI_Line10);
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource10);				//pc10�ⲿ�ж���
//	EXTI_InitStructure.EXTI_Line = EXTI_Line10;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                    //����EXTI��·��Ӧ�ж�����
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;         //�½�����Ӧ�ж�����
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;								//ѡ�е��ж���״̬
//	EXTI_Init(&EXTI_InitStructure);
}

//static bool flagOSig;
//static bool flagESig;
/*
�ⲿ�ж� 5~9
*/
uint8_t count1;
void EXTI9_5_IRQHandler(void)
{    
//	uint8_t  rs;
//	rs = 0;
	
	if(EXTI_GetITStatus(EXTI_IMR_MR8) != RESET)		
	{	
		EXTI_ClearITPendingBit(EXTI_IMR_MR8);//����жϱ�־
		count1++;
//		flagOSig = TRUE;
	}
}
/*
�ⲿ�ж� 10~15
*/
void EXTI15_10_IRQHandler(void)
{    
//	uint8_t  rs;
//	rs = 0;
	
	if(EXTI_GetITStatus(EXTI_IMR_MR10) != RESET)		
	{	
		EXTI_ClearITPendingBit(EXTI_IMR_MR10);//����жϱ�־
//		flagESig = TRUE;
	}
}

/*
���ܣ����λ�ò�ѯ
��ʽ����ѯ��ʽ,�����������(�ⲿ�жϷ�ʽ ���ļ���io_position.c����)

������id �����
����ֵ�������ƽ
*/
uint8_t check_position(uint8_t id)
{ 
	uint8_t rs = FALSE;
	switch(id % 2)
	{
		case 0://�������ź�
			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8))	//SIG1 
				rs = TRUE;
		break;
			
		case 1://ż�����ź�
			if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10))	//SIG2
				rs = TRUE;
		break;
	}	
	return rs;
}

/*
�򿪶�Ӧ�ź�ͨ��
*/
static void open_check_channel(uint8_t id, uint8_t sig)
{
	#if DRIVERBOARD_TYPE == 1	//�ӽ���ת�Ӱ壬ֻ��2·�ź�
	{
		switch(id % 2)
		{
			case 0:
				GPIO_ResetBits(GPIOB,GPIO_Pin_7);
				GPIO_ResetBits(GPIOB,GPIO_Pin_6);
				GPIO_ResetBits(GPIOB,GPIO_Pin_5);
			break;
			
			case 1:
				GPIO_ResetBits(GPIOC,GPIO_Pin_11);
				GPIO_ResetBits(GPIOC,GPIO_Pin_12);
				GPIO_ResetBits(GPIOD,GPIO_Pin_2);
			break;
		}
	}
	
	#else 	//ͨ�û���
	{
		switch(id)
		{
			case 0:
				//IO0ѡͨ
				if(sig == 0)//�����ź�ѡͨ
				{
					//LLL
					GPIO_ResetBits(GPIOB,GPIO_Pin_7);
					GPIO_ResetBits(GPIOB,GPIO_Pin_6);
					GPIO_ResetBits(GPIOB,GPIO_Pin_5);
				}
				else//ż���ź�ѡͨ
				{
					//LLL
					GPIO_ResetBits(GPIOC,GPIO_Pin_11);
					GPIO_ResetBits(GPIOC,GPIO_Pin_12);
					GPIO_ResetBits(GPIOD,GPIO_Pin_2);
				}
				break;
			case 1:
				//IO1ѡͨ
				if(sig == 0)//�����ź�ѡͨ
				{
					//LLH
					GPIO_ResetBits(GPIOB,GPIO_Pin_7);
					GPIO_ResetBits(GPIOB,GPIO_Pin_6);
					GPIO_SetBits(GPIOB,GPIO_Pin_5);
				}
				else//ż���ź�ѡͨ
				{
					//LLH
					GPIO_ResetBits(GPIOC,GPIO_Pin_11);
					GPIO_ResetBits(GPIOC,GPIO_Pin_12);
					GPIO_SetBits(GPIOD,GPIO_Pin_2);
				}
				break;
			case 2:
				//IO2ѡͨ
				if(sig == 0)//�����ź�ѡͨ
				{
					//LHL
					GPIO_ResetBits(GPIOB,GPIO_Pin_7);
					GPIO_SetBits(GPIOB,GPIO_Pin_6);
					GPIO_ResetBits(GPIOB,GPIO_Pin_5);
				}
				else//ż���ź�ѡͨ
				{
					//LHL
					GPIO_ResetBits(GPIOC,GPIO_Pin_11);
					GPIO_SetBits(GPIOC,GPIO_Pin_12);
					GPIO_ResetBits(GPIOD,GPIO_Pin_2);
				}
				break;
			case 3:
				//IO3ѡͨ
				if(sig == 0)//�����ź�ѡͨ
				{
					//LHH
					GPIO_ResetBits(GPIOB,GPIO_Pin_7);
					GPIO_SetBits(GPIOB,GPIO_Pin_6);
					GPIO_SetBits(GPIOB,GPIO_Pin_5);
				}
				else//ż���ź�ѡͨ
				{
					//LHH
					GPIO_ResetBits(GPIOC,GPIO_Pin_11);
					GPIO_SetBits(GPIOC,GPIO_Pin_12);
					GPIO_SetBits(GPIOD,GPIO_Pin_2);
				}
				break;
			case 4:
				//IO4ѡͨ
				if(sig == 0)//�����ź�ѡͨ
				{
					//HLL
					GPIO_SetBits(GPIOB,GPIO_Pin_7);
					GPIO_ResetBits(GPIOB,GPIO_Pin_6);
					GPIO_ResetBits(GPIOB,GPIO_Pin_5);
				}
				else//ż���ź�ѡͨ
				{
					//HLL
					GPIO_SetBits(GPIOC,GPIO_Pin_11);
					GPIO_ResetBits(GPIOC,GPIO_Pin_12);
					GPIO_ResetBits(GPIOD,GPIO_Pin_2);
				}
				break;
			case 5:
				//IO5ѡͨ
				if(sig == 0)//�����ź�ѡͨ
				{
					//HLH
					GPIO_SetBits(GPIOB,GPIO_Pin_7);
					GPIO_ResetBits(GPIOB,GPIO_Pin_6);
					GPIO_SetBits(GPIOB,GPIO_Pin_5);
				}
				else//ż���ź�ѡͨ
				{
					//HLH
					GPIO_SetBits(GPIOC,GPIO_Pin_11);
					GPIO_ResetBits(GPIOC,GPIO_Pin_12);
					GPIO_SetBits(GPIOD,GPIO_Pin_2);
				}
				break;
			case 6:
				//IO6ѡͨ
				if(sig == 0)//�����ź�ѡͨ
				{
					//HHL
					GPIO_SetBits(GPIOB,GPIO_Pin_7);
					GPIO_SetBits(GPIOB,GPIO_Pin_6);
					GPIO_ResetBits(GPIOB,GPIO_Pin_5);
				}
				else//ż���ź�ѡͨ
				{
					//HHL
					GPIO_SetBits(GPIOC,GPIO_Pin_11);
					GPIO_SetBits(GPIOC,GPIO_Pin_12);
					GPIO_ResetBits(GPIOD,GPIO_Pin_2);
				}
				break;
			case 7:
				//IO6ѡͨ
				if(sig == 0)//�����ź�ѡͨ
				{
					//HHH
					GPIO_SetBits(GPIOB,GPIO_Pin_7);
					GPIO_SetBits(GPIOB,GPIO_Pin_5);
					GPIO_SetBits(GPIOB,GPIO_Pin_6);
				}
				else//ż���ź�ѡͨ
				{
					//HHH
					GPIO_SetBits(GPIOC,GPIO_Pin_11);
					GPIO_SetBits(GPIOC,GPIO_Pin_12);
					GPIO_SetBits(GPIOD,GPIO_Pin_2);
				}
				break;
		}	
	}
	#endif
	
}
