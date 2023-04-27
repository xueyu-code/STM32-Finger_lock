#include "pwm.h"

 
void init_hardware_pwm(uint16_t arr, uint16_t psc)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  /* ʹ��GPIOAʱ��ʱ�� */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  /* ʹ�ܶ�ʱ��2ʱ�� */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = arr; //��ʱ���������� 0-999  1000	
  TIM_TimeBaseStructure.TIM_Prescaler = psc; //����Ԥ��Ƶ��8+1��Ƶ   8K PWMƵ��
  TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷ�Ƶϵ��������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
    //��������ֵ�������������������ֵʱ����ƽ��������(��ռ�ձ�) ��ʼֵ0
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    //����ʱ������ֵС�ڶ�ʱ�趨ֵʱΪ�ߵ�ƽ
  /* ʹ��ͨ��3 */
  TIM_OC3Init(TIM3, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
  /* ʹ��ͨ��4 */
  TIM_OC4Init(TIM3, &TIM_OCInitStructure);
  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM3, ENABLE); // ʹ��TIM2���ؼĴ���ARR
  TIM_Cmd(TIM3, ENABLE); //ʹ�ܶ�ʱ��2
   TIM3->CCR3=0;
   TIM3->CCR4=0; 
}
