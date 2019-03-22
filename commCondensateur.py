import serial

def sendTension(client, ser):
	ser.flushInput()
	data = ser.readline()
	client.sendall(data.encode('utf-8'))
