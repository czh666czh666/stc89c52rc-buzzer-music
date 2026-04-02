#include <STC89C5xRC.H>
#include "Delay.h"
#include "Timer0.h"
#include "Timer1.h"
#include "Music.h"
#include "Nixie.h"

sbit Key1=P3^1;  //按键1：暂停/继续
sbit Key2=P3^0;  //按键2：倍速切换

#define SPEED           500           //四分音符500ms
#define NOTE_VALUE_MS   (SPEED/4)     //音符时值对应的毫秒，即1时值对应四分之一的SPEED
#define NOTE_GAP_MS     5             //每个音符结束后进行5ms的小停顿，避免音符连在一起发声

//播放器的四种状态
#define PLAYER_LOAD     0      //空闲状态或者准备加载新音符
#define PLAYER_NOTE     1      //当前正在播放一个音符
#define PLAYER_GAP      2      //音符已经结束，正在经历音符间隔5ms
#define PLAYER_END      3      //整首歌已播放完

//在中断和主程序间共享的全局变量加volatile，每次使用都必须从内存重新读取
volatile unsigned char FreqSelect=0;     //当前正在播放的音符编号
unsigned char MusicSelect=0;             //当前读到乐谱数组的哪个位置，每加载一个音符，MusicSelect会加2

unsigned char PlayerMode=PLAYER_LOAD;    //表示现在播放器在哪个状态0,1,2,3
bit PauseFlag=0;                         //暂停标志，0：正常播放 1：暂停

unsigned int NoteRemainMs=0;             //当前音符还剩多少毫秒播放
unsigned int GapRemainMs=0;              //当前音符结束后的短暂停顿还剩多少毫秒

unsigned long PlayedTime=0;              //已经播放过的总时间
unsigned long TotalTime=1;               //整首歌的总时长

unsigned char SpeedIndex=1;                         //倍速档位索引
unsigned char SpeedLevelTable[]={1, 2, 4, 8};       //除2得到倍速0.5 1 2 4
unsigned char SpeedStep=2;                          //当前档位对应的实际步进值
unsigned char SpeedCounter=0;                       //速度计数，判断经过了多少毫秒

unsigned char Display[8]={0,0,0,10,10,10,10,2};     //数码管显示

volatile unsigned char Timer1_1msCount=0;           //Timer1已经进行了多少次1ms的计数

//函数声明
void DisplayScan(void);                               //动态扫描数码管
void DisplayUpdate(void);                             //显示内容更新
void KeyScan(void);                                   //按键扫描
void PlayerLoadNote(void);                            //装载音符
void Player1ms(void);                                 //播放器推进1ms
void PlayerPause(void);                               //暂停或继续
void PlayerChangeSpeed(void);                         //倍速切换
unsigned long GetMusicTotalTime(void);                //计算总时长


void main()
{
    TotalTime=GetMusicTotalTime();
    
    Timer0Init();
    Timer1Init();
    
    PlayerLoadNote();
    
    while(1)
    {
        while(Timer1_1msCount)
        {
            EA=0;
            Timer1_1msCount--;
            EA=1;
            
            DisplayScan();
            KeyScan();
            Player1ms();
            DisplayUpdate();
        }
    }
    
}

unsigned long GetMusicTotalTime(void)
{
    unsigned char i=0;
    unsigned long total=0;

    while(Music[i]!=0xFF)
    {
        total+=(unsigned long)NOTE_VALUE_MS * Music[i+1];
        total+= NOTE_GAP_MS;
        i+=2;  //读下一对
    }

    if(total==0) total=1;
    return total;
}


void PlayerLoadNote(void)
{
    unsigned char note;
    unsigned char timeValue;
    unsigned int reloadValue;

    if(Music[MusicSelect]==0xFF)  //判断歌曲有没有结束
    {
        PlayerMode=PLAYER_END;
        FreqSelect=0;
        TR0=0;
        PlayedTime=TotalTime;
        return;
    }

    //读取音符和时值
    note=Music[MusicSelect++];
    timeValue=Music[MusicSelect++];

    FreqSelect=note;
    NoteRemainMs=NOTE_VALUE_MS*timeValue;
    GapRemainMs=NOTE_GAP_MS;
    PlayerMode=PLAYER_NOTE;

    if((PauseFlag==0)&&(FreqSelect!= P))
    {
        reloadValue=FreqTable[FreqSelect];
        TH0=reloadValue/256;
        TL0=reloadValue%256;
        TR0=1;
    }
    else
    {
        TR0=0;
    }
}

void Player1ms(void)
{
    if(PauseFlag||PlayerMode==PLAYER_END)  //如果暂停或结束，就什么都不做
    {
        return;
    }

    SpeedCounter+=SpeedStep;  //按倍速推进时间

    while(SpeedCounter>=2)
    {
        SpeedCounter-=2;

        if(PlayerMode==PLAYER_LOAD)
        {
            PlayerLoadNote();
        }
        else if(PlayerMode==PLAYER_NOTE)  //减少剩余时间
        {
            if(NoteRemainMs>0)
            {
                NoteRemainMs--;
                if(PlayedTime<TotalTime) PlayedTime++;
            }

            if(NoteRemainMs==0)
            {
                TR0=0;
                PlayerMode=PLAYER_GAP;
            }
        }
        else if(PlayerMode==PLAYER_GAP)  //减少间隔时间
        {
            if(GapRemainMs>0)
            {
                GapRemainMs--;
                if(PlayedTime<TotalTime) PlayedTime++;
            }

            if(GapRemainMs==0)
            {
                PlayerLoadNote();
            }
        }
    }
}


void PlayerPause(void)
{
    unsigned int reloadValue;

    PauseFlag=!PauseFlag;  //翻转暂停状态

    if(PauseFlag)
    {
        TR0 = 0;
    }
    else
    {
        if((PlayerMode==PLAYER_NOTE)&&(FreqSelect!=P))
        {
            reloadValue=FreqTable[FreqSelect];
            TH0=reloadValue/256;
            TL0=reloadValue%256;
            TR0=1;
        }
    }
}


void PlayerChangeSpeed(void)
{
    SpeedIndex++;
    if(SpeedIndex>=4) SpeedIndex=0;
    SpeedStep=SpeedLevelTable[SpeedIndex];  //根据新档位，更新SpeedStep
}

void KeyScan(void)
{
    static unsigned char key5ms=0;  //5ms扫一次
    //按键消抖计数器
    static unsigned char k1_count=0;
    static unsigned char k2_count=0;
    //按键锁定标志
    static bit k1_lock=0;
    static bit k2_lock=0;

    key5ms++;
    if(key5ms < 5) return;
    key5ms=0;

    // Key1 -> 暂停/继续
    if(Key1==0)
    {
        if(k1_count<4) k1_count++;
        else
        {
            if(k1_lock==0)
            {
                k1_lock=1;
                PlayerPause();
            }
        }
    }
    else
    {
        k1_count=0;
        k1_lock=0;
    }

    // Key2 -> 倍速切换
    if(Key2==0)
    {
        if(k2_count<4) k2_count++;
        else
        {
            if(k2_lock==0)
            {
                k2_lock=1;
                PlayerChangeSpeed();
            }
        }
    }
    else
    {
        k2_count=0;
        k2_lock=0;
    }
}

void DisplayUpdate(void)
{
    unsigned char percent;

    if(PlayerMode==PLAYER_END)
    {
        percent=100;
    }
    else
    {
        percent=(unsigned char)((PlayedTime*100 + TotalTime/2)/TotalTime);  //四舍五入
        if(percent>100) percent=100;
    }

    Display[0]=percent/100;
    Display[1]=(percent/10)%10;
    Display[2]=percent%10;

    Display[3]=10;
    Display[4]=10;
    Display[5]=10;
    Display[6]=10;

    Display[7]=SpeedIndex+1;
}

void DisplayScan(void)  //每次只刷新一位数码管
{
    static unsigned char pos=0;

    P0=0x00;  //消影
    DisplaySelect(pos+1);
    P0=NixieTable[Display[pos]];

    pos++;
    if(pos>=8) pos=0;
}


//Timer0中断：蜂鸣器发声
void Timer0_Routine() interrupt 1
{
    if(FreqTable[FreqSelect])	//如果不是休止符
    {
        /*取对应频率值的重装载值到定时器*/
        TL0=FreqTable[FreqSelect]%256;
        TH0=FreqTable[FreqSelect]/256;
        Buzzer=!Buzzer;	//翻转蜂鸣器IO口
    }
	
    
}

//Timer1中断：给1ms的节拍 
void Timer1_Routine() interrupt 3
{
    TH1=0xFC;
    TL1=0x66;

    if(Timer1_1msCount<200)
    {
        Timer1_1msCount++;
    }
}