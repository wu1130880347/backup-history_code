#include "Source.h"
u8 Pi=4,Ii=4,Di=1;

void Trans_val(u16 Hex_Val)
{
    u32 caltmp;
    u16 v_value;
    caltmp = Hex_Val;
    caltmp = (caltmp << 5) + Hex_Val;           //caltmp = Hex_Val * 33
    caltmp = (caltmp << 3) + (caltmp << 1);     //caltmp = caltmp * 10
	caltmp = (caltmp << 3) + (caltmp << 1);     //caltmp = caltmp * 10
    v_value = caltmp >> 12;                   //Curr_Volt = caltmp / 2^n

        OLED_ShowChar(56,0,v_value/1000+'0',8);
        OLED_ShowChar(64,0,'.',8);
	OLED_ShowChar(72,0,v_value%1000/100+'0',8);
	OLED_ShowChar(80,0,v_value%100/10+'0',8);
	OLED_ShowChar(88,0,v_value%10+'0',8);
}
/*******************************************
函数名称：PutSting
功    能：向PC机发送字符串
参    数：ptr--指向发送字符串的指针
返回值  ：无
********************************************/
void PutString(u8 *ptr)
{
      while(*ptr != '\0')
      {
            while (!(IFG1 & UTXIFG0));             // TX缓存空闲？
            TXBUF0 = *(ptr++);                       // 发送数据
      }
      while (!(IFG1 & UTXIFG0));
      TXBUF0 = '\n';
}

void Osc_Init(void)
{
  OLED_ShowString(0,0,"SetSpeed:",8);
  OLED_ShowString(0,1,"NowSpeed:",8);
  OLED_ShowString(0,2,"Con_Freq:     Hz",8);
  OLED_ShowString(0,3,"NowOut_D:",8);
  OLED_ShowString(0,7,"Now_Freq:",8);
}

void work_pid(u16 now_Speed,u16 setspeed)
{
  static int t;
  t = PID_con(now_Speed-setspeed,Pi,Ii,Di);
  OLED_ShowString(0,4,"           ",8);
  OLED_ShowFloat(0,4,t,0,8);

  TB0CCR1 -= t;
  if(TB0CCR1>1000)TB0CCR1=1000;
  if(now_Speed<1000)TB0CCR1=700;
  OLED_ShowString(72,3,"     ",8);
  OLED_ShowNum(72,3,TB0CCR1,0,8);
}
int PID_con(int Error,u8 P,u8 I,u8 D)
{
        static int SumError;
        static int dError;
        static int LastError;
        static int PrevError;
        static u8 sum_f=1;
        
        Error /= 100;
        if(Error<20&&Error>-20||!sum_f){SumError += Error;sum_f=0;}
                                          
        dError = LastError - PrevError;    // 当前微分  
        PrevError = LastError;                           
        LastError = Error; 
        
        OLED_ShowString(0,5,"       ",8);
        OLED_ShowFloat(0,5,Error,0,8);
        OLED_ShowString(0,6,"       ",8);
        OLED_ShowFloat(0,6,SumError,0,8);
         
        return (P*Error + I*SumError/100 + D*dError)/10;
}