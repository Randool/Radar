// 非易失性存储

sbit SDA = P4 ^ 0;	// I2C总线数据线
sbit SCL = P5 ^ 0;	// I2C总线时钟线

void delay_4us() { 		//延时4us
	;;
}

void IIC_init() {	//I2C总线初始化
	SCL=1;
	delay_4us();
	SDA=1;
	delay_4us();
}

void start() {	 	//主机启动信号
 	SDA = 1;
	delay_4us();
	SCL = 1;
	delay_4us();
	SDA = 0;
	delay_4us();
}

void stop()	{	 	//停止信号
 	SDA=0;
	delay_4us();
	SCL=1;
	delay_4us();
	SDA=1;
	delay_4us();
}

void respons() {	//从机应答信号
	uint8 i=0;
	SCL=1;
	delay_4us();
	while(SDA==1 && (i<255))  //表示若在一段时间内没有收到从器件的应答则
		i++;				//主器件默认从期间已经收到数据而不再等待应答信号。 
	SCL=0;
	delay_4us();
}

void writebyte(uint8 date) { //对24C16写一个字节数据
	uint8 i, temp;
	temp = date;
	for(i=0;i<8;i++) {
		temp = temp << 1;
		SCL = 0;
		delay_4us();
		SDA = CY;
		delay_4us();
		SCL = 1;
		delay_4us();
	}
	SCL=0;
	delay_4us();
	SDA=1;
	delay_4us();	
}

uint8 readbyte() {	//从24C16读一个字节数据
	uint8 i,k;
	SCL=0;
	delay_4us();
	SDA=1;
	delay_4us();
	for(i=0;i<8;i++)
	{
		SCL=1;
		delay_4us();
		k=(k<<1)|SDA;
		delay_4us();
		SCL=0;
		delay_4us();
	}
	delay_4us();
	return k;	
}

void write_addr(uint8 addr, uint8 date) {  //对24C16的地址addr，写入一个数据date
	start();
	writebyte(0xa0);							
	respons();
	writebyte(addr);
	respons();
	writebyte(date);
	respons();
	stop();							
}

uint8 read_addr(uint8 addr) { //从24C16的addr地址，读一个字节数据
	uint8 date;
	start();
	writebyte(0xa0);
	respons();
	writebyte(addr);
	respons();
	
	start();
	writebyte(0xa1);
	respons();
	date = readbyte();
	stop();
	
	return date;
}
