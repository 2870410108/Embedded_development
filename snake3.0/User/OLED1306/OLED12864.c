#include <stdio.h>
#include "UserConfig.h"
#include "IIC.H"
#include "OLED_DISPLAY_DATA.H"




//初始化OLED硬件IIC引脚配置结构体
I2C_Device OLED_Device = {OLED_GPIO_PeriphClock,OLED_GPIO_Periph, OLED_GPIO_SCL_Pin, OLED_GPIO_SDA_Pin};

// OLED 写命令  
void OLED_WriteCommand(uint8_t command) {  
    I2C_Start(&OLED_Device);  
    I2C_Send_Byte(&OLED_Device, OLED_ADDRESS); // 发送设备地址  
    I2C_Send_Byte(&OLED_Device, OLED_COMMAND); // 写命令模式
	I2C_Send_Byte(&OLED_Device,command); // 发送命令
    I2C_Stop(&OLED_Device);  
}  

 // OLED 写数据 
void OLED_WriteData(uint8_t data) {  
    I2C_Start(&OLED_Device);  
    I2C_Send_Byte(&OLED_Device, OLED_ADDRESS); // 发送设备地址  
    I2C_Send_Byte(&OLED_Device, OLED_DATA); // 写命令模式
	I2C_Send_Byte(&OLED_Device,data); // 发送命令
    I2C_Stop(&OLED_Device);  
} 

//设置DDARM字符显示位置映射（该函数Y的范围0—7，X的范围0-127）
void OLED_SetCursor(uchar Y, uchar X)
{
	OLED_WriteCommand(0xB0 | Y);					//设置行起始地址
	OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4));	//设置低列起始地址高4位
	OLED_WriteCommand(0x00 | (X & 0x0F));			//设置高列起始地址低4位
}
//清屏函数
void OLED_Clear(void)
{  
	uchar i, j;
	for (j = 0; j < 8; j++)
	{
		OLED_SetCursor(j, 0);
		for(i = 0; i < 128; i++)
			OLED_WriteData(0x00);
	}
}
void OLED_Init(void)
{
	IIC_GPIO_Init(&OLED_Device);
	OLED_WriteCommand(0xAE);	//设置显示开或关 AEh, X[0]=0b:显示关（睡眠模式） (RESET) AFh X[0]=1b:显示开，正常模式
	
	OLED_WriteCommand(0xD5);	//设置显示时钟分频比/振荡器频率
	OLED_WriteCommand(0x80);
	
	OLED_WriteCommand(0xA8);	//设置多路复用率
	OLED_WriteCommand(0x3F);
	
	OLED_WriteCommand(0xD3);	//设置显示偏移
	OLED_WriteCommand(0x00);
	
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

	OLED_WriteCommand(0xA4);	//设置整个显示打开/关闭(A4,恢复 RAM内容的显示（RESET）输出跟随 RAM,A5h:锁定RAM内容  )

	OLED_WriteCommand(0xA6);	//设置正常/倒转显示(A6h:正常显示（RESET）A7h: 反相显示)
 
	OLED_WriteCommand(0x8D);	//设置充电泵
	OLED_WriteCommand(0x14);
	
	OLED_Clear();				//OLED清屏
	OLED_WriteCommand(0xAF);	//开启显示
}
 //反显函数
void OLED_ColorTurn(uchar i)
{
	OLED_WriteCommand(0x2E);   //关闭滚屏
	if(i==0)
		OLED_WriteCommand(0xA6);//正常显示
	if(i==1)
		OLED_WriteCommand(0xA7);//反色显示	
}

//开启OLED显示 
void OLED_DisPlay_On(void)
{
	OLED_WriteCommand(0x8D);//电荷泵使能
	OLED_WriteCommand(0x14);//开启电荷泵
	OLED_WriteCommand(0xAF);//点亮屏幕
}

//关闭OLED显示 
void OLED_DisPlay_Off()
{
	OLED_WriteCommand(0x8D);//电荷泵使能
	OLED_WriteCommand(0x10);//关闭电荷泵
	OLED_WriteCommand(0xAE);//关闭屏幕
}

//水平方向滚屏
void OLED_Scroll_horizontally(uchar set_R_or_L){

	OLED_WriteCommand(0x2E);    //先停止滚动,待配置好后再开始滚动
	if(set_R_or_L)
    OLED_WriteCommand(0x27);    //水平向左(0x27)/向右(0x26)滚动
	else
	OLED_WriteCommand(0x26);
    OLED_WriteCommand(0x00);    //dummy byte(空比特、虚拟字节),暂未发现其指令作用
    
    OLED_WriteCommand(0x00);    //设置滚动起始页地址
    OLED_WriteCommand(0x07);    //设置滚动速度(0x00~0x07数值越小速度越慢)
    OLED_WriteCommand(0x07);    //设置滚动结束页        
    
    OLED_WriteCommand(0x00);    //设置水平滚动起始行

    OLED_WriteCommand(0x7F);    //设置水平滚动终止行
    
    OLED_WriteCommand(0x2F);    //开始滚动
}
//显示一个字符
void OLED_showChar(uchar Y, uchar X,uchar C,uchar dis_pattern){
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
//显示字符串
void OLED_showString(uchar y, uchar x,uchar * str,uchar dis_pattern){
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

// 显示数字
void OLED_showNumber(unsigned char y, unsigned char x, float number, unsigned char display_type, unsigned char dis_pattern) {
    char str_num[16];  // 缓冲区用于存储数字字符串
    unsigned char j = 0;

    // 根据显示类型格式化数字
    switch (display_type) {
        case 0: // 显示为浮点数
            sprintf(str_num, "%0.2f", number);  // 格式化为浮点数，保留3位小数
            break;
        case 1: // 显示为整数
            sprintf(str_num, "%d", (int)number);  // 格式化为整数
            break;
        case 2: // 显示为十六进制
            sprintf(str_num, "0X%X", (int)number);  // 格式化为十六进制
            break;
        default:
            return; // 如果显示类型不正确，则返回
    }

    // 显示字符
    while (str_num[j] != '\0') {
        OLED_showChar(y, x, str_num[j], dis_pattern);  // 显示字符
        x += 8;  // 假设每个字符占8个像素

        if (x > 120) {  // 如果超出屏幕宽度，换行
            x = 0;
            y += 2;  // 假设每行占2个单位高度
        }
        j++;
    }
}

//显示图片
void OLED_showImage(uchar y, uchar x,uchar width ,uchar height, const uchar * image){
	uchar i,j;
	for(j=0;j<height/8;j++){
	OLED_SetCursor(y+j,x);
		for(i=0;i<width;i++)
			OLED_WriteData(image[j*width+i]);
	}
}
