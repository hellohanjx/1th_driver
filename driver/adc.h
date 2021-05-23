#ifndef __ADC_H__
#define __ADC_H__

#include "stdint.h"
#include "stm32f10x.h"

#define ADC1_DR_Address    ((u32)0x4001244C)	//adc寄存器地址

typedef void (*ADC1_67)(uint16_t);//定义一个回调函数类型

typedef struct ADC_67{
	uint16_t adcChannel6;
	uint16_t adcChannel7;
}ADC_67;

void adc1_config(void);
void adc1_67_start(void);
ITStatus adc1_67_state(void);
void get_adc67_value(uint16_t *adc6,uint16_t *adc7);
void get_adc45_value(uint16_t *adc1_4,uint16_t *adc1_5);

void get_adc67_init(ADC1_67 fun);//回调函数注册

#endif
