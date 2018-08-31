import os
import sys

import matplotlib.pyplot as plt
import numpy as np
from matplotlib import animation
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
from PyQt5 import QtCore, QtWidgets
from contral import Contral


class RaderCanvas(Contral, FigureCanvas):
    """ 用于展示雷达图 """

    def __init__(self, parent=None, width=5, height=4, dpi=100):
        super().__init__()

        # 设置图像属性
        self.fig = Figure(figsize=(width, height), dpi=dpi)
        self.axes = self.fig.add_subplot(111, projection="polar")

        FigureCanvas.__init__(self, self.fig)
        self.setParent(parent)
        FigureCanvas.setSizePolicy(
            self, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding
        )
        FigureCanvas.updateGeometry(self)

        # 数据部分
        self.rQueue = []
        self.thetaQueue = []
        self.buff = 0

        # 计时器
        self.timer = QtCore.QTimer(self)

    def start_measure(self, freq=150):
        """ 连接STC并启动定时器 """
        self.serial.write(b"\xee")  # 待改进【】
        self.timer.timeout.connect(self.update_figure)
        self.timer.start(freq)

    def pause_measure(self):
        """ 暂停定时器 """
        self.serial.write(self.ack)
        self.timer.disconnect()

    def update_figure(self, dotnum=48, step=np.pi / 24):
        """ 更新作图 """
        r = self.get_distance()
        print(f"r = {r}\r")
        if len(self.rQueue) == dotnum:
            self.rQueue.pop(0)
            self.thetaQueue.pop(0)
        self.rQueue.append(r)
        self.buff += step
        self.thetaQueue.append(self.buff)

        self.axes.cla()
        self.axes.plot(self.thetaQueue, self.rQueue)
        self.axes.set_rmax(4.3)
        self.axes.set_rticks([1, 2, 3, 4])
        self.axes.set_rlabel_position(0)
        self.draw()


class AppWindow(QtWidgets.QMainWindow):
    """ GUI主程序。添加help，最后加上按钮 """

    def __init__(self):
        QtWidgets.QMainWindow.__init__(self)
        self.setWindowTitle("Rader")

        self.set_main_widget()  # 设置主界面
        self.set_menu_bar()  # 设置菜单栏

    def set_menu_bar(self):
        self.menu_file = QtWidgets.QMenu("File", self)
        self.menu_file.addAction(
            "Connect", lambda: self.rc.connect_STC(), QtCore.Qt.CTRL + QtCore.Qt.Key_A
        )
        self.menu_file.addAction(
            "Start", self.start_func, QtCore.Qt.CTRL + QtCore.Qt.Key_S
        )
        self.menu_file.addAction(
            "Pause", self.pause_func, QtCore.Qt.CTRL + QtCore.Qt.Key_P
        )
        self.menu_file.addAction(
            "Quit", self.quit_func, QtCore.Qt.CTRL + QtCore.Qt.Key_Q
        )
        self.menuBar().addMenu(self.menu_file)

        self.menu_help = QtWidgets.QMenu("Help", self)
        self.menu_help.addAction(
            "Help", self.help_msg, QtCore.Qt.CTRL + QtCore.Qt.Key_H
        )
        self.menuBar().addMenu(self.menu_help)

    def set_main_widget(self):
        self.main_widget = QtWidgets.QWidget(self)
        dashboard = QtWidgets.QVBoxLayout(self.main_widget)
        self.rc = RaderCanvas(self.main_widget, 6, 5, dpi=100)
        dashboard.addWidget(self.rc)
        self.main_widget.setFocus()
        self.setCentralWidget(self.main_widget)

    def start_func(self):
        if self.rc.serial is None:
            QtWidgets.QMessageBox.about(self, "Prompt", "未连接！\n请检查端口连接，并按‘Ctrl+A’连接STC。")
            return
        self.rc.start_measure()

    def pause_func(self):
        if self.rc.serial is None:
            QtWidgets.QMessageBox.about(self, "Prompt", "未连接STC板，无法暂停。")
            return
        self.rc.pause_measure()

    def quit_func(self):
        if self.rc.serial is not None:
            self.rc.close_STC()
        self.close()

    def help_msg(self):
        QtWidgets.QMessageBox.about(self, "Help", "1.将C代码烧录进STC\n2.Ctrl+a连接STC\n3.Ctrl+s开始测量\n4.Ctrl+p暂停测量\n5.Ctrl+q退出程序")


if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    aw = AppWindow()
    aw.show()
    sys.exit(app.exec_())
