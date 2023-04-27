#include "key.h"
#include "delay.h"
#include "usart.h"
#define row1(x)  x ? GPIO_SetBits(GPIOA, GPIO_Pin_8): GPIO_ResetBits(GPIOA, GPIO_Pin_8)
#define row2(x)  x ? GPIO_SetBits(GPIOA, GPIO_Pin_11): GPIO_ResetBits(GPIOA, GPIO_Pin_11)
#define row3(x)  x ? GPIO_SetBits(GPIOA, GPIO_Pin_12): GPIO_ResetBits(GPIOA, GPIO_Pin_12)
#define row4(x)  x ? GPIO_SetBits(GPIOA, GPIO_Pin_4): GPIO_ResetBits(GPIOA, GPIO_Pin_4)
u8 rowflag=5,endflag=0;
int linflag=1,key1_flag=0;
int lin=0,row=0;
	u8 t=0;
extern 	u8 key;
void EXTI_Configuration(void)
{	
		EXTI_InitTypeDef EXTI_InitStructure; 
		NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);

	EXTI_InitStructure.EXTI_Line=EXTI_Line6|EXTI_Line7|EXTI_Line8|EXTI_Line9; //ía2??D????Line15 | Line5
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt; // ?????D?????
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling; //?????'??
	EXTI_InitStructure.EXTI_LineCmd=ENABLE; //???D??
	EXTI_Init(&EXTI_InitStructure); 
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource6); 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource7);  
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource8); 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource9); 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//?D???????×é?????×é1?o1???????????3??×ó????		
	NVIC_InitStructure.NVIC_IRQChannel=EXTI9_5_IRQn; //EXTI9_5?D???'
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; //???èó????éè??????a0-1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0; //'óó????éè????????a0-7
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	
}


void key_Init(void)
	{
		GPIO_InitTypeDef GPIO_InitStructure;//éù?÷GPIO3?????1????	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);//??üGPIOB?ê±??
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_4|GPIO_Pin_11|GPIO_Pin_12; //??????S1-S4
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; //IO?????????
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz; 
	  GPIO_Init(GPIOA,&GPIO_InitStructure); //???PB13-PB10,?????S4-S1
				//GPIO_SetBits(GPIOB,GPIO_InitStructure.GPIO_Pin); //???????
		
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; //????1??5
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU ; //IO??????a???? 
		GPIO_Init(GPIOB,&GPIO_InitStructure);      
		GPIOB->ODR|=1<<6;
		GPIOB->ODR|=1<<7;      
		GPIOB->ODR|=1<<8;
		GPIOB->ODR|=1<<9;
		EXTI_Configuration();
}
	
void EXTI9_5_IRQHandler(void) //ía2??D???'15-10??D??×ó3ìD?
{		

	if(linflag==1)
	{
		key1_flag=1;
		delay_ms(100);
		if (EXTI_GetITStatus(EXTI_Line6)!=RESET)
		{			
			lin=0;
			EXTI_ClearFlag(EXTI_Line6);
		}
		if (EXTI_GetITStatus(EXTI_Line7)!=RESET)
		{			
			lin=1;
			EXTI_ClearFlag(EXTI_Line7);
		}
		if (EXTI_GetITStatus(EXTI_Line8)!=RESET)
		{		
      lin=2;				
			EXTI_ClearFlag(EXTI_Line8);
		}
		if (EXTI_GetITStatus(EXTI_Line9)!=RESET)
		{		
      lin=3;				
			EXTI_ClearFlag(EXTI_Line9);
		}
		linflag=0;
	}
	else if(endflag==1)
	{
		if (EXTI_GetITStatus(EXTI_Line6)!=RESET)
			EXTI_ClearFlag(EXTI_Line6);
		if (EXTI_GetITStatus(EXTI_Line7)!=RESET)
			EXTI_ClearFlag(EXTI_Line7);
		if (EXTI_GetITStatus(EXTI_Line8)!=RESET)
			EXTI_ClearFlag(EXTI_Line8);
		if (EXTI_GetITStatus(EXTI_Line9)!=RESET)
			EXTI_ClearFlag(EXTI_Line9);
	}
	else
	{
		if (EXTI_GetITStatus(EXTI_Line6)!=RESET)
		{
			
			row=rowflag;
			rowflag=5;
			endflag=1;
			EXTI_ClearFlag(EXTI_Line6);
		}
		if (EXTI_GetITStatus(EXTI_Line7)!=RESET)
		{
      row=rowflag;
			rowflag=5;
			endflag=1;
			EXTI_ClearFlag(EXTI_Line7);
		}
		if (EXTI_GetITStatus(EXTI_Line8)!=RESET)
		{	
			row=rowflag;
			rowflag=5;
			endflag=1;
			EXTI_ClearFlag(EXTI_Line8); 
	}
				if (EXTI_GetITStatus(EXTI_Line9)!=RESET)
		{	
			row=rowflag;
			rowflag=5;
			endflag=1;
			EXTI_ClearFlag(EXTI_Line9); 
	}
		key1_flag=0;
	if(row<4&&lin<4)
	{	key=(3-row)+lin*4+1;
	printf("ss:%d  row:%d  lin:%d \r\n",key,row,lin);
	
	delay_ms(200);
	}
	else
		key=0;
 }
}	

u8 key_load(void)
{
	row1(0);row2(0);row3(0);row4(0);
 if(linflag==0)
	{		
		row1(1);row2(1);row3(1);row4(1);
		if(endflag==0)
		{		
		  rowflag=0;row1(0);row2(1);row3(1);row4(1);
			delay_ms(5);
		}
		if(endflag==0)
		{	
		  rowflag=1;row1(1);row2(0);row3(1);row4(1);
			delay_ms(5);
		}
		if(endflag==0)
		{
		  rowflag=2;;row1(1);row2(1);row3(0);row4(1);
			delay_ms(5);
		}
				if(endflag==0)
		{
		  rowflag=3;row1(1);row2(1);row3(1);row4(0);
			delay_ms(5);
		}
		row1(0);row2(0);row3(0);row4(0);
		endflag=0;
		linflag=1;	
	}
}


	