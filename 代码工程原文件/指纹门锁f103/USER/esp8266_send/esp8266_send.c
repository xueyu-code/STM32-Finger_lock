#include "esp8266_send.h"


extern u8 door;

void esp8266_rec(void)			/*主程序中调用来进行接收，数据处理不用这个，这个只是调用*/
{
	  unsigned char *dataPtr = NULL;
		dataPtr = ESP8266_GetIPD(0);
		if(dataPtr != NULL)
			OneNet_RevPro(dataPtr);
}
/*数据处理是下面这个子函数*/
void data_pro(char *req)			/*接收数据进行处理*/
{
	char *dataPtr = NULL;
	char numBuf[10];
	int num = 0;
	
	dataPtr = strchr(req, ':');							//搜索':'
	if(dataPtr != NULL)									//如果找到了
	{
		dataPtr++;
		
		while(*dataPtr >= '0' && *dataPtr <= '9')		//判断是否是下发的命令控制数据
		{
			numBuf[num++] = *dataPtr++;
		}
		
		num = atoi((const char *)numBuf);				//转为数值形式
		
		printf("flag:%d\r\n",num);
		if(strstr((char *)req, "door"))				//搜索"redled"
		{
		door=num;
		}		
	}	
}

void data_send(void)				/*要发送的数据*/
{
	/*
		要发送的数据在onenet.c中的OneNet_FillBuf()子函数那里，这里只是说明，没有用
	*/
}

