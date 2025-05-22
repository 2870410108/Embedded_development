#ifndef __MPU6050_I2C_H__
#define __MPU6050_I2C_H__
#define uchar  unsigned  char
	

void Init_MPU6050_I2C(void);

void MPU6050_WriteCommand(uchar Command,uchar data);

uchar MPU6050_ReadData(uchar Command);


#endif

