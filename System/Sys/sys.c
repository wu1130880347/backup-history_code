#include "sys.h"
#include "stm32f4xx_conf.h"
//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI
void WFI_SET(void)
{
    asm("WFI");
}
//�ر������ж�(���ǲ�����fault��NMI�ж�)
void INTX_DISABLE(void)
{
    asm("CPSID   I");
    asm("BX      LR");
}
//���������ж�
void INTX_ENABLE(void)
{
    asm("CPSIE   I");
    asm("BX      LR");
}
//����ջ����ַ
//addr:ջ����ַ
void MSR_MSP(u32 addr)
{
    asm("MSR MSP, r0"); //set Main Stack value
    asm("BX r14");
}

void Driver_MCU_Init(void)
{

#define PLL_M 8
#define PLL_N 336
#define PLL_P 2
#define PLL_Q 7
    //--------------------------- CLK INIT, HSE PLL ----------------------------
    ErrorStatus HSEStartUpStatus;
    //RCC reset
    RCC_DeInit();
    //Enable HSE
    RCC_HSEConfig(RCC_HSE_ON);
    //Wait HSE is ready
    HSEStartUpStatus = RCC_WaitForHSEStartUp();
    //If HSE start fail, wail and wail.
    while (HSEStartUpStatus == ERROR)
        ;
    //Set bus clock
    RCC_HCLKConfig(RCC_SYSCLK_Div1); //(HCLK=SYSCLK)=168MHz
    RCC_PCLK1Config(RCC_HCLK_Div4);  //(PCLK1=SYSCLK/4)=42MHz
    RCC_PCLK2Config(RCC_HCLK_Div2);  //(PCLK2=SYSCLK/2)=84MHz
    //HSE 8M PLL-> 168M
    //PLL=8MHz * N/ (M*P)=8MHz* 336 /(8*2) = 168MHz
    RCC_PLLConfig(RCC_PLLSource_HSE, PLL_M, PLL_N, PLL_P, PLL_Q);
    RCC_PLLCmd(ENABLE);
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        ;
    //Select PLL as system clock source
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    while (RCC_GetSYSCLKSource() != 0x08)
        ;

    //----------------------------- CLOSE HSI ---------------------------
    RCC_HSICmd(DISABLE);

    //--------------------------- OPEN GPIO CLK -------------------------
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, DISABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, DISABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, DISABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, DISABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, DISABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, DISABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, DISABLE);

    //---------------------------- INT CONFIG ---------------------------
    //2-level interrupt
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    __disable_irq();

    //---------------------------- JTAG CONFIG ---------------------------
    //JTAG/SWD disable
    //GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    //GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
}