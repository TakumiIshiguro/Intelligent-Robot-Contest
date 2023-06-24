import time
import serial

data = 0

# シリアルポートの設定
ser = serial.Serial('/dev/ttyACM0', 9600)  # COM1の部分を使用するシリアルポートに置き換えてください

# int型のデータを送信する関数
def sendIntData(value):
    header = b'H'  # ヘッダのバイト列
    value_bytes = value.to_bytes(2, 'little', signed=True)  # 値のバイト列
    ser.write(header + value_bytes)  # ヘッダと値のバイト列を連結して送信

# メインの処理
if __name__ == '__main__':
    while True:
        data += 25
        print(data)
        sendIntData(data)  # int型データの送信
        time.sleep(1)
  
