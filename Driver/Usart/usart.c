#include "usart.h"
#include "sys.h"
//debug info output
#if ((defined USART_DEBUG) && (USART_DEBUG == __ON__))
//�Ƿ�򿪸��ļ��ڵĵ���LOG
static const char EN_LOG = GREEN;
//LOG����ļ����
static const char TAG[] = "app[usart]: ";
#endif

void uart_init(u32 bound);
u8 Dprintf(u8 enable_flag, const char *tag, const char *format, ...);
//��ʼ��IO ����1
//bound:������
void uart_init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	USART_DeInit(USART1);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //ʹ��USART1ʱ��

	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);  //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); //GPIOA10����ΪUSART1

	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//��ʼ��PA9��PA10

	//USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;										//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure);										//��ʼ������1

	USART_Cmd(USART1, ENABLE); //ʹ�ܴ���1

	USART_ClearFlag(USART1, USART_FLAG_TC);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		  //����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							  //����ָ���Ĳ�����ʼ��VIC�Ĵ�����
        Dprintf(EN_LOG,TAG,"app hello \r\n");
}

void USART1_IRQHandler(void) //����1�жϷ������
{
	u8 Res = 0;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //��֤����״̬
	{
		Res = USART_ReceiveData(USART1) & 0xff; //��ȡ���յ�������
	}
	USART_ClearFlag(USART1, USART_IT_RXNE); //������ձ�־λ
}

//���ڵ�������ʵ��

#if ((defined USART_DEBUG) && (USART_DEBUG == __ON__))
int fputc(int ch, FILE *f)
{
	while ((USART1->SR & 0X40) == 0)
		; //ѭ������,ֱ���������
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
//30��ɫ 31��ɫ 32��ɫ 33��ɫ 34��ɫ 35Ʒ�� 36��ɫ 37��ɫ
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