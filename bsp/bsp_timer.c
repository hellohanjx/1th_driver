//========================================
//�����ʱ��3��4���õ�ʱ���ǹرյģ����������cpu�ٶ�
//rtc ʱ��û�����жϣ�Ҳ��Ϊ�˼ӿ�cpu�ٶ�

//========================================

#include "rtc.h"
#include "timer2.h"
#include "timer3.h"
#include "timer4.h"
#include "stdint.h"
#include "bsp_timer.h"

void bsp_timer_init(void)//systickʱ����led����������������Է���led��ʼ����
{
	rtc_config();			//RTC ʱ������
	timer2_config();
	timer3_config();
	timer4_config();
}

//========================================
//��ȡ��ǰrtcʱ����ģ���λ (s)
//========================================
uint32_t bsp_rtcTime_get(void)
{
	return get_rtc_count();
}



//========================================
//����/ֹͣ ��ϸ��ʱ�� 10us��׼
//========================================
void bsp_midTimer_start(uint16_t value)
{
	timer3_start(value);
}
void bsp_midTimer_stop(void)
{
	timer3_stop();
}
//��ȡ��ʱ��ʱ�ӽ���
uint32_t bsp_midTimer_ticks(void)
{
	return get_timer3_ticks();
}



//========================================
//����/ֹͣ ���Լ�ʱ�� 500us ��׼
//========================================
void bsp_roughTimer_start(uint16_t value)
{
	timer4_start(value);
}
void bsp_roughTimer_stop(void)
{
	timer4_stop();
}
//��ȡ��ʱ��ʱ�ӽ���
uint32_t bsp_roughTimer_ticks(void)
{
	return get_timer4_ticks();
}


