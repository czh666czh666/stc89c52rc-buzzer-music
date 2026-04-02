#include <STC89C5xRC.H>

/**
  * @brief  定时器1初始化，1毫秒@11.0592MHz
  * @param  无
  * @retval  无
  */
  
void Timer1Init(void)
{
    AUXR &= 0xBF;   //定时器时钟12T模式
    TMOD &= 0x0F;
    TMOD |= 0x10;   //定时器1模式1

    TL1 = 0x66;
    TH1 = 0xFC;

    TF1 = 0;  //清除定时器1溢出标志位
    ET1 = 1;
    TR1 = 1;  //定时器1开始计时
    EA  = 1;
}