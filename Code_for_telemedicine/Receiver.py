# -*- coding: utf-8
import socket
import threading
import paho.mqtt.client as mqtt
import socket
import re
import pyqtgraph as pg
from pyqtgraph.Qt import QtCore, QtGui


def connTCP():
    global tcp_client_socket
    tcp_client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_ip = 'bemfa.com'
    server_port = 8344
    try:
        # connect server
        tcp_client_socket.connect((server_ip, server_port))
        substr = 'cmd=1&uid=xxx&topic=Telemedicine\r\n'
        tcp_client_socket.send(substr.encode("utf-8"))
    except:
        time.sleep(2)
        connTCP()


# 心跳
def Ping():
    # 发送心跳
    try:
        keeplive = 'ping\r\n'
        tcp_client_socket.send(keeplive.encode("utf-8"))
    except:
        time.sleep(2)
        connTCP()
    t = threading.Timer(30, Ping)
    t.start()

def get_msg():
    r0 = tcp_client_socket.recv(1024).decode()
    if len(r0) > 30:
        s = r0.split("=")
        s = s[4]
        pattern = re.compile(r'\d+')
        res = re.findall(pattern, s)
        num = list(map(int, res))
        return num
    else:
        pass


connTCP()
Ping()

win = pg.GraphicsLayoutWidget(show=True)
win.setWindowTitle("Telemedicine")

p2 = win.addPlot()

num = get_msg()
k1 = len(num)
data1 = []
data1 = data1 + num

curve2 = p2.plot(data1)
ptr1 = 0

def update1():
    global data1, ptr1
    num1 = get_msg()
    k = len(num1)
    l = len(data1)
    if l < 1001:
        data1 = data1 + num1  # shift data in the array one sample left
    else:
        data1[:-k] = data1[k:]  # shift data in the array one sample left
        data1[-k:] = num1[:]
    ptr1 += 1
    curve2.setData(data1)
    curve2.setPos(ptr1, 0)

timer = pg.QtCore.QTimer()
timer.timeout.connect(update1)
timer.start(1000)

if __name__ == '__main__':
    import sys

    if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_VERSION'):
        QtGui.QApplication.instance().exec_()
