#include "esp8266_send.h"


extern u8 door;

void esp8266_rec(void)			/*�������е��������н��գ����ݴ�������������ֻ�ǵ���*/
{
	  unsigned char *dataPtr = NULL;
		dataPtr = ESP8266_GetIPD(0);
		if(dataPtr != NULL)
			OneNet_RevPro(dataPtr);
}
/*���ݴ�������������Ӻ���*/
void data_pro(char *req)			/*�������ݽ��д���*/
{
	char *dataPtr = NULL;
	char numBuf[10];
	int num = 0;
	
	dataPtr = strchr(req, ':');							//����':'
	if(dataPtr != NULL)									//����ҵ���
	{
		dataPtr++;
		
		while(*dataPtr >= '0' && *dataPtr <= '9')		//�ж��Ƿ����·��������������
		{
			numBuf[num++] = *dataPtr++;
		}
		
		num = atoi((const char *)numBuf);				//תΪ��ֵ��ʽ
		
		printf("flag:%d\r\n",num);
		if(strstr((char *)req, "door"))				//����"redled"
		{
		door=num;
		}		
	}	
}

void data_send(void)				/*Ҫ���͵�����*/
{
	/*
		Ҫ���͵�������onenet.c�е�OneNet_FillBuf()�Ӻ����������ֻ��˵����û����
	*/
}

