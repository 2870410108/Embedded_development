#include <REGX52.H>
#define uint unsigned int
#define uchar unsigned char
/*******************************************************************************
* function_name    	       : 
* functional_description   :
* parametric_1_description : bit select_timer0_OR_1 选择初始化的定时器0_1
* parametric_2_description : uchar select_pattern0_1_2_3  选择工作方式
  parametric_2_description : uchar times_us,float frequency  定时时间，与晶振频率
*******************************************************************************/

uint init_timer(bit select_timer0_OR_1,uchar select_pattern0_1_2_3,uint times_us,float frequency,bit is_start){
	uint num=0;
	
	if(select_timer0_OR_1==0){
		TMOD&=0xf0;
		if(select_pattern0_1_2_3==0){
			TMOD|=0X00;//0000 0000 选择定时器0/工作方式选择方式0
			num=(uint)8192-(times_us*frequency)/12;
			num&=0x7fff;
			TH0=num>>8;
			TL0=num;
		}else if(select_pattern0_1_2_3==1){
			TMOD|=0X01;//0000 0001 选择定时器0/工作方式选择方式1
			num=(uint)65536-(times_us*frequency)/12;
			TH0=num>>8;
			TL0=num;
		}else if(select_pattern0_1_2_3==2){
			TMOD|=0X02;//0000 0010 选择定时器0/工作方式选择方式2
			num=(uint)256-(times_us*frequency)/12;
			TH0=TL0=num;
		}else{
			TMOD|=0X03;//0000 0011 选择定时器0/工作方式选择方式3
			num=(uint)256-(times_us*frequency)/12;
			TH0=TL0=num;
		}
			ET0=1;
			EA=1;
		if(is_start){
			TR0=1;
		}
		
	}else{
		TMOD&=0x0f;
		if(select_pattern0_1_2_3==0){
			TMOD|=0X00;//0000 0000 选择定时器1/工作方式选择方式0
			num=(uint)8192-(times_us*frequency)/12;
			num&=0x7fff;
			TH0=num>>8;
			TL0=num;
		}else if(select_pattern0_1_2_3==1){
			TMOD|=0X10;//0001 0000 选择定时器1/工作方式选择方式1
			num=(uint)65536-(times_us*frequency)/12;
			TH0=num>>8;
			TL0=num;
		}else if(select_pattern0_1_2_3==2){
			TMOD|=0X20;//0010 0000 选择定时器1/工作方式选择方式2
			num=(uint)256-(times_us*frequency)/12;
			TH0=TL0=num;
		}else{
			TMOD|=0X30;//0011 0000 选择定时器1/工作方式选择方式3
			num=(uint)256-(times_us*frequency)/12;
			TH0=TL0=num;
		}
			ET1=1;
			EA=1;
		if(is_start){
			TR1=1;
		}		
	}
	return num;
}