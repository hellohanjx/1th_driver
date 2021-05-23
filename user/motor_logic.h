#ifndef __MOTOR_LOGIC__
#define __MOTOR_LOGIC__

#include "stdint.h"


//电机内部编号
#define MOTOR_11		0
#define MOTOR_12		1
#define MOTOR_13		2
#define MOTOR_14		3
#define MOTOR_15		4
#define MOTOR_16		5
#define MOTOR_17		6
#define MOTOR_18		7
#define MOTOR_19		8
#define MOTOR_10		9

#define MOTOR_21		10
#define MOTOR_22		11
#define MOTOR_23		12
#define MOTOR_24		13
#define MOTOR_25		14
#define MOTOR_26		15
#define MOTOR_27		16
#define MOTOR_28		17
#define MOTOR_29		18
#define MOTOR_20		19

#define MOTOR_31		20
#define MOTOR_32		21
#define MOTOR_33		22
#define MOTOR_34		23
#define MOTOR_35		24
#define MOTOR_36		25
#define MOTOR_37		26
#define MOTOR_38		27
#define MOTOR_39		28
#define MOTOR_30		29

#define MOTOR_41		30
#define MOTOR_42		31
#define MOTOR_43		32
#define MOTOR_44		33
#define MOTOR_45		34
#define MOTOR_46		35
#define MOTOR_47		36
#define MOTOR_48		37
#define MOTOR_49		38
#define MOTOR_40		39

#define MOTOR_51		40
#define MOTOR_52		41
#define MOTOR_53		42
#define MOTOR_54		43
#define MOTOR_55		44
#define MOTOR_56		45
#define MOTOR_57		46
#define MOTOR_58		47
#define MOTOR_59		48
#define MOTOR_50		49

#define MOTOR_61		50
#define MOTOR_62		51
#define MOTOR_63		52
#define MOTOR_64		53
#define MOTOR_65		54
#define MOTOR_66		55
#define MOTOR_67		56
#define MOTOR_68		57
#define MOTOR_69		58
#define MOTOR_60		59

#define MOTOR_71		60
#define MOTOR_72		61
#define MOTOR_73		62
#define MOTOR_74		63
#define MOTOR_75		64
#define MOTOR_76		65
#define MOTOR_77		66
#define MOTOR_78		67
#define MOTOR_79		68
#define MOTOR_70		69

#define MOTOR_81		70
#define MOTOR_82		71
#define MOTOR_83		72
#define MOTOR_84		73
#define MOTOR_85		74
#define MOTOR_86		75
#define MOTOR_87		76
#define MOTOR_88		77
#define MOTOR_89		78
#define MOTOR_80		79

uint8_t  logic_motor_check(uint8_t num,uint8_t *motorID);
uint8_t logic_motor_checkAll(void);
uint8_t  logic_motor_sale(uint8_t num,uint8_t *motorID);
uint8_t  logic_motor_reset(uint8_t num,uint8_t *motorID);
uint8_t logic_motor_resetAll(void );
uint8_t logic_ex_open(uint8_t id);
uint8_t logic_ex_close(uint8_t id);
uint8_t logic_ex_reset(void);
uint8_t logic_ex_check(uint8_t id);

#endif
