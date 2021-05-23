//====================================
//6·���������·�����ļ�
//��bsp���ṩ�ӿ�
//====================================
#include "stm32f10x.h"
#include "ctrl_output.h"

//====================================
//��� �ܽ�����
//˵����ʹ�� PB2,PB12��PB13��PB14��PB15,PC6
//====================================
void output_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB, ENABLE);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;					//PC6����Ϊͨ���������  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//���߷�ת�ٶ�Ϊ50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					
	GPIO_ResetBits(GPIOC,GPIO_Pin_6);	//Ĭ�Ϲر�ͨ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_2;		//PB2,PB12~PB15����Ϊͨ���������  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//���߷�ת�ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					
	GPIO_ResetBits(GPIOB,GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_2);
}
//====================================
//��������
//====================================
void set_PB2(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_2);
}
void set_PB12(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
}
void set_PB13(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_13);
}
void set_PB14(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
}
void set_PB15(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
}
void set_PC6(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_6);
}
//====================================
//��������
//====================================
void reset_PB2(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_2);
}
void reset_PB12(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
}
void reset_PB13(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_13);
}
void reset_PB14(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);
}
void reset_PB15(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);
}
void reset_PC6(void)
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_6);
} 

