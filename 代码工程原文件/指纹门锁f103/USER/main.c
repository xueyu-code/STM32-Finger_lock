#include "public.h"
#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_i2c.h"
#include "stm32f10x_exti.h"
#include "oled.h"
#include "as608.h"
#include "AS608_use.h"
#include "time.h"
#include "usart2.h"
#include "usart.h"
#include "string.h"
#include "beep.h"
#include "key.h"
#include "pwm.h"
#define usart2_baund  57600			/*串口2波特率，根据指纹模块波特率更改*/
SysPara AS608Para;							/*指纹模块AS608参数*/
u16 ValidN;		
u8 suo_flag=0,add_hao;
u8 key;
float price;
u8 plen=0,key_diff,key_flag;
u8 xianshi[18]={20,1,2,3,10,4,5,6,11,7,8,9,12,13,0,14,15,16};
u8 str[6]; 
u8 KEY[30]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x32};
u8 NewKey[6]={0xff,0xff,0xff,0xff,0xff,0x32};
u8 key_str[7]={' ',' ',' ',' ',' ',' ','\0'};
u8 mima_str[6]={6,6,6,6,6,6};
u8 i,j;
void suo_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能GPIOB端口时钟
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_12;				 //BEEP-->PB.8 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);	 //根据参数初始化GPIOB.8
 
 GPIO_ResetBits(GPIOB,GPIO_Pin_5|GPIO_Pin_12);//输出0，关闭蜂鸣器输出

}


void mima_set()
{
				OLED_ShowCHinese(32,0,38);
				OLED_ShowCHinese(48,0,39);  
				OLED_ShowCHinese(64,0,32);
				OLED_ShowCHinese(80,0,33); //修改密码
	while(1)
	{
		key_load();
		delay_ms(100);
		if(0<=xianshi[key]&&xianshi[key]<10)
		{
			key_str[plen]=48+xianshi[key];
			plen++;
			for(i=plen;i<6;i++) key_str[i]=' ';
			OLED_ShowString(40,2,key_str,16);
			if(plen==6)
			{
				key_diff=1;
				for(i=0;i<6;i++)
				{
					if(mima_str[i]!=(key_str[i]-48))
						key_diff=0;					
				}
				if(key_diff)
				{							
				OLED_ShowCHinese(32,4,32);
				OLED_ShowCHinese(48,4,33);  
				OLED_ShowCHinese(64,4,36);
				OLED_ShowCHinese(80,4,37);
				delay_ms(3000);
				OLED_ShowString(32,4,"          ",16);					
				}
				else
				{
				OLED_ShowCHinese(32,4,32);
				OLED_ShowCHinese(48,4,33);  
				OLED_ShowCHinese(64,4,30);
				OLED_ShowCHinese(80,4,31);
						//密码错误
				delay_ms(3000);
				OLED_ShowString(32,4,"          ",16);
				BEEP=0;
				}
				plen=0;
		for(i=0;i<6;i++)
			key_str[i]=' ';
			OLED_ShowString(40,2,key_str,16);
				break;
			}
			key=0;
		}
  }
	key=0;
	if(key_diff)
	{
	while(1)
	{
		key_load();
		delay_ms(200);
		if(0<=xianshi[key]&&xianshi[key]<10)
		{
			key_str[plen]=48+xianshi[key];
			plen++;
			for(i=plen;i<6;i++) key_str[i]=' ';
			OLED_ShowString(40,2,key_str,16);
			if(plen==6)
			{
				key_diff=1;
				for(i=0;i<6;i++)
					mima_str[i]=(key_str[i]-48);				
				plen=0;
				OLED_ShowCHinese(32,4,38);
				OLED_ShowCHinese(48,4,39);  
				OLED_ShowCHinese(64,4,11);
				OLED_ShowCHinese(80,4,12); //修改成功
				break;
			}
			key=0;
		}
		
	}
}
			OLED_Clear(); 	
		OLED_ShowCHinese(28,0,0);
		OLED_ShowCHinese(46,0,1);
		OLED_ShowCHinese(64,0,2);
		OLED_ShowCHinese(82,0,3);//智能门锁
}
void mima_dis()
{
				OLED_ShowCHinese(28,0,5);
		OLED_ShowCHinese(46,0,6);
		OLED_ShowCHinese(64,0,32);
		OLED_ShowCHinese(82,0,33);//输入密码
	while(1)
	{
		key_load();
		delay_ms(200);
		if(0<=xianshi[key]&&xianshi[key]<10)
		{
			key_str[plen]=48+xianshi[key];
			plen++;
			for(i=plen;i<6;i++) key_str[i]=' ';
			OLED_ShowString(40,2,key_str,16);
			if(plen==6)
			{
				key_diff=1;
				for(i=0;i<6;i++)
				{
					if(mima_str[i]!=(key_str[i]-48))
						key_diff=0;					
				}
				if(key_diff)
				{					
					key_flag=1;
					suo_flag=1;				
				OLED_ShowCHinese(32,4,32);
				OLED_ShowCHinese(48,4,33);  
				OLED_ShowCHinese(64,4,36);
				OLED_ShowCHinese(80,4,37);
				delay_ms(3000);
				OLED_ShowString(32,4,"          ",16);								
				}
				else
				{key_flag=0;
					BEEP=0;
	OLED_ShowCHinese(32,4,32);
	OLED_ShowCHinese(48,4,33);  
	OLED_ShowCHinese(64,4,30);
	OLED_ShowCHinese(80,4,31);
						//密码错误
	delay_ms(3000);
	OLED_ShowString(32,4,"          ",16);
				BEEP=1;
				}
				plen=0;
		for(i=0;i<6;i++)
			key_str[i]=' ';
			OLED_ShowString(40,2,key_str,16);
				break;
			}
			key=0;
		}
	}
			OLED_Clear(); 	
		OLED_ShowCHinese(28,0,0);
		OLED_ShowCHinese(46,0,1);
		OLED_ShowCHinese(64,0,2);
		OLED_ShowCHinese(82,0,3);//智能门锁
}
void add_FR_dis()
{
		OLED_ShowCHinese(0,4,41);
	  OLED_ShowCHinese(16,4,42);  //添加
		OLED_ShowCHinese(28,0,5);
		OLED_ShowCHinese(46,0,6);
		OLED_ShowCHinese(64,0,7);
		OLED_ShowCHinese(82,0,8);
		OLED_ShowCHinese(98,0,40);//输入指纹号
			while(1)
			{
				key_load();
				if(0<xianshi[key]&&xianshi[key]<10)
				{
					add_hao=xianshi[key];
					OLED_ShowNum(32,2,add_hao,1,16);
					delay_ms(1000);
					break;
				}
				delay_ms(100);
			}
			Add_FR((u16)(add_hao));
					OLED_Clear(); 	
		OLED_ShowCHinese(28,0,0);
		OLED_ShowCHinese(46,0,1);
		OLED_ShowCHinese(64,0,2);
		OLED_ShowCHinese(82,0,3);//智能门锁
}

void del_FR_dis()
{
		OLED_ShowCHinese(0,4,13);
	  OLED_ShowCHinese(16,4,14);  //删除
		OLED_ShowCHinese(28,0,5);
		OLED_ShowCHinese(46,0,6);
		OLED_ShowCHinese(64,0,7);
		OLED_ShowCHinese(82,0,8);
		OLED_ShowCHinese(98,0,40);//输入指纹号
			while(1)
			{
				key_load();
				if(0<xianshi[key]&&xianshi[key]<10)
				{
					add_hao=xianshi[key];
					OLED_ShowNum(32,2,add_hao,1,16);
					delay_ms(1000);
					break;
				}
				delay_ms(100);
			}
			Del_FR((u16)(add_hao));
					OLED_Clear(); 	
		OLED_ShowCHinese(28,0,0);
		OLED_ShowCHinese(46,0,1);
		OLED_ShowCHinese(64,0,2);
		OLED_ShowCHinese(82,0,3);//智能门锁
}

int main(void)
 {
	 u8 count=0;
	 u16 timeCount,ensure;	
		delay_init();
	 uart_init(115200);							//串口1，打印信息用
	 usart2_init(usart2_baund);
	//usart3_init(9600);
	 key_Init();  //初始化按键

		while(PS_HandShake(&AS608Addr))		
	{delay_ms(400);
	}
	ensure=PS_ValidTempleteNum(&ValidN);										
	if(ensure!=0x700)
		ShowErrMessage(ensure);							
	ensure=PS_ReadSysPara(&AS608Para); 	 		
	if(ensure==0x00)
	{
	}
	else
		ShowErrMessage(ensure);	

	  PS_StaGPIO_Init();
		OLED_Init();			//初始化OLED  
		OLED_Clear(); 	
		OLED_ShowCHinese(28,0,0);
		OLED_ShowCHinese(46,0,1);
		OLED_ShowCHinese(64,0,2);
		OLED_ShowCHinese(82,0,3);//智能门锁	
	  key=0;
	BEEP_Init();
		 init_hardware_pwm(199,7199);
		 delay_ms(1000);
	//TIM_SetCompare3(TIM3, 3600);
	//func_set_pwm_compare(50);
	TIM3->CCR3=10;
	BEEP=1;
	while(1) 
	{		
		 key_load();
	if(key==4)	mima_dis();
	if(key==8)	mima_set();		
	if(key==12)add_FR_dis();
	if(key==16)del_FR_dis();
		
	delay_ms(100);
if(suo_flag==1)	
{
	TIM_SetCompare3(TIM3, 20);
		OLED_Clear();
		OLED_ShowCHinese(28,2,29);
		OLED_ShowCHinese(46,2,3);
		OLED_ShowCHinese(64,2,11);
		OLED_ShowCHinese(82,2,12);//开锁成功
		BEEP=0;
		delay_ms(500);	
		BEEP=1;
		delay_ms(500);	
		BEEP=0;
		delay_ms(500);	
		BEEP=1;
		delay_ms(500);	
		BEEP=0;
		delay_ms(500);	
		BEEP=1;
		delay_ms(500);			
	  TIM_SetCompare3(TIM3, 10);
		OLED_Clear(); 	
		OLED_ShowCHinese(28,0,0);
		OLED_ShowCHinese(46,0,1);
		OLED_ShowCHinese(64,0,2);
		OLED_ShowCHinese(82,0,3);//智能门锁
		suo_flag=0;
}

	if(PS_Sta)	 //检测PS_Sta状态，如果有手指按下
		if(suo_flag==0)
	{	press_FR();//刷指纹	
		if(suo_flag==0)
		{	
		OLED_Clear(); 	
		OLED_ShowCHinese(28,0,0);
		OLED_ShowCHinese(46,0,1);
		OLED_ShowCHinese(64,0,2);
		OLED_ShowCHinese(82,0,3);//智能门锁		
		}
		}
	if(key)				key=0;
}	

}

