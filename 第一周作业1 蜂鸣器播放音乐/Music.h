#ifndef __MUSIC_H__
#define __MUSIC_H__

//蜂鸣器端口定义
sbit Buzzer=P2^5;

//播放速度，值为四分音符的时长(ms)
#define SPEED  500

//音符与索引对应表，P：休止符，L：低音，M：中音，H：高音，下划线：升半音符号#
#define P	0
#define L1	1
#define L1_	2
#define L2	3
#define L2_	4
#define L3	5
#define L4	6
#define L4_	7
#define L5	8
#define L5_	9
#define L6	10
#define L6_	11
#define L7	12
#define M1	13
#define M1_	14
#define M2	15
#define M2_	16
#define M3	17
#define M4	18
#define M4_	19
#define M5	20
#define M5_	21
#define M6	22
#define M6_	23
#define M7	24
#define H1	25
#define H1_	26
#define H2	27
#define H2_	28
#define H3	29
#define H4	30
#define H4_	31
#define H5	32
#define H5_	33
#define H6	34
#define H6_	35
#define H7	36

//晶振频率11.0592Mhz，重装载值 = 65536 - 460800 / f
unsigned int code FreqTable[]={
    0,  //休止符
    63778, 63873, 63969, 64054, 64140, 64216,64290, 64361, 64426, 64489, 64547, 64603,// 低音区
    64655, 64705, 64751, 64795, 64836, 64887,64913, 64949, 64981, 65012, 65041, 65071,// 中音区
    65104, 65120, 65144, 65165, 65185, 65205,65224, 65242, 65259, 65275, 65289, 65303// 高音区
};

//乐谱
unsigned char code Music[]={
	//音符,时值,
	
	//1
	P,	4,
	P,	4,
	P,	4,
	M6,	2,
	M7,	2,
	
	H1,	4+2,
	M7,	2,
	H1,	4,
	H3,	4,
	
	M7,	4+4+4,
	M3,	2,
	M3,	2,
	
	//2
	M6,	4+2,
	M5,	2,
	M6, 4,
	H1,	4,
	
	M5,	4+4+4,
	M3,	4,
	
	M4,	4+2,
	M3,	2,
	M4,	4,
	H1,	4,
	
	//3
	M3,	4+4,
	P,	2,
	H1,	2,
	H1,	2,
	H1,	2,
	
	M7,	4+2,
	M4_,2,
	M4_,4,
	M7,	4,
	
	M7,	8,
	P,	4,
	M6,	2,
	M7,	2,
	
	//4
	H1,	4+2,
	M7,	2,
	H1,	4,
	H3,	4,
	
	M7,	4+4+4,
	M3,	2,
	M3,	2,
	
	M6,	4+2,
	M5,	2,
	M6, 4,
	H1,	4,
	
	//5
	M5,	4+4+4,
	M2,	2,
	M3,	2,
	
	M4,	4,
	H1,	2,
	M7,	2+2,
	H1,	2+4,
	
	H2,	2,
	H2,	2,
	H3,	2,
	H1,	2+4+4,
	
	//6
	H1,	2,
	M7,	2,
	M6,	2,
	M6,	2,
	M7,	4,
	M5_,4,
	
	
	M6,	4+4+4,
	H1,	2,
	H2,	2,
	
	H3,	4+2,
	H2,	2,
	H3,	4,
	H5,	4,
	
	//7
	H2,	4+4+4,
	M5,	2,
	M5,	2,
	
	H1,	4+2,
	M7,	2,
	H1,	4,
	H3,	4,
	
	H3,	4+4+4+4,
	
	//8
	M6,	2,
	M7,	2,
	H1,	4,
	M7,	4,
	H2,	2,
	H2,	2,
	
	H1,	4+2,
	M5,	2+4+4,
	
	H4,	4,
	H3,	4,
	H3,	4,
	H1,	4,
	
	//9
	H3,	4+4+4,
	H3,	4,
	
	H6,	4+4,
	H5,	4,
	H5,	4,
	
	H3,	2,
	H2,	2,
	H1,	4+4,
	P,	2,
	H1,	2,
	
	//10
	H2,	4,
	H1,	2,
	H2,	2,
	H2,	4,
	H5,	4,
	
	H3,	4+4+4,
	H3,	4,
	
	H6,	4+4,
	H5,	4+4,
	
	//11
	H3,	2,
	H2,	2,
	H1,	4+4,
	P,	2,
	H1,	2,
	
	H2,	4,
	H1,	2,
	H2,	2+4,
	M7,	4,
	
	M6,	4+4+4,
	P,	4,
	
	0xFF	//终止标志
};

#endif