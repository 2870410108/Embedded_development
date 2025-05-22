#include <REGX52.H>

void init_external_interrupter(bit select_external_0_OR_external_1){
	if(select_external_0_OR_external_1==0){
		IT0=1;//选择沿触发方式
		EX0=1;//开启外部中断0
	}else{
		IT1=1;//选择沿触发方式
		EX1=1;//开启外部中断1
	}
	EA=1;
}