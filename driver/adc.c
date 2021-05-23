//============================
//adcת������
//============================
#include "stdint.h"
#include "stm32f10x.h"
#include "misc.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "adc.h"

uint16_t adc67Value[2];		//�������
uint8_t convert;			//ת����־

static ITStatus adcDmaFlag = RESET;//adc�жϱ�־
static ADC1_67 adc67Fun = 0;
//================================
//�ص�����ע��
//================================
void get_adc67_init(ADC1_67 fun)
{
	adc67Fun = fun;
}
//================================
//�����ĵ������ݴ���
//================================
static uint8_t  current_dealwith(ADC1_67 callBack) 
{
	uint32_t po = (adc67Value[0]*3300*100/(4095*3) - adc67Value[1]*3300*100/(4095*3))/8;
	if(po >100 && (adc67Value[0] > adc67Value[1]))
		po=po;
	if(callBack == 0)
		return FALSE;
	if(adc67Value[0] < adc67Value[1])//δ��װ����������������Ӧ�Ǹ������
		callBack(0);
	else
		callBack(po);
	return TRUE;
}

//================================
//adc��ʼ��
//˵����ADCת���ٶȺ�������Ҫ���� 1us
//��ʼ��ADC1 ͨ��6��7,ʹ�ù�����
//��ʼ��ADC1 ͨ��4��5��ʹ��ע����
//ʹ��DMA����
//================================
void adc1_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	//GPIO����
	ADC_InitTypeDef ADC_InitStructure;		//ADC ����
	DMA_InitTypeDef DMA_InitStructure;		//DMA ����
	NVIC_InitTypeDef NVIC_InitStructure;	//�ж� ����

	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_ADC1 , ENABLE );//ʹ��GPIOA ʱ���븴��ʱ�ӣ�ʹ��ADC ʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//DMAʱ�Ӵ�
	//RCC_ADCCLKConfig(RCC_PCLK2_Div4);	   				//ADCʱ��Ϊ PCLK2��4��Ƶ 56/4��pclk2���� apb2��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);		//����Ϊ12M ��adcʱ����<=14MHz
	
	//gpio ��������
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;//ADC1ͨ�� PA4,PA5,PA6,PA7  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	                    //ģ������ģʽ
	GPIO_Init(GPIOA, &GPIO_InitStructure);								//��ʼ������
	
	//DMA1 ͨ����ʼ��
	DMA_DeInit(DMA1_Channel1);		//�ָ��Ĵ���Ĭ��ֵ
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;		//�����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&adc67Value;	//�ڴ��ַ��Ҫ�������ݵĵ�ַ��
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;				//DMAת��ģʽSCRģʽ����������˵��ڴ�
	DMA_InitStructure.DMA_BufferSize = 2;							//DMA�����С(DMA����ʱ����������)
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//����һ�����ݺ�DMA��������ַ��ֹ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;			//�ڴ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//DMA �������ݳ���
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			//�ڴ����ݳ���
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;					//ѭ��ģʽ������bufд�����Զ��ص���ʼ��ַ����
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;				//���ȼ���
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;					//�ڴ浽�ڴ洫��ر�
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC ,ENABLE);					//���� DMA ��������ж�
	
	//�ж����� DMA1
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;		//DMA1 ͨ��1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//ռ�����ȼ� 0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		//�����ȼ� 0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	DMA_Cmd(DMA1_Channel1, ENABLE);                                	//ʹ��DMA ͨ��1

	//ADC1 ��ʼ��
	ADC_DeInit(ADC1);  //ADC1 �Ĵ����ָ�Ĭ��ֵ
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	                //��������ģʽ,��������ADCX��������
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;					    //������ͨ��ɨ�裬��ͨ��
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;					//����ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//�����������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	            //ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 2;	                            //���й���ת����ADCͨ����2��
	ADC_Init(ADC1, &ADC_InitStructure);

	//���ù���ͨ�� ADC1ʹ��6��7ת��ͨ����ת��˳��1��2
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6 , 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7 , 2, ADC_SampleTime_239Cycles5);
	
	//����ע����ͨ��
	ADC_InjectedSequencerLengthConfig(ADC1, 2);//ע����ͨ���� ����Ҫ������
    ADC_InjectedChannelConfig(ADC1,ADC_Channel_4,1,ADC_SampleTime_239Cycles5);
	ADC_InjectedChannelConfig(ADC1,ADC_Channel_5,2,ADC_SampleTime_239Cycles5);
    ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_None);//�����ⲿ�������߶�ʱ�������ȣ�ʹ���������
	//ADC_AutoInjectedConvCmd (ADC1,ENABLE);	//������ת������Զ���ʼע����ת��
	//ADC_SoftwareStartInjectedConvCmd(ADC1, ENABLE);//��ʼע��ͨ��ת��
	
	
	
	ADC_DMACmd(ADC1, ENABLE);                                           //ʹ�� ADC1 DMA
	
	/* Enable ADC1 external trigger */
    ADC_ExternalTrigConvCmd(ADC1, DISABLE);
    ADC_ExternalTrigInjectedConvCmd(ADC1, DISABLE);

	ADC_Cmd(ADC1, ENABLE);	                                            //ʹ��ADC1
	ADC_ResetCalibration(ADC1);	                                        //ʹ��ADC1��λУ׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1));	                        //�ȴ���λУ׼�Ĵ�������
	ADC_StartCalibration(ADC1);			                                //����ADC1У׼
	while(ADC_GetCalibrationStatus(ADC1));	                            //�ȴ�ADC1У׼����
	//ADC_SoftwareStartConvCmd(ADC1, ENABLE);		                        //�������ת��
}

//============================
//DMA�ж�
//ʹ�� DMA1 �����ͨ��1��channel1��
//DMA1 ��Ӧ ADC1 �Ĺ�����
//============================
void DMA1_Channel1_IRQHandler(void)
{          
	if(DMA_GetITStatus(DMA1_IT_TC1) == SET)    
	{
		DMA_ClearITPendingBit(DMA1_IT_TC1); //����жϱ�־ 
		//�ص�
		current_dealwith(adc67Fun);
	}
	else
    if(DMA_GetITStatus(DMA1_IT_HT1) == SET)
    {
        DMA_ClearITPendingBit(DMA1_IT_HT1);	//��������ж�
        DMA_ClearITPendingBit(DMA1_IT_GL1); //ȫ���ж�                               
    }

}
//================================
//adc1 ͨ��67�������鿪ʼ���� 
//================================
void adc1_67_start(void)
{
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
//================================
//adc1 ͨ��67������Ƿ�������
//================================
ITStatus adc1_67_state(void)
{
	if(adcDmaFlag == SET)//���DMA ��ɽ���
	{
		adcDmaFlag = RESET;
		//��������
		return SET;
	}
	else 
	{
		return RESET;
	}
}
//================================
//adc1 ͨ��67����ȡadcͨ������ֵ
//������adc6 ͨ��6����ָ�룬bsp���ݹ���
//adc7 ͨ��7����ָ�룬bsp��ȡ���������ֵ
//================================
void get_adc67_value(uint16_t *adc6,uint16_t *adc7)
{
	*adc6 = adc67Value[0];
	*adc7 = adc67Value[1];
}




//================================
//���adc�ɼ�
//˵�����������˲����ɼ���5�Σ�ȡƽ��ֵ��
//����ֵ��������� mA
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
//		//�ȴ���������
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
//����ֵ��adc1 ͨ��4��5 ֵ
//================================
void  get_adc45_value(uint16_t *adc1_4,uint16_t *adc1_5)
{
	ADC_SoftwareStartInjectedConvCmd(ADC1,ENABLE);
	//�ȴ�ע����ת�����
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_JEOC ));
//	rs1 = ADC_GetFlagStatus(ADC1,ADC_FLAG_JEOC);
//	rs1 = ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC);
	ADC_ClearFlag(ADC1, ADC_FLAG_JEOC);
	*adc1_4 = ADC_GetInjectedConversionValue(ADC1,ADC_InjectedChannel_1 );
	*adc1_5 = ADC_GetInjectedConversionValue(ADC1,ADC_InjectedChannel_2 );
}

