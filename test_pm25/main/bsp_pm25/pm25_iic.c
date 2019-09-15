#include "pm25_iic.h"

static const char *TAG = "iic_drv";
#define delay_us os_delay_us

u8 Dprintf(u8 enable_flag, const char *format, ...);

u8 Dprintf(u8 enable_flag, const char *format, ...)
{

#if (defined DEBUG_UART) && (DEBUG_UART == TRUE)
	if (enable_flag)
	{
		va_list args;
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
	}
	else
		return 1;
#endif
	return 1;
}

void IIC_Init(void); //��ʼ��IIC��IO��
u8 IIC_Write_One_Byte(u8 daddr, u8 addr, u8 data);
u8 IIC_Read_One_Byte(u8 daddr, u8 addr);
void IIC_Send_Byte(u8 txd);			 //IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack); //IIC��ȡһ���ֽ�
s8 test_i2c(void *arg);

#if (defined SOFT_IIC) && (SOFT_IIC == TRUE)

//IIC���в�������

void IIC_Start(void);  //����IIC��ʼ�ź�
void IIC_Stop(void);   //����IICֹͣ�ź�
void IIC_Ack(void);	//IIC����ACK�ź�
void IIC_NAck(void);   //IIC������ACK�ź�
u8 IIC_Wait_Ack(void); //IIC�ȴ�ACK�ź�
u8 IIC_Detection(u8 address);

//��ʼ��IIC
void IIC_Init(void)
{
	static u8 static_flag = 1;
	gpio_config_t io_conf;
	if (static_flag)
	{
		// disable interrupt
		io_conf.intr_type = GPIO_INTR_DISABLE;
		// set as output mode
		io_conf.mode = GPIO_MODE_OUTPUT_OD;
		// bit mask of the pins that you want to set
		io_conf.pin_bit_mask = I2C_SCL_MASK | I2C_SDA_MASK;
		// disable pull-down mode
		io_conf.pull_down_en = 0;
		// disable pull-up mode
		io_conf.pull_up_en = 0;
		// configure GPIO with the given settings
		gpio_config(&io_conf);

		GPIO.out_w1ts |= I2C_SCL_MASK;
		GPIO.out_w1ts |= I2C_SDA_MASK;

		IIC_SCL_1;
		IIC_SDA_1;
		static_flag = 0;
		ESP_LOGI(TAG, "i2c init successful ... \n");
	}
}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT(); //sda�����
	IIC_SDA_1;
	IIC_SCL_1;
	delay_us(4);
	IIC_SDA_0; //START:when CLK is high,DATA change form high to low
	delay_us(4);
	IIC_SCL_0; //ǯסI2C���ߣ�׼�����ͻ��������
}
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT(); //sda�����
	IIC_SCL_0;
	IIC_SDA_0; //STOP:when CLK is high DATA change form low to high
	delay_us(4);
	IIC_SCL_1;
	IIC_SDA_1; //����I2C���߽����ź�
	delay_us(4);
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	SDA_IN(); //SDA����Ϊ����
	IIC_SDA_1;
	IIC_SCL_1;
	while (READ_SDA)
	{
		ucErrTime++;
		if (ucErrTime > 250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL_0; //ʱ�����0
	return 0;
}
//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL_0;
	SDA_OUT();
	IIC_SDA_0;
	delay_us(2);
	IIC_SCL_1;
	delay_us(2);
	IIC_SCL_0;
}
//������ACKӦ��
void IIC_NAck(void)
{
	IIC_SCL_0;
	SDA_OUT();
	IIC_SDA_1;
	delay_us(2);
	IIC_SCL_1;
	delay_us(2);
	IIC_SCL_0;
}
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
void IIC_Send_Byte(u8 txd)
{
	u8 t;
	SDA_OUT();
	IIC_SCL_0; //����ʱ�ӿ�ʼ���ݴ���
	for (t = 0; t < 8; t++)
	{
		if ((txd & 0x80) >> 7)
		{
			IIC_SDA_1;
		}
		else
		{
			IIC_SDA_0;
		}
		txd <<= 1;
		delay_us(2); //��TEA5767��������ʱ���Ǳ����
		IIC_SCL_1;
		delay_us(2);
		IIC_SCL_0;
		delay_us(2);
	}
}
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i, receive = 0;
	SDA_IN(); //SDA����Ϊ����
	for (i = 0; i < 8; i++)
	{
		IIC_SCL_0;
		delay_us(2);
		IIC_SCL_1;
		receive <<= 1;
		if (READ_SDA)
			receive++;
		//delay_us(1);
	}
	if (!ack)
		IIC_NAck(); //����nACK
	else
		IIC_Ack(); //����ACK
	return receive;
}

//IICд����
//���� �ӻ���ַ  �ӻ��Ĵ���  ����
u8 IIC_Write_One_Byte(u8 daddr, u8 addr, u8 data)
{
	IIC_Start();
	IIC_Send_Byte(daddr);
	if (IIC_Wait_Ack())
		return 1;
	IIC_Send_Byte(addr);
	if (IIC_Wait_Ack())
		return 1;
	IIC_Send_Byte(data);
	if (IIC_Wait_Ack())
		return 1;
	IIC_Stop();
	return 0;
}

//IIC ������
//���� �ӻ���ַ  �ӻ��Ĵ���
u8 IIC_Read_One_Byte(u8 daddr, u8 addr)
{
	u8 temp = 0;
	IIC_Start();
	IIC_Send_Byte(daddr);
	if (IIC_Wait_Ack())
		return 1;
	IIC_Send_Byte(addr);
	if (IIC_Wait_Ack())
		return 1;

	IIC_Start();
	IIC_Send_Byte(daddr + 1);
	if (IIC_Wait_Ack())
		return 1;
	temp = IIC_Read_Byte(1);
	IIC_Stop();

	return temp;
}
//IIC̽�����
u8 IIC_Detection(u8 address)
{
	u8 temp = 0;
	IIC_Start();
	IIC_Send_Byte(address);
	if (IIC_Wait_Ack())
		temp = 1;
	IIC_Stop();
	return temp;
}

s8 test_i2c(void *arg)
{
	u16 address = 0;
	IIC_Init();
	Dprintf(IIC_DECTION, "\r\n\r\n/*******************************/\r\n");
	Dprintf(IIC_DECTION, "/*******************************/\r\n");
	Dprintf(IIC_DECTION, "I2C find address Map : \r\n");
	for (address = 0; address <= 0xFF; address += 2)
	{
		if ((address) % 16 == 0)
			Dprintf(IIC_DECTION, "\r\n");
		if (!IIC_Detection((u8)address))
			Dprintf(IIC_DECTION, " 0x%x ", address >> 1);
		else
			Dprintf(IIC_DECTION, "  No  ");
	}
	Dprintf(IIC_DECTION, "\r\n/*******************************/\r\n");
	Dprintf(IIC_DECTION, "/*******************************/\r\n");
	return TRUE;
}

#elif (defined HARD_IIC) && (HARD_IIC == TRUE)

u16 timeout = 0;
u8 IIC_Write_One_Byte(u8 daddr, u8 addr, u8 data)
{
	I2C_GenerateSTART(I2C1, ENABLE);							//������ʼ�ź�
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) //���	EV5�¼�
	{
		if ((timeout--) == 0)
			Dprintf(IIC_DECTION, "W_EV5 Fail");
	}
	timeout = 1000;

	I2C_Send7bitAddress(I2C1, daddr, I2C_Direction_Transmitter);			  //����7λEEPROM��Ӳ����ַ
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) //���EV6�¼�
	{
		if ((timeout--) == 0)
			Dprintf(IIC_DECTION, "W_EV6 Fail");
	}
	timeout = 1000;

	I2C_SendData(I2C1, addr);										 //���Ͳ������ڴ��ַ
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) //���EV8�¼�
	{
		if ((timeout--) == 0)
			Dprintf(IIC_DECTION, "W_EV8 Fail");
	}
	timeout = 1000;

	I2C_SendData(I2C1, data);										 //Ҫд������ݣ�һ���ֽڣ�
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) //���EV8�¼�
	{
		if ((timeout--) == 0)
			Dprintf(IIC_DECTION, "W_EV8 Fail");
	}
	timeout = 1000;

	I2C_GenerateSTOP(I2C1, ENABLE); //���ͽ����ź�
	return 0;
}
u8 IIC_Read_One_Byte(u8 daddr, u8 addr)
{
	u8 readtemp;
	I2C_GenerateSTART(I2C1, ENABLE);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) //5
	{
		if ((timeout--) == 0)
			Dprintf(IIC_DECTION, "R_EV5 Fail");
	}
	timeout = 1000;

	I2C_Send7bitAddress(I2C1, daddr, I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) //6
	{
		if ((timeout--) == 0)
			Dprintf(IIC_DECTION, "R_EV6 Fail");
	}
	timeout = 1000;

	I2C_SendData(I2C1, addr);
	while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS)
	{
		if ((timeout--) == 0)
			Dprintf(IIC_DECTION, "R_EV8 Fail");
	}
	timeout = 1000;

	I2C_GenerateSTART(I2C1, ENABLE);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
	{
		if ((timeout--) == 0)
			Dprintf(IIC_DECTION, "R_EV5 Fail");
	}
	timeout = 1000;

	I2C_Send7bitAddress(I2C1, daddr, I2C_Direction_Receiver);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
	{
		if ((timeout--) == 0)
			Dprintf(IIC_DECTION, "R_EV6 Fail");
	}
	timeout = 1000;

	I2C_AcknowledgeConfig(I2C1, DISABLE);

	/* ��� EV7 �¼� */
	while (I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS)
	{
		if ((timeout--) == 0)
			Dprintf(IIC_DECTION, "R_EV7 Fail");
	}
	timeout = 1000;

	/* ��ȡ�������� */
	readtemp = I2C_ReceiveData(I2C1);

	/* ֹͣ�ź� */
	I2C_GenerateSTOP(I2C1, ENABLE);

	return readtemp;
}
u8 IIC_Detection(u8 address)
{
	u8 temp = 0;
	I2C_GenerateSTART(I2C1, ENABLE);
	timeout = 100;
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
	{
		if ((timeout--) == 0)
		{
			temp = 0;
			break;
		}
	}
	timeout = 100;
	I2C_Send7bitAddress(I2C1, address, I2C_Direction_Transmitter);
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) //6
	{
		if ((timeout--) == 0)
		{
			temp = 1;
			break;
		}
	}
	I2C_GenerateSTOP(I2C1, ENABLE);
	return temp;
}

s8 test_i2c(void *arg)
{
	u16 address = 0;
	IIC_Init();
	Dprintf(IIC_DECTION, "\r\n\r\n/*******************************/\r\n");
	Dprintf(IIC_DECTION, "/*******************************/\r\n");
	Dprintf(IIC_DECTION, "I2C find address Map : \r\n");
	for (address = 0; address <= 0xFF; address += 2)
	{
		if ((address) % 16 == 0)
			Dprintf(IIC_DECTION, "\r\n");
		if (!IIC_Detection((u8)address))
			Dprintf(IIC_DECTION, " 0x%x ", address);
		else
			Dprintf(IIC_DECTION, "  No  ");
	}
	Dprintf(IIC_DECTION, "\r\n/*******************************/\r\n");
	Dprintf(IIC_DECTION, "/*******************************/\r\n");
	return 0;
}

u8 IIC_Read_Byte(unsigned char ack)
{
	return 0;
}

void IIC_Send_Byte(u8 txd)
{
}

void IIC_Init(void)
{
	static u8 static_flag = 1;
	if (static_flag)
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		I2C_InitTypeDef I2C_InitStruct;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE); //ʹ��I2C1ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

		GPIO_InitStructure.GPIO_Pin = I2C1_SCL_PIN_MASK;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(I2C1_SCL_SOURCE, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = I2C1_SDA_PIN_MASK;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(I2C1_SDA_SOURCE, &GPIO_InitStructure);

		I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
		I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
		I2C_InitStruct.I2C_ClockSpeed = 100000;
		I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
		I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
		I2C_InitStruct.I2C_OwnAddress1 = 0x55;
		I2C_Init(I2C1, &I2C_InitStruct);

		I2C_Cmd(I2C1, ENABLE);
	}
}
#endif
