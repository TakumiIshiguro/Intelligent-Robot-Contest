#!/usr/bin/env python3
import cv2
import numpy as np
import serial
import time

data = 0

centerX = 480
centerY = 375

right_correctionX = 0
left_correctionX = 0
correctionY = 0

right_speed = 0
left_speed = 0

# シリアルポートを開きます
ser = serial.Serial('/dev/ttyACM0', 9600)

def sendData(right_speed, left_speed):
    header = b'RL'  # ヘッダのバイト列
    r_bytes = int(right_speed).to_bytes(2, 'little', signed=True)
    l_bytes = int(left_speed).to_bytes(2, 'little', signed=True)
    ser.write(header + r_bytes + l_bytes)  # ヘッダと値のバイト列を連結して送信

print(ser.write)

#カメラのキャプチャを開始
cap = cv2.VideoCapture(4)

while True:
    # フレームを1つずつ読み込む
    ret, frame = cap.read()

    # フレームが正常に読み込まれなかった場合は終了する
    if not ret:
        break

    # フレームをグレースケールに変換する
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # 平滑化
    gaus = cv2.GaussianBlur(gray, (33, 33), sigmaX=3)
  
    # 円を検出
    circles = cv2.HoughCircles(gaus, cv2.HOUGH_GRADIENT, 1, 250, param1=100, param2=25, minRadius=50, maxRadius=170)
    
    # 検出した円が存在する場合にのみ描画する
    if circles is not None:
        circles = np.uint16(np.around(circles))
        
        # 検出した円を画像に描き、ファイルを書き出す 
        for i in circles[0,:]:
            cv2.circle(frame, (i[0], i[1]), i[2], (0, 255, 0), 2)
            cv2.circle(frame, (i[0], i[1]), 2, (0, 0, 255), 3)
            x = i[0]
            y = i[1]
            
            right_correctionX = centerX - x
            left_correctionX = right_correctionX * -1
            correctionY = centerY - y

            right_speed = 2048 + right_correctionX + correctionY
            left_speed = 2048 + left_correctionX + correctionY
   
            if right_speed > 3048 or right_speed < 1048:
              right_speed = 2048
            if left_speed > 3048 or left_speed < 1048:
              left_speed = 2048

            sendData(right_speed, left_speed)
            print(right_speed)
            print(left_speed)

            time.sleep(0.2)
   
    # 画像を表示する
    cv2.imshow('hcvideo', frame)

    # 'q'キーを押すとループを抜ける
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# キャプチャを解放し、ウィンドウを閉じる
ser.close()
cap.release()
cv2.destroyAllWindows()
