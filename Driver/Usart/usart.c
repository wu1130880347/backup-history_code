#include "usart.h"
#include "sys.h"
//debug info output
#if ((defined USART_DEBUG) && (USART_DEBUG == __ON__))
//是否打开该文件内的调试LOG
static const char EN_LOG = GREEN;
//LOG输出文件标记
static const char TAG[] = "app[usart]: ";
#endif

void uart_init(u32 bound);
u8 Dprintf(u8 enable_flag, const char *tag, const char *format, ...);
//初始化IO 串口1
//bound:波特率
void uart_init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	USART_DeInit(USART1);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //使能USART1时钟

	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);  //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); //GPIOA10复用为USART1

	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//初始化PA9，PA10

	//USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;										//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式
	USART_Init(USART1, &USART_InitStructure);										//初始化串口1

	USART_Cmd(USART1, ENABLE); //使能串口1

	USART_ClearFlag(USART1, USART_FLAG_TC);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		  //串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							  //根据指定的参数初始化VIC寄存器、
        Dprintf(EN_LOG,TAG,"app hello \r\n");
}

void USART1_IRQHandler(void) //串口1中断服务程序
{
	u8 Res = 0;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //验证接收状态
	{
		Res = USART_ReceiveData(USART1) & 0xff; //读取接收到的数据
	}
	USART_ClearFlag(USART1, USART_IT_RXNE); //清零接收标志位
}

//串口调试驱动实现

#if ((defined USART_DEBUG) && (USART_DEBUG == __ON__))
int fputc(int ch, FILE *f)
{
	while ((USART1->SR & 0X40) == 0)
		; //循环发送,直到发送完毕
	USART1->DR = (u8)ch;
	return ch;
}

int fgetc(FILE *f)
{
	while (!(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET))
		;
	return (USART_ReceiveData(USART1));
}
#endif

#include <stdarg.h>
//30黑色 31红色 32绿色 33黄色 34蓝色 35品红 36青色 37白色
#if ((defined USART_DEBUG) && (USART_DEBUG == __ON__))
u8 Dprintf(u8 enable_flag, const char *tag, const char *format, ...)
{

	if (enable_flag & 0x01)
	{
		va_list args;
		va_start(args, tag);
		if (enable_flag & 0x02)
			vprintf("\033[33m", args);
		else if (enable_flag & 0x04)
			vprintf("\033[34m", args);
		else if (enable_flag & 0x08)
			vprintf("\033[35m", args);
		else if (enable_flag & 0x10)
			vprintf("\033[36m", args);
		else if (enable_flag & 0x20)
			vprintf("\033[37m", args);
		else if (enable_flag & 0x40)
			vprintf("\033[31m", args);
		else if (enable_flag & 0x80)
			vprintf("\033[30m", args);
		else
			vprintf("\033[32m", args);

		vprintf(tag, args);
		vprintf(format, args);

		vprintf("\033[0m", args);
		va_end(args);
	}
	else
		return 1;
	return 0;
}
#endif