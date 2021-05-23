//====================================
//�ӿ� 485
//�����ذ�ͨ��
//====================================
#ifndef __COM_TO_MASTER__
#define __COM_TO_MASTER__

#include "stdint.h"

#define MASTER_COMBAUD		9600		//������ͨѶ������
//====================================
//��ͷ��β
//====================================
#define M_STX		'$'	//�����˰�ͷ
#define S_STX		'@'	//�������ͷ
#define ETX			'!'	//��β
//====================================







typedef uint8_t (*MASTER_RECV_DEALWITH)(volatile  uint8_t*, volatile uint16_t);//����һ���ص���������

void usart1_config(void);
void uart1_dmaRx_config(void);
void uart1_dmaTx_config(void);

void recv_fun_init(MASTER_RECV_DEALWITH fun);
void usart1_send(uint8_t *dat, uint16_t size)  ;

#endif
