#include "stm32f10x.h"
uint8_t  ms;
void init_timer_3(){
	
	//配置定时器3
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM3,DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	//打开预加载特性
	TIM_ARRPreloadConfig(TIM3,ENABLE);
	//初始化时基单元
	TIM_TimeBaseInitTypeDef tIM_TimeBaseInitTypeDef;
	//设置预分频
	tIM_TimeBaseInitTypeDef.TIM_Prescaler=72-1;
	//设置定时时间
	tIM_TimeBaseInitTypeDef.TIM_Period=50000-1;
	tIM_TimeBaseInitTypeDef.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3,&tIM_TimeBaseInitTypeDef);
	TIM_GenerateEvent(TIM3,TIM_EventSource_Update);
	//打开定时中断
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	NVIC_InitTypeDef nvic;
	//选择中断源为：TIM3
	nvic.NVIC_IRQChannel=TIM3_IRQn;
	//配置抢占优先级
	nvic.NVIC_IRQChannelPreemptionPriority=0;
	//配置子优先级
	nvic.NVIC_IRQChannelSubPriority=0;
	//打开中断源
	nvic.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&nvic);
	//定时启动
	TIM_Cmd(TIM3,ENABLE);
}
//配置中断服务函数
void TIM3_IRQHandler(void){
	if(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update)==SET){
		TIM_ClearFlag(TIM3,TIM_FLAG_Update);
		ms++;
	}
}

uint64_t get_ms(){
	return ms;
}
void set_ms(uint8_t temp){
	ms=temp;
}
