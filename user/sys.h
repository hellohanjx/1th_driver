#ifndef __SYS_H__
#define __SYS_H__

#include "stdint.h"
#include "user_config.h"

typedef void (*MSG)( );//����һ���ص���������

//======================
//�������ݻص�����
//˵���������������������
//======================
#define MASTER_CLASS	1	//������
#define DROP_CLASS		2	//���������


//====================================
//������������
//====================================
#define MOTOR_CMD_CHECK		1	
#define MOTOR_CMD_SALE		2
#define MOTOR_CMD_RESET 	3


#define TEMP_CMD_CHECK		6	//˫�ֽ�ָ��
#define OUTPUT_CMD_CHECK	7
#define	OUTPUT_CMD_SET		8
#define OUTPUT_CMD_RESET	9

//���ֽ�ָ��
#define MOTOR_CMD_CHECKALL	4	//���е����ѯ
#define MOTOR_CMD_RESETALL	5	//���е����λ
#define VERSION_CMD_CHECK	11	//�汾��ѯ
#define EXECUTE_CMD_STATE	222	//��ѯָ��
#define OUTPUT_CMD_RESETALL	10



#define DRIVERBOARD_RESET	15	//�����帴λ
#define DROP_CMD_CHECK		16	//�������
//====================================
//���״̬
//====================================
#define MOTOR_STATE_NORMAL		1	//����
#define MOTOR_STATE_OUT			2	//����λ
#define MOTOR_STATE_OVERLOAD	3	//����
#define MOTOR_STATE_NOSTART		4	//��ʱδ����
#define MOTOR_STATE_NOSTOP		5	//��ʱδֹͣ
#define MOTOR_STATE_NOINSTALL	6	//δ��װ���
#define MOTOR_STATE_SHORT		7	//�����·
#define MOTOR_STATE_UNKNOWN		8	//���λ��δ֪

//====================================
//�¶�̽ͷ״̬
//====================================
#define TEMP_STATE_NORMAL		1
#define TEMP_STATE_NOINSTALL	2
#define TEMP_STATE_ERR			3
//====================================
//����ִ�н��
//====================================
#define DONE_RESULT_COMPLETE	1	//ִ�гɹ�
#define DONE_RESULT_FAIL		2	//ִ��ʧ��
#define DONE_RESULT_BUSY		3	//ִ��æ
#define	DONE_RESULT_CHKERR		4	//У��ʹ���
#define DONE_RESULT_LENERR		5	//���Ȳ���
#define DONE_RESULT_NOSUPPORT	6	//��֧�ֵ�ָ��
#define DONE_RESULT_FORMATEERR	7	//��ͷ��β��
#define DONE_RESULT_STOPLESS	8	//����ʱС����С�˶�ʱ��
#define DONE_RESULT_NOSTART		9	//��ʱδ����
#define DONE_RESULT_NOSTOP		10//��ʱδֹͣ
#define DONE_RESULT_SHORT			11//��·
#define DONE_RESULT_OVER			12//����

//====================================
//����ִ��״̬
//====================================
#define CMD_STATE_IDLE		0	//����
#define CMD_STATE_BUSY		1	//����ִ��
#define CMD_STATE_DONE		2	//ִ�����
#define CMD_STATE_NEED		3	//��Ҫִ��


//=================================
//��������
//=================================
typedef struct {
	 uint8_t cclass;		//��Ϣ����,(�����豸��������������⣬�Լ�����)
	 uint8_t type;		//��Ϣ������
	 uint8_t state;		//ִ��״̬��ÿ��ֻ����ִ��1��ָ�ע�⣺ִ��״̬���ǿ��е�ʱ�������������������������
	 uint8_t result;	//����ִ�н����������������������		
	 uint8_t cur;		//��ǰ����ִ�е�ָ��
}CMD_CLASS;
//=================================
//�¶�����
//=================================
typedef struct 
{
	 uint8_t id;					//�������¶Ȼ�·id
	 uint8_t state[TEMP_SIZE]; 		//�¶Ȼ�·״̬
	 int8_t  value[TEMP_SIZE]; 		//�¶�ֵ
}TEMP_CLASS;

//=================================
//�������
//=================================
typedef struct 
{
	 uint8_t curNum;					//��ǰҪ�����ĵ������
	 uint8_t curID[MOTOR_TOGETHER];	//��ǰҪ�����ĵ�����
	 uint8_t state[MOTOR_SIZE]; 		//���״̬
}MOTOR_CLASS;
//=================================
//�������
//=================================
typedef struct 
{
	 uint8_t id; 					//�����������·id
	 uint8_t state[OUTPUT_SIZE]; 	//�����·״̬
}EXPORT_CLASS;

//=================================
//˵������������ṹ��������ʾ������
//״̬���ۻ������������
//=================================
typedef struct 
{
	volatile uint8_t addr;				//�������ַ
	volatile CMD_CLASS cmd;
	volatile MOTOR_CLASS motor;			//���
	volatile TEMP_CLASS temperature;	//�¶�
	volatile EXPORT_CLASS exChannel;	//���
//	volatile uint16_t param[G_PARAM_SIZE];	//����
}DRV_STATE; 


void sys_set_addr(uint8_t rs);
uint8_t  sys_get_addr(void);
void msg_set_class(uint8_t rs);
uint8_t  msg_get_class(void);
void msg_set_type(uint8_t rs);
uint8_t  msg_get_type(void);
void msg_set_state(uint8_t rs);
uint8_t  msg_get_state(void);
void msg_set_result(uint8_t rs);
uint8_t  msg_get_result(void);
void temp_set_value(uint8_t id ,uint8_t rs);
int  temp_get_value(uint8_t id);//ע��������з�����
void temp_set_state(uint8_t id,uint8_t rs);
uint8_t  temp_get_state(uint8_t id);
void mo_set_state(uint8_t id,uint8_t rs);
uint8_t  get_motor_state(uint8_t id);
void mo_set_num(uint8_t rs);
uint8_t  mo_get_num(void);
void mo_set_id(uint8_t id,uint8_t rs);

//uint8_t  sys_get_motorid(uint8_t id);

void ex_set_state(uint8_t id,uint8_t rs);
uint8_t  ex_get_state(uint8_t id);
 uint8_t *sys_get_motorid(void);
					
uint8_t  ex_get_id(void);
void ex_set_id(uint8_t id);
uint8_t  sys_get_channeState(void);
void sys_set_channeState(uint8_t rs);

void sys_set_cmd(uint8_t rs);
uint8_t  sys_get_cmd(void);

void msg_set_cur(uint8_t rs);
uint8_t msg_get_cur(void);

void temp_set_id(uint8_t id);
uint8_t  temp_get_id(void);

void param_set(volatile uint8_t *param);
volatile uint16_t *param_get(void);

#endif

