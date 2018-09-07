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
	// �������
	P4M1 = 0x00, P4M0 = 0xff;
	P4 = 0;

	// LED��Ч
	P0M1 = 0x00, P0M0 = 0xff;	// 8�ο���
	P2M1 = 0x00, P2M0 = 0x08;
	sLED = 1;
	P0 = 0;
	
	// ������������ʱ��0
	TMOD = 0x01;
	TH0 = 0;
	TL0 = 0;
	
	// ����ͨ�š�����ʱ��1
	SCON |= 0x50;
	TH1 = TM >> 8;
	TL1 = TM;
	AUXR = 0x40;
	TI = RI = 0;
	TR1 = 1;
	ES = 1;
	EA = 1;
}

// ������ѭ��
void loop() {
	uint8 i;
	while (1) {
		// ��֤��λ��״̬
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
	// �豸��ʼ��
	setup();
	IIC_init();
	
	adjust();	// �����������
	loop();		// ��ѭ��
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
