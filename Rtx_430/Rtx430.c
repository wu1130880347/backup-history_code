#include "Rtx430.h"
unsigned int    STKP[RTX_MAXTASKN];   /* 指向前一任务堆栈的尾地址 */
unsigned char   RTX_RobinTime;          /* 每个任务最长的运行周期 */
unsigned char   TASK_Current;         /* 当前运行的任务号 */
unsigned char   RTX_TS;
unsigned int    RTX_SP;

unsigned int    SAVE_SR;
unsigned int    SAVE_PC;

struct
{
   unsigned char  time;
   unsigned char  st;
}
STATE[RTX_MAXTASKN];

/*******************************************************************/
#pragma vector = TIMERA0_VECTOR
__interrupt  void WD( void )
{
    unsigned int *i,*j;
    clear_watchdog();
    i=(unsigned int *)getsp();
    j=i-1;
    putsp((unsigned int)j);
 
    *j=*i;
    j++;
    i++;
    
    *j=*i;
    j++;
    i++;
    
    *j=*i;
    j++;
    i++;
    
    *j=*i;
    j++;
    i++;

    *j=*i;
    j++;
    i++;
    
    *j=*i;
    j++;
    i++;
        
    SAVE_SR=*i;
    *j=*i;
    *i=(unsigned int)timer0_comm;
}


void timer0_comm(void)
{  
     unsigned char i;
     unsigned char stack_free;

//Clear_watchdog:     
     clear_watchdog();
     
//Update_Timer0:
     CCR0+=RTX_CLOCK;

//Chcec_Stack:   /* 堆栈检查，如果剩余堆栈 <  RTX_STACKFREE 转去错误处理程序 */

     stack_free=TASK_Current==RTX_MAXTASKN ? RTX_RAMBOTTOM : STKP[TASK_Current+1];

     stack_free=getsp()-stack_free;

     if(stack_free<RTX_STACKFREE)
       {
           _DINT();
           while(1)
           {
              /* ************此处加入堆栈溢出处理程序*************** */
             
           };
       }


//Update_Check_Task_Timers:

      for(i=0;i<RTX_MAXTASKN;i++)
         {
             STATE[i].time--;
             _DINT();
             if((STATE[i].st&K_TMO)&&(STATE[i].time==0))STATE[i].st|=K_READY+TMO_EVENT;
             _EINT();
         }

//Check_Round_Robin_TimeOut:

      if(RTX_TIMESHARING==0)     /* 没有任务切换 */
        {
            restore_sr();
            return;
        }

      if(STATE[TASK_Current].time!=RTX_RobinTime) /* 没有任务切换 */
        {
            restore_sr();
            return;
        }

      if(RTX_TS&RTX_TS_DELAY)
        {
           RTX_TS|=RTX_TS_REQ;
           restore_sr();
           return;
        }

     save_push();
     
     _DINT();
     STATE[TASK_Current].st|=K_ROBIN;
     _EINT();

     task_switch();
}



unsigned char task_switch(void)
{
     unsigned char i;
     unsigned int *j,*p1,*p2;

     clear_watchdog();
     
     RTX_SP=getsp();
     p1=(unsigned int *)(RTX_SP+8);
     p2=p1-1;
       
     for(i=0;i<4;i++)
        {      
           *p1=*p2;
           p1--;
           p2--;
        }
   
     RTX_SP+=2;
     putsp(RTX_SP);
     j=(unsigned int *)RTX_SP;
       
     RTX_TS|=RTX_TS_DELAY;

     i=TASK_Current;


     while(1)
     {
        if ((++i)==(RTX_MAXTASKN))i=0;
        if (STATE[i].st&K_READY)break;
     }
     
     while(TASK_Current<i)
     {
        TASK_Current++;
        
        p1=(unsigned int *)STKP[TASK_Current];
        p2=(unsigned int *)(TASK_Current==RTX_MAXTASKN ? RTX_RAMBOTTOM : STKP[TASK_Current+1]);

        STKP[TASK_Current]=RTX_SP;
        
        while(p1!=p2)
        {
           RTX_SP-=2;
           putsp(RTX_SP);
           j--;
           p1--;
           *j=*p1;
        }
     }

     while(TASK_Current>i)
     {
        p1=(unsigned int *)(TASK_Current==RTX_MAXTASKN ? RTX_SP : STKP[TASK_Current+1]);
        p2=(unsigned int *)STKP[TASK_Current];
        
        while(j!=p2)
        {
           *p1=*j;
           p1++;
           j++;
           RTX_SP+=2;
           putsp(RTX_SP);
        }
        STKP[TASK_Current] = (unsigned int)p1;

        TASK_Current--;
     }

     RTX_RobinTime = STATE[TASK_Current].time + RTX_TIMESHARING;
     
     _DINT();
     
     if(STATE[TASK_Current].st & K_ROBIN)
       {
          _EINT();
          RTX_TS&=~RTX_TS_DELAY;
          RTX_TS&=~RTX_TS_REQ;
          
          save_pop();
          
          return(0x00);
       }

     if((STATE[TASK_Current].st & K_SIG) && (STATE[TASK_Current].st & SIG_EVENT))
       {
          STATE[TASK_Current].st&=0xf0;
          _EINT();
          RTX_TS&=~RTX_TS_DELAY;
          RTX_TS&=~RTX_TS_REQ;
          return(SIG_EVENT);
       }

     if((STATE[TASK_Current].st & K_TMO) && (STATE[TASK_Current].st & TMO_EVENT))
       {
          STATE[TASK_Current].st&=0xf4;
          _EINT();
          RTX_TS&=~RTX_TS_DELAY;
          RTX_TS&=~RTX_TS_REQ;
          return(TMO_EVENT);
       }

     _EINT();
     RTX_TS&=~RTX_TS_DELAY;
     RTX_TS&=~RTX_TS_REQ;
     return(0x00);
}

unsigned char os_wait(unsigned type, unsigned timeout)
{
   unsigned char st = 0;
   
   clear_watchdog();

   if(type==0)
     {
        _DINT();
        STATE[TASK_Current].st &= ~ (st | K_SIG | K_TMO);
        _EINT();
        return (st);
     }

   TACTL&=~MC1;           

   if(type&K_IVL)
     {
        STATE[TASK_Current].time+=timeout;
        if(!C)
          {
             st = TMO_EVENT;
             _DINT();
             STATE[TASK_Current].st &= ~ (st | K_SIG | K_TMO);
             _EINT();
             TACTL|=MC1;           
             return (st);
          }
        _DINT();
        STATE[TASK_Current].st |= K_TMO;
        _EINT();
      }

    if(type&K_TMO)
      {
         if(timeout==0)
           {
              st = TMO_EVENT;
              _DINT();
              STATE[TASK_Current].st &= ~ (st | K_SIG | K_TMO);
              _EINT();
              TACTL|=MC1;           
              return (st);
           }
         STATE[TASK_Current].time = timeout;

         _DINT();
         STATE[TASK_Current].st |= K_TMO;
         _EINT();
      }

    if(type&K_SIG)
      {
         if(STATE[TASK_Current].st&SIG_EVENT)
           {
              st=SIG_EVENT;
              _DINT();
              STATE[TASK_Current].st &= ~ (st | K_SIG | K_TMO);
              _EINT();
              TACTL|=MC1;           
              return (st);
           }
         _DINT();
         STATE[TASK_Current].st |= K_SIG;
         _EINT();
    }

    _DINT();
    STATE[TASK_Current].st &= ~K_READY;
    _EINT();
    
    TACTL|=MC1;           
    task_switch();
    
    return(0xff);
}


unsigned char os_clear_signal(unsigned char task_no)
{
   unsigned char *p;
   
   if (task_no>=RTX_MAXTASKN)return(0xff);
   
   _DINT();
   
   p=&STATE[task_no].st;
   *p&=SIG_EVENT;
   
   _EINT();
   
   return(0);
}

unsigned char os_send_signal(unsigned char task_no)
{
   unsigned char *p;
   
   clear_watchdog();

   if (task_no>=RTX_MAXTASKN)  return (0xff);

   _DINT();
   
   p=&STATE[task_no].st;

   if(*p&K_ACTIVE)
     {
        if (*p&K_SIG)*p|=K_READY;
     }
   *p|=SIG_EVENT;

   _EINT();

   return(0);
}

unsigned char os_delete_task(unsigned char task_no)
{
    unsigned int *j,*p1,*p2;

    clear_watchdog();
    
    if(task_no>=RTX_MAXTASKN)return(0xff);

    if(!(STATE[task_no].st & K_ACTIVE))return(0xff);

    _DINT();
    STATE[task_no].st &= ~(K_ACTIVE | K_READY | K_SIG | K_TMO | K_ROBIN);
    _EINT();

  

    if(TASK_Current==task_no)
      {
         RTX_SP=STKP[task_no];
         putsp(RTX_SP);
         task_switch();
      }
    
    if(TASK_Current<task_no)
      {
         p1=(unsigned int *)(task_no==RTX_MAXTASKN ? RTX_RAMBOTTOM : STKP[task_no+1]);
         j=(unsigned int *)STKP[task_no];
         
         do{
              p2=(unsigned int *)(STKP[task_no]);     
              while(p1!=p2)
              {
                 *p1=*j;
                 p1++;
                 j++;
              }
              STKP[task_no]=(unsigned int)p1;
              task_no--;
           }while (TASK_Current!=task_no);
         return (0);
      }

    if(TASK_Current>task_no)
      {
         p1=(unsigned int *)(STKP[task_no+1]);
         j=(unsigned int *)(STKP[task_no]);
         do{
              task_no++;
              STKP[task_no]=(unsigned int)(j);
              p2=(unsigned int *)(task_no==TASK_Current ? RTX_SP : STKP[task_no+1]);
              while(p1!=p2)
              {
                 j++;
                 p1++;
                 *j=*p1;
              }
           }while(TASK_Current!=task_no);
         RTX_SP=(unsigned int)j;
         putsp(RTX_SP);
         return (0);
      }
      
   return(0xff);   
}

unsigned char os_create_task(unsigned int proc_name, unsigned char task_no)
{
   unsigned int i;
   unsigned int *j,*p1,*p2;
  
   clear_watchdog();
   
   if(task_no>=RTX_MAXTASKN)return(0xff);

   if(STATE[task_no].st&K_ACTIVE)return(0xff);

   STATE[task_no].st|=K_ACTIVE+K_READY;

   i=TASK_Current;
   
   RTX_SP=getsp();
   
   while(i<task_no)
   {
      i++;
      p1=(unsigned int *)STKP[i];
      p2=(unsigned int *)(i==RTX_MAXTASKN ? RTX_RAMBOTTOM : STKP[i+1]);
      p2--;
      while(p1!=p2)
      {
	 j=p1+5;
	 *j=*p1;
         p1--;
      }
      STKP[i]+=10;
   }
   
   if(i>task_no)
     {
        RTX_SP-=10;
        putsp(RTX_SP);
     }   

   while(i>task_no)
   {
      p1=(unsigned int *)(i==TASK_Current ? RTX_SP : STKP[i+1]);
      p2=(unsigned int *)STKP[i];
      STKP[i]-=10;
      while(p1!=p2)
      {
         j=p1+5;
         *p1=*j;
         p1++;
      }
      i--;
   }

   
   j=(unsigned int *)(STKP[task_no]-2);
   *j=proc_name;
   
   j++;
   *j=0;
   j++;
   *j=0;
   j++;
   *j=0;
   j++;
   *j=0;
    
   return(0);
}


unsigned char os_system_start(void)  /* 操作系统初始化 */
{
     unsigned char i;
     unsigned int *j,*p1,*p2;

     _DINT();
    
     clear_watchdog();
     
     RTX_TS&=~RTX_TS_REQ;

     RTX_TS&=~RTX_TS_DELAY;

     TASK_Current=0;

     i=TASK_Current;

     while(1)
     {
        if ((++i)==(RTX_MAXTASKN))i=0;
        if (STATE[i].st&K_READY)break;
     }

     RTX_SP=getsp();
     RTX_SP+=8;
     putsp(RTX_SP);
     j=(unsigned int *)RTX_SP;
     
     while(TASK_Current<i)
     {
        TASK_Current++;
        
        p1=(unsigned int *)STKP[TASK_Current];
        p2=(unsigned int *)(TASK_Current==RTX_MAXTASKN ? RTX_RAMBOTTOM : STKP[TASK_Current+1]);

        STKP[TASK_Current]=RTX_SP;
        
        while(p1!=p2)
        {
           RTX_SP-=2;
           putsp(RTX_SP);
           j--;
           p1--;
           *j=*p1;
        }
     }

     while(TASK_Current>i)
     {
        p1=(unsigned int *)(TASK_Current==RTX_MAXTASKN ? RTX_SP : STKP[TASK_Current+1]);
        p2=(unsigned int *)STKP[TASK_Current];
        
        while(j!=p2)
        {
           *p1=*j;
           p1++;
           j++;
           RTX_SP+=2;
           putsp(RTX_SP);
        }
        STKP[TASK_Current] = (unsigned int)p1;

        TASK_Current--;
      }

      RTX_RobinTime = RTX_TIMESHARING;
      
      TACTL=TASSEL_2+TACLR;     //SMCL , CLREAR TAR 
      CCR0=RTX_CLOCK;
      CCTL0|=CCIE;
      TACTL|=MC1;
      
      _EINT();

      return(0x00);
}


void os_system_init(void)    /* 操作系统初始化 */
{
     unsigned char i;

     _DINT();
     
     
     clear_watchdog();
         
     for(i=0;i<RTX_MAXTASKN;i++)
        {
	   STKP[i]=RTX_RAMBOTTOM;
	   STATE[i].st=0;
	}
     
     STKP[0]=getsp();
     
     TASK_Current=0;
}

void clear_watchdog(void)
{
     //if(!(WDTCTL&WDTTMSEL))WDTCTL=WDT_ARST_250;      /* WDT 250 MS */
  WDTCTL = WDTPW + WDTHOLD;                       //关闭看门狗
}

void Rtx430_Init(void)
{
     os_system_init();                             /* 系统初始化 */
     os_create_task((unsigned int)task_test1,1);   /* 任务创建   */
     os_create_task((unsigned int)task_test2,2);   /* 任务创建   */
     os_create_task((unsigned int)task_test3,3);   /* 任务创建   */
     os_system_start();                            /* 系统运行   */
}   










