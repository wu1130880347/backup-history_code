
#include "rf24L01.h"
//===========================RF24L01端口设置====================================
void RF24L01_IO_set(void)
{
      P1DIR &= 0xEF;    P1DIR |= 0x20;  P1SEL &= 0xCF; P1IE &= 0xCF;
      P2DIR = 0x80;  
      P3DIR &= 0xFB;    P3DIR |= 0x0B; P3SEL &= 0xF0;
}
//==============================================================================
//函数：uint SPI_RW(uint uu8)
//功能：NRF24L01的SPI写时序
//******************************************************************************
u8 SPI_RW(u8 data)
{
	u8 i,temp=0;
   	for(i=0;i<8;i++)                // output 8-bit
   	{
	if((data & 0x80)==0x80)
	{
		RF24L01_MOSI_1;         // output 'uu8', MSB to MOSI
	}
	else
	{
	 	RF24L01_MOSI_0; 
	}	
//==============================================================================
		data = (data << 1);            // shift next bit into MSB..
		temp<<=1;
		RF24L01_SCK_1;                // Set SCK high..
		if((P3IN&0x04)==0x04)temp++;         // capture current MISO bit
		RF24L01_SCK_0;              // ..then set SCK low again
   	}
    return(temp);           		  // return read uu8
}
//****************************************************************************************************
//函数：uu8 SPI_Read(uu8 reg)
//功能：NRF24L01的SPI时序
//****************************************************************************************************
u8 SPI_Read(u8 reg)
{
	u8 reg_val;
	RF24L01_CSN_0;           // CSN low, initialize SPI communication...
	SPI_RW(reg);            // Select register to read from..
	reg_val = SPI_RW(0);    // ..then read registervalue
	RF24L01_CSN_1;         // CSN high, terminate SPI communication
	return(reg_val);       // return register value
}
//****************************************************************************************************/
//功能：NRF24L01读写寄存器函数
//****************************************************************************************************/
u8 SPI_RW_Reg(u8 reg, u8 value)
{
	u8 status1;
	RF24L01_CSN_0;                   // CSN low, init SPI transaction
	status1 = SPI_RW(reg);      // select register
	SPI_RW(value);             // ..and write value to it..
	RF24L01_CSN_1;                   // CSN high again
	return (status1);            // return nRF24L01 status uu8
}
//****************************************************************************************************/
//函数：uint SPI_Read_Buf(uu8 reg, uu8 *pBuf, uu8 uu8s)
//功能: 用于读数据，reg：为寄存器地址，pBuf：为待读出数据地址，uu8s：读出数据的个数
//****************************************************************************************************/
u8 SPI_Read_Buf(u8 reg, u8 *pBuf, u8 u8s)
{
	u8 status2,uu8_ctr;
	RF24L01_CSN_0;                    		// Set CSN low, init SPI tranaction
	status2 = SPI_RW(reg);       		// Select register to write to and read status uu8
	for(uu8_ctr=0;uu8_ctr<u8s;uu8_ctr++)
        {
	pBuf[uu8_ctr] = SPI_RW(0);    // 
        }
	RF24L01_CSN_1;                           
	return(status2);                    // return nRF24L01 status uu8
}
//*********************************************************************************************************
//函数：uint SPI_Write_Buf(uu8 reg, uu8 *pBuf, uu8 uu8s)
//功能: 用于写数据：为寄存器地址，pBuf：为待写入数据地址，uu8s：写入数据的个数
//*********************************************************************************************************/
u8 SPI_Write_Buf(u8 reg, u8 *pBuf, u8 u8s)
{
	u8 status1,uu8_ctr;
	RF24L01_CSN_0;             //SPI使能       
	status1 = SPI_RW(reg);   
	for(uu8_ctr=0; uu8_ctr<u8s; uu8_ctr++) //
        {
	SPI_RW(*pBuf++);
        }
	RF24L01_CSN_1;           //关闭SPI
	return(status1);    		  // 
}
//****************************************************************************************************/
//函数：void SetRX_Mode(void)
//功能：数据接收配置 
//****************************************************************************************************/
void SetRX_Mode(void)
{
	RF24L01_CE_0 ;
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x7f);   		// IRQ收发完成中断响应，16位CRC	，主接收
	RF24L01_CE_1; 
	Delay_us(150);//注意不能太小
}
void SetTX_Mode(void)
{
	RF24L01_CE_0;
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x7E);   		// IRQ收发完成中断响应，16位CRC	，主接收
        RF24L01_CE_1;
	Delay_us(150);//注意不能太小
}
//******************************************************************************************************/
//函数：unsigned u8 nRF24L01_RxPacket(unsigned u8* rx_buf)
//功能：数据读取后放如rx_buf接收缓冲区中
//******************************************************************************************************/
u8 nRF24L01_RxPacket(u8* rx_buf)
{
        u8 revale=0;
	sta=SPI_Read(STATUS);	     // 读取状态寄存其来判断数据接收状况		
	if(sta&0x40)                 // 判断是否接收到数据
	{
	    RF24L01_CE_0 ; 			//SPI使能
	    SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
	    revale =1;			//读取数据完成标志
	}
	SPI_RW_Reg(WRITE_REG+STATUS,sta);   //接收到数据后RX_DR,TX_DS,MAX_PT都置高为1，通过写1来清楚中断标志
	return revale;
}
//***********************************************************************************************************
//函数：void nRF24L01_TxPacket(u8 * tx_buf)
//功能：发送 tx_buf中数据
//**********************************************************************************************************/
void nRF24L01_TxPacket(u8 *tx_buf)
{
	RF24L01_CE_0;			//StandBy I模式	
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // 装载接收端地址
	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 	        // 装载数据	
   //     SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);   		        // IRQ收发完成中断响应，16位CRC，主发送
	RF24L01_CE_1;		 //置高CE，激发数据发送
	Delay_us(20);
}
//****************************************************************************************
//NRF24L01初始化
//***************************************************************************************/
void init_NRF24L01(void)
{
 	RF24L01_CE_0;    // chip enable
 	RF24L01_CSN_1;   // Spi disable 
 	RF24L01_SCK_0;   // Spi clock line init high
	SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // 写本地地址	
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); // 写接收端地址
	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      //  频道0自动	ACK应答允许	
	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  //  允许接收地址只有频道0，如果需要多频道可以参考Page21  
	SPI_RW_Reg(WRITE_REG + RF_CH, 0);        //   设置信道工作为2.4GHZ，收发必须一致
	SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //设置接收数据长度，本次设置为32字节
	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);   //设置发射速率为1MHZ，发射功率为最大值0dB
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x7E);   // IRQ收发完成中断响应，16位CRC	，主发射}
        Delay_ms(2);
}
