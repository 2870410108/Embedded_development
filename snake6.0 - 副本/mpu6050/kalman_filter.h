#ifndef __Kalman_Filter_H__
#define __Kalman_Filter_H__

#include "math.h"
#define sq(x) ((x) * (x))

/*计算偏移量*/
float i;                                    //计算偏移量时的循环次数
float ax_offset = 0, ay_offset = 0;         //x,y轴的加速度偏移量
float gx_offset = 0, gy_offset = 0;         //x,y轴的角速度偏移量

/*参数*/
float rad2deg = 57.29578;                   //弧度到角度的换算系数
float roll_v = 0, pitch_v = 0;              //换算到x,y轴上的角速度

/*定义微分时间*/
float now = 0, lasttime = 0, dt = 0.005;        //定义微分时间

/*三个状态，先验状态，观测状态，最优估计状态*/
float gyro_roll = 0, gyro_pitch = 0;        //陀螺仪积分计算出的角度，先验状态
float acc_roll = 0, acc_pitch = 0;          //加速度计观测出的角度，观测状态
float k_roll = 0, k_pitch = 0;              //卡尔曼滤波后估计出最优角度，最优估计状态

/*误差协方差矩阵P*/
float e_P[2][2] ={{1,0},{0,1}};             //误差协方差矩阵，这里的e_P既是先验估计的P，也是最后更新的P

/*卡尔曼增益K*/
float k_k[2][2] ={{0,0},{0,0}};             //这里的卡尔曼增益矩阵K是一个2X2的方阵


float  data_array [7];//用来存放原始数据

#endif

