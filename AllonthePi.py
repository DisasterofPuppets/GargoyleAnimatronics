from gpiozero import MCP3008, Button
from time import sleep

# Create an instance of the button
button1 = Button(22, pull_up=True)
button2 = Button(27, pull_up=True)
button3 = Button(17, pull_up=True)
button4 = Button(4, pull_up=True)

#create objects called that refer to MCP3008 channel 0 and 1
c0 = MCP3008(0) #X-Axis
c1 = MCP3008(1) #Y-Axis



while True:
    
    # Read the button state
    button1_state = button1.is_pressed
    button2_state = button2.is_pressed
    button3_state = button3.is_pressed
    button4_state = button4.is_pressed
        
    # Print the values
    print(f"X: {c0.value}")
    #print(f"Y: {c1.value}")
    #print(f"YButton 1 : {button1_state}")
    #print(f"YButton 2 : {button2_state}")
    #print(f"YButton 3 : {button3_state}")
    #print(f"YButton 4 : {button4_state}")
    sleep(0.5)

   
