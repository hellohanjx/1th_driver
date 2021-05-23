//============================
//电机控制驱动层
//功能：向bsp提供驱动接口
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
//电机列
//========================================================================================================================
//==============================
//列初始化，74hc237初始化
//==============================
void motor_line_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);	

	//奇数列引脚 PA12 PA11 PA8 ,237的地址控制引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_11 |  GPIO_Pin_8;	//PA12->PO1,PA11->PO2，PA8->PA3 配置为通用推挽输出  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			
	GPIO_Init(GPIOA, &GPIO_InitStructure);		
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);								//74HC237 选择不存在的通道
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	
	//偶数列引脚 PC9 PC8 PC7, 237的地址控制引脚
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 |  GPIO_Pin_8 | GPIO_Pin_9;	//PC7->PE3, PC8->PE2，PC9->PE1 配置为通用推挽输出  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//口线翻转速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					
	GPIO_ResetBits(GPIOC,GPIO_Pin_9);								//74HC237 选择不存在的通道
	GPIO_ResetBits(GPIOC,GPIO_Pin_8);		
	GPIO_ResetBits(GPIOC,GPIO_Pin_7);
}
//==============================
//列关闭
//==============================
void line_stop(void)
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_9);								//74HC237 选择不存在的通道
	GPIO_ResetBits(GPIOC,GPIO_Pin_8);		
	GPIO_ResetBits(GPIOC,GPIO_Pin_7);
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);								//74HC237 选择不存在的通道
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
}
//==============================
//列接通
//==============================
void line_start(uint8_t id)
{
	switch(id){
		//(奇数列) 1,3,5,7,9
		case 0://Y1输出高 HLL
			GPIO_SetBits(GPIOA,GPIO_Pin_12);
			GPIO_ResetBits(GPIOA,GPIO_Pin_11);
			GPIO_ResetBits(GPIOA,GPIO_Pin_8);
		break;
		case 2://Y2输出高 LHL
			GPIO_ResetBits(GPIOA,GPIO_Pin_12);
			GPIO_SetBits(GPIOA,GPIO_Pin_11);
			GPIO_ResetBits(GPIOA,GPIO_Pin_8);
		break;
		case 4://Y3输出高 HHL
			GPIO_SetBits(GPIOA,GPIO_Pin_12);
			GPIO_SetBits(GPIOA,GPIO_Pin_11);
			GPIO_ResetBits(GPIOA,GPIO_Pin_8);
		break;
		case 6://Y4输出高 LLH
			GPIO_ResetBits(GPIOA,GPIO_Pin_12);
			GPIO_ResetBits(GPIOA,GPIO_Pin_11);
			GPIO_SetBits(GPIOA,GPIO_Pin_8);
		break;
		case 8://Y5输出高 HLH
			GPIO_SetBits(GPIOA,GPIO_Pin_12);
			GPIO_ResetBits(GPIOA,GPIO_Pin_11);
			GPIO_SetBits(GPIOA,GPIO_Pin_8);
		break;
		
		//偶数列 2,4,6,8,10
		case 1://Y1输出高 HLL
			GPIO_SetBits(GPIOC,GPIO_Pin_9);		
			GPIO_ResetBits(GPIOC,GPIO_Pin_8);		
			GPIO_ResetBits(GPIOC,GPIO_Pin_7);
		break;
		case 3://Y2输出高 LHL
			GPIO_ResetBits(GPIOC,GPIO_Pin_9);		
			GPIO_SetBits(GPIOC,GPIO_Pin_8);		
			GPIO_ResetBits(GPIOC,GPIO_Pin_7);
		break;
		case 5://Y3输出高 HHL
			GPIO_SetBits(GPIOC,GPIO_Pin_9);		
			GPIO_SetBits(GPIOC,GPIO_Pin_8);		
			GPIO_ResetBits(GPIOC,GPIO_Pin_7);
		break;
		case 7://Y4输出高 LLH
			GPIO_ResetBits(GPIOC,GPIO_Pin_9);		
			GPIO_ResetBits(GPIOC,GPIO_Pin_8);		
			GPIO_SetBits(GPIOC,GPIO_Pin_7);
		break;
		case 9://Y5输出高 HLH
			GPIO_SetBits(GPIOC,GPIO_Pin_9);		
			GPIO_ResetBits(GPIOC,GPIO_Pin_8);		
			GPIO_SetBits(GPIOC,GPIO_Pin_7);
		break;
		default:
			break;
	}
}
//========================================================================================================================
//电机行
//========================================================================================================================
//==============================
//行初始化，74hc138初始化
//==============================
void motor_row_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB, ENABLE);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;	//PB0->D6,PB1->D5 配置为通用推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;	//PC4->DT2,PC5->D7 配置为通用推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_4);						//74HC138 输出全部拉高
}

//==============================
//行关闭
//==============================
void row_stop(void)
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_4);						//74HC138 输出全部拉高
}
//==============================
//行接通
//==============================
static void open_check_channel(uint8_t id, uint8_t sig);

void row_start(uint8_t id, uint8_t sig)
{
	switch(id)
	{
		case 0:
			//Y0 输出低 	LLL
			GPIO_SetBits(GPIOC,GPIO_Pin_4);//E3拉高
			GPIO_ResetBits(GPIOB,GPIO_Pin_1);//D5拉低
			GPIO_ResetBits(GPIOB,GPIO_Pin_0);//D6拉低
			GPIO_ResetBits(GPIOC,GPIO_Pin_5);//D7拉低
		
			open_check_channel(id, sig);//开启信号通道
			break;
		case 1:
			//Y1 输出低		HLL
			GPIO_SetBits(GPIOC,GPIO_Pin_4);//E3拉高
			GPIO_SetBits(GPIOB,GPIO_Pin_1);//D5拉高
			GPIO_ResetBits(GPIOB,GPIO_Pin_0);//D6拉低
			GPIO_ResetBits(GPIOC,GPIO_Pin_5);//D7拉低
		
			open_check_channel(id, sig);//开启信号通道

			break;
		case 2:
			//Y2 输出低		LHL
			GPIO_SetBits(GPIOC,GPIO_Pin_4);//E3拉高
			GPIO_ResetBits(GPIOB,GPIO_Pin_1);//D5拉低
			GPIO_SetBits(GPIOB,GPIO_Pin_0);//D6拉高
			GPIO_ResetBits(GPIOC,GPIO_Pin_5);//D7拉低
		
			open_check_channel(id, sig);//开启信号通道

			break;
		case 3:
			//Y3 输出低		HHL
			GPIO_SetBits(GPIOC,GPIO_Pin_4);//E3拉高
			GPIO_SetBits(GPIOB,GPIO_Pin_1);//D5拉高
			GPIO_SetBits(GPIOB,GPIO_Pin_0);//D6拉高
			GPIO_ResetBits(GPIOC,GPIO_Pin_5);//D7拉低
		
			open_check_channel(id, sig);//开启信号通道

			break;
		case 4:
			//Y4 输出低		LLH
			GPIO_SetBits(GPIOC,GPIO_Pin_4);//E3拉高
			GPIO_ResetBits(GPIOB,GPIO_Pin_1);//D5拉低
			GPIO_ResetBits(GPIOB,GPIO_Pin_0);//D6拉低
			GPIO_SetBits(GPIOC,GPIO_Pin_5);//D7拉高
		
			open_check_channel(id, sig);//开启信号通道

			break;
		case 5:
			//Y5 输出低		HLH
			GPIO_SetBits(GPIOC,GPIO_Pin_4);//E3拉高
			GPIO_SetBits(GPIOB,GPIO_Pin_1);//D5拉高
			GPIO_ResetBits(GPIOB,GPIO_Pin_0);//D6拉低
			GPIO_SetBits(GPIOC,GPIO_Pin_5);//D7拉高
		
			open_check_channel(id, sig);//开启信号通道
			break;
		case 6:
			//Y6 输出低		LHH
			GPIO_SetBits(GPIOC,GPIO_Pin_4);//E3拉高
			GPIO_ResetBits(GPIOB,GPIO_Pin_1);//D5拉低
			GPIO_SetBits(GPIOB,GPIO_Pin_0);//D6拉高
			GPIO_SetBits(GPIOC,GPIO_Pin_5);//D7拉高
		
			open_check_channel(id, sig);//开启信号通道
			break;
		case 7:
			//Y7输出低		HHH
			GPIO_SetBits(GPIOC,GPIO_Pin_4);//E3拉高
			GPIO_SetBits(GPIOB,GPIO_Pin_1);//D5拉高
			GPIO_SetBits(GPIOB,GPIO_Pin_0);//D6拉高
			GPIO_SetBits(GPIOC,GPIO_Pin_5);//D7拉高
		
			open_check_channel(id, sig);//开启信号通道
			break;
	}
	
}


//========================================================================================================================
//电机卡位检测
//========================================================================================================================
//==============================
//电机卡位检测引脚初始化
//==============================
void motor_signal_config(void)
{
	//IO 引脚初始化
	GPIO_InitTypeDef GPIO_InitStructure;
	
//	NVIC_InitTypeDef NVIC_InitStructure;
//	EXTI_InitTypeDef EXTI_InitStructure;                                   

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA 
	| RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);	

	//SIG1 -> PB8 浮空输入
	//SO1- > PB7	推挽输出
	//SO2 -> PB6	推挽输出
	//SO3 -> PB5	推挽输出
	
	//SIG2 -> PC10 //浮空输入
	//SE1- > PC11	推挽输出
	//SE2 -> PC12	推挽输出
	//SE3 -> PD2	推挽输出
	
	//奇数列信号
	//控制4051引脚
	//PB7，PB6，PB5 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//工作模式：推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	//信号引脚
	//PB8 -> SIG
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//工作模式：浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	/*
	信号引脚中断
	*/
//	NVIC_InitStructure.NVIC_IRQChannel =   EXTI9_5_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;            //1级先占优先级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                   //0级次占优先级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	
//	EXTI_ClearITPendingBit(EXTI_Line8);
//	EXTI_ClearFlag(EXTI_Line8);
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource8);				//pb8与外部中断线8连接
//	EXTI_InitStructure.EXTI_Line = EXTI_Line8;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                    	//设置EXTI线路响应中断请求
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;         	//下降沿响应中断请求
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;								//选中的中断线状态
//	EXTI_Init(&EXTI_InitStructure);


	//偶数列信号
	//控制4051引脚
	//PD2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//工作模式：推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	//PC12，PC11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//工作模式：推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	//信号引脚
	//PC10 -> SIG
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//工作模式：浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	/*
	信号引脚中断
	*/
//	NVIC_InitStructure.NVIC_IRQChannel =   EXTI15_10_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;            //1级先占优先级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                   //0级次占优先级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

//	EXTI_ClearFlag(EXTI_Line10);
//	EXTI_ClearITPendingBit(EXTI_Line10);
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource10);				//pc10外部中断线
//	EXTI_InitStructure.EXTI_Line = EXTI_Line10;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;                    //设置EXTI线路响应中断请求
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;         //下降沿响应中断请求
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;								//选中的中断线状态
//	EXTI_Init(&EXTI_InitStructure);
}

//static bool flagOSig;
//static bool flagESig;
/*
外部中断 5~9
*/
uint8_t count1;
void EXTI9_5_IRQHandler(void)
{    
//	uint8_t  rs;
//	rs = 0;
	
	if(EXTI_GetITStatus(EXTI_IMR_MR8) != RESET)		
	{	
		EXTI_ClearITPendingBit(EXTI_IMR_MR8);//清除中断标志
		count1++;
//		flagOSig = TRUE;
	}
}
/*
外部中断 10~15
*/
void EXTI15_10_IRQHandler(void)
{    
//	uint8_t  rs;
//	rs = 0;
	
	if(EXTI_GetITStatus(EXTI_IMR_MR10) != RESET)		
	{	
		EXTI_ClearITPendingBit(EXTI_IMR_MR10);//清除中断标志
//		flagESig = TRUE;
	}
}

/*
功能：电机位置查询
方式：查询方式,下面这个函数(外部中断方式 在文件“io_position.c”中)

参数：id 电机号
返回值：电机电平
*/
uint8_t check_position(uint8_t id)
{ 
	uint8_t rs = FALSE;
	switch(id % 2)
	{
		case 0://奇数列信号
			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8))	//SIG1 
				rs = TRUE;
		break;
			
		case 1://偶数列信号
			if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10))	//SIG2
				rs = TRUE;
		break;
	}	
	return rs;
}

/*
打开对应信号通道
*/
static void open_check_channel(uint8_t id, uint8_t sig)
{
	#if DRIVERBOARD_TYPE == 1	//接金码转接板，只有2路信号
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
	
	#else 	//通用机型
	{
		switch(id)
		{
			case 0:
				//IO0选通
				if(sig == 0)//奇数信号选通
				{
					//LLL
					GPIO_ResetBits(GPIOB,GPIO_Pin_7);
					GPIO_ResetBits(GPIOB,GPIO_Pin_6);
					GPIO_ResetBits(GPIOB,GPIO_Pin_5);
				}
				else//偶数信号选通
				{
					//LLL
					GPIO_ResetBits(GPIOC,GPIO_Pin_11);
					GPIO_ResetBits(GPIOC,GPIO_Pin_12);
					GPIO_ResetBits(GPIOD,GPIO_Pin_2);
				}
				break;
			case 1:
				//IO1选通
				if(sig == 0)//奇数信号选通
				{
					//LLH
					GPIO_ResetBits(GPIOB,GPIO_Pin_7);
					GPIO_ResetBits(GPIOB,GPIO_Pin_6);
					GPIO_SetBits(GPIOB,GPIO_Pin_5);
				}
				else//偶数信号选通
				{
					//LLH
					GPIO_ResetBits(GPIOC,GPIO_Pin_11);
					GPIO_ResetBits(GPIOC,GPIO_Pin_12);
					GPIO_SetBits(GPIOD,GPIO_Pin_2);
				}
				break;
			case 2:
				//IO2选通
				if(sig == 0)//奇数信号选通
				{
					//LHL
					GPIO_ResetBits(GPIOB,GPIO_Pin_7);
					GPIO_SetBits(GPIOB,GPIO_Pin_6);
					GPIO_ResetBits(GPIOB,GPIO_Pin_5);
				}
				else//偶数信号选通
				{
					//LHL
					GPIO_ResetBits(GPIOC,GPIO_Pin_11);
					GPIO_SetBits(GPIOC,GPIO_Pin_12);
					GPIO_ResetBits(GPIOD,GPIO_Pin_2);
				}
				break;
			case 3:
				//IO3选通
				if(sig == 0)//奇数信号选通
				{
					//LHH
					GPIO_ResetBits(GPIOB,GPIO_Pin_7);
					GPIO_SetBits(GPIOB,GPIO_Pin_6);
					GPIO_SetBits(GPIOB,GPIO_Pin_5);
				}
				else//偶数信号选通
				{
					//LHH
					GPIO_ResetBits(GPIOC,GPIO_Pin_11);
					GPIO_SetBits(GPIOC,GPIO_Pin_12);
					GPIO_SetBits(GPIOD,GPIO_Pin_2);
				}
				break;
			case 4:
				//IO4选通
				if(sig == 0)//奇数信号选通
				{
					//HLL
					GPIO_SetBits(GPIOB,GPIO_Pin_7);
					GPIO_ResetBits(GPIOB,GPIO_Pin_6);
					GPIO_ResetBits(GPIOB,GPIO_Pin_5);
				}
				else//偶数信号选通
				{
					//HLL
					GPIO_SetBits(GPIOC,GPIO_Pin_11);
					GPIO_ResetBits(GPIOC,GPIO_Pin_12);
					GPIO_ResetBits(GPIOD,GPIO_Pin_2);
				}
				break;
			case 5:
				//IO5选通
				if(sig == 0)//奇数信号选通
				{
					//HLH
					GPIO_SetBits(GPIOB,GPIO_Pin_7);
					GPIO_ResetBits(GPIOB,GPIO_Pin_6);
					GPIO_SetBits(GPIOB,GPIO_Pin_5);
				}
				else//偶数信号选通
				{
					//HLH
					GPIO_SetBits(GPIOC,GPIO_Pin_11);
					GPIO_ResetBits(GPIOC,GPIO_Pin_12);
					GPIO_SetBits(GPIOD,GPIO_Pin_2);
				}
				break;
			case 6:
				//IO6选通
				if(sig == 0)//奇数信号选通
				{
					//HHL
					GPIO_SetBits(GPIOB,GPIO_Pin_7);
					GPIO_SetBits(GPIOB,GPIO_Pin_6);
					GPIO_ResetBits(GPIOB,GPIO_Pin_5);
				}
				else//偶数信号选通
				{
					//HHL
					GPIO_SetBits(GPIOC,GPIO_Pin_11);
					GPIO_SetBits(GPIOC,GPIO_Pin_12);
					GPIO_ResetBits(GPIOD,GPIO_Pin_2);
				}
				break;
			case 7:
				//IO6选通
				if(sig == 0)//奇数信号选通
				{
					//HHH
					GPIO_SetBits(GPIOB,GPIO_Pin_7);
					GPIO_SetBits(GPIOB,GPIO_Pin_5);
					GPIO_SetBits(GPIOB,GPIO_Pin_6);
				}
				else//偶数信号选通
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
