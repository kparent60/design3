import socket
import sys
import cv2
import pickle
import struct
import time
import serial
import RPi.GPIO as GPIO
import base64
import commCondensateur as cond
from lib import servo

def main():
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.bind(('', 15555))
	s.listen(1)

	servo_hori = servo(2000, 8000, 0.1, 4)
	servo_vert = servo(2000, 8000, 0.1, 5)

	GPIO.setmode(GPIO.BCM)
	GPIO.setup(18,GPIO.OUT)
	GPIO.setup(21,GPIO.IN)
	GPIO.setup(20,GPIO.OUT)
	#cam = None
	try:
		#cam = cv2.VideoCapture(0)
                #cam.set(3, 1280);
                #cam.set(4, 800);
		encode_param = [int(cv2.IMWRITE_JPEG_QUALITY), 90]
	except:
		raise Exception("Camera not working")
	try:
		ser = serial.Serial('/dev/ttyACM0', 1000000)
	except:
		ser = serial.Serial('/dev/ttyACM1', 1000000)
	while True:
		GPIO.output(18,GPIO.HIGH)
		print("waiting for connection")
		client, address = s.accept()
		print("Connecte au client")
		GPIO.output(18,GPIO.LOW)
		GPIO.output(20,GPIO.LOW)
		loop(encode_param, s, client, ser)
		client.close()
		#s.close()
		time.sleep(2)
		GPIO.output(18,GPIO.HIGH)
		time.sleep(2)
		print("reboot")
	#main()

def loop(encode_param, s, client, ser):
	while True:
		coor = client.recv(255)
		if coor == 'sendPosition':
			position = client.recv(255)
			data = bytearray(position,'utf-8')
			ser.write(data)
			ser.flush()
			time.sleep(0.005)
			state = GPIO.input(21)
			while(state != 1):
				state = GPIO.input(21)
			print("Pin est egale a 1")
			data2 = "ok"
			client.sendall(data2.encode('utf-8'))
        	elif coor == 'getImage':
                	GPIO.output(18,GPIO.HIGH)
			#ret, frame = cam.read()
			frame = cv2.imread('cam.jpg')
			frame = cv2.resize(frame, (320, 180))
			_, buffer = cv2.imencode('.jpg', frame)
			encoded = base64.b64encode(buffer)
			#result, frame = cv2.imencode('.jpg', frame, encode_param)
                	#data = pickle.dumps(frame)
			time.sleep(1)
                	#size = len(data)
                	#print("{}: {}".format(size, size))
                	#client.sendall(struct.pack(">L", size) + data)
                	#client.sendall(encoded.encode('utf-8'))
			msg = encoded
			data = struct.pack('>I', len(msg)) + msg
			client.sendall(data)
			print("Send fonctionne")
			GPIO.output(18,GPIO.LOW)
	
		elif coor == 'deconnect':
			print("deconnection")
			#GPIO.output(18,GPIO.LOW)
			break;
		
		elif coor == 'condensateurChange':
			GPIO.output(20,GPIO.HIGH)
		
		elif coor == 'gettension':
			cond.sendTension(client, ser)
		elif coor == 'servoHori':
			position = client.recv(255)
			data = bytearray(position,'utf-8')
			servo_hori.run(data)
		elif coor == 'servoVert':
			position = client.recv(255)
			data = bytearray(position,'utf-8')
			servo_vert.run(data)
if __name__ == "__main__":
	try:
		main()
	except Exception:
		#s.close()
		print("Exception occured")
		GPIO.output(18,GPIO.LOW)
