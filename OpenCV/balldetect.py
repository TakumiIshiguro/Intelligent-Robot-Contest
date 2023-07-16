import cv2
import numpy as np
import serial
import time

# シリアルポートを開きます
ser = serial.Serial('/dev/ttyACM0', 9600)

def sendXYData(x, y):
    header = b'XY'  # ヘッダのバイト列
    x_bytes = int(x).to_bytes(2, 'little', signed=True)
    y_bytes = int(y).to_bytes(2, 'little', signed=True)
    ser.write(header + x_bytes + y_bytes)  # ヘッダと値のバイト列を連結して送信
    print(x, y)

#カメラのキャプチャを開始
cap = cv2.VideoCapture(4)

cap.set(cv2.CAP_PROP_FPS, 10)

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
    circles = cv2.HoughCircles(gaus, cv2.HOUGH_GRADIENT, 1, 250, param1=100, param2=25, minRadius=25, maxRadius=130)

    # 検出した円が存在する場合にのみ処理する
    if circles is not None:
        circles = np.uint16(np.around(circles))

        # 最大の円を探す
        max_radius = 0
        max_circle = None
        for circle in circles[0, :]:
            radius = circle[2]
            if radius > max_radius:
                max_radius = radius
                max_circle = circle

        # 最大の円が見つかった場合にのみ送信する
        if max_circle is not None:
            x = max_circle[0]
            y = max_circle[1]
            sendXYData(x, y)

            # 検出した円を描画する
            cv2.circle(frame, (x, y), max_radius, (0, 255, 0), 2)
            cv2.circle(frame, (x, y), 2, (0, 0, 255), 3)

    # 画像を表示する
    cv2.imshow('hcvideo', frame)

    # 'q'キーを押すとループを抜ける
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# キャプチャを解放し、ウィンドウを閉じる
ser.close()
cap.release()
cv2.destroyAllWindows()

