import random
from gpiozero import MCP3008, Button
import time
import socket
import board
import busio
from adafruit_motor import servo
from adafruit_pca9685 import PCA9685
import threading
import math

i2c = busio.I2C(board.SCL, board.SDA)
pca = PCA9685(i2c)
pca.frequency = 60

#SERVOS
#How did I come up ith these values? Retest?
wingLeft = servo.Servo(pca.channels[0], min_pulse=400, max_pulse=2400)
wingRight = servo.Servo(pca.channels[1], min_pulse=400, max_pulse=2400)
wingMid = servo.Servo(pca.channels[2], min_pulse=400, max_pulse=2400)
lEyeX = servo.Servo(pca.channels[3], min_pulse=400, max_pulse=2400)
lEyeY = servo.Servo(pca.channels[4], min_pulse=400, max_pulse=2400)
lLidTop = servo.Servo(pca.channels[5], min_pulse=400, max_pulse=2400)
lLidBot = servo.Servo(pca.channels[6], min_pulse=400, max_pulse=2400)
rEyeX = servo.Servo(pca.channels[7], min_pulse=400, max_pulse=2400)
rEyeY = servo.Servo(pca.channels[8], min_pulse=400, max_pulse=2400)
rLidTop = servo.Servo(pca.channels[9], min_pulse=400, max_pulse=2400)
rLidBot = servo.Servo(pca.channels[10], min_pulse=400, max_pulse=2400)
neck = servo.Servo(pca.channels[11], min_pulse=400, max_pulse=2400)
servo12 = servo.Servo(pca.channels[12], min_pulse=400, max_pulse=2400)
servo13 = servo.Servo(pca.channels[13], min_pulse=400, max_pulse=2400)
servo14 = servo.Servo(pca.channels[14], min_pulse=400, max_pulse=2400)
servo15 = servo.Servo(pca.channels[15], min_pulse=400, max_pulse=2400)

# Create an instance of the button
button1 = Button(22, pull_up=True)
button2 = Button(27, pull_up=True)
button3 = Button(17, pull_up=True)
button4 = Button(4, pull_up=True)

#Servo fine tuning
midMin = 82
midMax = 20

#create objects called that refer to MCP3008 channel 0 and 1
xAxis = MCP3008(0) #X-Axis
yAxis = MCP3008(1) #Y-Axis

#global variables

xpos = 0
ypos = 0
mod1b = 1
mod2b = 1
mod3b = 1
mod4b = 1


#Parameters
port=8888
bufferSize=1024
host = '192.168.1.80'

#Create UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

#Bind socket to address and port
sock.bind((host,port))

################################### SOCK DATA ##################################
def sockread():
	global xpos, ypos, mod1b, mod2b, mod3b, mod4b
	while True:
		data, addr = sock.recvfrom(1024)
		#print(f"Data received: {data}")           
		data_list = data.decode().split(',')
		xpos, ypos, mod1b, mod2b, mod3b, mod4b = map(int,data_list)

####################################### INITIALISE SERVOS
def startup():      
	print(f"Initialising Servos")
	wingLeft.angle = 90 #90 is closed, 0 is open
	wingRight.angle = 90 #90 is closed, 0 
	wingMid.angle = midMin #20 is open / 82 closed
	lEyeX.angle = 0 #using for testing default is 8
	lEyeY.angle = 90
	lLidTop.angle = 180
	lLidBot.angle = 180
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
	global xpos, ypos, mod1b, mod2b, mod3b, mod4b
	while True:
		lEyeX.angle = xpos
		rEyeX.angle = xpos
		lEyeY.angle = ypos
		rEyeY.angle = ypos
		time.sleep(0.3)
		

#-----------------------------------------------------------------------------------------------------------------

#------------------------------PRINT INPUT VALUES-----------

def print_inputs():
	global xpos, ypos, mod1b, mod2b, mod3b, mod4b
	
	while True:
		try:
			# Print the mapped values
			print(f"      ")
			print(f"X pos: {xpos}")
			print(f"Y pos: {ypos}")
			print(f"modb1 : {mod1b}")
			print(f"modb2 : {mod2b}")
			print(f"modb3 : {mod3b}")
			print(f"modb4 : {mod4b}")
			time.sleep(0.5)
		except Exception as e:
			print(f"Exception: {e}")

#------------------------------------------------------------


###############################TO DO################################

#-------------------------------wings-X-----------------------------

def wingx():
	global xpos,ypos, mod1b
	open_state = False
	last_state = False
	
	while True:
		if mod1b:
			if xpos >= 800:
				open_state = True
			elif xpos <= 600:
				open_state = False
				
			if open_state != last_state:
				if open_state:
					print("Wing X : OPEN")
					wingMid.angle = midMax
					time.sleep(.3)
				else:
					print("Wing X : ClOSED")
					wingMid.angle = midMin
					time.sleep(.3)
				
				last_state = open_state
		

#------------------------------------------------------------

#-------------------------------wings-Y----------------------------

#Wing Y - 

def wingy():
	global xpos, ypos, mod1b
	open_state = False
	last_state = False
	
	while True:
		if mod1b:
			if ypos >= 800:
				open_state = True
			elif ypos <= 600:
				open_state = False
				
			if open_state != last_state:
				if open_state:
					print("Wings : OPEN")
					wingLeft.angle = 0
					wingRight.angle = 0
					time.sleep(.3)
				else:
					print("Wings : ClOSED")
					wingLeft.angle = 90
					wingRight.angle = 90
					time.sleep(.3)
				
				last_state = open_state

#------------------------------------------------------------

#blink

def blink():
	global xpos, ypos, mod1b, mod2b, mod3b, mod4b
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
	global xpos, ypos, mod1b, mod2b, mod3b, mod4b
	while True:
		print("Random")
		time.sleep(random.randint(5, 12)) #seconds
		blink()
	time.sleep()

#------------------------------------------------------------

def wakeup():
	global xpos, ypos, mod1b, mod2b, mod3b, mod4b
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

#Wings X (change to button pushes and make flapping animation)
#Wings Y (change to button pushes and make flapping animation)
#Eyes X & Y
#Blink
#Random Blink
#Wake up
#Neck
#Button modifiers

#Threading
sock_thread = threading.Thread(target=sockread)
print_thread = threading.Thread(target=print_inputs)
wingx_thread = threading.Thread(target=wingx)
wingy_thread = threading.Thread(target=wingy)
#eye_thread = threading.Thread(target=eyemovement)
#random_blink = threading.Thread(target=randomblink)


#Logic flow / main code
startup()
sock_thread.start()
print_thread.start()
wingx_thread.start()
wingy_thread.start()
#eye_thread.start()
#random_blink.start()

