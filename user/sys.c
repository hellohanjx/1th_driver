//========================================
//ȫ����أ�ϵͳ��
//========================================
#include "user_config.h"
#include "stdint.h"
#include "sys.h"

//========================================
//ȫ�ֱ���
//========================================
static volatile uint8_t address;			//�������ַ
volatile DRV_STATE drvInfo ;		//�ۻ���״̬

//========================================
//��װ����
//˵�����ڴ�������������ݣ�Ϊ�������ṩһ��״̬��ȡ�Ľӿ�
//�������������㣬bsp���ṩ
//��ȡ����ҵ����ṩ
//========================================

//================================================================================
//ִ����
//================================================================================

//���õ�ַ
void sys_set_addr(uint8_t rs){
	drvInfo.addr = rs;
}
//��ȡ��ַ
uint8_t  sys_get_addr(void){
	return drvInfo.addr;
}

//================================================================================
//�����װ����
//================================================================================
//������������
void msg_set_class(uint8_t rs){
	drvInfo.cmd.cclass  = rs;
}
//��ȡ��������
uint8_t  msg_get_class(void){
	return drvInfo.cmd.cclass;
}

//========================================
//��������������
void msg_set_type(uint8_t rs){
	drvInfo.cmd.type = rs;
}
//��ȡ����������
uint8_t  msg_get_type(void){
	return drvInfo.cmd.type;
}


//========================================
//����״̬
void msg_set_state(uint8_t rs){
	drvInfo.cmd.state = rs;
	
}
//��ȡ״̬
uint8_t  msg_get_state(void){
	return drvInfo.cmd.state;
}


//========================================
//���ý��
void msg_set_result(uint8_t rs){
	drvInfo.cmd.result = rs;
}
//��ȡ���
uint8_t  msg_get_result(void){
	return drvInfo.cmd.result;
}

//========================================
void msg_set_cur(uint8_t rs){
	drvInfo.cmd.cur = rs;
}
uint8_t msg_get_cur(void){
	return drvInfo.cmd.cur ;
}


//================================================================================
//�¶���
//================================================================================
//�����¶�ֵ
void temp_set_value(uint8_t id ,uint8_t rs){
	drvInfo.temperature.value[id] = rs;
}
//��ȡ�¶�ֵ
int  temp_get_value(uint8_t id){
	return drvInfo.temperature.value[id];
}

//========================================
//�����¶�̽ͷ״̬
void temp_set_state(uint8_t id,uint8_t rs){
	if(id < 2)
	drvInfo.temperature.state[id] = rs;
}
//��ȡ�¶�̽ͷ״̬
uint8_t  temp_get_state(uint8_t id){ 
	if(id < 2)
	return 	drvInfo.temperature.state[id];
	return FALSE; 
} 
//========================================
//�����¶�̽ͷid
void temp_set_id(uint8_t id){
	drvInfo.temperature.id = id;
}
//��ȡ�¶�̽ͷid
uint8_t  temp_get_id(void){ 
	return 	drvInfo.temperature.id;
}


//================================================================================
//�����
//================================================================================
//���õ��״̬
void mo_set_state(uint8_t id,uint8_t rs){
	drvInfo.motor.state[id]  = rs;
}
//��ȡ���״̬
uint8_t  get_motor_state(uint8_t id){
	return drvInfo.motor.state[id];
}

//========================================
//���õ�ǰ�������ĵ����
void mo_set_num(uint8_t rs){
	drvInfo.motor.curNum  = rs;
}
//��ȡ��ǰ�������ĵ����
uint8_t  mo_get_num(void){
	return drvInfo.motor.curNum;
}

//========================================
//���õ�ǰ����������ı��
void mo_set_id(uint8_t id,uint8_t rs){
	drvInfo.motor.curID[id]  = rs;
}
//��ȡ��ǰ����������ı��
uint8_t *sys_get_motorid(void){
	return  (uint8_t *)&(drvInfo.motor.curID[0]);
}


//================================================================================
//�����·��
//================================================================================
//���������·״̬
void ex_set_state(uint8_t id,uint8_t rs){
	drvInfo.exChannel.state[id]  = rs;
}
//��ȡ��������
uint8_t  ex_get_state(uint8_t id){
	return drvInfo.exChannel.state[id];
}

uint8_t  ex_get_id(void){
	return drvInfo.exChannel.id ;
}
void ex_set_id(uint8_t id){
	 drvInfo.exChannel.id = id ;
}

