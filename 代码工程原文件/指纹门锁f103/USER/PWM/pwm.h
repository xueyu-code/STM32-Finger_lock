#ifndef __STM32F10X_H
#define __STM32F10X_H
#include "stm32f10x.h"
#endif
 
void init_hardware_pwm(u16 arr, u16 psc);
void func_pwm_enable(void);
void func_pwm_disable(void);
void func_set_pwm_compare(u8 rate);