#include "STC15F2K60S2.H"
#include <intrins.h>

#include "support.h"
#include "servo.h"
#include "ultrasonic.h"
#include "store.h"
#include "serial.h"


#define FOCS 11059200L
#define BAUD 9600
#define TM   (65536 - FOCS/BAUD/4)


sbit sLED = P3^3;

uint8 recv_buff;


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
}

// 主功能循环
void loop() {
	uint8 i;
	while (1) {
		// 验证上位机状态
		while (SBUF != ALIVE);
		
		while (i < direction) {
			getDistance();
			Duang();
			send_data(distance);
			
			write_addr(i<<1, distance >> 8);
			write_addr((i<<1)+1, distance & 0xff);
			++i;

			step(anticlockwise);
		}
		
		while (i) {
			getDistance();
			Duang();
			send_data(distance);
			
			write_addr(i<<1, distance >> 8);
			write_addr((i<<1)+1, distance & 0xff);
			--i;

			step(clockwise);
		}
	}
}

void main() {
	// 设备初始化
	setup();
	IIC_init();
	
	adjust();	// 步进电机调整
	loop();		// 主循环
}

void Uart1() interrupt 4 using 1 {
	if (RI) {
		recv_buff = SBUF;
		RI = 0;
	}
	if (TI) {
		busy = 0;
		TI = 0;
	}
}
