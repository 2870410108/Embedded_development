#ifndef __IIC_H__
#define __IIC_H__

#include "stm32f10x.h"


// 定义设备的结构体  
typedef struct {
	uint32_t RCC_APB2Periph_GPIOX;                                  //IIC设备待开启的GPIO时钟
    GPIO_TypeDef* GPIOX;   											//IIC设备GPIO类型
    uint16_t SCL_Pin;  											    //IIC设备SCL时钟线引脚的定义
    uint16_t SDA_Pin;  												//IIC设备SDA时钟线引脚的定义
} I2C_Device; 


// 初始化GPIO
void IIC_GPIO_Init(I2C_Device* device);
// 生成起始信号  
void  I2C_Start(I2C_Device* device); 

// 生成停止信号  
void  I2C_Stop(I2C_Device* device);  

// 发送应答  
void  I2C_Ack(I2C_Device* device);

// 发送非应答  
void  I2C_Nack(I2C_Device* device); 

// 发送一个字节,并返回从机是否应答  
uint8_t I2C_Send_Byte(I2C_Device* device, uint8_t data);

// 读取一个字节,并发送是否应答发送方 
uint8_t I2C_Read_Byte(I2C_Device* device, uint8_t ack);

#endif

