#include "STC15F2K60S2.H"
#include <intrins.h>

/* 自定义头文件 */
#include "support.h"
#include "servo.h"
#include "ultrasonic.h"
#include "store.h"
#include "serial.h"


/* 通过波特率和晶振计算定时器时间 */
#define FOCS 11059200L
#define BAUD 9600
#define TM   (65536 - FOCS/BAUD/4)


/**
 * 初始化函数
 */
void setup() {
	// 步进电机
	P4M1 = 0x00, P4M0 = 0xff;
	P4 = 0;

	// LED光效
	P0M1 = 0x00, P0M0 = 0xff;	// 8段可亮
	P2M1 = 0x00, P2M0 = 0x08;
	sLED = 1;
	P0 = 0;
	
	// 超声波——定时器0
	TMOD = 0x01;
	TH0 = 0;
	TL0 = 0;
	
	// 串口通信——定时器1
	SCON |= 0x50;
	TH1 = TM >> 8;
	TL1 = TM;
	AUXR = 0x40;
	TI = RI = 0;
	TR1 = 1;
	ES = 1;
	
	EA = 1;
	
	IIC_init();	// IIC总线初始化
	adjust();	// 步进电机调整
}

/**
 * 主功能循环
 */
void loop() {
	uint8 i;
	while (1) {
		// 验证上位机状态
		while (SBUF != ALIVE);
		
		while (i < direction) {
			getDistance();		// 测距
			Duang();			// 光效
			send_data(distance);	// 发送数据到上位机
			
			/* 存距离数据到非易失性存储器 */
			write_addr(i<<1, distance >> 8);		delay_ms(1);
			write_addr((i<<1)+1, distance & 0xff);	delay_ms(1);
			++i;
			
			step(anticlockwise);	// 步进电机角度调整
		}
		
		while (i) {
			getDistance();		// 测距
			Duang();			// 光效
			send_data(distance);	// 发送数据到上位机
			
			/* 存距离数据到非易失性存储器 */
			write_addr(i<<1, distance >> 8);		delay_ms(1);
			write_addr((i<<1)+1, distance & 0xff);	delay_ms(1);
			--i;
			
			step(clockwise);	// 步进电机角度调整
		}
	}
}

void main() {
	setup();
	loop();
}
