import socket
import sys  
import cv2
import pickle
import struct
import time
import serial
import RPi.GPIO as GPIO

def main():

	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

	s.bind(('', 15555))
	img_counter = 0
	s.listen(1)

	GPIO.setmode(GPIO.BCM)
	GPIO.setup(18,GPIO.OUT)

	try:
		ser = serial.Serial('/dev/ttyACM0', 1000000)
	except:
		ser = serial.Serial('/dev/ttyACM1', 1000000)

	GPIO.output(18,GPIO.HIGH)
	client, address = s.accept()
	print("Connecte au client")
	GPIO.output(18,GPIO.LOW)

	while True:
	
		coor = client.recv(255)
		if coor == 'sendPosition':
			position = client.recv(255)
			print('Recu:{}'.format(position))
			data = bytearray(position,'utf-8')
			ser.write(data)
			ser.flush()
        	elif coor == 'getImage':
                	cam = cv2.VideoCapture(0)
                	print("Capture video ok")
                	cam.set(3, 320);
                	cam.set(4, 240);
                	encode_param = [int(cv2.IMWRITE_JPEG_QUALITY), 90]
                	ret, frame = cam.read()
                	result, frame = cv2.imencode('.jpg', frame, encode_param)
                
                	data = pickle.dumps(frame, 0)
                	size = len(data)
                	print("{}: {}".format(img_counter, size))
                	client.sendall(struct.pack(">L", size) + data)
                	img_counter += 1
                	print("Send fonctionne")
                	cam.release()
	
		elif coor == 'deconnect':
			#GPIO.output(18,GPIO.LOW)
			break;
	client.close()
	s.close()
	time.sleep(2)
	main()

if __name__ == "__main__":
    main()
