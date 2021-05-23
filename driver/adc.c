//============================
//adc转换驱动
//============================
#include "stdint.h"
#include "stm32f10x.h"
#include "misc.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "adc.h"

uint16_t adc67Value[2];		//电机采样
uint8_t convert;			//转换标志

static ITStatus adcDmaFlag = RESET;//adc中断标志
static ADC1_67 adc67Fun = 0;
//================================
//回调函数注册
//================================
void get_adc67_init(ADC1_67 fun)
{
	adc67Fun = fun;
}
//================================
//采样的电流数据处理
//================================
static uint8_t  current_dealwith(ADC1_67 callBack) 
{
	uint32_t po = (adc67Value[0]*3300*100/(4095*3) - adc67Value[1]*3300*100/(4095*3))/8;
	if(po >100 && (adc67Value[0] > adc67Value[1]))
		po=po;
	if(callBack == 0)
		return FALSE;
	if(adc67Value[0] < adc67Value[1])//未安装电机，有这种情况，应是干扰造成
		callBack(0);
	else
		callBack(po);
	return TRUE;
}

//================================
//adc初始化
//说明：ADC转换速度很慢，需要至少 1us
//初始化ADC1 通道6，7,使用规则组
//初始化ADC1 通道4，5，使用注入组
//使用DMA传输
//================================
void adc1_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	//GPIO类型
	ADC_InitTypeDef ADC_InitStructure;		//ADC 类型
	DMA_InitTypeDef DMA_InitStructure;		//DMA 类型
	NVIC_InitTypeDef NVIC_InitStructure;	//中断 类型

	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_ADC1 , ENABLE );//使能GPIOA 时钟与复用时钟，使能ADC 时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//DMA时钟打开
	//RCC_ADCCLKConfig(RCC_PCLK2_Div4);	   				//ADC时钟为 PCLK2的4分频 56/4（pclk2就是 apb2）
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);		//设置为12M ，adc时钟须<=14MHz
	
	//gpio 引脚设置
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;//ADC1通道 PA4,PA5,PA6,PA7  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	                    //模拟输入模式
	GPIO_Init(GPIOA, &GPIO_InitStructure);								//初始化引脚
	
	//DMA1 通道初始化
	DMA_DeInit(DMA1_Channel1);		//恢复寄存器默认值
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;		//外设地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&adc67Value;	//内存地址（要存入数据的地址）
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;				//DMA转换模式SCR模式，由外设搬运到内存
	DMA_InitStructure.DMA_BufferSize = 2;							//DMA缓存大小(DMA传输时缓冲区长度)
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//接收一次数据后，DMA缓冲区地址禁止递增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;			//内存地址递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//DMA 外设数据长度
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			//内存数据长度
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;					//循环模式开启，buf写满后，自动回到初始地址传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;				//优先级高
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;					//内存到内存传输关闭
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC ,ENABLE);					//开启 DMA 传输完成中断
	
	//中断设置 DMA1
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;		//DMA1 通道1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//占先优先级 0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		//从优先级 0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	DMA_Cmd(DMA1_Channel1, ENABLE);                                	//使能DMA 通道1

	//ADC1 初始化
	ADC_DeInit(ADC1);  //ADC1 寄存器恢复默认值
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	                //独立工作模式,不和其他ADCX交互工作
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;					    //开启多通道扫描，单通道
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;					//单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//软件触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	            //ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 2;	                            //进行规则转换的ADC通道有2个
	ADC_Init(ADC1, &ADC_InitStructure);

	//设置规则通道 ADC1使用6，7转换通道，转换顺序1，2
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6 , 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7 , 2, ADC_SampleTime_239Cycles5);
	
	//设置注入组通道
	ADC_InjectedSequencerLengthConfig(ADC1, 2);//注入组通道数 ，需要先配置
    ADC_InjectedChannelConfig(ADC1,ADC_Channel_4,1,ADC_SampleTime_239Cycles5);
	ADC_InjectedChannelConfig(ADC1,ADC_Channel_5,2,ADC_SampleTime_239Cycles5);
    ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_None);//不用外部触发或者定时器出发等，使用软件触发
	//ADC_AutoInjectedConvCmd (ADC1,ENABLE);	//规则组转换完成自动开始注入组转换
	//ADC_SoftwareStartInjectedConvCmd(ADC1, ENABLE);//开始注入通道转换
	
	
	
	ADC_DMACmd(ADC1, ENABLE);                                           //使能 ADC1 DMA
	
	/* Enable ADC1 external trigger */
    ADC_ExternalTrigConvCmd(ADC1, DISABLE);
    ADC_ExternalTrigInjectedConvCmd(ADC1, DISABLE);

	ADC_Cmd(ADC1, ENABLE);	                                            //使能ADC1
	ADC_ResetCalibration(ADC1);	                                        //使能ADC1复位校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC1));	                        //等待复位校准寄存器接收
	ADC_StartCalibration(ADC1);			                                //启动ADC1校准
	while(ADC_GetCalibrationStatus(ADC1));	                            //等待ADC1校准结束
	//ADC_SoftwareStartConvCmd(ADC1, ENABLE);		                        //启动软件转换
}

//============================
//DMA中断
//使用 DMA1 外设的通道1（channel1）
//DMA1 对应 ADC1 的规则组
//============================
void DMA1_Channel1_IRQHandler(void)
{          
	if(DMA_GetITStatus(DMA1_IT_TC1) == SET)    
	{
		DMA_ClearITPendingBit(DMA1_IT_TC1); //清除中断标志 
		//回调
		current_dealwith(adc67Fun);
	}
	else
    if(DMA_GetITStatus(DMA1_IT_HT1) == SET)
    {
        DMA_ClearITPendingBit(DMA1_IT_HT1);	//传输过半中断
        DMA_ClearITPendingBit(DMA1_IT_GL1); //全局中断                               
    }

}
//================================
//adc1 通道67，规则组开始采样 
//================================
void adc1_67_start(void)
{
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
//================================
//adc1 通道67，检测是否采样完毕
//================================
ITStatus adc1_67_state(void)
{
	if(adcDmaFlag == SET)//如果DMA 完成接收
	{
		adcDmaFlag = RESET;
		//返回数据
		return SET;
	}
	else 
	{
		return RESET;
	}
}
//================================
//adc1 通道67，获取adc通道采样值
//参数：adc6 通道6数据指针，bsp传递过来
//adc7 通道7数据指针，bsp获取驱动层变量值
//================================
void get_adc67_value(uint16_t *adc6,uint16_t *adc7)
{
	*adc6 = adc67Value[0];
	*adc7 = adc67Value[1];
}




//================================
//电机adc采集
//说明：经过了滤波（采集了5次，取平均值）
//返回值：电机电流 mA
//================================
//uint16_t motor_current_get(void)
//{
//	uint16_t  current[SAMPLING_ANT],cur,i;
//	uint32_t  tmp;
//	
//	convert = 0;
//	for(i = 0; i < SAMPLING_ANT ; i++)
//	{
//		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
//		//等待采样结束
//		while(convert == 0);
////		rs1 = ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC);
////		while(!(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC  )));
//		current[i] = (motorADC[0] - motorADC[1])/(8*SAMPLING_RES);
//		convert = 0;
//	}
//	for(i = 0,tmp = 0; i < SAMPLING_ANT ; i++)
//		tmp += current[i];
//	cur = tmp/5;
//	return cur;
//}
 
//================================
//adc
//返回值：adc1 通道4，5 值
//================================
void  get_adc45_value(uint16_t *adc1_4,uint16_t *adc1_5)
{
	ADC_SoftwareStartInjectedConvCmd(ADC1,ENABLE);
	//等待注入组转换完成
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_JEOC ));
//	rs1 = ADC_GetFlagStatus(ADC1,ADC_FLAG_JEOC);
//	rs1 = ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC);
	ADC_ClearFlag(ADC1, ADC_FLAG_JEOC);
	*adc1_4 = ADC_GetInjectedConversionValue(ADC1,ADC_InjectedChannel_1 );
	*adc1_5 = ADC_GetInjectedConversionValue(ADC1,ADC_InjectedChannel_2 );
}

