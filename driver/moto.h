#ifndef __MOTO_H__
#define __MOTO_H__

#include "stdint.h"

#define SAMPLING_RES	0.03	//采样电阻阻值
#define SAMPLING_ANT	5		//采样次数


void motor_line_config(void);
void motor_row_config(void);
void motor_signal_config(void);
void line_start(uint8_t id);
void row_start(uint8_t id , uint8_t sig);
void line_stop(void);
void row_stop(void);

uint8_t check_position(uint8_t id);

#endif
