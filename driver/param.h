///*========================================
//��������ͷ�ļ�
//����ռ��flash�ؼ����һҳ
//����������flash��128k��������ַ 0x0801FC00��ÿҳ1k

//��1���� ��־λ 
//��2~3����	
//��4~5����
//��6~7����
//��8~9����

//*========================================*/
//#include "stm32f10x_flash.h"
//#include "stdint.h"

//#define PARAM_FLAG			0xBAAA			//������־λ
//#define PARAM_ADDR_BASE		0x0801FC00		//�����������ַ

///*
//ÿ����������������
//ÿ������ 2 Byte
//*/
//#define PARAM_ADDR_DRIVERBOARD_TYPE			(0x0801FC00 + 4)
//#define PARAM_ADDR_TEMPERATURE_TYPE			(0x0801FC00 + 8)
//#define PARAM_ADDR_CURRENT_START			(0x0801FC00 + 12)
//#define PARAM_ADDR_CURRENT_SHORT			(0x0801FC00 + 16)
//#define PARAM_ADDR_CURRENT_OVERLOAD			(0x0801FC00 + 20)
//#define PARAM_ADDR_MOTOR_OVERLOAD_TIMEOUT	(0x0801FC00 + 24)
//#define PARAM_ADDR_MOTOR_START_TIMEOUT		(0x0801FC00 + 28)
//#define PARAM_ADDR_MOTOR_STOP_TIMEOUT		(0x0801FC00 + 32)
//#define PARAM_ADDR_MOTOR_STOP_DELAY_TIMEOUT	(0x0801FC00 + 36)
//#define PARAM_ADDR_SIGNAL_SALE_COUNT		(0x0801FC00 + 40)
//#define PARAM_ADDR_SIGNAL_SALE_INTERVAL		(0x0801FC00 + 44)
//#define PARAM_ADDR_SIGNAL_CHECK_COUNT		(0x0801FC00 + 48)
//#define PARAM_ADDR_SIGNAL_CHECK_INTERVAL	(0x0801FC00 + 52)


//void init_flash(void);
//uint8_t reset_flash(void);
//uint8_t set_flash(void);

