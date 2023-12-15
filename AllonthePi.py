import spidev
from gpiozero import Button

# Create an instance of the spidev SPI device (for the analog ADC)
spi = spidev.SpiDev()
spi.open(0, 0)

# Create an instance of the button
button = Button(22, pull_up=True)

# Main loop
while True:
    # Read the X-axis and Y-axis values from the MCP3008
    x_value = spi.xfer2([0x01, 0x80, 0x00])[1] & 0x3F
    y_value = spi.xfer2([0x01, 0xC0, 0x00])[1] & 0x3F
    
    # Read the button state
    button_state = button.is_pressed
    
    # Print the values
    print(f"X-axis: {x_value}, Y-axis: {y_value}, Button: {button_state}")

#MCP3008 wiring

# 1 16
# 2 15
# 3 14
# 4 13
# 5 12
# 6 11
# 7 10
# 8 09

# Pin 16 VDD to Pi 3.3V
# Pin 15 VRef to Pi 5V
# Pin 14 AGND - Pi ground
# Pin 13 SCLK - Pi GPIO11 (SCLK pin 23)
# Pin 12 DOut - pi GPIO09 (MISO pin 21)
# Pin 11 DIn - pi GPIO10 (MOSI pin 10)
# Pin 10 CS/SHDN - Pi ground
# Pin 09 DGND - Pi ground
# Pin 01 X Joystick Output
# Pin 02 Y Joystick Output
