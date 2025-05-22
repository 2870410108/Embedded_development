#include "mpu6050.h"
#include "UserConfig.h"
#include "IIC.h"
#include "stdint.h"
#include "math.h"
#include "sys_tick.h"

static float  data_array [10];

//初始化OLED硬件IIC引脚配置结构体
I2C_Device MPU6050_Device = {MPU6050_GPIO_PeriphClock,
MPU6050_GPIO_Periph, MPU6050_GPIO_SCL_Pin, MPU6050_GPIO_SDA_Pin};

//写入一个字节的命令
void MPU6050_WriteCommand(uint8_t Command,uint8_t data)
{	
	I2C_Start(&MPU6050_Device);
	I2C_Send_Byte(&MPU6050_Device, 0xd0);//从机地址写
	I2C_Send_Byte(&MPU6050_Device, Command);//寄存器地址
	I2C_Send_Byte(&MPU6050_Device, data);//写入寄存器数据
	
	I2C_Stop(&MPU6050_Device);  	
}
 
//向指定寄存器读取一个字节的数据
uint8_t MPU6050_ReadData(uint8_t Command)
{
	uint8_t data=0;
	I2C_Start(&MPU6050_Device);
	I2C_Send_Byte(&MPU6050_Device, 0xd0);//先写从机地址
	I2C_Send_Byte(&MPU6050_Device, Command);//发送待读取的寄存器地址
	I2C_Start(&MPU6050_Device);
	I2C_Send_Byte(&MPU6050_Device, 0xd1);//从机地址读
	data= I2C_Read_Byte(&MPU6050_Device,1);//读取一个字节并发送非应答
	I2C_Stop(&MPU6050_Device); 
	return data;
}

void  MPU6050_Init(void){
	
	IIC_GPIO_Init(&MPU6050_Device);								     //初始化I2C
	MPU6050_WriteCommand(0x6b,0X80);            //复位MPU6050
    Delay_ms(100);                                           //延时
	MPU6050_WriteCommand(0x6b,0X00);            //唤醒MPU6050 
	MPU6050_WriteCommand(0x19,0X04);	//采样率设置为200HZ	1000/4+1  /5ms
	MPU6050_WriteCommand(0x1c,0X00);			//加速度量程±2G
	MPU6050_WriteCommand(0x1b,0X18);//	陀螺仪量程2000
		
}
float * MPU6050_GetData()
{	
	Delay_ms(5);  
	uint8_t DataH, DataL;
	
	DataH = MPU6050_ReadData(MPU_ACCEL_XOUTH_REG);
	DataL = MPU6050_ReadData(MPU_ACCEL_XOUTL_REG);
	data_array[0] = (int16_t)(DataH << 8 | DataL)/16384.0f;
	
	DataH = MPU6050_ReadData(MPU_ACCEL_YOUTH_REG);
	DataL = MPU6050_ReadData(MPU_ACCEL_YOUTL_REG);
	data_array[1] =(int16_t)(DataH << 8 | DataL)/16384.0f;
	
	DataH = MPU6050_ReadData(MPU_ACCEL_ZOUTH_REG);
	DataL = MPU6050_ReadData(MPU_ACCEL_ZOUTL_REG);
	data_array[2] =(int16_t)(DataH << 8 | DataL)/16384.0f;
	
	DataH = MPU6050_ReadData(MPU_GYRO_XOUTH_REG);
	DataL = MPU6050_ReadData(MPU_GYRO_XOUTL_REG);
	data_array[3] =(int16_t)(DataH << 8 | DataL)/16.40f;
	
	DataH = MPU6050_ReadData(MPU_GYRO_YOUTH_REG);  
	DataL = MPU6050_ReadData(MPU_GYRO_YOUTL_REG);
	data_array[4] = (int16_t)(DataH << 8 | DataL)/16.40f;
	
	DataH = MPU6050_ReadData(MPU_GYRO_ZOUTH_REG);
	DataL = MPU6050_ReadData(MPU_GYRO_ZOUTL_REG);
	data_array[5] =(int16_t)(DataH << 8 | DataL)/16.40f;
	
	DataH = MPU6050_ReadData(MPU_TEMP_OUTH_REG);
	DataL = MPU6050_ReadData(MPU_TEMP_OUTL_REG);
	data_array[6] = (int16_t)((DataH << 8) | DataL)/340.0f+36.53f;
	
	
	float roll_a = atan2(data_array[1], data_array[2]) / 3.141593f * 180.0f;
	float pitch_a=-atan2 (data_array[0], data_array[2]) /3.141593f * 180.0f;
	float yaw_g= data_array[7] + data_array[5] * 0.005;
	float roll_g = data_array[8] + data_array[3]* 0.005;
	float pitch_g = data_array[9] + data_array[4] * 0.005;
	const float alpha = 0.95238;
	data_array[7]= yaw_g;
	data_array[8] = alpha * roll_g + (1-alpha) * roll_a; 
	data_array[9] = alpha * pitch_g + (1-alpha) * pitch_a;
	
	return data_array;
}

