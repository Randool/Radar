#include "support.h"	// ����
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
	// ���
	P4M1 = 0x00, P4M0 = 0xff;
	P4 = 0;

	// LED��Ч
	P0M1 = 0x00, P0M0 = 0xff;	// 8�ο���
	P2M1 = 0x00, P2M0 = 0x08;	// 8�������ȫ����
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
	
	store_init();	// �洢�豸�ĳ�ʼ��
}

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

void loop() {
	uint8 i;
	while (1) {
		// ������λ�����͵�������������֤ʧ������ͣ����
		while (SBUF != ALIVE);
		
		// ˳ʱ����ת
		for (i = 0; i < direction; ++i) {
			getDistance();
			Duang();
			send_data(distance);
			
			write_addr(i<<1, distance >> 8);
			write_addr((i<<1)+1, distance & 0xff);

			step(clockwise);
		}
		
		// ��ʱ����ת
		for (i = direction-1; i >= 0; --i) {
			getDistance();
			Duang();
			send_data(distance);
			
			write_addr(i<<1, distance >> 8);
			write_addr((i<<1)+1, distance & 0xff);

			step(anticlockwise);
		}
	}
}

void main() {
	setup();
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