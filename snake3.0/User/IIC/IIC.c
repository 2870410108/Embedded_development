#include "IIC.h"  
#include "sys_tick.h"

// 初始化引脚  
void IIC_GPIO_Init(I2C_Device* device) {

    RCC_APB2PeriphClockCmd(device->RCC_APB2Periph_GPIOX, ENABLE);//开启GPIO时钟
  
    GPIO_InitTypeDef GPIO_InitStructure;//定义结构变量
  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;//结构体实例化  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Pin = device->SCL_Pin | device->SDA_Pin ;
  
    GPIO_Init(device->GPIOX, &GPIO_InitStructure);//初始化GPIO  
	 
    GPIO_SetBits(device->GPIOX, device->SCL_Pin);//设置引脚初始状态为高（被IIC上拉为高）
    GPIO_SetBits(device->GPIOX, device->SDA_Pin); 	
}  

// 设置 SCL 引脚  
void static I2C_Write_SCL(I2C_Device* device, uint8_t BitValue) {  
    GPIO_WriteBit(device->GPIOX, device->SCL_Pin, (BitAction)BitValue); 
	Delay_us(5);
}  

// 设置 SDA 引脚  
void static I2C_Write_SDA(I2C_Device* device, uint8_t BitValue) {  
    GPIO_WriteBit(device->GPIOX, device->SDA_Pin, (BitAction)BitValue);

Delay_us(5);	
}  

// 读取 SDA 引脚  
uint8_t static I2C_Read_SDA(I2C_Device* device) { 
Delay_us(5);	
    return GPIO_ReadInputDataBit(device->GPIOX, device->SDA_Pin);     
}  

/***********************************************************
*软件模拟IIC起始信号
*    SCL  1___________
*				      |
*			          |_______0
*    SDA  1_____
*				|
*			    |_____________0
*
************************************************************/
  
void  I2C_Start(I2C_Device* device) {  
    I2C_Write_SDA(device, 1); // SDA 线高  
    I2C_Write_SCL(device, 1); // SCL 线高  
    I2C_Write_SDA(device, 0); // SDA 线低 
	I2C_Write_SCL(device, 0); // SCL 线低  
}  

/***********************************************************
*软件模拟IIC停止信号
*   			     ___________1				
*			     	|
*		 SCL 0______|
*      				 	  ______1
*				    	 |
*		 SDA 0___________|
*
************************************************************/ 

void  I2C_Stop(I2C_Device* device) { 
    //I2C_Write_SCL(device, 0); // SCL 线低	
    I2C_Write_SDA(device, 0); // SDA 线低 
    I2C_Write_SCL(device, 1); // SCL 线高
	I2C_Write_SDA(device, 1); // SDA 线高	
} 

/***********************************************************
*软件模拟IIC发送一个字节数据,并返回从机是否应答
*在SCL为高电平期间，主机可以操纵SDA高低变化从而循环发送8位的数据
*      				___________			   ___________
*				   |           |          |			  |
*		SCL _______|		   |__________|			  |____________.......
*      							
*			  __1__________________	  __1__________________   __1__	 
*		SDA _|					   |_|                     |_|
*			 |__0__________________| |__0__________________| |__0__.......
*		
************************************************************/   

uint8_t I2C_Send_Byte(I2C_Device* device, uint8_t data) { 
		uint8_t i,ack=0;	
    for (i = 0; i < 8; i++) {  
        I2C_Write_SDA(device, data & (0x80 >> i)); // 依次从高到低取一位发送  
        I2C_Write_SCL(device, 1); // SCL 线高   
        I2C_Write_SCL(device, 0); // SCL 线低  
    }  

    // 等待应答  
    I2C_Write_SDA(device, 1); // 释放 SDA 线
    I2C_Write_SCL(device, 1); // SCL 线高  
    ack = I2C_Read_SDA(device); // 读取应答  
    I2C_Write_SCL(device, 0); // SCL 线低  
    
    return ack; // 返回应答位  
}  
/***********************************************************
*软件模拟IIC发送应答信号(接收一帧数据之后应答对方，SDA=0，表示应答)
*      				 _____________
*				    |			  |
*		SCL 0_______|             |________
*      							
*							 		 
*		SDA 0__________________________
*
************************************************************/   
void  I2C_Ack(I2C_Device* device) { 
	I2C_Write_SDA(device, 0); // SDA 线低
    I2C_Write_SCL(device, 1); // SCL 线高	
    I2C_Write_SCL(device, 0); // SCL 线低    
}  
/***********************************************************
*软件模拟IIC发送应答信号(接收一帧数据之后应答对方，SDA=1，表示非应答)
*      				 _____________
*				    |			  |
*		SCL 0_______|             |________
*      							
*							 		 
*		SDA 1__________________________
*
************************************************************/    
void  I2C_Nack(I2C_Device* device) {  
    I2C_Write_SDA(device, 1); // SDA 线低
    I2C_Write_SCL(device, 1); // SCL 线高	
    I2C_Write_SCL(device, 0); // SCL 线低      
}  
/***********************************************************
*软件模拟IIC接收一个字节数据,并发送是否应答发送方
*在SCL为高电平期间，主机可以从SDA中读取从机发送的数据，循环接收8位
*      				___________			   ___________
*				   |           |          |			  |
*		SCL _______|		   |__________|			  |____________.......
*      							
*			   __1__________________   __1__________________   __1__	 
*		SDA 1_|					    |_|                     |_|
*			  |__0__________________| |__0__________________| |__0__.......
*		
************************************************************/ 
uint8_t I2C_Read_Byte(I2C_Device* device, uint8_t ack) {  
    uint8_t data =0,i = 0;  
 
    I2C_Write_SDA(device, 1); // 释放 SDA 线  
    for (; i < 8; i++) {  
        I2C_Write_SCL(device, 1); // SCL 线高   
		if(I2C_Read_SDA(device))
			data|=(0x80>>i);
        I2C_Write_SCL(device, 0); // SCL 线低  
    }  
    if (ack) {  
        I2C_Ack(device); // 发送应答  
    } else {  
        I2C_Nack(device); // 发送非应答  
    }  
    return data; // 返回读取的数据  
}  

