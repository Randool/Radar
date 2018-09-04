import os
import sys

from PyQt5 import QtWidgets

from canvas import *


class AppWindow(QtWidgets.QMainWindow):
    """ GUI主程序。添加help，最后加上按钮 """

    def __init__(self):
        QtWidgets.QMainWindow.__init__(self)
        self.setWindowTitle("Rader")

        self.set_main_widget()  # 设置主界面
        self.set_menu_bar()  # 设置菜单栏

    def set_menu_bar(self):
        """ 菜单栏设置 """
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
            "READ", self.read_func, QtCore.Qt.CTRL + QtCore.Qt.Key_R
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
        """ 主界面设置 """
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

    def read_func(self):
        if self.rc.serial is None:
            QtWidgets.QMessageBox.about(self, "Prompt", "未连接STC板，无法读取信息。")
            return
        self.rc.take_out_log()

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
