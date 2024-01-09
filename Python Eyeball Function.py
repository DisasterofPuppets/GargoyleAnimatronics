#!/usr/bin/env python
# -*- coding: utf-8 -*-

#Libraries
import socket    #https://wiki.python.org/moin/UdpCommunication
from adafruit_motor import servo
from adafruit_pca9685 import PCA9685
import board
import busio
import time
import threading

#Parameters
localPort=8888
bufferSize=1024

#Objects
sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)  ## Internet,UDP

i2c = busio.I2C(board.SCL, board.SDA)
pca = PCA9685(i2c)
pca.frequency = 50

#SERVO array for startup / testing
#PW based on motor specs sheets
servos = [
servo.Servo(pca.channels[0], min_pulse=500, max_pulse=2500), #MG996R Servo
servo.Servo(pca.channels[1], min_pulse=500, max_pulse=2500), #MG996R Servo
servo.Servo(pca.channels[2], min_pulse=500, max_pulse=2500), #MG996R Servo
servo.Servo(pca.channels[3], min_pulse=500, max_pulse=2500), #MG996R Servo
servo.Servo(pca.channels[4], min_pulse=500, max_pulse=2500), #MG996R Servo
servo.Servo(pca.channels[5], min_pulse=500, max_pulse=2500), #MG996R Servo
servo.Servo(pca.channels[6], min_pulse=500, max_pulse=2500), #MG996R Servo
servo.Servo(pca.channels[7], min_pulse=500, max_pulse=2500), #MG996R Servo
servo.Servo(pca.channels[8], min_pulse=500, max_pulse=2500), #MG996R Servo
servo.Servo(pca.channels[9], min_pulse=500, max_pulse=2500), #MG996R Servo
servo.Servo(pca.channels[10], min_pulse=500, max_pulse=2500), #MG996R Servo
servo.Servo(pca.channels[11], min_pulse=500, max_pulse=2500), #MG996R Servrvo
servo.Servo(pca.channels[12], min_pulse=500, max_pulse=2500), #MG996R Servo
servo.Servo(pca.channels[13], min_pulse=500, max_pulse=2500), #DS3240 Servo
servo.Servo(pca.channels[14], min_pulse=500, max_pulse=2500), #DS3240 Servo
servo.Servo(pca.channels[15], min_pulse=500, max_pulse=2500), #DS3240 Servo
]
#for direct access by name

lLidTop = servos[0]
lLidBot = servos[1]
lEyeLeft = servos[2]
lEyeRight = servos[3]
lEyeUp = servos[4]
lEyeDown = servos[5]
rLidTop = servos[6]
rLidBot = servos[7]
rEyeLeft = servos[8]
rEyeRight = servos[9]
rEyeUp = servos[10]
rEyeDown = servos[11]
wingMid = servos[12]
wingLeft = servos[13]
wingRight = servos[14]
neck = servos[15]


#Servo fine tuning (MAY NO LONGER BE REQUIRED. ...180 is all the way forward / (slack on the string) 0 pulls the string tight
midMin = 82
midMax = 20
LexMin = 0
LexMax = 80
LeyMin = 65
LeyMax = 120
RexMin = 0
ReyMax = 0
LlidTMin = 90
LlidTMax = 180
LlidBMin = 90
LlidBMax = 180

#global variables
xpos = 2200
ypos = 2850
mod1 = "OFF"
mod2= "OFF"
mod3= "OFF"
mod4= "OFF"
wingXToggle = False
wingYToggle = False

#############################################180 servo sweep test

def servotest():
    print("Sweep from 0 to 180")
    for i in range(180):
        for servo_obj in servos:
            servo_obj.angle = i
        time.sleep(0.01)
        
    print("Sweep from 180 to 0")
    for i in range(180):
        for servo_obj in servos:
            servo_obj.angle = 180 - i
        time.sleep(0.01)
        
        
    print("Setting to 90")
    for servo_obj in servos:
        servo_obj.angle = 90
        time.sleep(0.01)

####################################### INITIALISE SERVOS
def startup():
	print(f"Initialising Servos")
	#REPLACE THESE VALUES WITH THE MIN MAX VARABLES^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^OR use html config...to be coded
	
	lLidTop.angle = 0
	lLidBot.angle = 0
	lEyeLeft.angle = 90
	lEyeRight.angle = 90
	lEyeUp.angle = 90
	lEyeDown.angle = 90
	rLidTop.angle = 0
	rLidBot.angle = 0
	rEyeLeft.angle = 0
	rEyeRight.angle = 0
	rEyeUp.angle = 0
	rEyeDown.angle = 0
	wingMid.angle = 0
	wingLeft.angle = 0
	wingRight.angle = 0
	neck.angle = 0
	time.sleep(5)
	
	


################################################# READ THE UDP DATA ####################################

def init():
    print(threading.current_thread().name)
    try:
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
        sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
        sock.bind(('', localPort))
        
        # function get_ip_address 
        def get_ip_address():
            """get host ip address"""
            ip_address = ''
            s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            s.connect(("8.8.8.8", 80))
            ip_address = s.getsockname()[0]
            s.close()
            return ip_address
        
        print("UDP server : {}:{}".format(get_ip_address(), localPort))

        while True:
            data, addr = sock.recvfrom(bufferSize)
            print("Received message: {} from {}\n".format(data, addr))

            # Split the data string into a list
            data_list = data.decode('utf-8').split(',')

            try:
                # Assign the values to your variables
                global xpos, ypos, mod1, mod2, mod3, mod4
                xpos, ypos = map(int, data_list[:2])
                mod1, mod2, mod3, mod4 = map(str, data_list[2:])

                # Print the variables to check
                print(f"xpos: {xpos}\n"
                      f"ypos: {ypos}\n"
                      f"mod1: {mod1}\n"
                      f"mod2: {mod2}\n"
                      f"mod3: {mod3}\n"
                      f"mod4: {mod4}")

                sock.sendto("RPi received OK".encode(), addr)  # Write data back to the client

            except ValueError as ve:
                print(f"Error converting data to integers: {ve}")

    except Exception as e:
        print(f"Error in UDP server: {e}")


############################## EYE MOVEMENT X########################
def Xeyemovement():
	print(threading.current_thread().name)
	global xpos, ypos, mod1, mod2, mod3,mod4
	print(f"mod1: {mod1}, mod2: {mod2}, mod3: {mod3}, mod4: {mod4}")
	
	while mod1 == "OFF" and mod2 == "OFF" and mod3 == "OFF" and mod4 == "OFF":
		# Map the xpos values to the angles for lEyeLeft servo
		if 0 <= xpos <= 2800:
			angle = (xpos / 2800) * 89
		elif 2801 <= xpos <= 2900:
			angle = 90
		elif 2901 <= xpos <= 4095:
			angle = 90 + ((xpos - 2900) / (4095 - 2900)) * 90
		print(angle)
		 #Set the angles for the servos
		lEyeLeft.angle = angle
		lEyeRight.angle = 180 - angle  # Mirrored version of lEyeLeft


############################## EYE MOVEMENT Y########################
def Yeyemovement():
	print(threading.current_thread().name)
	global xpos, ypos, mod1, mod2, mod3,mod4
	print(f"mod1: {mod1}, mod2: {mod2}, mod3: {mod3}, mod4: {mod4}")
	
	while mod1 == "OFF" and mod2 == "OFF" and mod3 == "OFF" and mod4 == "OFF":
		# Map the ypos values to the angles for lEyeLeft servo
		if 0 <= ypos <= 2900:
			angle = (ypos / 2900) * 89
		elif 2901 <= ypos <= 3000:
			yangle = 90
		elif 3001 <= ypos <= 4095:
			angle = 90 + ((ypos - 2900) / (4095 - 2900)) * 90
		print(angle)
		 #Set the angles for the servos
		lEyeUp.angle = angle
		lEyeDown.angle = 180 - angle # Mirrored version of lEyeLeft
#------------------------------------------------------------

if __name__ == '__main__': #checks to see if this file is the main program (it should be:)

	#startup()
	#servotest()
	
	#Threading
	init_thread = threading.Thread(target=init)
	Xeye_thread = threading.Thread(target=Xeyemovement)
	Yeye_thread = threading.Thread(target=Yeyemovement)


	#Logic flow / main code
	init_thread.start()
	Xeye_thread.start()
	Yeye_thread.start()

#############################################################################################
