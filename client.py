import socket
import sys  
import cv2  
import pickle
import time
import numpy as np
import struct ## new        
     
# Host = Adresse ip du serveur (ici Raspberry pi)
# Port = valeur predefinie (doit etre la meme pour le serveur)  
host = '192.168.0.38'
port = 15555    

brasup = "1900"
brasdown = "7600"

cameraup = "5700"
cameradown = "2000"

cameraneutral = "5700"

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

def connectToPi():         
	print(host)
	s.connect((host, port))
	print("Connecte au serveur")

def getImage():
	data = b""
	payload_size = struct.calcsize(">L")
	print("payload_size: {}".format(payload_size))

	output = "getImage"
	s.send(output.encode('utf-8'))

	while len(data) < payload_size:
		print("Recv: {}".format(len(data)))
		data += s.recv(4096)

	print("Done Recv: {}".format(len(data)))
	
	packed_msg_size = data[:payload_size]
	data = data[payload_size:]
	msg_size = struct.unpack(">L", packed_msg_size)[0]
	print("msg_size: {}".format(msg_size))

	while len(data) < msg_size:
        	data += s.recv(4096)
	frame_data = data[:msg_size]
	data = data[msg_size:]

	frame= pickle.loads(frame_data, fix_imports=True, encoding="bytes")
	frame = cv2.imdecode(frame, cv2.IMREAD_COLOR)

	cv2.imwrite('messigray.png',frame)

def sendCoordinates(str):
	signal = 'sendPosition'
	s.sendall(signal.encode('utf-8'))
	s.sendall(str.encode('utf-8'))
	print("Coordonnees envoyees")

def changeServoHori(str):
	signal = 'servoHori'
	s.sendall(signal.encode('utf-8'))
	s.sendall(str.encode('utf-8'))
	print("Servo Horizontal envoyees")

def changeServoVert(str):
	signal = 'servoVert'
	s.sendall(signal.encode('utf-8'))
	s.sendall(str.encode('utf-8'))
	print("Servo Vertical envoyees")

def changeBras(str):
	signal = 'bras'
	s.sendall(signal.encode('utf-8'))
	s.sendall(str.encode('utf-8'))
	print("Change Bras envoyees")

def brasUp():
	changeBras(brasup)

def brasDown():
	changeBras(brasdown)

def cameraUp():
	changeServoVert(cameraup)

def cameraDown():
	changeServoVert(cameradown)

def initServo():
	changeServoVert(cameraup)
	changeServoHori(cameraneutral)
	changeBras(brasup)

# A appeler directement apres sendCoordinates!!
def robotReady():
	okSignal = s.recv(255)
	print(okSignal)

def changeCondensateur():
	signal = 'condensateurChange'
	s.sendall(signal.encode('utf-8'))
	print("Signal envoye!")

def deconnectToPi():
	signal = 'deconnect'
	s.sendall(signal.encode('utf-8'))
	print("Deconnected")
	#s.close()

def getTension():
	signal = 'gettension'
	s.sendall(signal.encode('utf-8'))
	tension = s.recv(255)
	print(tension)
