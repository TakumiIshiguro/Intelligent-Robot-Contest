#!/usr/bin/env python3
import serial
import time

data = 0

port = '/dev/ttyACM0'
baud_rate = 9600

# シリアルポートを開きます
ser = serial.Serial(port, baud_rate)

ser.write(bytes([0]))  # bytes() を使って送信する値をバイト列に変換します

# Arduinoにデータを送信します
while True:
    data += 1
    ser.write(bytes([data]))  # bytes() を使って送信する値をバイト列に変換します
    print(data)
    time.sleep(1)

    if data == 1000:
        break

ser.close()
