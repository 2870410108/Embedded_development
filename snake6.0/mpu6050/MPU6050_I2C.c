#include "stm32f10x.h"


#define PeriphClock         RCC_APB2Periph_GPIOB    //待开启的时钟外设
#define GPIO_Periph         GPIOB					//待初始化的GPIO寄存器
#define GPIO_SCL_Pin        GPIO_Pin_10			    //待初始化SCL的引脚编号
#define GPIO_SDA_Pin        GPIO_Pin_11			    //待初始化SDA的引脚编号
#define delay			    5


/*引脚配置*/

void I2C_W_SCL(uint8_t BitValue)//这三个函数将读写io口封装起来，增强可读性
{	uint8_t i=delay;
	GPIO_WriteBit(GPIO_Periph, GPIO_SCL_Pin, (BitAction)BitValue);
	while(i--);
}

void I2C_W_SDA(uint8_t BitValue)
{	uint8_t i=delay;
	GPIO_WriteBit(GPIO_Periph, GPIO_SDA_Pin, (BitAction)BitValue);
	while(i--);
}

uint8_t I2C_R_SDA(void)
{
	uint8_t i=delay,BitValue;
	BitValue = GPIO_ReadInputDataBit(GPIO_Periph, GPIO_SDA_Pin);
	while(i--);
	return BitValue;
}


//初始化引脚
void Init_MPU6050_I2C(void){
	RCC_APB2PeriphClockCmd(PeriphClock,ENABLE);//开启GPIOA寄存器的工作时钟
	GPIO_InitTypeDef GPIO_InitStructure;//初始化GPIOA的输出模式（开漏），速度（最大），引脚（GPIOA10，GPIOA11）
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin=GPIO_SCL_Pin | GPIO_SDA_Pin;
	GPIO_Init(GPIO_Periph,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIO_Periph, GPIO_SCL_Pin | GPIO_SDA_Pin);
}

//I2C通信基本时序块
//起始时序块
void mpu6050_start (void){
	I2C_W_SDA(1);
	I2C_W_SCL(1);
	I2C_W_SDA(0);
	I2C_W_SCL(0);	
}

//停止时序块
void mpu6050_stop (void){
   I2C_W_SCL(0);	
	I2C_W_SDA(0);
	I2C_W_SCL(1);	
	I2C_W_SDA(1);	
}
//发送一个字节
void mpu6050_send_byte(uint8_t byte){
	uint8_t i;
	for(i=0;i<8;i++){
		I2C_W_SDA(byte & (0x80 >> i));//依次从高到低取一位发送		
		I2C_W_SCL(1);				
		I2C_W_SCL(0);				
	}
}

//接收一个字节
uint8_t mpu6050_accept_byte(void){
	uint8_t i,byte=0x00;
	I2C_W_SDA(1);//释放SDA控制权
	for(i=0;i<8;i++){
		I2C_W_SCL(1);
		if(I2C_R_SDA()==1)byte|=(0x80>>i);//在SCL为高电平期间，主机可以从SDA中读取从机发送的数据，循环接收8位
		I2C_W_SCL(0);		
	}
	return byte;
}

//发送应答(接收一帧数据之后应答对方，SDA=0，表示应答，反之)
void mpu6050_send_response(uint8_t ACK){
	I2C_W_SDA(ACK);
	I2C_W_SCL(1);	
	I2C_W_SCL(0);
	
}
//接收应答(需要释放SDA，发送完一帧数据之后等待对方应答，SDA=0，表示应答，反之)
uint8_t  mpu6050_accept_response(void){
	uint8_t ack;
	I2C_W_SDA(1);//释放SDA控制权	
	I2C_W_SCL(1);	
	ack=I2C_R_SDA();
	I2C_W_SCL(0);
	return ack;
}

//写入一个字节的命令
void MPU6050_WriteCommand(uint8_t Command,uint8_t data)
{	
	mpu6050_start ();
	mpu6050_send_byte(0xd0);//从机地址写
	mpu6050_accept_response();
	mpu6050_send_byte(Command);//寄存器地址
	mpu6050_accept_response();
	mpu6050_send_byte(data);//写入寄存器数据
	mpu6050_accept_response();
	mpu6050_stop ();
}
 
//向指定寄存器读取一个字节的数据
uint8_t MPU6050_ReadData(uint8_t Command)
{
	uint8_t data;
	mpu6050_start ();
	mpu6050_send_byte(0xd0);//先写从机地址
	mpu6050_accept_response();
	mpu6050_send_byte(Command);//发送待读取的寄存器地址
	mpu6050_accept_response();
	mpu6050_start ();//开始读取
	mpu6050_send_byte(0xd0 | 0x01);//从机地址读
	mpu6050_accept_response();
	data= mpu6050_accept_byte();
	mpu6050_send_response(1);//发送响应
	mpu6050_stop ();
	return data;
}


