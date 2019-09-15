#include "adc.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//ADC 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/6
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	


//初始化ADC															   
void  Adc_Init(void)
{    
        GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
        NVIC_InitTypeDef   NVIC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟

  //先初始化ADC1通道5 IO口
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//PA5 通道5
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
        GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化  
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//初始化
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
  ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化
  
  ADC_ITConfig(ADC1,ADC_IT_EOC,DISABLE);
  ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
  
  NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;//ADC中断0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
	
  ADC_Cmd(ADC1, ENABLE);//开启AD转换器	
  
  ADC_RegularChannelConfig(ADC1,ADC_Channel_5,1,ADC_SampleTime_480Cycles );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度			    
  ADC_SoftwareStartConv(ADC1);		//使能指定的ADC1的软件转换启动功能

}				  
//获得ADC值
//ch: @ref ADC_channels 
//通道值 0~16取值范围为：ADC_Channel_0~ADC_Channel_16
//返回值:转换结果
u16 Get_Adc(u8 ch)   
{
	  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度			    
  
	ADC_SoftwareStartConv(ADC1);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}
//获取通道ch的转换值，取times次,然后平均 
//ch:通道编号
//times:获取次数
//返回值:通道ch的times次转换结果平均值
u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
                //LED1 ^= 1;
		//delay_ms(5);
	}
	return temp_val/times;
} 
	 
void ADC_IRQHandler(void)
{
    static u32 adc_data = 0,index = 0;
    static u8 adc_flag = 0;
    
    //if(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC))
   // {
      adc_data += ADC_GetConversionValue(ADC1);
      adc_flag++;      
      if(adc_flag == 33)
      {
        index = adc_data;
        index <<= 1;
        //adc_data *= 10;
        adc_data <<= 3;
        adc_data += index;
        adc_data >>= 12;
        OLED_ShowNum(0,1,(u16)adc_data,3,8);
     //   delay_ms(500);
        adc_flag = 0;
      }
   // }
    //OLED_ShowNum(0,1,(u16)adc_data,5,8);
    ADC_ClearFlag(ADC1,ADC_FLAG_EOC);
    ADC_RegularChannelConfig(ADC1,ADC_Channel_5,1,ADC_SampleTime_480Cycles );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度			    
    ADC_SoftwareStartConv(ADC1);		//使能指定的ADC1的软件转换启动功能
}








