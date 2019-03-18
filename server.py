import socket
import sys  
import cv2
import pickle
import struct
import time
import serial
import RPi.GPIO as GPIO
from lib import servo

def main():

	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

	s.bind(('', 15555))
	s.listen(1)

	GPIO.setmode(GPIO.BCM)
	GPIO.setup(18,GPIO.OUT)
	GPIO.setup(21,GPIO.IN)
	GPIO.setup(20,GPIO.OUT)

	servo_hori = servo(2000, 8000, 0.0001, 5)
	servo_vert = servo(2000, 8000, 0.0001, 5)

	try:
		cam = cv2.VideoCapture(0)
                cam.set(3, 320);
                cam.set(4, 240);
		encode_param = [int(cv2.IMWRITE_JPEG_QUALITY), 90]
	except:
		pass

	try:
		ser = serial.Serial('/dev/ttyACM0', 1000000)
	except:
		ser = serial.Serial('/dev/ttyACM1', 1000000)

	GPIO.output(18,GPIO.HIGH)
	client, address = s.accept()
	print("Connecte au client")
	GPIO.output(18,GPIO.LOW)
	GPIO.output(20,GPIO.LOW)

	while True:
		GPIO.output(21,GPIO.LOW)
		coor = client.recv(255)

		if coor == 'servoHori':
			position = client.recv(255)
			data = bytearray(position,'utf-8')
			servo_hori.run(data)
		if coor == 'servoVert':
			position = client.recv(255)
			data = bytearray(position,'utf-8')
			servo_vert.run(data)
		if coor == 'sendPosition':
			position = client.recv(255)
			data = bytearray(position,'utf-8')
			ser.write(data)
			ser.flush()
			state = GPIO.input(21)
			while(state != 1)
				state = GPIO.input(21)
			print("Pin est �gale � 1!")
			client.sendall(data2.encode('utf-8'))

        	elif coor == 'getImage':
                	ret, frame = cam.read()
                	result, frame = cv2.imencode('.jpg', frame, encode_param)
                	data = pickle.dumps(frame, 0)
                	size = len(data)
                	print("{}: {}".format(img_counter, size))
                	client.sendall(struct.pack(">L", size) + data)
                	print("Send fonctionne")
	
		elif coor == 'deconnect':
			#GPIO.output(18,GPIO.LOW)
			break;
		
		elif coor == 'condensateurChange':
			GPIO.output(20,GPIO.HIGH)

	client.close()
	s.close()
	time.sleep(2)
	main()

if __name__ == "__main__":
    main()
