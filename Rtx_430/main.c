//#include "msp430x14x.h"
//#include "lcd_12864.h"
#include "Clock.h"                    //ʱ�ӳ�ʼ������Դ����
#include "Source.h"                   //�������Ӻ���������
#include "Rtx430.h"

//    OLED_ShowNum(0,4,TB0CCR5-TB0CCR4,5,8);
//    TBCCTL4 = CM_1 + CAP + SCS + CCIS1;
//    TBCCTL5 = CM_1 + CAP + SCS + CCIS1;
//    TBCCTL4 |= CCIS0;                         // start
//    TBCCTL5 |= CCIS0; 			            // stop
//    TB0CCR4 = 0;
//    TB0CCR5 = 0;

void main(void)
{
        Clock_Init();
	P1_Init();
	Wdt_1s();
        OLED_Init();			//��ʼ��OLED
	Osc_Init();
        
        
    P4DIR = BIT1;                      //TA1_PWM���
    P4SEL |= BIT1;

    P1DIR &= ~BIT0;
    P1SEL |= BIT0;                     //TB_CLK����

    P5DIR |= BIT4 + BIT6 + BIT5;
    P5SEL |= BIT4 + BIT6 + BIT5;       //clock 8Mhz,1Mhz,32768hz OUT
	//Ago_Init();
       InitUART();
       //CAPD_Start();
       //DAC12_Init();
      //ADC_12Start();

     //  Rtx430_Init();
       _EINT();                           //��ȫ���жϿ���
       LPM1;
}













