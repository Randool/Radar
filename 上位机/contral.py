import serial
from serial.tools import list_ports


directions = 32


class Contral:
    """ 串口控制\n说明：ack为暂停……"""

    def __init__(self, BAUD=9600):
        self.BAUD = BAUD
        self.ack = b"\x11"
        self.READ = b'\x22'
        self.port = None
        self.serial = None
        self.counter = 0    # 用于和下位机保持计数一致

    def connect_STC(self):
        """ 自动扫描端口并连接 """
        if self.serial is not None:
            print("已连接")
            return
        print("端口扫描中...")
        for port in list_ports.comports():
            port_info = list(port)
            if "CH340" in port_info[1]:
                print(f"STC 在 [{port_info[0]}]")
                self.port = port_info[0]
                break
        if self.port is not None:
            self.serial = serial.Serial(self.port, self.BAUD)
            print("安排上了")
        else:
            print("请检查端口是否连接")

    def close_STC(self):
        """ 断开与STC的连接 """
        if self.serial is None:
            return
        try:
            self.serial.write(self.ack)
            self.serial.close()
            self.serial = None
            self.port = None
            print("取消安排")
        except Exception as e:
            print(e)

    def get_distance(self) -> float:
        """ 获得当前的距离 """
        self.serial.flushInput()
        while True:
            a = self.serial.read()
            if a == self.ack:
                continue
            else:
                b = self.serial.read()
                return (ord(a) * 256 + ord(b)) / 1000

    def take_out_log(self):
        """ 读取log信息 """
        self.serial.flushInput()
        self.serial.write(b'\xee')  # 防止陷入等待状态
        self.serial.write(self.READ)
        for i in range(directions):
            a = self.serial.read(2)
            distance = (a[0] * 256 + a[1]) / 1000
            print(f'{i+1}\t{distance}')
        self.serial.write(b'\xee')  # 防止陷入等待状态
