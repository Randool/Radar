sbit SDA = P4^0;	//I2C数据发送线
sbit SCL = P5^5;	//I2C时钟线

void delay() {
	;;
}

/* I2C总线初始化 */
void IIC_init() {
	SCL=1;
	delay();
	SDA=1;
	delay();
}

/* 起始信号 */
void start() {
 	SDA = 1;
	delay();
	SCL = 1;
	delay();
	SDA = 0;
	delay();
}

/* 停止信号 */
void stop()	{
 	SDA=0;
	delay();
	SCL=1;
	delay();
	SDA=1;
	delay();
}

/* 应答信号 */
void respons() {
	uint8 i=0;
	SCL=1;
	delay();
	while(SDA==1 && (i<255))
		i++;
	SCL=0;
	delay();
}

/* 写一字节数据 */
void writebyte(uint8 date) {
	uint8 i, temp;
	temp=date;
	for(i=0;i<8;i++) {
		temp = temp << 1;
		SCL = 0;
		delay();
		SDA = CY;
		delay();
		SCL = 1;
		delay();
	}
	SCL=0;
	delay();
	SDA=1;
	delay();	
}

/* 读一字节数据 */
uint8 readbyte() {
	uint8 i,k;
	SCL=0;
	delay();
	SDA=1;
	delay();
	for(i=0;i<8;i++)
	{
		SCL=1;
		delay();
		k=(k<<1)|SDA;
		delay();
		SCL=0;
		delay();
	}
	delay();
	return k;	
}

/* 向指定地址写数据 */
void write_addr(uint8 addr,uint8 date) {
	start();
	writebyte(0xa0);							
	respons();
	writebyte(addr);
	respons();
	writebyte(date);
	respons();
	stop();							
}

/* 从指定地址读数据 */
uint8 read_addr(uint8 addr) {
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
