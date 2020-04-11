#include "main.h" 

void main(void)
{
    Driver_MCU_Init();
    uart_init(921600);
    delay_init(168);
    LED_Init();
    
    static int i = 0;
    while(1)
    {
        delay_ms(50);
        LED0 ^= 1;
        if (i++ >= 5)
            break;
    }
    //跳转到用户程序
    Jump_To_App();
    while (1)
        ;
}