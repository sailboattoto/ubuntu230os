/*
set timer frequency=100HZ T=10ms
frequency=1193200/(value)HZ
below value=11932
*/
#include<x86.h>
#include<timer.h>

TIMERCTL *gtimerctl;
void init_pit(TIMERCTL *timerctl)
{

 outb(0x43,0x34);//port(0x43)=0x34
 outb(0x40,11932%256);//0x9c
 outb(0x40,11932/256);//0x2e
 int i;
 for(i=0;i<MAX_TIMER;i++)
 {
  timerctl->timer[i].flag=0;//not used
 }
 return;
}


void inthandler20(int* esp)
{
#define PIC0_OCW2		0x0020
    outb(PIC0_OCW2,0x60);
    //TIMERCTL *gtimerctl=(TIMERCTL *)0x412a00;
    gtimerctl->count++;
    int i;
     //printdebug(timerctl.timer[0].flag,80);
    for(i=0;i<gtimerctl->number;i++)
    {   // printdebug(i,100);
        if(gtimerctl->timer[i].flag==RUNNING)
        {
          // printdebug(i,100);
            gtimerctl->timer[i].timeout--;


            if(gtimerctl->timer[i].timeout==0)
            {

                gtimerctl->timer[i].flag=ALLOC;
                fifo8_write(gtimerctl->timer[i].fifo,gtimerctl->timer[i].data);
            }
        }
    }

  return;
}

TIMER * timer_alloc(TIMERCTL * timerctl,unsigned i)
{

    //int i;
    for(i=0;i<MAX_TIMER;i++)
    {
      if((timerctl->timer[i].flag)==0)//not used
      {
       // printdebug(i,0);
        timerctl->timer[i].flag=1;
       // printdebug(i,150);
        timerctl->number++;
        return &(timerctl->timer[i]);
      }
     }

     /*
     timerctl->number++;
      timerctl->timer[i].flag=ALLOC;
           return &(timerctl->timer[i]);
 */

     return 0;
}


void timer_free(TIMER *timer)
{
  timer->flag=0;
  return;
}

void timer_init(TIMER *timer,struct FIFO8 *fifo,unsigned char data)
{
 timer->fifo=fifo;
 timer->data=data;
 return;
}


void timer_settime(TIMER *timer,unsigned timeout)
{
    timer->timeout=timeout;
    timer->flag=RUNNING;
    return;
}





















