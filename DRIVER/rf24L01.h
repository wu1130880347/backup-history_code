#include "lcd_12864.h"
//延时函数，IAR自带，经常使用到
#define CPU_F ((double)8000000)   //外部高频晶振8MHZ
#define Delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) 
#define Delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0)) 

//自定义数据结构，方便使用
#define u8 unsigned char
#define u16  unsigned int
#define u32 unsigned long

//8个LED灯，连接在P6口，可通过断开电源停止使用，ADC使用时断开电源
#define LED8DIR         P6DIR
#define LED8            P6OUT                             //P6口接LED灯，8个


//串口波特率计算，当BRCLK=CPU_F时用下面的公式可以计算，否则要根据设置加入分频系数
#define baud           9600                                //设置波特率的大小
#define baud_setting   (uint)((ulong)CPU_F/((ulong)baud))  //波特率计算公式
#define baud_h         (uchar)(baud_setting>>8)            //提取高位
#define baud_l         (uchar)(baud_setting)               //低位


//NRF2401模块控制线
#define  RF24L01_CE_0        P1OUT &=~BIT5         //CE在P15         
#define  RF24L01_CE_1        P1OUT |= BIT5        

#define  RF24L01_CSN_0       P2OUT &=~BIT7         //CS在P27
#define  RF24L01_CSN_1       P2OUT |= BIT7     

#define  RF24L01_SCK_0       P3OUT &=~BIT3         //SCK在P33
#define  RF24L01_SCK_1       P3OUT |= BIT3   

#define  RF24L01_MISO_0      P3OUT &=~BIT2         //MISO在P32
#define  RF24L01_MISO_1      P3OUT |= BIT2

#define  RF24L01_MOSI_0      P3OUT &=~BIT1         //MOSI在P31
#define  RF24L01_MOSI_1      P3OUT |= BIT1

#define  RF24L01_IRQ_0       P1OUT &=~BIT4         //IRQ在P14     
#define  RF24L01_IRQ_1       P1OUT |= BIT4


static u8  sta;
static u8 TxBuf[8]={'A','B','C','D','E','F','G','H'};
static u8  RxBuf[8]={0};

//==========================NRF24L01============================================
#define TX_ADR_WIDTH    5   	// 5 uints TX address width
#define RX_ADR_WIDTH    5   	// 5 uints RX address width
#define TX_PLOAD_WIDTH  8  	// 32 TX payload
#define RX_PLOAD_WIDTH  8  	// 32 uints TX payload
//=========================NRF24L01寄存器指令===================================
#define READ_REG        0x00  	// 读寄存器指令
#define WRITE_REG       0x20 	// 写寄存器指令
#define RD_RX_PLOAD     0x61  	// 读取接收数据指令
#define WR_TX_PLOAD     0xA0  	// 写待发数据指令
#define FLUSH_TX        0xE1 	// 冲洗发送 FIFO指令
#define FLUSH_RX        0xE2  	// 冲洗接收 FIFO指令
#define REUSE_TX_PL     0xE3  	// 定义重复装载数据指令
#define NOP1            0xFF  	// 保留
//========================SPI(nRF24L01)寄存器地址===============================
#define CONFIG          0x00  // 配置收发状态，CRC校验模式以及收发状态响应方式
#define EN_AA           0x01  // 自动应答功能设置
#define EN_RXADDR       0x02  // 可用信道设置
#define SETUP_AW        0x03  // 收发地址宽度设置
#define SETUP_RETR      0x04  // 自动重发功能设置
#define RF_CH           0x05  // 工作频率设置
#define RF_SETUP        0x06  // 发射速率、功耗功能设置
#define STATUS          0x07  // 状态寄存器
#define OBSERVE_TX      0x08  // 发送监测功能
#define CD              0x09  // 地址检测           
#define RX_ADDR_P0      0x0A  // 频道0接收数据地址
#define RX_ADDR_P1      0x0B  // 频道1接收数据地址
#define RX_ADDR_P2      0x0C  // 频道2接收数据地址
#define RX_ADDR_P3      0x0D  // 频道3接收数据地址
#define RX_ADDR_P4      0x0E  // 频道4接收数据地址
#define RX_ADDR_P5      0x0F  // 频道5接收数据地址
#define TX_ADDR         0x10  // 发送地址寄存器
#define RX_PW_P0        0x11  // 接收频道0接收数据长度
#define RX_PW_P1        0x12  // 接收频道0接收数据长度
#define RX_PW_P2        0x13  // 接收频道0接收数据长度
#define RX_PW_P3        0x14  // 接收频道0接收数据长度
#define RX_PW_P4        0x15  // 接收频道0接收数据长度
#define RX_PW_P5        0x16  // 接收频道0接收数据长度
#define FIFO_STATUS     0x17  // FIFO栈入栈出状态寄存器设置

//=============================RF24l01状态=====================================
static u8 TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//本地地址
static u8 RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};	//接收地址
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