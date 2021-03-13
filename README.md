# BoxFish STM32 （YSZ分支）

#### 介绍

箱鲀鱼的下位机程序（平台为STM32F407VGT6）

#### 软件架构

- 软件基于Keil 5开发
- 软件采用UCOS III实时操作系统
- APPLICATION文件夹：操作系统的上层应用，如游动控制、数据传输等
- MIDDLEWARE文件夹：中间件，包括通讯协议、舵机控制、数学函数等
- HARDWARE文件夹：硬件驱动，包括电机驱动、传感器驱动、舵机PWM等
- README文件夹：文档
- USER文件夹：工程文件，main.cpp

