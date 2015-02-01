#!/usr/bin/python

import serial
import redis
import os.path
import Adafruit_BBIO.UART as UART
from time import sleep

#Setup connection redis
r = redis.StrictRedis(host='localhost', port=6379, db=0)

#Start serial connection with Arduino UART
if os.path.exists('/dev/ttyO1'):
    ser = serial.Serial('/dev/ttyO1', 115200, timeout=2)
else:
    UART.setup("UART1") #This creates the serial device
    ser = serial.Serial('/dev/ttyO1', 115200, timeout=2)


while True:
    line = ser.readline()
    data = line.rstrip().split(',')
    
    #print(line.rstrip())
    #print(data)
    if len(data) > 4:
    	#print "SPEED: " + str(data[3])
    	#print str(data[3])
	r.set('speed', data[3])
ser.close
