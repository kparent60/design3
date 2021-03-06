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
from servo import servo
import traceback

def main():
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.bind(('', 15555))
	s.listen(1)

	servo_vert = servo(2000, 8000, 0.1, 0)
	servo_hori = servo(2000, 8000, 0.1, 1)
	servo_pre  = servo(2000, 8000, 0.1, 2)

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
		loop(encode_param, s, client, ser, servo_hori, servo_vert, servo_pre)
		client.close()
		#s.close()
		time.sleep(2)
		GPIO.output(18,GPIO.HIGH)
		time.sleep(2)
		print("reboot")
	#main()

def loop(encode_param, s, client, ser, servo_hori, servo_vert, servo_pre):
	image_counter = 0
	while True:
		ser.flushInput()
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
			cam = cv2.VideoCapture(0)
            #GPIO.output(18,GPIO.HIGH)
			#ret, frame = cam.read()
			ret_val, frame = cam.read()
			time.sleep(3)
			#frame = cv2.imread('cam.jpg')
			frame = cv2.resize(frame, (320, 180))
			encode_param = [int(cv2.IMWRITE_JPEG_QUALITY), 90]
			result, buffer = cv2.imencode('.jpg', frame, encode_param)
			data = pickle.dumps(buffer,0)
			size = len(data)

			print("{}: {}".format(image_counter, size))
			client.sendall(struct.pack(">L", size) + data)
			image_counter += 1
			#encoded = base64.b64encode(buffer)
			#result, frame = cv2.imencode('.jpg', frame, encode_param)
                	#data = pickle.dumps(frame)
			time.sleep(1)
                	#size = len(data)
                	#print("{}: {}".format(size, size))
                	#client.sendall(struct.pack(">L", size) + data)
                	#client.sendall(encoded.encode('utf-8'))
			# msg = encoded
			# data = struct.pack('>I', len(msg)) + msg
			# client.sendall(data)
			frame = ""
			cam.release()
			print("Send fonctionne")
			#GPIO.output(18,GPIO.LOW)
	
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
		elif coor == 'bras':
			position = client.recv(255)
			data = bytearray(position,'utf-8')
			servo_pre.run(data)

if __name__ == "__main__":
	try:
		main()
	except Exception:
		#s.close()
		print("Exception occured")
		traceback.print_exc()

