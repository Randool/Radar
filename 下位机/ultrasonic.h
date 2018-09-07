#ifndef ULTRASONIC_H
#define ULTRASONIC_H

sbit echo = P1^0;
sbit trig = P1^1;

uint16 distance;

// ²â¾àº¯Êý
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

#endif
