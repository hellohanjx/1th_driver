//====================================
//此文件用来处理电机卡位检测
//注意：电机启动时，电机卡位的微动开关
//压住时,电机IO输出24v左右，这时cpu引脚
//为低电平，当微动开关松开（弹起）时，
//cpu 引脚为高电平
//所以：
//从微动开关压下到弹起，是一个上升沿
//从微动开关弹起到压下，是一个下降沿
//微动开关压下，低电位
//微动开关弹起，高电位
//====================================
#include "stm32f10x.h"
#include "misc.h"
#include "stm32f10x_exti.h"
#include "delay.h"

//====================================
//电机卡位检测 初始化
//====================================
void io_position_init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;                                   

	//打开 GPIO 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB | 
						   RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
	//IO 引脚初始化
	//PB9 -> IO1
	//PB8 -> IO2
	//PB7 -> IO3
	//PB6 -> IO4
	//PB5 -> IO5
	//PD2 -> IO6
	//PC12 -> IO7
	//PC11 -> IO8
	//PC10 -> IO9
	//PA12 -> IO10（这个引脚和PB12有可能冲突）

	//PB9，PB8，PB7，PB6，PB5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8 | GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//工作模式：浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//PD2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//工作模式：浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStructure);

	//PC12，PC11，PC10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_11 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//工作模式：浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	//PA12
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//工作模式：浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	//==================================================
	//外部中断设置
	//==================================================
	//外部中断线5~9，10~15，2
	//NVIC_InitStructure.NVIC_IRQChannel =   EXTI2_IRQn | EXTI9_5_IRQn;    
	NVIC_InitStructure.NVIC_IRQChannel =   EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;            //1级先占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                   //0级次占优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel =   EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;            //1级先占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                   //0级次占优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel =   EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;            //1级先占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                   //0级次占优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//==================================================
	//中断线与中断引脚相连
	//==================================================
	//选择GPIO 引脚作为外部中断线
	//PB5,PB6,PB7,PB8,PB9
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5 | GPIO_PinSource6 | GPIO_PinSource7 | GPIO_PinSource8 | GPIO_PinSource9);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource2);		//PD2 外部中断线
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource10 | GPIO_PinSource11 | GPIO_PinSource10);//PC10，PC11，PC12 外部中断线
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource12);	//PA12 外部中断线

	EXTI_InitStructure.EXTI_Line = EXTI_Line5 | EXTI_Line6 | EXTI_Line7 | EXTI_Line8 | EXTI_Line9 | EXTI_Line2 | EXTI_Line10 | EXTI_Line11 | EXTI_Line12 ;
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
//	EXTI_InitStructure.EXTI_Line = EXTI_IMR_MR5 | EXTI_IMR_MR6 | EXTI_IMR_MR7 | EXTI_IMR_MR8 | EXTI_IMR_MR9 | EXTI_IMR_MR2 | EXTI_IMR_MR10 | EXTI_IMR_MR11 | EXTI_IMR_MR12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                    //设置EXTI线路响应中断请求
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;                //下降沿响应中断请求
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;			//选中的中断线状态
	EXTI_Init(&EXTI_InitStructure);

}

//=================================
//5~9外部中断
//=================================
void EXTI9_5_IRQHandler1(void)
{    
//	uint8_t  rs;
//	rs = 0;
	
	if(EXTI_GetITStatus(EXTI_IMR_MR5) != RESET)		
	{	
		EXTI_ClearITPendingBit(EXTI_IMR_MR5);                  //清除按键挂起位	
	}
	if(EXTI_GetITStatus(EXTI_IMR_MR6) != RESET)		
	{	
		EXTI_ClearITPendingBit(EXTI_IMR_MR6);                  //清除按键挂起位	
	}
	if(EXTI_GetITStatus(EXTI_IMR_MR7) != RESET)		
	{	
		EXTI_ClearITPendingBit(EXTI_IMR_MR7);                  //清除按键挂起位	
	}
	if(EXTI_GetITStatus(EXTI_IMR_MR8) != RESET)		
	{	
		EXTI_ClearITPendingBit(EXTI_IMR_MR8);                  //清除按键挂起位	
	}
	if(EXTI_GetITStatus(EXTI_IMR_MR9) != RESET)		
	{	
		EXTI_ClearITPendingBit(EXTI_IMR_MR9);                  //清除按键挂起位	
	}

}

//=================================
//2外部中断
//=================================
void EXTI2_IRQHandler(void)
{    
	delay_ms(2);//这里必须等待，使电平稳定
	if(EXTI_GetITStatus(EXTI_Line2) != RESET)
	{	
		//这里有点问题，设置为上升沿触发，但是实际是上升沿和下降沿都进入中断
		if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2))//读引脚电平，高电平表示到卡位
		//这里不要直接操作。搞个标志位来操作
		//motor_stop();
		EXTI_ClearITPendingBit(EXTI_Line2);                  //清除挂起位	
	}
}
//=================================
//10~15外部中断
//=================================

void EXTI15_10_IRQHandler1(void)
{    

	if(EXTI_GetITStatus(EXTI_IMR_MR12) != RESET)
	{	
		EXTI_ClearITPendingBit(EXTI_IMR_MR12);                  //清除按键挂起位	
	}
}

