
#include "rf24L01.h"
//===========================RF24L01�˿�����====================================
void RF24L01_IO_set(void)
{
      P1DIR &= 0xEF;    P1DIR |= 0x20;  P1SEL &= 0xCF; P1IE &= 0xCF;
      P2DIR = 0x80;  
      P3DIR &= 0xFB;    P3DIR |= 0x0B; P3SEL &= 0xF0;
}
//==============================================================================
//������uint SPI_RW(uint uu8)
//���ܣ�NRF24L01��SPIдʱ��
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
//������uu8 SPI_Read(uu8 reg)
//���ܣ�NRF24L01��SPIʱ��
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
//���ܣ�NRF24L01��д�Ĵ�������
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
//������uint SPI_Read_Buf(uu8 reg, uu8 *pBuf, uu8 uu8s)
//����: ���ڶ����ݣ�reg��Ϊ�Ĵ�����ַ��pBuf��Ϊ���������ݵ�ַ��uu8s���������ݵĸ���
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
//������uint SPI_Write_Buf(uu8 reg, uu8 *pBuf, uu8 uu8s)
//����: ����д���ݣ�Ϊ�Ĵ�����ַ��pBuf��Ϊ��д�����ݵ�ַ��uu8s��д�����ݵĸ���
//*********************************************************************************************************/
u8 SPI_Write_Buf(u8 reg, u8 *pBuf, u8 u8s)
{
	u8 status1,uu8_ctr;
	RF24L01_CSN_0;             //SPIʹ��       
	status1 = SPI_RW(reg);   
	for(uu8_ctr=0; uu8_ctr<u8s; uu8_ctr++) //
        {
	SPI_RW(*pBuf++);
        }
	RF24L01_CSN_1;           //�ر�SPI
	return(status1);    		  // 
}
//****************************************************************************************************/
//������void SetRX_Mode(void)
//���ܣ����ݽ������� 
//****************************************************************************************************/
void SetRX_Mode(void)
{
	RF24L01_CE_0 ;
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x7f);   		// IRQ�շ�����ж���Ӧ��16λCRC	��������
	RF24L01_CE_1; 
	Delay_us(150);//ע�ⲻ��̫С
}
void SetTX_Mode(void)
{
	RF24L01_CE_0;
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x7E);   		// IRQ�շ�����ж���Ӧ��16λCRC	��������
        RF24L01_CE_1;
	Delay_us(150);//ע�ⲻ��̫С
}
//******************************************************************************************************/
//������unsigned u8 nRF24L01_RxPacket(unsigned u8* rx_buf)
//���ܣ����ݶ�ȡ�����rx_buf���ջ�������
//******************************************************************************************************/
u8 nRF24L01_RxPacket(u8* rx_buf)
{
        u8 revale=0;
	sta=SPI_Read(STATUS);	     // ��ȡ״̬�Ĵ������ж����ݽ���״��		
	if(sta&0x40)                 // �ж��Ƿ���յ�����
	{
	    RF24L01_CE_0 ; 			//SPIʹ��
	    SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
	    revale =1;			//��ȡ������ɱ�־
	}
	SPI_RW_Reg(WRITE_REG+STATUS,sta);   //���յ����ݺ�RX_DR,TX_DS,MAX_PT���ø�Ϊ1��ͨ��д1������жϱ�־
	return revale;
}
//***********************************************************************************************************
//������void nRF24L01_TxPacket(u8 * tx_buf)
//���ܣ����� tx_buf������
//**********************************************************************************************************/
void nRF24L01_TxPacket(u8 *tx_buf)
{
	RF24L01_CE_0;			//StandBy Iģʽ	
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // װ�ؽ��ն˵�ַ
	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 	        // װ������	
   //     SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);   		        // IRQ�շ�����ж���Ӧ��16λCRC��������
	RF24L01_CE_1;		 //�ø�CE���������ݷ���
	Delay_us(20);
}
//****************************************************************************************
//NRF24L01��ʼ��
//***************************************************************************************/
void init_NRF24L01(void)
{
 	RF24L01_CE_0;    // chip enable
 	RF24L01_CSN_1;   // Spi disable 
 	RF24L01_SCK_0;   // Spi clock line init high
	SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // д���ص�ַ	
	SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); // д���ն˵�ַ
	SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      //  Ƶ��0�Զ�	ACKӦ������	
	SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  //  ������յ�ַֻ��Ƶ��0�������Ҫ��Ƶ�����Բο�Page21  
	SPI_RW_Reg(WRITE_REG + RF_CH, 0);        //   �����ŵ�����Ϊ2.4GHZ���շ�����һ��
	SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //���ý������ݳ��ȣ���������Ϊ32�ֽ�
	SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);   //���÷�������Ϊ1MHZ�����书��Ϊ���ֵ0dB
	SPI_RW_Reg(WRITE_REG + CONFIG, 0x7E);   // IRQ�շ�����ж���Ӧ��16λCRC	��������}
        Delay_ms(2);
}
