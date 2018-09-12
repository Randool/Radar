#ifndef SERIAL_H
#define SERIAL_H

/* 自定义上位机传输指令 */
#define ACK   0x11
#define READ  0x22
#define ALIVE 0xee

/* 从SBUF中读取数据的暂存器 */
uint8 recv_buff;

static bit busy = 1;
void send_byte(uint8 byte) {
	SBUF = byte;
	while (busy);
	busy = 1;
}

/**
 * 数据发送
 */
uint8 data_buff[direction*2];

void send_data(uint16 Data) {
	// 如果上位机请求读取存储数据就进入该循环
	if ( SBUF == READ ) {
		uint8 i = 0;
		EA = 0;
		for (i = 0; i < direction; ++i) {
			data_buff[i<<1] = read_addr(i<<1);
			delay_ms(1);
			data_buff[(i<<1)+1] = read_addr((i<<1)+1);
			delay_ms(1);
		}
		EA = 1;
		delay_ms(1);
		for (i = 0; i < direction; ++i) {
			send_byte( data_buff[i<<1] );
			send_byte( data_buff[(i<<1)+1] );
		}
	}
	
	do {  // 发送实时距离数据
		send_byte(ACK);
		delay_ms( wait_ack );
	} while ( SBUF^ALIVE );	// SBUF != ALIVE
	
	send_byte(Data >> 8);
	send_byte(Data & 0xff);
}

/* 串口中断 */
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

#endif
