#include "timer.h"
#include "stm32f10x_tim.h"
extern vu16 USART2_RX_STA;

//��ʱ��4�жϷ������		    
void TIM4_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//�Ǹ����ж�
	{	 			   
		USART2_RX_STA|=1<<15;	//��ǽ������
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIM4�����жϱ�־    
		TIM_Cmd(TIM4, DISABLE);  //�ر�TIM7 
	}	    
}
 
void TIM4_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//TIM4ʱ��ʹ��    
	
	//��ʱ��TIM4��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM7�ж�,��������ж�
	
	TIM_Cmd(TIM4,ENABLE);//������ʱ��4
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
}
	 
void TIM1_UP_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM1,TIM_IT_Update) != RESET)//�Ǹ����ж�
	{	 
		
		USART2_RX_STA|=1<<15;	//��ǽ������
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);  //���TIM7�����жϱ�־    
		
    TIM_Cmd(TIM1, DISABLE);  
	    
}
}	
 
void TIM1_Int_Init(u16 arr,u16 psc)
{
   TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // ��һ�� ʹ��TIM1ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //ʱ�� TIM1 ʹ��

    // �ڶ��� ��ʱ�� TIM1 ��ʼ��������ʱ��72MHz��ʱ�ӷָ�1����Ƶϵ��720��PWMƵ��72M/720/65535=1.5Hz
    TIM_TimeBaseStructure.TIM_Period = 499;  //�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler =1799; //����ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //����ʱ�ӷָ�
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM ���ϼ���
    TIM_TimeBaseStructure.TIM_RepetitionCounter=0; //�߼���ʱ��1���ö�ʱ��������������ſ����������ļ���Ƶ��һ��ʼ��72mhz ֵ��ע��ĵط�
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //��ʼ�� TIM1
    TIM_ITConfig(TIM1,TIM_IT_Update, ENABLE );   //��������ж�
   // TIM_ITConfig(TIM1,TIM_IT_Update, DISABLE );     //��ֹ�����ж�

    // ������ �ж����ȼ� NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;        //TIM1 �ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ� 1 ��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        //�����ȼ� 3 ��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQ ͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure); //��ʼ�� NVIC �Ĵ���
	}

	


