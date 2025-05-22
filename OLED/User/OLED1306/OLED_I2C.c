#include "stm32f10x.h"
#define uchar unsigned char

#define PeriphClock         RCC_APB2Periph_GPIOB    //待开启的时钟外设
#define GPIO_SCL_Pin        GPIO_Pin_8			    //待初始化SCL的引脚编号
#define GPIO_SDA_Pin        GPIO_Pin_9			    //待初始化SDA的引脚编号
#define GPIO_Periph         GPIOB					//待初始化的GPIO寄存器

/*引脚配置*/
#define I2C_W_SCL(x)		GPIO_WriteBit(GPIO_Periph, GPIO_SCL_Pin, (BitAction)(x))
#define I2C_W_SDA(x)		GPIO_WriteBit(GPIO_Periph, GPIO_SDA_Pin, (BitAction)(x))
#define I2C_R_SDA(x)		GPIO_ReadInputDataBit(GPIO_Periph, GPIO_SDA_Pin)

//延时
void I2C_delay(void)
{
	uint8_t t=2;
	while(t--);
}
//I2C通信基本时序块
//起始时序块
void start (void){
	I2C_W_SDA(1);
	I2C_W_SCL(1);
	I2C_delay();
	I2C_W_SDA(0);
	I2C_delay();
	I2C_W_SCL(0);
	I2C_delay();
}

//停止时序块
void stop (void){
	
	I2C_W_SDA(0);
	I2C_W_SCL(1);
	I2C_delay();
	I2C_W_SDA(1);
}
//发送一个字节
void send_byte(uint8_t byte){
	uint8_t i;
	for(i=0;i<8;i++){
		I2C_W_SDA(byte & (0x80 >> i));//依次从高到低取一位发送
		I2C_delay();
		I2C_W_SCL(1);
		I2C_delay();
		I2C_W_SCL(0);
		I2C_delay();
	}
}


//接收应答(需要释放SDA，发送完一帧数据之后等待对方应答，SDA=0，表示应答，反之)
uint8_t  accept_response(void){
	uint8_t ask;
	I2C_W_SDA(1);//释放SDA控制权
	I2C_delay();
	I2C_W_SCL(1);
	I2C_delay();
	//ask=SDA;
	I2C_W_SCL(0);
	I2C_delay();
	return ask;
}

//写入一个字节的命令
void OLED_WriteCommand(uchar Command)
{	
	start ();
	send_byte(0x78);//从机地址
	accept_response();
	send_byte(0x00);//写命令
	accept_response();
	send_byte(Command);
	accept_response();
	stop ();
}
 
//写入一个字节的数据
void OLED_WriteData(uchar Data)
{
	start ();
	send_byte(0x78);//从机地址
	accept_response();
	send_byte(0x40);//写数据
	accept_response();
	send_byte(Data);
	accept_response();
	stop ();
}


