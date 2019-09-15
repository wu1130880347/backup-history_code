#include "Clock.h"
void Clock_Init()
{
    WDTCTL = WDTPW + WDTHOLD;                       //�رտ��Ź�
    BCSCTL1 &= ~XT2OFF;
    BCSCTL2 |= SELM_2 + SELS + DIVS_3;          //MCLK 8MHz SMCLK 1MHz
    do                                          //ACLK 32.768KHz
    {
      IFG1 &= ~OFIFG;
      for(u16 i = 0xff;i>0;i--)_NOP();
    }
    while((IFG1&OFIFG) != 0);
}

void Wdt_1s(void)
{
    WDTCTL = WDTPW+WDTTMSEL+WDTCNTCL+WDTSSEL+WDTIS0;   //WDT_250ms 
    IE1 |= WDTIE;
    IFG1 &= ~WDTIFG;
}

void P1_Init(void)
{
   //P1 IO �ж�
    P1DIR |= 0xF0;
    P1IES = 0x0e;               // P1.0~P1.3ѡ���½����ж�
    P1IE  = 0x0e;               // ���ж�ʹ��
    P1IFG = 0;
    P1OUT = 0;
}


void InitUART(void)
{
    P3SEL|= BIT4 + BIT5;                      //����IO��Ϊ�ڶ�����ģʽ������UART����
    P3DIR|= BIT4;                             //����TXD0�ڷ���Ϊ���
    ME1 |= URXE0 + UTXE0;                     // Enable USART0 T/RXD
    UCTL0 |= CHAR;                            // 8-bit character
    UTCTL0 |= SSEL0;                          // UCLK = ACLK
    UBR00 = 0x03;                             // 32k/9600 - 3.41
    UBR10 = 0x00;                             //
    UMCTL0 = 0x4A;                            // Modulation
    UCTL0 &= ~SWRST;                          // Initialize USART state machine
    IE1 |= URXIE0;
}

void CAPD_Start(void)
{
    P2DIR = BIT2;
    P2SEL |= BIT2 + BIT3;
    CAPD |= CAPD3;
    CACTL1 |= CAON + CAREF_2 + CARSEL;
    CACTL2 |= P2CA0;
}

void ADC_12Start(void)
{
    P6SEL |= 0x01;                            // ʹ��ADCͨ��
    ADC12CTL0 = ADC12ON+SHT0_15+MSC;           // ��ADC�����ò���ʱ��
    ADC12CTL1 = SHP+CONSEQ_2+ADC12SSEL_3;                 // ʹ�ò�����ʱ��
    ADC12IE = 0x01;                           // ʹ��ADC�ж�
    ADC12CTL0 |= ENC;                         // ʹ��ת��
    //ADC12CTL0 |= ADC12SC;                     // ��ʼת��
}

/*void DAC12_Init(void)
{
  DAC12_0CTL |= DAC12CALON;                        //����У��DAC
  while((DAC12_0CTL & DAC12CALON) != 0){_NOP();}    //�ȴ�У�����

  ADC12CTL0 = REFON + REF2_5V;                        // �ο���ѹΪ�ڲ�2.5v
  DAC12_0CTL = DAC12AMP_5 + DAC12IR + DAC12LSEL_0;   //���ƼĴ������ã��Զ��������ݣ��ڻ�׼Ϊ������
  DAC12_0CTL |= DAC12SREF_0;
  DAC12_0CTL |= DAC12ENC; // ����DACģ��(DAC12LSEL_0ʱ�˾����ʡ����������Ϊ1)
  DAC12_0DAT = 0x0000;    //DAC��ֵΪ0����������Ĵ����Ϳ��Ը���DAC�������
}
  */
void Ago_Init(void)
{
    OLED_ShowString(0,0,"K_1:",8);
    OLED_ShowString(0,1,"K_2:",8);
    OLED_ShowString(0,2,"K_3:",8);
    OLED_ShowString(0,3,"K_4:",8);
    OLED_ShowString(0,7,"        00:00:00",8);

    P4DIR = BIT1;                      //TA1_PWM���
    P4SEL |= BIT1;

    P1DIR &= ~BIT0;
    P1SEL |= BIT0;                     //TB_CLK����

    P5DIR |= BIT4 + BIT6 + BIT5;
    P5SEL |= BIT4 + BIT6 + BIT5;       //clock 8Mhz,1Mhz,32768hz OUT
}