// 非易失性存储

sbit SDA = P4 ^ 0;	// I2C总线数据线
sbit SCL = P5 ^ 0;	// I2C总线时钟线

uint8 pointer;		// 地址指针
uint8 write_data;	// 写数据
uint8 read_data;	// 读数据

void store_init() {
	// I2C总线初始化
	SCL = 1;
	delay_4us();
	SDA = 1;
	delay_4us();
	
	pointer = 0x00;
}

void start() {		// 主机启动信号
 	SDA = 1;
	delay_4us();
	SCL = 1;
	delay_4us();
	SDA = 0;
	delay_4us(); 	
}

void stop()	{		// 停止信号
 	SDA = 0;
	delay_4us();
	SCL = 1;
	delay_4us();
	SDA = 1;
	delay_4us();
}

void response() {	// 从机应答信号
	uint8 i = 0;
	SCL = 1;
	delay_4us();
	/*表示若在一段时间内没有收到从器件的应答
	  则主器件默认从期间已经收到数据而不再等待应答信号。*/
	while(SDA == 1 && (i < 255))
		i++;
	SCL = 0;
	delay_4us();
}

void writebyte(uint8 date) {	// 写一个字节数据
	uint8 i, temp;
	temp = date;
	for(i = 0; i < 8; ++i) {
		temp = temp << 1;
		SCL = 0;
		delay_4us();
		SDA = CY;
		delay_4us();
		SCL = 1;
		delay_4us();
	}
	SCL = 0;
	delay_4us();
	SDA = 1;
	delay_4us();
}

uint8 readbyte() {	// 读一个字节数据
	uint8 i,k;
	SCL = 0;
	delay_4us();
	SDA = 1;
	delay_4us();
	for(i = 0; i < 8; i++) {
		SCL = 1;
		delay_4us();
		k = (k<<1) | SDA;
		delay_4us();
		SCL = 0;
		delay_4us();
	}
	delay_4us();
	return k;	
}

void write_addr(uint8 addr, uint8 date) {	// 将data写到addr中
	start();
	writebyte(0xa0);	// 选择24C16
	response();
	writebyte(addr);	// 选择地址
	response();
	writebyte(date);	// 写数据
	response();
	stop();
}

uint8 read_addr(uint8 addr) { // 从addr读取data
	uint8 date;
	start();
	writebyte(0xa0);	// 选择第0块24C16
	response();
	writebyte(addr);	// 选择地址
	response();	
	start();
	writebyte(0xa1);	// 写数据
	response();
	date = readbyte();
	stop();
	return date;
}
