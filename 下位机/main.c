#include "support.h"
#include "store.h"
#include "servo.h"
#include "serial.h"


#define FOCS 11059200L
#define BAUD 9600
#define TM   (65536 - FOCS/BAUD/4)


sbit sLED = P3^3;
sbit echo = P1^0;
sbit trig = P1^1;


uint16 distance;
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
	
	store_init();	// 存储设备的初始化
}

// 测距函数
void getDistance() {
	TR1 = 0;
	trig = 1;
	Delay25us();
	trig = 0;
	TH0 = TL0 = 0;
	while (!echo);
	TR0 = 1;
	while (echo);
	TR0 = 0;
	distance = (TH0 * 256 + TL0) * 0.172;
	if (distance > 4000) distance = 4000;
	TR1 = 1;
}

// 由于步进电机无法自动调整角度，
// 需要手动调整初始角度

sbit KEY1 = P3^2;
sbit KEY2 = P3^3;
sbit KEY3 = P1^7;

uint16 cnt1, cnt2, cnt3, cnt;
#define UPDOUND   10
#define THRESHOLD 7

void adjust() {
	while (1) {
		++cnt;
		if (!KEY1) ++cnt1;
		if (!KEY2) ++cnt2;
		if (!KEY3) ++cnt3;
		if (cnt >= UPDOUND) {
			cnt = cnt1 = cnt2 = cnt3 = 0;
		}
		
		if (cnt1 >= THRESHOLD) {	// 调整结束
			Duang();
			break;
		}
		else if (cnt2 >= THRESHOLD) {	// 逆时针旋转
			step(clockwise);
		}
		else if (cnt3 >= THRESHOLD) {	// 顺时针旋转
			step(anticlockwise);
		}
	}
}

// 主功能循环
void loop() {
	uint8 i;
	while (1) {
		// 验证上位机状态
		while (SBUF != ALIVE);
		
		for (i = 0; i < direction; ++i) {
			getDistance();
			Duang();
			send_data(distance);
			
			write_addr(i<<1, distance >> 8);
			write_addr((i<<1)+1, distance & 0xff);

			step(anticlockwise);
		}
		for (i = direction-1; i >= 0; --i) {
			getDistance();
			Duang();
			send_data(distance);
			
			write_addr(i<<1, distance >> 8);
			write_addr((i<<1)+1, distance & 0xff);

			step(clockwise);
		}
	}
}

void main() {
	setup();
	adjust();
	loop();
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