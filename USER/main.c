#include "stm8s_clk.h"
#include "led.h"
#include "stm8s_tim1.h"
#include  <intrinsics.h>


/****СС��������ʼ**********************************************/
#define MAXTASKS 3
volatile unsigned char timers[MAXTASKS];
#define _SS static unsigned char _lc=0; switch(_lc){default: 
#define _EE ;}; _lc=0; return 255;
#define WaitX(tickets)  do {_lc=(__LINE__+((__LINE__%256)==0))%256; return tickets ;} while(0); case (__LINE__+((__LINE__%256)==0))%256: 

#define RunTask(TaskName,TaskID) do { if (timers[TaskID]==0) timers[TaskID]=TaskName(); }  while(0); 
#define RunTaskA(TaskName,TaskID) { if (timers[TaskID]==0) {timers[TaskID]=TaskName(); continue;} }   //ǰ����������ȱ�ִ֤��

#define CallSub(SubTaskName) do {unsigned char currdt; _lc=(__LINE__+((__LINE__%256)==0))%256; return 0; case (__LINE__+((__LINE__%256)==0))%256:  currdt=SubTaskName(); if(currdt!=255) return currdt;} while(0);
#define InitTasks() {unsigned char i; for(i=MAXTASKS;i>0 ;i--) timers[i-1]=0; }
#define UpdateTimers() {unsigned char i; for(i=MAXTASKS;i>0 ;i--){if((timers[i-1]!=0)&&(timers[i-1]!=255)) timers[i-1]--;}}

#define SEM unsigned int 
//��ʼ���ź���
#define InitSem(sem) sem=0;
//�ȴ��ź���
#define WaitSem(sem) do{ sem=1; WaitX(0); if (sem>0) return 1;} while(0);
//�ȴ��ź�����ʱ������� ��ʱ��tickets ���Ϊ0xFFFE
#define WaitSemX(sem,tickets)  do { sem=tickets+1; WaitX(0); if(sem>1){ sem--;  return 1;} } while(0);
//�����ź���
#define SendSem(sem)  do {sem=0;} while(0);

/*****СС����������*******************************************************/

#include "stm8s.h"

/*****************************************************************
/�������ƣ�Timer1_Init
/����������Timer4��ʼ��
/��ڲ�����void
/���ز�����void
/��    ע��
******************************************************************/
void Timer1_Init(void)
{
    TIM1_DeInit();
    TIM1_TimeBaseInit(16000, TIM1_COUNTERMODE_UP, 9, 0x00);//��ʱ����Ƶ��ʱ��16000000/16000=1000����ʱʱ��1ms*10=10ms
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
������
*******************************/
int main(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
    Timer1_Init();
    LED_Init();
    InitTasks(); //��ʼ������ʵ�����Ǹ�timers����
    __enable_interrupt(); 

    while(1)
    {
        RunTask(task0,0);
        RunTask(task1,1);
    }
}





