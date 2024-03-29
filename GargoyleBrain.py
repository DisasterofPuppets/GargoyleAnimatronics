import random
import time
import serial
import board
import busio
from adafruit_motor import servo
from adafruit_pca9685 import PCA9685
import threading
import math

i2c = busio.I2C(board.SCL, board.SDA)
pca = PCA9685(i2c)
pca.frequency = 50

#SERVOS
#PW based on motor specs sheets
wingLeft = servo.Servo(pca.channels[0], min_pulse=500, max_pulse=2500) #DS3240 Servo
wingRight = servo.Servo(pca.channels[1], min_pulse=500, max_pulse=2500) #DS3240 Servo
wingMid = servo.Servo(pca.channels[2], min_pulse=500, max_pulse=2500) #MG996R Servo
lEyeX = servo.Servo(pca.channels[3], min_pulse=500, max_pulse=2500) #MG996R Servo
lEyeY = servo.Servo(pca.channels[4], min_pulse=500, max_pulse=2500) #MG996R Servo
lLidTop = servo.Servo(pca.channels[5], min_pulse=500, max_pulse=2500) #MG996R Servo
lLidBot = servo.Servo(pca.channels[6], min_pulse=500, max_pulse=2500) #MG996R Servo
rEyeX = servo.Servo(pca.channels[7], min_pulse=500, max_pulse=2500) #MG996R Servo
rEyeY = servo.Servo(pca.channels[8], min_pulse=500, max_pulse=2500) #MG996R Servo
rLidTop = servo.Servo(pca.channels[9], min_pulse=500, max_pulse=2500) #MG996R Servo
rLidBot = servo.Servo(pca.channels[10], min_pulse=500, max_pulse=2500) #MG996R Servo
neck = servo.Servo(pca.channels[11], min_pulse=500, max_pulse=2500) #MG996R Servo
servo12 = servo.Servo(pca.channels[12], min_pulse=500, max_pulse=2500) #MG996R Servo
servo13 = servo.Servo(pca.channels[13], min_pulse=500, max_pulse=2500) #MG996R Servo
servo14 = servo.Servo(pca.channels[14], min_pulse=500, max_pulse=2500) #MG996R Servo
servo15 = servo.Servo(pca.channels[15], min_pulse=500, max_pulse=2500) #MG996R Servo

#Servo fine tuning
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
xpos = 0
ypos = 0
mod1 = 0
mod2 = 0
mod3 = 0
mod4 = 0
wingXToggle = False
wingYToggle = False


################################### SOCK DATA ##################################
def dataread():
	global xpos, ypos, mod1, mod2, mod3, mod4
	# Open the serial port
	ser = serial.Serial('/dev/rfcomm0', 9600)

	try:
		while True:
			if ser.in_waiting > 0:
				# Read data from the serial port
				data = ser.readline().decode('utf-8').strip()
				
				# Split the data string into a list
				data_list = data.split(',')
				
				# Assign the values to your variables
				xpos, ypos = map(int, data_list[:2])
				mod1, mod2, mod3, mod4 = data_list[2:]
				
				# Print the variables to check
				print(f"xpos: {xpos}, ypos: {ypos}, mod1: {mod1}, mod2: {mod2}, mod3: {mod3}, mod4: {mod4}")

				# Send acknowledgment (take this line out when satisified all works)
				ser.write(b'ACK\n')
				
	except OSError as e:
		
		print (f"Error reading from serial port {ser.port}: {e}")
		

####################################### INITIALISE SERVOS
def startup():      
	print(f"Initialising Servos")
	#REPLACE THESE VALUES WITH THE MIN MAX VARABLES^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^OR use html config...to be coded
	wingLeft.angle = 90 #90 is closed, 0 is open
	wingRight.angle = 90 #90 is closed, 0 
	wingMid.angle = midMin #20 is open / 82 closed
	lEyeX.angle = (LexMin + LexMax) / 2 #40 is middle,0 right, 80 left
	lEyeY.angle = (LeyMin + LeyMax) / 2 #90 is middle, 120 top, 65 bottom
	lLidTop.angle = 90#LlidTMax #180 is closed, 90 open
	lLidBot.angle = 90#LlidBMax #180 is closed, 90 open
	rEyeX.angle = 90
	rEyeY.angle = 90
	rLidTop.angle = 180
	rLidBot.angle = 180
	neck.angle = 90
	servo12.angle = 0
	servo13.angle = 0
	servo14.angle = 0
	servo15.angle = 0
	time.sleep(1)




############################## EYE MOVEMENT########################
def eyemovement():
	global xpos, ypos, mod1b, mod2b, mod3b, mod4b,LexMin,LexMax,LeyMin,LeyMax,RexMin,ReyMax,LlidTMin,LlidTMax,LlidBMin,LlidBMax
	
	while True:
		#Map Left Eye X
		
		#####THIS IS NOT MAPPING WITH 40 degress as the centre.........................
		scaled_input = xpos / 1023
		mapped_angle = scaled_input * LexMax #angle range
		Lxpos = mapped_angle + LexMin #starting angle
		#Map Left Eye Y
		#Map Left Top Lid
		#Map Left Bot Lid
		print(f"Left X Mapped Angle: ", Lxpos)
		lEyeX.angle = Lxpos
		#rEyeX.angle = Lxpos
		#lEyeY.angle = Lxpos
		#rEyeY.angle = Lxpos
		time.sleep(0.3)
		

#-----------------------------------------------------------------------------------------------------------------


###############################TO DO################################

#-------------------------------wings-X-----------------------------

def wingx():
	global wingXToggle, mod1
	while True:
		if mod1:
			if wingXToggle:
				print(f"Mod1 Pressed")
				wingMid.angle = midMax
			else:
				print(f"Closed")
				wingMid.angle = midMin
			time.sleep(1)
			wingXToggle = not wingXToggle

#------------------------------------------------------------
				
#-------------------------------wings-Y----------------------------

#Wing Y - 

def wingy():
	global wingYToggle, mod2, runOnce
	while True:
		if mod2:
			if wingYToggle:
				print(f"Open")
				wingLeft.angle = 0
				wingRight.angle = 0
			else:
				print(f"Closed")
				wingLeft.angle = 90
				wingRight.angle = 90
		time.sleep(1)
		wingYToggle = not wingYToggle




#------------------------------------------------------------

#blink

def blink():
	global xpos, ypos, mod1, mod2, mod3, mod4
	print("Blinking....")
	lLidTop.angle = 180
	lLidBot.angle = 180
	rLidTop.angle = 180
	rLidBot.angle = 180
	time.sleep(50 / 1000)
	lLidTop.angle = 135
	lLidBot.angle = 145
	rLidTop.angle = 135
	rLidBot.angle = 145
	time.sleep(100 / 1000) #100 ms

#------------------------------------------------------------
#randomblink

def randomblink():
	global xpos, ypos, mod1, mod2, mod3, mod4
	while True:
		print("Random")
		time.sleep(random.randint(5, 12)) #seconds
		blink()
	time.sleep()

#------------------------------------------------------------

def wakeup():
	global xpos, ypos, mod1, mod2, mod3, mod4
	print("Wake up")
	lLidTop.angle = 170
	lLidBot.angle = 170
	rLidTop.angle = 170
	rLidBot.angle = 170
	time.sleep(200 / 1000)
	lLidTop.angle = 160
	lLidBot.angle = 160
	rLidTop.angle = 160
	rLidBot.angle = 160
	time.sleep(300 / 1000)
	lLidTop.angle = 180
	lLidBot.angle = 180
	rLidTop.angle = 180
	rLidBot.angle = 180
	time.sleep(900 / 1000)
	lLidTop.angle = 160
	lLidBot.angle = 160
	rLidTop.angle = 160
	rLidBot.angle = 160
	time.sleep(200 / 1000)
	lLidTop.angle = 150
	lLidBot.angle = 150
	rLidTop.angle = 150
	rLidBot.angle = 150
	time.sleep(300 / 1000)
	lLidTop.angle = 180
	lLidBot.angle = 180
	rLidTop.angle = 180
	rLidBot.angle = 180
	time.sleep(700 / 1000)	
	lLidTop.angle = 150
	lLidBot.angle = 150
	rLidTop.angle = 150
	rLidBot.angle = 150
	time.sleep(600 / 1000)
	lLidTop.angle = 155
	lLidBot.angle = 155
	rLidTop.angle = 155
	rLidBot.angle = 155
	time.sleep(50 / 1000)
	lLidTop.angle = 150
	lLidBot.angle = 150
	rLidTop.angle = 150
	rLidBot.angle = 150
	time.sleep(300 / 1000)
	lLidTop.angle = 180
	lLidBot.angle = 180
	rLidTop.angle = 180
	rLidBot.angle = 180
	time.sleep(100 / 1000)
	lLidTop.angle = 135
	lLidBot.angle = 145
	rLidTop.angle = 135
	rLidBot.angle = 145
	time.sleep(200 / 1000)
	
#------------------------------------------------------------


#buttons

#------------------------------------------------------------


#180 servo sweep test

def servotest():
	print("Sweep from 0 to 180")
	for i in range(180):
		lEyeX.angle = i
		time.sleep(0.01)
	print("Sweep from 180 to 0")
	for i in range(180):
		lEyeX.angle = 180 - i
		time.sleep(0.01)
#------------------------------------------------------------

#ToDO LIST%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#HTML based Serv config page ...read values from .txt / html
#Wings X ******DONE
#Wings Y ******DONE
#wing flap / stretch animations
#Eyes X & Y
#Blink
#Random Blink
#Wake up
#Neck
#Button modifiers

#Threading
data_thread = threading.Thread(target=dataread)
#wingx_thread = threading.Thread(target=wingx)
#wingy_thread = threading.Thread(target=wingy)
#eye_thread = threading.Thread(target=eyemovement)
#random_blink = threading.Thread(target=randomblink)


#Logic flow / main code
#startup()
data_thread.start()
#wingx_thread.start()
#wingy_thread.start()
#eye_thread.start()
#random_blink.start()

#keep main loop running unti data_thread has finished
data_thread.join()

