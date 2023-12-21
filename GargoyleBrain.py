import random
from gpiozero import MCP3008, Button
import time
import socket
import board
import busio
from adafruit_motor import servo
from adafruit_pca9685 import PCA9685
import threading

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

#create objects called that refer to MCP3008 channel 0 and 1
xAxis = MCP3008(0) #X-Axis
yAxis = MCP3008(1) #Y-Axis

#global variables

x_raw = 0
y_raw = 0
x_average = 0
y_average = 0
mapped_x = 0
mapped_y = 0
button1_state = False
button2_state = False
button3_state = False
button4_state = False

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
    while True:
        data = sock.recvfrom(1024)
        print(f"Data received: {data}")           
        #xPosition, yPosition, mod1B, mod2B, mod3B, mod4B = data.split(",")
   
####################################### INITIALISE SERVOS
def startup():      
	print(f"Initialising Servos")
	wingLeft.angle = 0
	wingRight.angle = 0
	wingMid.angle = 0
	lEyeX.angle = 0 #using for testing default is 90
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
	global x_raw, y_raw, x_average, y_average, mapped_x, mapped_y, button1_state, button2_state, button3_state, button4_state
	while True:
		lEyeX.angle = mapped_x
		rEyeX.angle = mapped_x
		lEyeY.angle = mapped_y	
		rEyeY.angle = mapped_y
		time.sleep(0.3)
		

#-----------------------------------------------------------------------------------------------------------------

#------------------------------PRINT INPUT VALUES-----------

def print_inputs():
	global x_raw, y_raw, x_average, y_average, mapped_x, mapped_y, button1_state, button2_state, button3_state, button4_state
	
	while True:
		try:
			# Print the mapped values
			print(f"      ")
			print(f"X Raw: {x_raw}")
			print(f"Y Raw: {y_raw}")
			print(f"X Average: {x_average}")
			print(f"Y Mapped: {y_average}")
			print(f"X Angle: {mapped_x}")
			print(f"Y Angle: {mapped_y}")
			print(f"Button 1 : {button1_state}")
			print(f"Button 2 : {button2_state}")
			print(f"Button 3 : {button3_state}")
			print(f"Button 4 : {button4_state}")
			time.sleep(0.5)
		except Exception as e:
			print(f"Exception: {e}")

#------------------------------------------------------------


###############################TO DO################################

#-------------------------------wings-X-----------------------------

#Wing X - We will need to re-map from 90 to 0 and have some previous state logic so position sticks

def wingx(btn):
	global x_raw, x_average, mapped_x, button1_state, button2_state, button3_state, button4_state
	while True:
		if mapped_x >= 0 and mapped_x <= 90:
			wingMid.angle = mapped_x
			time.sleep(.3)
	

#------------------------------------------------------------

#-------------------------------wings-Y----------------------------

#Wing Y - 

def wingy(btn):
	global y_raw,y_average, mapped_y, button1_state, button2_state, button3_state, button4_state
	while True:
		if mapped_y >= 0 and mapped_y <= 90:
			wingLeft.angle = mapped_y
			wingRight.angle = mapped_y
			time.sleep(.3)
	

#------------------------------------------------------------

#blink

def blink():
	global button1_state
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
	while True:
		print("Random")
		time.sleep(random.randint(5, 12)) #seconds
		blink()
	time.sleep()

#------------------------------------------------------------

def wakeup():
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

#Threading
sock_read = threading.Thread(target=sockread)
#read_thread = threading.Thread(target=read_inputs)
#print_thread = threading.Thread(target=print_inputs)
eye_thread = threading.Thread(target=eyemovement)
#random_blink = threading.Thread(target=randomblink)


#Logic flow / main code
startup()
sock_read.start()
#read_thread.start()
#print_thread.start()
eye_thread.start()
#random_blink.start()

#function of buttons when pressed

