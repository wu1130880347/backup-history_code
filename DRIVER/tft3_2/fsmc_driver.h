#ifndef __Driver__
#define __Driver__
#include "sys.h"
//lcd
	    

//extern LCD;
void Driver_LcdFSMCON(void);
extern void Driver_LcdFSMCWriteData(u16 Data);
void Driver_LcdFSMCWriteReg(u16 Index);
#endif



