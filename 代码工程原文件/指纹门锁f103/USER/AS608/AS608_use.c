#include "AS608_use.h"
#include "OLED_I2C.h"
#include "usart2.h"
extern SysPara AS608Para;							/*指纹模块AS608参数*/
extern u16 ValidN;											/*模块内有效指纹个数*/
extern u8 suo_flag;
//显示确认码错误信息
void ShowErrMessage(u8 ensure)
{
	printf("\r\n错误信息：%s \r\n",(u8*)EnsureMessage(ensure));
}

//刷指纹
void press_FR(void)
{
	SearchResult seach;
	u8 ensure;
	ensure=PS_GetImage();
	if(ensure==0x00)//获取图像成功 
	{	

		ensure=PS_GenChar(CharBuffer1);
		if(ensure==0x00) //生成特征成功
		{		
			ensure=PS_HighSpeedSearch(CharBuffer1,0,AS608Para.PS_max,&seach);
			if(ensure==0x00)//搜索成功
			{		
   		OLED_Clear()  	; 	
		OLED_ShowCHinese(28,2,7);
		OLED_ShowCHinese(46,2,8);
		OLED_ShowCHinese(64,2,27);
		OLED_ShowCHinese(82,2,28);//指纹正确
				delay_ms(1000);	
				suo_flag=1;
			}
			else {
		OLED_ShowCHinese(28,2,7);
		OLED_ShowCHinese(46,2,8);
		OLED_ShowCHinese(64,2,30);
		OLED_ShowCHinese(82,2,31);//指纹错误
		ShowErrMessage(ensure);
		delay_ms(1000);						
			}				
	  }
		else
			ShowErrMessage(ensure);

	}
		
}

//录指纹
void Add_FR(u16 FR_ID)
{
	u8 i,ensure ,processnum=0,l;
	u16 ID;
	while(1)
	{
		switch (processnum)
		{
			case 0:
				i++;
		OLED_Clear()  	; 	
		OLED_ShowCHinese(20,0,4);
		OLED_ShowCHinese(38,0,5);
		OLED_ShowCHinese(56,0,6);
		OLED_ShowCHinese(74,0,7);
    OLED_ShowCHinese(92,0,8);//请输入指纹
			delay(1);
				ensure=PS_GetImage();		
				if(ensure==0x00) 
				{
					ensure=PS_GenChar(CharBuffer1);//生成特征
					if(ensure==0x00)
					{
						i=0;
						processnum=1;//跳到第二步						
					}else ShowErrMessage(ensure);				
				}else ShowErrMessage(ensure);						
				break;
			
			case 1:
				i++;
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure=PS_GenChar(CharBuffer2);//生成特征
					if(ensure==0x00)
					{
						i=0;
						processnum=2;//跳到第三步
					}else {
						ShowErrMessage(ensure);	
								OLED_Clear()  	; 	
		OLED_ShowCHinese(28,0,21);
		OLED_ShowCHinese(46,0,22);
		OLED_ShowCHinese(64,0,23);
		OLED_ShowCHinese(82,0,24);//对比失败
		OLED_ShowCHinese(20,2,4);
		OLED_ShowCHinese(38,2,25);
		OLED_ShowCHinese(56,2,26);
		OLED_ShowCHinese(74,2,5);
    OLED_ShowCHinese(92,2,6);//请输入指纹
					delay_ms(3000);
					}
					}else {
						ShowErrMessage(ensure);	
		OLED_Clear()  	; 	
		OLED_ShowCHinese(28,0,21);
		OLED_ShowCHinese(46,0,22);
		OLED_ShowCHinese(64,0,23);
		OLED_ShowCHinese(82,0,24);//对比失败
		OLED_ShowCHinese(20,2,4);
		OLED_ShowCHinese(38,2,25);
		OLED_ShowCHinese(56,2,26);
		OLED_ShowCHinese(74,2,5);
    OLED_ShowCHinese(92,2,6);//请输入指纹
						delay_ms(3000);
					}						
				break;

			case 2:
				ensure=PS_Match();
				if(ensure==0x00) 
				{
					processnum=3;//跳到第四步///对比成功
				}
				else 
				{
			OLED_Clear()  	; 	
		OLED_ShowCHinese(20,0,4);
	OLED_ShowCHinese(38,0,5);
		OLED_ShowCHinese(56,0,6);
		OLED_ShowCHinese(74,0,7);
    	OLED_ShowCHinese(92,0,8);//请输入指纹
					delay(1);
					printf("对比失败，请重新录入指纹\r\n");
					ShowErrMessage(ensure);
					i=0;
					processnum=0;//跳回第一步		
				}
				delay_ms(1200);
				break;

			case 3:
				printf("生成指纹模板\r\n");
				ensure=PS_RegModel();
				if(ensure==0x00) 
				{
					printf("生成指纹模板成功\r\n");
					 l=1;	
					processnum=4;//跳到第五步
				}else {processnum=0;ShowErrMessage(ensure);}
				delay_ms(1200);
				break;
				
			case 4:	
				printf("储存指纹ID\r\n");
				printf("0=< ID <=299\r\n");
			printf("ID:%d\r\n",FR_ID);
				printf("ID_max:%d",AS608Para.PS_max);
			AS608Para.PS_max=30;
				do
					ID=FR_ID;
				while(!(ID<AS608Para.PS_max));//输入ID必须小于指纹容量的最大值
				ensure=PS_StoreChar(CharBuffer2,ID);//储存模板
				if(ensure==0x00) 
				{					
		OLED_Clear()  	; 	
		 OLED_ShowCHinese(28,1,10);
	  OLED_ShowCHinese(46,1,6);
		OLED_ShowCHinese(64,1,11);
		OLED_ShowCHinese(82,1,12);//录入成功
					delay(1);
					PS_ValidTempleteNum(&ValidN);//读库指纹个数
					return ;
				}else {processnum=0;ShowErrMessage(ensure);}				
				break;				
		}
		delay_ms(1000);
		if(i==5)//超过5次没有按手指则退出
		{
				OLED_Clear()  	; 	
		OLED_ShowCHinese(15,1,15);
	  OLED_ShowCHinese(33,1,16);
		OLED_ShowCHinese(51,1,17);
		OLED_ShowCHinese(69,1,18);
    OLED_ShowCHinese(87,1,19);
		OLED_ShowCHinese(105,1,20);//超过5次退出
			OLED_Clear();
		OLED_ShowCHinese(28,1,0);
	OLED_ShowCHinese(46,1,1);
		OLED_ShowCHinese(64,1,2);
		OLED_ShowCHinese(82,1,3);//智能门锁			
			delay(1);
			break;	
		}	
	}
}

//删除指纹
void Del_FR(u16 FR_ID)	/*输入删除指纹ID*/
{
	u8  ensure;
				OLED_Clear()  	;//删除指纹m
		OLED_ShowCHinese(20,1,13);
	OLED_ShowCHinese(38,1,14);
		OLED_ShowCHinese(56,1,7);
		OLED_ShowCHinese(74,1,8);	
    	OLED_ShowNum(92,1,FR_ID,1,16);//请输入指纹
	delay(1);
	ensure=PS_DeletChar(FR_ID,1);//删除单个指纹
	if(ensure==0)
	{
		OLED_Clear()  	;
				OLED_ShowCHinese(20,1,13);
	OLED_ShowCHinese(38,1,14);
		OLED_ShowCHinese(56,1,11);
		OLED_ShowCHinese(74,1,12);
	}
  else
		ShowErrMessage(ensure);	
	delay_ms(1200);
	PS_ValidTempleteNum(&ValidN);//读库指纹个数
					OLED_Clear()  	;//删除指纹m
		OLED_ShowCHinese(20,1,13);
	OLED_ShowCHinese(38,1,14);
		OLED_ShowCHinese(56,1,11);
		OLED_ShowCHinese(74,1,12);
	delay(1);

}

