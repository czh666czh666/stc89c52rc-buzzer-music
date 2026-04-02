#include <STC89C5xRC.H>

/**
  * @brief  定时器0初始化，1毫秒@11.0592MHz
  * @param  无
  * @retval  无
  */

void Timer0Init()
{
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x66;		//设置定时初值 64614  11.0592MHz     64535  12MHz
	TH0 = 0xFC;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
    ET0=1;
    EA=1;
    PT0=0;
}