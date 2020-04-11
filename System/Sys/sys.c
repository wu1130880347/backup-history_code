#include "sys.h"
#include "stm32f4xx_conf.h"
//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI
void WFI_SET(void)
{
    asm("WFI");
}
//关闭所有中断(但是不包括fault和NMI中断)
void INTX_DISABLE(void)
{
    asm("CPSID   I");
    asm("BX      LR");
}
//开启所有中断
void INTX_ENABLE(void)
{
    asm("CPSIE   I");
    asm("BX      LR");
}
//设置栈顶地址
//addr:栈顶地址
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