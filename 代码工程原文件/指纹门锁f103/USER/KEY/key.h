#ifndef __KEY_H
#define	__KEY_H

#include "sys.h"

#define uchar unsigned char 
#define uint unsigned int
/******************************** ???? ------------------------------------*/


u8 key_load(void);
u8 key_scan(void);
void key_Init(void);
void EXTI_Configuration(void);
	
#endif
