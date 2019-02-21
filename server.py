import socket
import sys  
import cv2
import pickle
import struct
import time
import serial

# Port = valeur predefinie (doit etre la meme pour le client)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#s.setblocking(0)
s.bind(('', 15555))
img_counter = 0
s.listen(1)
ser = serial.Serial('/dev/ttyACM0', 9600)
client, address = s.accept()
print("Connecte au client")

print("Attente commande de depart")
client.recv(255)
print("Commande de depart recu")

while True:
	
	coor = client.recv(255)
	if coor == 'sendPosition':
		position = client.recv(255)
		print('Recu:{}'.format(position))
		data = bytearray(position[0],'utf-8')
		data2 = bytearray(position[1],'utf-8')
		ser.write(data)
		ser.write(data2)
		#ser.write(position.encode())
		print(ser.read())
		print(ser.read())
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
                
client.close()
