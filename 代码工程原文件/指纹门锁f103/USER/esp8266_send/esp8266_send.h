#ifndef _esp8266_send_H_
#define _esp8266_send_H_
#include "stm32f10x.h"
#include <stdio.h>
#include "string.h"
#include "usart.h"
#include "esp8266.h"
#include "onenet.h"
#include "edpkit.h"


void esp8266_rec(void);				/*�������е��������н��գ����ݴ�������������ֻ�ǵ���*/
void data_pro(char *req);			/*�������ݽ��д���*/
void data_send(void);					/*Ҫ���͵�����*/


#endif

