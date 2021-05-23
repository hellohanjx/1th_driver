//======================
//与主机通讯 bsp
//向业务提供接口
//======================
#include "stdint.h"
#include "uart1.h"
#include "bsp_master.h"
#include "sys.h" 
#include "master_protocol.h"
#include "led.h"
#include "timer2.h"

//====================================
//发送串
//====================================
void bsp_master_sendString(uint8_t *data,uint16_t len)
{
	usart1_send(data,  len)  ;

}
//====================================
//发送单字节
//====================================
 void bsp_master_sendByte(uint8_t data)
{
//	usart1_send(data);
}

//====================================
//bsp 接收回调函数
//====================================
uint8_t bsp_master_recvCallBack(volatile  uint8_t *data, volatile uint16_t len) 
{
	LED_RECV_ON;
	timer2_start(1);
	cmd_dealwith(data,len);
	return 1;
}

//====================================
//定时器2回调函数
//====================================
void bsp_led_recvCallBack(void)
{
	LED_RECV_OFF ;
}
//======================
//与主机通信初始化
//======================
void bsp_master_init(void)
{
	usart1_config();	//uart1 配置
	uart1_dmaRx_config();	//uart1 DMA配置
	uart1_dmaTx_config();	//uart1 DMA配置
	recv_fun_init(bsp_master_recvCallBack);
	timer2_callbackFun_init(bsp_led_recvCallBack);
}

