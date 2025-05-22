#include <REGX52.H>
#include <stdio.h>
#include "tools/I2C.H"
#include "tools/OLED_DISPLAY_DATA.H"
#define uchar unsigned char
//#define uint  unsigned int	

void OLED_WriteCommand(uchar Command)
{	
	start ();
	send_byte(0x78);//从机地址
	accept_response();
	send_byte(0x00);//写命令
	accept_response();
	send_byte(Command);
	accept_response();
	stop ();
}
 

void OLED_WriteData(uchar Data)
{
	
	start ();
	send_byte(0x78);//从机地址
	accept_response();
	send_byte(0x40);//写命令
	accept_response();
	send_byte(Data);
	accept_response();
	stop ();
}
 
//该函数Y的范围0—7，X的范围0-127
void OLED_SetCursor(uchar Y, uchar X)
{
	OLED_WriteCommand(0xB0 | Y);					//设置Y位置
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//设置X位置高4位
	OLED_WriteCommand(0x00 | (X & 0x0F));			//设置X位置低4位
}
 

void OLED_Clear(void)
{  
	uchar i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor(j, 0);
		for(i = 0; i < 128; i++)
		{
			OLED_WriteData(0x00);
		}
	}
}
 

 

void OLED_Init(void)
{
	uchar i, j;
	for (i = 0; i < 100; i++)			//上电延时
		for (j = 0; j < 100; j++);
	
	
	OLED_WriteCommand(0xAE);	//关闭显示
	
	OLED_WriteCommand(0xD5);	//设置显示时钟分频比/振荡器频率
	OLED_WriteCommand(0x80);
	
	OLED_WriteCommand(0xA8);	//设置多路复用率
	OLED_WriteCommand(0x3F);
	
	OLED_WriteCommand(0xD3);	//设置显示偏移
	OLED_WriteCommand(0x00);
	
	OLED_WriteCommand(0x40);	//设置显示开始行
	
	OLED_WriteCommand(0xA1);	//设置左右方向，0xA1正常 0xA0左右反置
	
	OLED_WriteCommand(0xC8);	//设置上下方向，0xC8正常 0xC0上下反置
 
	OLED_WriteCommand(0xDA);	//设置COM引脚硬件配置
	OLED_WriteCommand(0x12);
	
	OLED_WriteCommand(0x81);	//设置对比度控制
	OLED_WriteCommand(0xCF);
 
	OLED_WriteCommand(0xD9);	//设置预充电周期
	OLED_WriteCommand(0xF1);
 
	OLED_WriteCommand(0xDB);	//设置VCOMH取消选择级别
	OLED_WriteCommand(0x30);
 
	OLED_WriteCommand(0xA4);	//设置整个显示打开/关闭
 
	OLED_WriteCommand(0xA6);	//设置正常/倒转显示
 
	OLED_WriteCommand(0x8D);	//设置充电泵
	OLED_WriteCommand(0x14);
 
	OLED_WriteCommand(0xAF);	//开启显示
		
	OLED_Clear();				//OLED清屏
}
void OLED_showChar(uchar Y, uchar X,uchar C,bit dis_pattern){
	uchar c,i;
	c = C-' ';
	OLED_SetCursor(Y,X);
	if(dis_pattern){
		for(i=0;i<8;i++)
			OLED_WriteData(F8X16[c][i]);
		OLED_SetCursor(Y+1,X);
		for(i=0;i<8;i++)
			OLED_WriteData(F8X16[c][i+8]);
	}else{
		for(i=0;i<6;i++)
			OLED_WriteData(F6x8[c][i]);
	}
}

void OLED_showString(uchar y, uchar x,uchar * str,bit dis_pattern){
	uchar j=0;
	while (str[j]!='\0') {
	  OLED_showChar(y,x,str[j],dis_pattern);
		x+=8;
		if (x>120) {
		   x = 0;
			 y+=2;
		}
		j++;
	}
}
/*void show_number(uchar y, uchar x,float unmber,bit is_float,bit dis_pattern){
	uchar str_num[5],i,j=0;
	if(is_float)
		sprintf( str_num, "%0.3f" ,unmber);
	else
		sprintf( str_num, "%d" ,(int)unmber);
	while (str_num[j]!='\0') {
	  OLED_showChar(y,x,str_num[j],dis_pattern);
		x+=8;
		if (x>120) {
		   x = 0;
			 y+=2;
		}
		j++;
	}
}*/
void OLED_showImage(uchar y, uchar x,uchar width ,uchar height, uchar * image){
	uchar i,j;
	for(j=0;j<height/8;j++){
	OLED_SetCursor(y+j,x);
		for(i=0;i<width;i++)
			OLED_WriteData(image[j*width+i]);
	}
}
void OLED_showImage_64X64(){
	OLED_showImage(0,0,64 ,64,image2_64X64);
	
}
void OLED_showImage_128X64(){
	OLED_showImage(0,0,128 ,64,image1_128X64);
	
}