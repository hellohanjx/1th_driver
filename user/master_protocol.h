#ifndef __MASTER_PROTOCOL__
#define __MASTER_PROTOCOL__

#include "stdint.h"
#include "sys.h"



void cmd_dealwith(volatile uint8_t *data,volatile uint16_t len);
void send_motor_msg(void);
void send_motorAll_msg(void);
void send_temp_msg(void);
void send_output_msg(void);
void send_outputAll_msg(void);
void send_version_msg(void);
void send_paramSet_msg(void);
void send_paramGet_msg(void);
void send_paramReset_msg(void);

#define RESTART() ((void (*)())(0x0)) ()	//÷ÿ∆Ùcpu

#endif
