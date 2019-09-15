#include "lcd_12864.h"
//��ʱ������IAR�Դ�������ʹ�õ�
#define CPU_F ((double)8000000)   //�ⲿ��Ƶ����8MHZ
#define Delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) 
#define Delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0)) 

//�Զ������ݽṹ������ʹ��
#define u8 unsigned char
#define u16  unsigned int
#define u32 unsigned long

//8��LED�ƣ�������P6�ڣ���ͨ���Ͽ���Դֹͣʹ�ã�ADCʹ��ʱ�Ͽ���Դ
#define LED8DIR         P6DIR
#define LED8            P6OUT                             //P6�ڽ�LED�ƣ�8��


//���ڲ����ʼ��㣬��BRCLK=CPU_Fʱ������Ĺ�ʽ���Լ��㣬����Ҫ�������ü����Ƶϵ��
#define baud           9600                                //���ò����ʵĴ�С
#define baud_setting   (uint)((ulong)CPU_F/((ulong)baud))  //�����ʼ��㹫ʽ
#define baud_h         (uchar)(baud_setting>>8)            //��ȡ��λ
#define baud_l         (uchar)(baud_setting)               //��λ


//NRF2401ģ�������
#define  RF24L01_CE_0        P1OUT &=~BIT5         //CE��P15         
#define  RF24L01_CE_1        P1OUT |= BIT5        

#define  RF24L01_CSN_0       P2OUT &=~BIT7         //CS��P27
#define  RF24L01_CSN_1       P2OUT |= BIT7     

#define  RF24L01_SCK_0       P3OUT &=~BIT3         //SCK��P33
#define  RF24L01_SCK_1       P3OUT |= BIT3   

#define  RF24L01_MISO_0      P3OUT &=~BIT2         //MISO��P32
#define  RF24L01_MISO_1      P3OUT |= BIT2

#define  RF24L01_MOSI_0      P3OUT &=~BIT1         //MOSI��P31
#define  RF24L01_MOSI_1      P3OUT |= BIT1

#define  RF24L01_IRQ_0       P1OUT &=~BIT4         //IRQ��P14     
#define  RF24L01_IRQ_1       P1OUT |= BIT4


static u8  sta;
static u8 TxBuf[8]={'A','B','C','D','E','F','G','H'};
static u8  RxBuf[8]={0};

//==========================NRF24L01============================================
#define TX_ADR_WIDTH    5   	// 5 uints TX address width
#define RX_ADR_WIDTH    5   	// 5 uints RX address width
#define TX_PLOAD_WIDTH  8  	// 32 TX payload
#define RX_PLOAD_WIDTH  8  	// 32 uints TX payload
//=========================NRF24L01�Ĵ���ָ��===================================
#define READ_REG        0x00  	// ���Ĵ���ָ��
#define WRITE_REG       0x20 	// д�Ĵ���ָ��
#define RD_RX_PLOAD     0x61  	// ��ȡ��������ָ��
#define WR_TX_PLOAD     0xA0  	// д��������ָ��
#define FLUSH_TX        0xE1 	// ��ϴ���� FIFOָ��
#define FLUSH_RX        0xE2  	// ��ϴ���� FIFOָ��
#define REUSE_TX_PL     0xE3  	// �����ظ�װ������ָ��
#define NOP1            0xFF  	// ����
//========================SPI(nRF24L01)�Ĵ�����ַ===============================
#define CONFIG          0x00  // �����շ�״̬��CRCУ��ģʽ�Լ��շ�״̬��Ӧ��ʽ
#define EN_AA           0x01  // �Զ�Ӧ��������
#define EN_RXADDR       0x02  // �����ŵ�����
#define SETUP_AW        0x03  // �շ���ַ�������
#define SETUP_RETR      0x04  // �Զ��ط���������
#define RF_CH           0x05  // ����Ƶ������
#define RF_SETUP        0x06  // �������ʡ����Ĺ�������
#define STATUS          0x07  // ״̬�Ĵ���
#define OBSERVE_TX      0x08  // ���ͼ�⹦��
#define CD              0x09  // ��ַ���           
#define RX_ADDR_P0      0x0A  // Ƶ��0�������ݵ�ַ
#define RX_ADDR_P1      0x0B  // Ƶ��1�������ݵ�ַ
#define RX_ADDR_P2      0x0C  // Ƶ��2�������ݵ�ַ
#define RX_ADDR_P3      0x0D  // Ƶ��3�������ݵ�ַ
#define RX_ADDR_P4      0x0E  // Ƶ��4�������ݵ�ַ
#define RX_ADDR_P5      0x0F  // Ƶ��5�������ݵ�ַ
#define TX_ADDR         0x10  // ���͵�ַ�Ĵ���
#define RX_PW_P0        0x11  // ����Ƶ��0�������ݳ���
#define RX_PW_P1        0x12  // ����Ƶ��0�������ݳ���
#define RX_PW_P2        0x13  // ����Ƶ��0�������ݳ���
#define RX_PW_P3        0x14  // ����Ƶ��0�������ݳ���
#define RX_PW_P4        0x15  // ����Ƶ��0�������ݳ���
#define RX_PW_P5        0x16  // ����Ƶ��0�������ݳ���
#define FIFO_STATUS     0x17  // FIFOջ��ջ��״̬�Ĵ�������

//=============================RF24l01״̬=====================================
static u8 TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//���ص�ַ
static u8 RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//���յ�ַ
//==============================================================================

void RF24L01_IO_set(void);
u8 SPI_RW(u8 data);
u8 SPI_Read(u8 reg);
u8 SPI_RW_Reg(u8 reg, u8 value);
u8 SPI_Read_Buf(u8 reg, u8 *pBuf, u8 uu8s);
u8 SPI_Write_Buf(u8 reg, u8 *pBuf, u8 uu8s);
void SetRX_Mode(void);
void SetTX_Mode(void);
u8 nRF24L01_RxPacket(u8* rx_buf);
void nRF24L01_TxPacket(u8 * tx_buf);
void init_NRF24L01(void);