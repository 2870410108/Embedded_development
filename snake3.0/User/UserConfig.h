#ifndef __UserConfig_H__
#define __UserConfig_H__

#include "stm32f10x.h"
///////////////////////////////////GPIO配置///////////////////////////////////////////////////


#define OLED_GPIO_PeriphClock       RCC_APB2Periph_GPIOB            //待开启的时钟外设
#define OLED_GPIO_Periph        	GPIOB							//待初始化的GPIO寄存器
#define OLED_GPIO_SCL_Pin        	GPIO_Pin_8			    		//待初始化SCL的引脚编号
#define OLED_GPIO_SDA_Pin        	GPIO_Pin_9			    		//待初始化SDA的引脚编号

#define MPU6050_GPIO_PeriphClock    RCC_APB2Periph_GPIOB            //待开启的时钟外设
#define MPU6050_GPIO_Periph        	GPIOB							//待初始化的GPIO寄存器
#define MPU6050_GPIO_SCL_Pin        GPIO_Pin_10			    		//待初始化SCL的引脚编号
#define MPU6050_GPIO_SDA_Pin        GPIO_Pin_11			    		//待初始化SDA的引脚编号

///////////////////////////////////OLED配置///////////////////////////////////////////////////

#define OLED_ADDRESS           0x78    // OLED I2C 地址  
#define OLED_COMMAND           0x00    // OLED 命令模式  
#define OLED_DATA              0x40    // OLED 数据模式

///////////////////////////////////MPU配置///////////////////////////////////////////////////

#define MPU_PWR_MGMT1_REG		0X6B	//电源管理寄存器1
#define MPU_PWR_MGMT2_REG		0X6C	//电源管理寄存器2 
#define MPU_DEVICE_ID_REG		0X75	//器件ID寄存器


#define MPU_SAMPLE_RATE_REG		0X19	//采样频率分频器
#define MPU_CFG_REG				0X1A	//配置寄存器
#define MPU_GYRO_CFG_REG		0X1B	//陀螺仪配置寄存器
#define MPU_ACCEL_CFG_REG		0X1C	//加速度计配置寄存器


#define MPU_ACCEL_XOUTH_REG		0X3B	//加速度值,X轴高8位寄存器
#define MPU_ACCEL_XOUTL_REG		0X3C	//加速度值,X轴低8位寄存器
#define MPU_ACCEL_YOUTH_REG		0X3D	//加速度值,Y轴高8位寄存器
#define MPU_ACCEL_YOUTL_REG		0X3E	//加速度值,Y轴低8位寄存器
#define MPU_ACCEL_ZOUTH_REG		0X3F	//加速度值,Z轴高8位寄存器
#define MPU_ACCEL_ZOUTL_REG		0X40	//加速度值,Z轴低8位寄存器

#define MPU_TEMP_OUTH_REG		0X41	//温度值高八位寄存器
#define MPU_TEMP_OUTL_REG		0X42	//温度值低8位寄存器

#define MPU_GYRO_XOUTH_REG		0X43	//陀螺仪值,X轴高8位寄存器
#define MPU_GYRO_XOUTL_REG		0X44	//陀螺仪值,X轴低8位寄存器
#define MPU_GYRO_YOUTH_REG		0X45	//陀螺仪值,Y轴高8位寄存器
#define MPU_GYRO_YOUTL_REG		0X46	//陀螺仪值,Y轴低8位寄存器
#define MPU_GYRO_ZOUTH_REG		0X47	//陀螺仪值,Z轴高8位寄存器
#define MPU_GYRO_ZOUTL_REG		0X48	//陀螺仪值,Z轴低8位寄存器


#endif

