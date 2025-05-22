#include "MPU6050_I2C.h"
#include "mpu6050.h"
#include "sys_tick.h"
#include "stdint.h"
#include "math.h"

// 常量定义
#define ACCEL_SCALE 8192.0f // ±4g LSB/g
#define GYRO_SCALE 65.5f    // ±500°/s LSB/°/s
#define DT 0.008f           // 采样间隔，125Hz
#define ALPHA 0.95238f      // 互补滤波系数
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
// 数据结构体
typedef struct {
    float accel[3]; // 加速度 (g)
    float gyro[3];  // 角速度 (°/s)
    float roll;     // 横滚角 (°)
    float pitch;    // 俯仰角 (°)
} MPU6050_Data_t;

// 偏移校准
static float accel_bias[3] = {0}, gyro_bias[3] = {0};

// 校准函数
void MPU6050_Calibrate(int samples) {
    float accel_sum[3] = {0}, gyro_sum[3] = {0};
    for (int i = 0; i < samples; i++) {
        uint8_t DataH, DataL;
        DataH = MPU6050_ReadData(MPU_ACCEL_XOUTH_REG);
        DataL = MPU6050_ReadData(MPU_ACCEL_XOUTL_REG);
        accel_sum[0] += (int16_t)(DataH << 8 | DataL) / ACCEL_SCALE;
        DataH = MPU6050_ReadData(MPU_ACCEL_YOUTH_REG);
        DataL = MPU6050_ReadData(MPU_ACCEL_YOUTL_REG);
        accel_sum[1] += (int16_t)(DataH << 8 | DataL) / ACCEL_SCALE;
        DataH = MPU6050_ReadData(MPU_ACCEL_ZOUTH_REG);
        DataL = MPU6050_ReadData(MPU_ACCEL_ZOUTL_REG);
        accel_sum[2] += (int16_t)(DataH << 8 | DataL) / ACCEL_SCALE;
        DataH = MPU6050_ReadData(MPU_GYRO_XOUTH_REG);
        DataL = MPU6050_ReadData(MPU_GYRO_XOUTL_REG);
        gyro_sum[0] += (int16_t)(DataH << 8 | DataL) / GYRO_SCALE;
        DataH = MPU6050_ReadData(MPU_GYRO_YOUTH_REG);
        DataL = MPU6050_ReadData(MPU_GYRO_YOUTL_REG);
        gyro_sum[1] += (int16_t)(DataH << 8 | DataL) / GYRO_SCALE;
        DataH = MPU6050_ReadData(MPU_GYRO_ZOUTH_REG);
        DataL = MPU6050_ReadData(MPU_GYRO_ZOUTL_REG);
        gyro_sum[2] += (int16_t)(DataH << 8 | DataL) / GYRO_SCALE;
        Delay_ms(8);
    }
    for (int i = 0; i < 3; i++) {
        accel_bias[i] = accel_sum[i] / samples;
        gyro_bias[i] = gyro_sum[i] / samples;
    }
}

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
	MPU6050_Calibrate(100);//静止矫正零偏
}

// 数据读取与姿态计算
MPU6050_Data_t MPU6050_GetData(void) {
    static uint32_t last_tick = 0;
    MPU6050_Data_t data = {0};
    uint32_t current_tick = Get_System_Tick();

    // 非阻塞延时
    if (current_tick - last_tick < 8) {
        return data; // 未到采样时间，返回空数据
    }
    last_tick = current_tick;

    uint8_t DataH, DataL;
    // 读取加速度计数据
    DataH = MPU6050_ReadData(MPU_ACCEL_XOUTH_REG);
    DataL = MPU6050_ReadData(MPU_ACCEL_XOUTL_REG);
    data.accel[0] = (int16_t)(DataH << 8 | DataL) / ACCEL_SCALE - accel_bias[0];
    DataH = MPU6050_ReadData(MPU_ACCEL_YOUTH_REG);
    DataL = MPU6050_ReadData(MPU_ACCEL_YOUTL_REG);
    data.accel[1] = (int16_t)(DataH << 8 | DataL) / ACCEL_SCALE - accel_bias[1];
    DataH = MPU6050_ReadData(MPU_ACCEL_ZOUTH_REG);
    DataL = MPU6050_ReadData(MPU_ACCEL_ZOUTL_REG);
    data.accel[2] = (int16_t)(DataH << 8 | DataL) / ACCEL_SCALE - accel_bias[2];

    // 读取陀螺仪数据
    DataH = MPU6050_ReadData(MPU_GYRO_XOUTH_REG);
    DataL = MPU6050_ReadData(MPU_GYRO_XOUTL_REG);
    data.gyro[0] = (int16_t)(DataH << 8 | DataL) / GYRO_SCALE - gyro_bias[0];
    DataH = MPU6050_ReadData(MPU_GYRO_YOUTH_REG);
    DataL = MPU6050_ReadData(MPU_GYRO_YOUTL_REG);
    data.gyro[1] = (int16_t)(DataH << 8 | DataL) / GYRO_SCALE - gyro_bias[1];
    DataH = MPU6050_ReadData(MPU_GYRO_ZOUTH_REG);
    DataL = MPU6050_ReadData(MPU_GYRO_ZOUTL_REG);
    data.gyro[2] = (int16_t)(DataH << 8 | DataL) / GYRO_SCALE - gyro_bias[2];

    // 加速度计姿态解算
    float roll_a = atan2(data.accel[1], data.accel[2]) * 180.0f / M_PI;
    float pitch_a = atan2(-data.accel[0], sqrt(data.accel[1] * data.accel[1] + data.accel[2] * data.accel[2])) * 180.0f / M_PI;

    // 陀螺仪积分
    static float roll_prev = 0, pitch_prev = 0;
    float roll_g = roll_prev + data.gyro[0] * DT;
    float pitch_g = pitch_prev + data.gyro[1] * DT;

    // 一阶互补滤波
    data.roll = ALPHA * roll_g + (1 - ALPHA) * roll_a;
    data.pitch = ALPHA * pitch_g + (1 - ALPHA) * pitch_a;

    roll_prev = data.roll;
    pitch_prev = data.pitch;

    return data;
}

// 贪吃蛇方向映射
typedef enum { UP, DOWN, LEFT, RIGHT, NONE } Snake_Direction_t;
Snake_Direction_t Get_Snake_Direction(float roll, float pitch) {
    if (roll > 15) return RIGHT;
    if (roll < -15) return LEFT;
    if (pitch > 15) return DOWN;
    if (pitch < -15) return UP;
    return NONE;
}