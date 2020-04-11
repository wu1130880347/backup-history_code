#include "boot2.h"
#include "usart.h"
#include "sys.h"
//debug info output
#if ((defined USART_DEBUG) && (USART_DEBUG == __ON__))
//是否打开该文件内的调试LOG
static const char EN_LOG = GREEN;
//LOG输出文件标记
static const char TAG[] = "boot2[boot2]: ";
#endif
typedef void (*pFunction)(void);
pFunction Jump_To_Application;

void Jump_To_App(void);

void Jump_To_App(void)
{
  Dprintf(EN_LOG, TAG, "Jump Address = 0x%8x\r\n", (__IO uint32_t *)IAPSTART);
  delay_ms(100);
  Jump_To_Application = (pFunction)(*(__IO uint32_t *)(IAPSTART + 4));
  __set_MSP(*(__IO uint32_t *)IAPSTART);
  Jump_To_Application();
}