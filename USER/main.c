#include "stm8s_clk.h"
#include "led.h"
#include "stm8s_tim1.h"
#include  <intrinsics.h>


/****小小调度器开始**********************************************/
#define MAXTASKS 3
volatile unsigned char timers[MAXTASKS];
#define _SS static unsigned char _lc=0; switch(_lc){default: 
#define _EE ;}; _lc=0; return 255;
#define WaitX(tickets)  do {_lc=(__LINE__+((__LINE__%256)==0))%256; return tickets ;} while(0); case (__LINE__+((__LINE__%256)==0))%256: 

#define RunTask(TaskName,TaskID) do { if (timers[TaskID]==0) timers[TaskID]=TaskName(); }  while(0); 
#define RunTaskA(TaskName,TaskID) { if (timers[TaskID]==0) {timers[TaskID]=TaskName(); continue;} }   //前面的任务优先保证执行

#define CallSub(SubTaskName) do {unsigned char currdt; _lc=(__LINE__+((__LINE__%256)==0))%256; return 0; case (__LINE__+((__LINE__%256)==0))%256:  currdt=SubTaskName(); if(currdt!=255) return currdt;} while(0);
#define InitTasks() {unsigned char i; for(i=MAXTASKS;i>0 ;i--) timers[i-1]=0; }
#define UpdateTimers() {unsigned char i; for(i=MAXTASKS;i>0 ;i--){if((timers[i-1]!=0)&&(timers[i-1]!=255)) timers[i-1]--;}}

#define SEM unsigned int 
//初始化信号量
#define InitSem(sem) sem=0;
//等待信号量
#define WaitSem(sem) do{ sem=1; WaitX(0); if (sem>0) return 1;} while(0);
//等待信号量或定时器溢出， 定时器tickets 最大为0xFFFE
#define WaitSemX(sem,tickets)  do { sem=tickets+1; WaitX(0); if(sem>1){ sem--;  return 1;} } while(0);
//发送信号量
#define SendSem(sem)  do {sem=0;} while(0);

/*****小小调度器结束*******************************************************/

#include "stm8s.h"

/*****************************************************************
/函数名称：Timer1_Init
/函数描述：Timer4初始化
/入口参数：void
/返回参数：void
/备    注：
******************************************************************/
void Timer1_Init(void)
{
    TIM1_DeInit();
    TIM1_TimeBaseInit(16000, TIM1_COUNTERMODE_UP, 9, 0x00);//定时器分频后时钟16000000/16000=1000，定时时间1ms*10=10ms
    TIM1_PrescalerConfig(16000, TIM1_PSCRELOADMODE_IMMEDIATE);   
    TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);   
    TIM1_Cmd(ENABLE);
    
}    

/*******************************
task1
*******************************/
u8 task1(void)
{
    _SS
        while(1)
        {
            LED1_ON();
            WaitX(30);
            LED1_OFF();
            WaitX(30);           
        }
    _EE
}

/*******************************
task0
*******************************/
u8 task0(void)
{
    _SS
        while(1)
        {
            LED0_ON();
            WaitX(50);
            LED0_OFF();
            WaitX(50);
        }
    _EE
}


/*******************************
主程序
*******************************/
int main(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
    Timer1_Init();
    LED_Init();
    InitTasks(); //初始化任务，实际上是给timers清零
    __enable_interrupt(); 

    while(1)
    {
        RunTask(task0,0);
        RunTask(task1,1);
    }
}





