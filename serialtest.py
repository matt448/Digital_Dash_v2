#!/usr/bin/python

import serial
import os.path
from time import sleep
import Adafruit_BBIO.UART as UART

#Start serial connection with Arduino UART
#Create device if it doesn't exist.
if os.path.exists('/dev/ttyO1'):
    ser = serial.Serial('/dev/ttyO1', 115200, timeout=2)
else:
    UART.setup("UART1") #This creates the serial device
    ser = serial.Serial('/dev/ttyO1', 115200, timeout=2)


#Start serial connection with Arduino on UART1
ser = serial.Serial('/dev/ttyO1', 115200, timeout=2)

while True:
    line = ser.readline()
    print(line)
    sleep(0.025)

ser.close
