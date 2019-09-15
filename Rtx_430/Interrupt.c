#include "Source.h"
#include "Clock.h"

#pragma vector = UART0RX_VECTOR
__interrupt void UART0_RXISR(void)
{
  //   while(1)
  //  {
  //      while (!(IFG1 & UTXIFG0));                                             //等待以前的字符发送完毕
	TXBUF0 = RXBUF0;                                                             //将收到的字符发送出去
	OLED_ShowNum(104,7,RXBUF0,3,8);

  //  }

    switch(RXBUF0)
        {
        case 0x01:          Pi+=1;
                            return;
        case 0x02:          Pi-=1;                               
                            return;
        case 0x03:          Ii+=1;
                            return;
        case 0x04:          Ii-=1;
                            return;
        case 0x05:          Di+=1;
                            return;
        case 0x06:          Di-=1;
                            return;
        default:
                            return;
        }
}
#pragma vector=PORT1_VECTOR
__interrupt void  PORT1_ISR(void)
{

        switch(P1IFG&0x0f)
        {
        case 0x01:
                            P1IFG = 0;
                            return;

        case 0x02:          OLED_ShowString(72,0,"       ",8);
                            if(set_Speed == 0||set_Speed == 40000)set_Speed = 10000;
                            set_Speed += 1000;
                            OLED_ShowNum(72,0,set_Speed,0,8);
                            P1IFG = 0;
                            return;

        case 0x04:          TACTL |= MC_2 + TACLR + TAIE;

                            P1IFG = 0;
                            return;

        case 0x08:          TB0CCR0 = 1000;
                            TB0CCR1 = 500;   
                            TBCTL |= TASSEL_2 + MC_1;                            //定时钟选择SMLK=1MHz
                            TBCCTL1 = OUTMOD_7;
                            OLED_ShowString(72,2,"  1000",8);
                            
                            if(pwm_on==0){P3DIR = 0xC0;
                            P3OUT = 0x80;pwm_on=1;}
                            else {P3DIR = 0xC0;
                            P3OUT = 0x40;pwm_on=0;} 
                            P1IFG = 0;
                            return;
        default:
                            P1IFG = 0;
                            return;
        }
}
/*******************************************
函数名称：ADC12ISR
功    能：ADC中断服务函数，在这里用多次平均的
          计算P6.0口的模拟电压数值
参    数：无
返回值  ：无
********************************************/
#pragma vector = ADC_VECTOR
__interrupt void ADC12ISR (void)
{
  static u16 index = 0;

  results[index++] = ADC12MEM0;               // Move results
  if(index == Num_of_Results)
  {
        u8 i;
        u32 sum = 0;

        index = 0;
        for(i = 0; i < Num_of_Results; i++)
        {
            sum += results[i];
        }
        sum >>= 5;                            //除以32

        Trans_val(sum);

  }
}



#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR (void)
{
    fag += TAR;
    fag <<=2;
    OLED_ShowString(72,7,"       ",8);
    OLED_ShowNum(72,7,fag,0,8);
    OLED_ShowString(72,1,"       ",8);
    fag *= 30;
    OLED_ShowNum(72,1,fag,0,8);
    work_pid((u16)fag,set_Speed);
    fag = 0;
    TACTL |= TACLR;
    WDTCTL = WDTPW+WDTTMSEL+WDTCNTCL+WDTSSEL+WDTIS0;
    IFG1 &= ~WDTIFG;
}


#pragma vector = TIMERA1_VECTOR
__interrupt void TB_1_ISR (void)
{
  fag += 65536;
  TACTL &= ~TAIFG;
}