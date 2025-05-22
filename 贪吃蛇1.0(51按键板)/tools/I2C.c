#include <REGX52.H>
#define uchar unsigned char

/*引脚配置*/
sbit SCL=P1^0;
sbit SDA=P1^1;

//I2C通信基本时序块

//起始时序块
void start (){
	SCL=1;
	SDA=1;
	SDA=0;
	SCL=0;
}
//停止时序块
void stop (){
	//SCL=0;
	SDA=0;
	SCL=1;
	SDA=1;
}
//发送一个字节
void send_byte(uchar byte){
	uchar i;
	for(i=0;i<8;i++){
		SDA= byte & (0x80>>i);//依次从高到低取一位发送
		SCL=1;
		SCL=0;
	}
}
//接收一个字节（需要释放SDA）
uchar accept_byte(){
	uchar i,byte;
	SDA=1;//释放SDA控制权
	for(i=0;i<8;i++){
		SCL=1;
		if(SDA)byte=byte|(0x80>>i);
		SCL=0;
	}
	return byte;
}

//发送应答(接收一帧数据之后应答对方，SDA=0，表示应答，反之)
void send_response(){
	SDA=0;
	SCL=1;
	SCL=0;
}
//接收应答(需要释放SDA，发送完一帧数据之后等待对方应答，SDA=0，表示应答，反之)
bit  accept_response(){
	bit ask;
	SDA=1;//释放SDA控制权
	SCL=1;
	ask=SDA;
	SCL=0;
	return ask;
}


