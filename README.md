# stc89c52rc-buzzer-music
Keil5单片机代码：基于STC89C52RC的51单片机蜂鸣器音乐播放

## 开发环境
- Keil5
- C语言
- 单片机：STC89C52RC

## 实现功能
1. 完整播放一段音乐
2. 以数码管显示播放进度
3. 独立按键控制音乐的暂停和继续
4. 独立按键控制音乐的播放倍速

## 工程文件
本工程文件位于：
第一周作业1 蜂鸣器播放音乐

主要文件包括：
- `main.c`：主程序
- `Music.c / Music.h`：音乐播放相关程序
- `Timer0.c / Timer0.h`：定时器0相关程序
- `Timer1.c / Timer1.h`：定时器1相关程序
- `Delay.c / Delay.h`：延时函数
- `Key.c / Key.h`：按键模块
- `Nixie.c / Nixie.h`：数码管模块

## 运行方法
1. 使用 STC-ISP 打开 `Project.hex` 文件
2. 下载到 STC89C52RC 开发板
3. 上电后蜂鸣器自动播放设定音乐
