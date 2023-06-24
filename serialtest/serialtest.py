#!/usr/bin/env python3
import serial
import time

datax = 0

port = '/dev/ttyACM0'
baud_rate = 9600

# シリアルポートを開きます
ser = serial.Serial(port, baud_rate)

ser.write(bytes([0]))  # bytes() を使って送信する値をバイト列に変換します

# Arduinoにデータを送信します
while True:
    datax += 1
    ser.write(bytes([datax]))  # bytes() を使って送信する値をバイト列に変換します
    print('x:',datax,)
    time.sleep(1)

    if datax == 1000:
        break

ser.close()
