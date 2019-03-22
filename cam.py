import cv2
import time

cap  = cv2.VideoCapture(0)

while True:
	_, frame = cap.read()
	cv2.imwrite('cam.jpg', frame)
	time.sleep(0.2)
	
