import serial
from time import sleep

ser = serial.Serial('COM6', 9600)

controlBuffer = [2, 32, 127, 4, 68, 69, 70]

string = '<'

for key in controlBuffer:
	ch = chr(key)
	string += ch
	print key
	print ch
	print "_"
	
string += '>'
print string

while True:
	ser.write(str(string))
	print ser.readline()
	sleep(.1)