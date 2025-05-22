#include "MPU6050_I2C.h"
#include "mpu6050.h"
#include "sys_tick.h"
#include "stdint.h"
#include "math.h"

// 定义全局数组存储传感器数据，索引说明：
// [0-2]: 加速度计X/Y/Z轴数据（单位：g）
// [3-5]: 陀螺仪X/Y/Z轴数据（单位：dps，度/秒）
// [6]:   温度（单位：℃）
// [7-9]: 融合后的偏航角(yaw)、横滚角(roll)、俯仰角(pitch)（单位：度）
static float  data_array [10];

void  MPU6050_Init(void){
	
	Init_MPU6050_I2C();                // 初始化I2C通信接口，配置MPU6050的I2C参数
	
	MPU6050_WriteCommand(MPU_PWR_MGMT1_REG, 0X80);  // 写入PWR_MGMT_1寄存器，触发设备复位
	Delay_ms(100);	                   
	MPU6050_WriteCommand(MPU_PWR_MGMT1_REG, 0X09);  // 再次配置PWR_MGMT_1寄存器，退出复位状态，温度传感器睡眠，选择X轴陀螺仪8Khz
	Delay_ms(100);
	MPU6050_WriteCommand(MPU_SAMPLE_RATE_REG, 0X07);  // 设置SMPLRT_DIV寄存器（0x19），采样率分频值为4，采样率 = 1kHz / (1+7) = 125Hz
	MPU6050_WriteCommand(MPU_ACCEL_CFG_REG, 0X08);  // 配置ACCEL_CONFIG寄存器（0x1C），设置加速度计量程为±4G
	MPU6050_WriteCommand(MPU_GYRO_CFG_REG, 0X08);  // 配置GYRO_CONFIG寄存器（0x1B），设置陀螺仪量程为±500度/秒
	MPU6050_WriteCommand(MPU_CFG_REG, 0X03);  // 配置MPU_CFG_REG寄存器（0x1A），设置低通滤波适中44Hz
	
}


float * MPU6050_GetData()
{	
	
	uint8_t DataH, DataL;// 存储高8位和低8位数据
	


	Delay_ms(8);// 延时8ms,对应125hz频率采样
	// 加速度计数据读取与转换（±4g 量程，LSB 灵敏度为 8192/g）
	DataH = MPU6050_ReadData(MPU_ACCEL_XOUTH_REG);   // 读取X轴高字节（寄存器地址0x3B） 			
	DataL = MPU6050_ReadData(MPU_ACCEL_XOUTL_REG);	 // 读取X轴低字节（寄存器地址0x3C）			
	data_array[0] = (DataH << 8 | DataL)/8192.0f; // 合并高低字节并转换为g值
	
	DataH = MPU6050_ReadData(MPU_ACCEL_YOUTH_REG);
	DataL = MPU6050_ReadData(MPU_ACCEL_YOUTL_REG);
	data_array[1] =(DataH << 8 | DataL)/8192.0f;
	
	DataH = MPU6050_ReadData(MPU_ACCEL_ZOUTH_REG);
	DataL = MPU6050_ReadData(MPU_ACCEL_ZOUTL_REG);
	data_array[2] =(DataH << 8 | DataL)/8192.0f;
	
	// 陀螺仪数据读取与转换（±500°/s 量程，LSB 灵敏度为 65.5°/s）
	DataH = MPU6050_ReadData(MPU_GYRO_XOUTH_REG);
	DataL = MPU6050_ReadData(MPU_GYRO_XOUTL_REG);
	data_array[3] =(DataH << 8 | DataL)/65.5f;// 转换为dps
	
	DataH = MPU6050_ReadData(MPU_GYRO_YOUTH_REG);  
	DataL = MPU6050_ReadData(MPU_GYRO_YOUTL_REG);
	data_array[4] = (DataH << 8 | DataL)/65.5f;
	
	DataH = MPU6050_ReadData(MPU_GYRO_ZOUTH_REG);
	DataL = MPU6050_ReadData(MPU_GYRO_ZOUTL_REG);
	data_array[5] =(DataH << 8 | DataL)/65.5f;
	
//	//获取温度值
//	DataH = MPU6050_ReadData(MPU_TEMP_OUTH_REG);
//	DataL = MPU6050_ReadData(MPU_TEMP_OUTL_REG);
//	data_array[6] = (int16_t)((DataH << 8) | DataL)/340.0f+36.53f;//转换为摄氏度
	
	//加速度计姿态解算（传感器Z轴垂直于地面）
	// roll角计算：Y轴和Z轴的反正切值（范围：-180°~180°）
	float roll_a = atan2(data_array[1], data_array[2]) * 180.0f/ 3.141593f;//转换为角度
	//pitch角计算：X轴和Z轴的反正切值，取负号适配载体坐标系
	float pitch_a=-atan2 (data_array[0], data_array[2]) * 180.0f/ 3.141593f;
	// 陀螺仪积分计算角度（假设采样间隔dt=0.008s，即125Hz采样率）
	//float yaw_g= data_array[7] + data_array[5] * 0.008f;// 用Z轴角速度积分偏航角
	float roll_g = data_array[8] + data_array[3]* 0.008f;// X轴角速度积分横滚角
	float pitch_g = data_array[9] + data_array[4] * 0.008f;// Y轴角速度积分俯仰角
	// 互补滤波器融合加速度计和陀螺仪数据
	const float alpha = 0.95238;// 滤波器系数（陀螺仪权重95.238%，加速度计4.762%）
	//data_array[7]= yaw_g;
	data_array[8] = alpha * roll_g + (1-alpha) * roll_a; // 横滚角融合
	data_array[9] = alpha * pitch_g + (1-alpha) * pitch_a;// 俯仰角融合
	return data_array;
}

