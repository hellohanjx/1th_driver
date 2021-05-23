//======================
//������ͨѶ bsp
//��ҵ���ṩ�ӿ�
//======================
#include "stdint.h"
#include "uart1.h"
#include "bsp_master.h"
#include "sys.h" 
#include "master_protocol.h"
#include "led.h"
#include "timer2.h"

//====================================
//���ʹ�
//====================================
void bsp_master_sendString(uint8_t *data,uint16_t len)
{
	usart1_send(data,  len)  ;

}
//====================================
//���͵��ֽ�
//====================================
 void bsp_master_sendByte(uint8_t data)
{
//	usart1_send(data);
}

//====================================
//bsp ���ջص�����
//====================================
uint8_t bsp_master_recvCallBack(volatile  uint8_t *data, volatile uint16_t len) 
{
	LED_RECV_ON;
	timer2_start(1);
	cmd_dealwith(data,len);
	return 1;
}

//====================================
//��ʱ��2�ص�����
//====================================
void bsp_led_recvCallBack(void)
{
	LED_RECV_OFF ;
}
//======================
//������ͨ�ų�ʼ��
//======================
void bsp_master_init(void)
{
	usart1_config();	//uart1 ����
	uart1_dmaRx_config();	//uart1 DMA����
	uart1_dmaTx_config();	//uart1 DMA����
	recv_fun_init(bsp_master_recvCallBack);
	timer2_callbackFun_init(bsp_led_recvCallBack);
}

