#include "timer.h"
#include "stm32f10x_tim.h"
extern vu16 USART2_RX_STA;

//定时器4中断服务程序		    
void TIM4_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//是更新中断
	{	 			   
		USART2_RX_STA|=1<<15;	//标记接收完成
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIM4更新中断标志    
		TIM_Cmd(TIM4, DISABLE);  //关闭TIM7 
	}	    
}
 
void TIM4_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//TIM4时钟使能    
	
	//定时器TIM4初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM7中断,允许更新中断
	
	TIM_Cmd(TIM4,ENABLE);//开启定时器4
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
}
	 
void TIM1_UP_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM1,TIM_IT_Update) != RESET)//是更新中断
	{	 
		
		USART2_RX_STA|=1<<15;	//标记接收完成
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);  //清除TIM7更新中断标志    
		
    TIM_Cmd(TIM1, DISABLE);  
	    
}
}	
 
void TIM1_Int_Init(u16 arr,u16 psc)
{
   TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 第一步 使能TIM1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //时钟 TIM1 使能

    // 第二步 定时器 TIM1 初始化，输入时钟72MHz，时钟分割1，分频系数720，PWM频率72M/720/65535=1.5Hz
    TIM_TimeBaseStructure.TIM_Period = 499;  //设置自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler =1799; //设置时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //设置时钟分割
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM 向上计数
    TIM_TimeBaseStructure.TIM_RepetitionCounter=0; //高级定时器1是用定时器功能配置这个才可以是正常的计数频率一开始的72mhz 值得注意的地方
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //初始化 TIM1
    TIM_ITConfig(TIM1,TIM_IT_Update, ENABLE );   //允许更新中断
   // TIM_ITConfig(TIM1,TIM_IT_Update, DISABLE );     //禁止更新中断

    // 第三步 中断优先级 NVIC 设置
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;        //TIM1 中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占优先级 1 级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //从优先级 3 级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQ 通道被使能
    NVIC_Init(&NVIC_InitStructure); //初始化 NVIC 寄存器
	}

	


