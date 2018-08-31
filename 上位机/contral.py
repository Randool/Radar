import serial
from serial.tools import list_ports


class Contral1:
    def __init__(self, auto_connect=True):
        self.BAUD = 9600
        self.ack = b"\x11"
        self.port = None
        self.serial = None
        self.auto_select()
        if auto_connect:
            self.connect()

    def __del__(self):
        self.close()

    def auto_select(self):
        """ 自动扫描 STC51 的端口 """
        print("扫描端口中...")
        for port_id in list_ports.comports():
            port_info = list(port_id)
            if "CH340" in port_info[1]:
                print(f"找到了！STC51 在 [{port_info[0]}]")
                self.port = port_info[0]
                return
        print("没找到STC")

    def connect(self):
        if self.port is None:
            print("无STC端口")
            return
        print("连接中...")
        try:
            self.serial = serial.Serial(self.port, self.BAUD)
            print("安排上了！")
        except Exception as e:
            print(e)

    def close(self):
        if self.serial is None:
            return
        try:
            self.pause_rader()
            self.serial.close()
            print("取消安排了！")
        except Exception as e:
            print(e)

    def start_rader(self):
        if self.serial is None:
            print("没有连接...")
            return
        self.serial.flushInput()
        self.serial.write(b"\xee")
        cnt = 0
        while True:
            a = self.serial.read()
            if a == self.ack:
                continue
            else:
                b = self.serial.read()
                dist = ord(a) * 256 + ord(b)
                cnt += 1
                yield dist
        self.pause_rader()

    def pause_rader(self, prompt=False):
        comp = str.encode(bytes.decode(self.ack) * 2)
        try:
            while self.serial.read(2) != comp:
                self.serial.write(self.ack)
            if prompt:
                print("暂停成功")
        except Exception as e:
            print(e)


class Contral:
    """ 串口控制\n说明：ack为暂停……"""

    def __init__(self, BAUD=9600):
        self.BAUD = BAUD
        self.ack = b"\x11"
        self.port = None
        self.serial = None

    def connect_STC(self):
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
        try:
            self.serial = serial.Serial(self.port, self.BAUD)
            print("安排上了")
        except Exception as e:
            print(e)

    def close_STC(self):
        try:
            self.serial.write(self.ack)
            self.serial.close()
            self.serial = None
            print("取消安排")
        except Exception as e:
            print(e)

    def get_distance(self) -> float:
        self.serial.flushInput()
        while True:
            a = self.serial.read()
            if a == self.ack:
                continue
            else:
                b = self.serial.read()
                return (ord(a) * 256 + ord(b)) / 1000
