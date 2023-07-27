import os
import time
import socket
from piracer.vehicles import PiRacerStandard
from piracer.gamepads import ShanWanGamepad

def get_battery_voltage(vehicle: PiRacerStandard):
    batteryVoltage = vehicle.get_battery_voltage()
    text = 'U={0:0>6.3f}V'.format(batteryVoltage)
    return text

def get_ip_address():
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)    # Create a socket object
        s.connect(("1.1.1.1", 1))                               # Connect to a non-existent external server
        local_ip_address = s.getsockname()[0]                   # Get the local IP address from the socket
        s.close()                                               # Close the socket
        return local_ip_address
    except Exception as e:
        print("Error while getting local IP address:", e)
        return None
    
def display_carinfo(ipAddress, batteryVoltage, vehicle: PiRacerStandard):
    display = vehicle.get_display()         # Get SSD1306_I2C object 
    display.fill(0)                         # Clear display
    display.text(batteryVoltage, 0, 0)      # Print first row 
    display.text(ipAddress, 0, 16)          # Print second row, assuming the text is 16 pixels in height 
    display.show()                          # Show the updated display with both texts

if __name__ == '__main__':
    piracer = PiRacerStandard()             
    shanwan_gamepad = ShanWanGamepad()
    try:      
        while True:
            # Control Piracer using Controller
            gamepad_input = shanwan_gamepad.read_data()
            throttle = gamepad_input.analog_stick_left.y
            steering = gamepad_input.analog_stick_right.y

            if (abs(throttle) == 0 and abs(steering) == 0):
                ipAddr = get_ip_address()
                batVoltage = get_battery_voltage()
                display_carinfo(ipAddr, batVoltage, piracer)
            else:
                piracer.set_throttle_percent(throttle * 0.5)
                piracer.set_steering_percent(steering)
            
            print(f'throttle: {throttle}, steering: {steering}')
            
    except KeyboardInterrupt:
        # Ctrl+C 
        piracer.set_throttle_percent(0)
        piracer.set_steering_percent(0)