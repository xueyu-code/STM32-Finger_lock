#include "AS608_use.h"
#include "OLED_I2C.h"
#include "usart2.h"
extern SysPara AS608Para;							/*ָ��ģ��AS608����*/
extern u16 ValidN;											/*ģ������Чָ�Ƹ���*/
extern u8 suo_flag;
//��ʾȷ���������Ϣ
void ShowErrMessage(u8 ensure)
{
	printf("\r\n������Ϣ��%s \r\n",(u8*)EnsureMessage(ensure));
}

//ˢָ��
void press_FR(void)
{
	SearchResult seach;
	u8 ensure;
	ensure=PS_GetImage();
	if(ensure==0x00)//��ȡͼ��ɹ� 
	{	

		ensure=PS_GenChar(CharBuffer1);
		if(ensure==0x00) //���������ɹ�
		{		
			ensure=PS_HighSpeedSearch(CharBuffer1,0,AS608Para.PS_max,&seach);
			if(ensure==0x00)//�����ɹ�
			{		
   		OLED_Clear()  	; 	
		OLED_ShowCHinese(28,2,7);
		OLED_ShowCHinese(46,2,8);
		OLED_ShowCHinese(64,2,27);
		OLED_ShowCHinese(82,2,28);//ָ����ȷ
				delay_ms(1000);	
				suo_flag=1;
			}
			else {
		OLED_ShowCHinese(28,2,7);
		OLED_ShowCHinese(46,2,8);
		OLED_ShowCHinese(64,2,30);
		OLED_ShowCHinese(82,2,31);//ָ�ƴ���
		ShowErrMessage(ensure);
		delay_ms(1000);						
			}				
	  }
		else
			ShowErrMessage(ensure);

	}
		
}

//¼ָ��
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
    OLED_ShowCHinese(92,0,8);//������ָ��
			delay(1);
				ensure=PS_GetImage();		
				if(ensure==0x00) 
				{
					ensure=PS_GenChar(CharBuffer1);//��������
					if(ensure==0x00)
					{
						i=0;
						processnum=1;//�����ڶ���						
					}else ShowErrMessage(ensure);				
				}else ShowErrMessage(ensure);						
				break;
			
			case 1:
				i++;
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure=PS_GenChar(CharBuffer2);//��������
					if(ensure==0x00)
					{
						i=0;
						processnum=2;//����������
					}else {
						ShowErrMessage(ensure);	
								OLED_Clear()  	; 	
		OLED_ShowCHinese(28,0,21);
		OLED_ShowCHinese(46,0,22);
		OLED_ShowCHinese(64,0,23);
		OLED_ShowCHinese(82,0,24);//�Ա�ʧ��
		OLED_ShowCHinese(20,2,4);
		OLED_ShowCHinese(38,2,25);
		OLED_ShowCHinese(56,2,26);
		OLED_ShowCHinese(74,2,5);
    OLED_ShowCHinese(92,2,6);//������ָ��
					delay_ms(3000);
					}
					}else {
						ShowErrMessage(ensure);	
		OLED_Clear()  	; 	
		OLED_ShowCHinese(28,0,21);
		OLED_ShowCHinese(46,0,22);
		OLED_ShowCHinese(64,0,23);
		OLED_ShowCHinese(82,0,24);//�Ա�ʧ��
		OLED_ShowCHinese(20,2,4);
		OLED_ShowCHinese(38,2,25);
		OLED_ShowCHinese(56,2,26);
		OLED_ShowCHinese(74,2,5);
    OLED_ShowCHinese(92,2,6);//������ָ��
						delay_ms(3000);
					}						
				break;

			case 2:
				ensure=PS_Match();
				if(ensure==0x00) 
				{
					processnum=3;//�������Ĳ�///�Աȳɹ�
				}
				else 
				{
			OLED_Clear()  	; 	
		OLED_ShowCHinese(20,0,4);
	OLED_ShowCHinese(38,0,5);
		OLED_ShowCHinese(56,0,6);
		OLED_ShowCHinese(74,0,7);
    	OLED_ShowCHinese(92,0,8);//������ָ��
					delay(1);
					printf("�Ա�ʧ�ܣ�������¼��ָ��\r\n");
					ShowErrMessage(ensure);
					i=0;
					processnum=0;//���ص�һ��		
				}
				delay_ms(1200);
				break;

			case 3:
				printf("����ָ��ģ��\r\n");
				ensure=PS_RegModel();
				if(ensure==0x00) 
				{
					printf("����ָ��ģ��ɹ�\r\n");
					 l=1;	
					processnum=4;//�������岽
				}else {processnum=0;ShowErrMessage(ensure);}
				delay_ms(1200);
				break;
				
			case 4:	
				printf("����ָ��ID\r\n");
				printf("0=< ID <=299\r\n");
			printf("ID:%d\r\n",FR_ID);
				printf("ID_max:%d",AS608Para.PS_max);
			AS608Para.PS_max=30;
				do
					ID=FR_ID;
				while(!(ID<AS608Para.PS_max));//����ID����С��ָ�����������ֵ
				ensure=PS_StoreChar(CharBuffer2,ID);//����ģ��
				if(ensure==0x00) 
				{					
		OLED_Clear()  	; 	
		 OLED_ShowCHinese(28,1,10);
	  OLED_ShowCHinese(46,1,6);
		OLED_ShowCHinese(64,1,11);
		OLED_ShowCHinese(82,1,12);//¼��ɹ�
					delay(1);
					PS_ValidTempleteNum(&ValidN);//����ָ�Ƹ���
					return ;
				}else {processnum=0;ShowErrMessage(ensure);}				
				break;				
		}
		delay_ms(1000);
		if(i==5)//����5��û�а���ָ���˳�
		{
				OLED_Clear()  	; 	
		OLED_ShowCHinese(15,1,15);
	  OLED_ShowCHinese(33,1,16);
		OLED_ShowCHinese(51,1,17);
		OLED_ShowCHinese(69,1,18);
    OLED_ShowCHinese(87,1,19);
		OLED_ShowCHinese(105,1,20);//����5���˳�
			OLED_Clear();
		OLED_ShowCHinese(28,1,0);
	OLED_ShowCHinese(46,1,1);
		OLED_ShowCHinese(64,1,2);
		OLED_ShowCHinese(82,1,3);//��������			
			delay(1);
			break;	
		}	
	}
}

//ɾ��ָ��
void Del_FR(u16 FR_ID)	/*����ɾ��ָ��ID*/
{
	u8  ensure;
				OLED_Clear()  	;//ɾ��ָ��m
		OLED_ShowCHinese(20,1,13);
	OLED_ShowCHinese(38,1,14);
		OLED_ShowCHinese(56,1,7);
		OLED_ShowCHinese(74,1,8);	
    	OLED_ShowNum(92,1,FR_ID,1,16);//������ָ��
	delay(1);
	ensure=PS_DeletChar(FR_ID,1);//ɾ������ָ��
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
	PS_ValidTempleteNum(&ValidN);//����ָ�Ƹ���
					OLED_Clear()  	;//ɾ��ָ��m
		OLED_ShowCHinese(20,1,13);
	OLED_ShowCHinese(38,1,14);
		OLED_ShowCHinese(56,1,11);
		OLED_ShowCHinese(74,1,12);
	delay(1);

}

