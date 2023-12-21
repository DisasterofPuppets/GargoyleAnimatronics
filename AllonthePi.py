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



################################### WIFI ##################################
def wifi():
    while True:
        data = client_socket.recv(1024).decode()
        if data:
            global xPosition
            global yPosition
            global joyB
            global mod1B
            global mod2B
            global mod3B
            xPosition, yPosition, joyB, mod1B, mod2B, mod3B = data.split(",")
        # Process the received variables
        print("X:", xPosition)
        print("Y:", yPosition)
        print("Joy Button:", joyB)
        print("Mod1 Button:", mod1B)
        print("Mod2 Button:", mod2B)
        print("Mod3 Button:", mod3B)
        pass


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

#----------------------------------------MAP JOYSTICK TO SERVO ANGLE---------------------------------------------------
def map_joystick_value(raw_value, axis):
    # Replace min_input and max_input with the actual range of your joystick
    min_input = 0.0004885197850512668  # replace with actual minimum raw value
    max_input = 1  # replace with actual maximum raw value

    # Replace min_output and max_output with the desired output range
    min_output = 0
    max_output = 180

    # Map the raw value to the desired output range
    mapped_value = ((raw_value - min_input) * (max_output - min_output)) / (max_input - min_input) + min_output
    return int(mapped_value)  # Convert to integer if needed
#----------------------------------------------------------------------------------------------------------------------- 

#---------------------------------Average Joystick Reading----------------------------------

def average_data(joystick_axis_value):
    num_readings = 10
    total_reading = 0

    for _ in range(num_readings):
        try:
            reading = joystick_axis_value  # Joystick Axis
            #print(f"Reading: {reading}")  # Add this line for debugging
        except Exception as e:
        #    print(f"Error in joystick_axis: {e}")
            reading = 0  # Set a default value in case of an error

        total_reading += reading

    average_reading = total_reading / num_readings
    return average_reading
#-----------------------------------------------------------------------------------



#-------------------------------READ ADC-------------------------------
def read_adc(axis):
    if axis == 'x':
        return xAxis.value
    elif axis == 'y':
        return yAxis.value
    else:
        raise ValueError("Invalid axis. Use 'x' or 'y'.")

#----------------------------------------------------------------------

#-------------------------------READ JOYSTICK-------------------------------
def read_inputs():
	global x_raw, y_raw, x_average, y_average, mapped_x, mapped_y, button1_state, button2_state, button3_state, button4_state
	
	while True:
		x_raw = read_adc('x')
		y_raw = read_adc('y')
		x_average = average_data(x_raw)
		y_average = average_data(y_raw)
		mapped_x = map_joystick_value(x_average, 'x')
		mapped_y = map_joystick_value(y_average, 'y')
		# Read the button states
		button1_state = button1.is_pressed
		button2_state = button2.is_pressed
		button3_state = button3.is_pressed
		button4_state = button4.is_pressed

#-------------------------------READ ADC-------------------------------

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
read_thread = threading.Thread(target=read_inputs)
print_thread = threading.Thread(target=print_inputs)
eye_thread = threading.Thread(target=eyemovement)
#random_blink = threading.Thread(target=randomblink)


#Logic flow / main code
startup()
read_thread.start()
print_thread.start()
eye_thread.start()
#random_blink.start()

#function of buttons when pressed
#This will change substantially when we switch to serial input from the esp32
button1.when_pressed = wingx
button1.when_pressed = wingy

