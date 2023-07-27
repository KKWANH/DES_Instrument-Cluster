import os
import time
import socket
import pathlib
import threading
from piracer.vehicles import PiRacerStandard
from piracer.gamepads import ShanWanGamepad

FILE_DIR = pathlib.Path(os.path.abspath(os.path.dirname(__file__)))

def display_carinfo_thread(vehicle: PiRacerStandard):

    def get_battery_level(vehicle: PiRacerStandard):
        batteryVoltage = vehicle.get_battery_voltage()
        batterylevel = int((11.25 / batteryVoltage) * 100) # new batteries fully charched with 11.25V
        batterylevel_text = f'{batterylevel}%'
        return batterylevel_text

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
        display = vehicle.get_display()                                                            # Get SSD1306_I2C object 
        display.fill(0)                                                                            # Clear display
        display.text(batteryVoltage, 0, 0, 'white', font_name=FILE_DIR/'fonts'/'font5x8.bin')      # Print first row 
        display.text(ipAddress, 0, 16, 'white', font_name=FILE_DIR/'fonts'/'font5x8.bin')          # Print second row, assuming the text is 16 pixels in height 
        display.show()                                                                             # Show the updated display with both texts


    try:
        while True:
                # print ip address and battery level when the programm started to oled
            ipAddr = get_ip_address()
            batlvl = get_battery_level(piracer)
            display_carinfo(ipAddr, batlvl, piracer)
            time.sleep(1)  # Update the display every 1 second
    except KeyboardInterrupt:
        pass

def car_control_thread(vehicle: PiRacerStandard, gamepad: ShanWanGamepad):
    try:
        while True:
            gamepad_input = gamepad.read_data()

            throttle = gamepad_input.analog_stick_left.y
            steering = gamepad_input.analog_stick_left.x

            vehicle.set_throttle_percent(throttle)
            vehicle.set_steering_percent(steering)

            print(f'throttle: {throttle}, steering: {steering}')
    except KeyboardInterrupt:
        vehicle.set_steering_percent(0)

if __name__ == '__main__':
    
    piracer = PiRacerStandard()             
    shanwan_gamepad = ShanWanGamepad()

    try:      
        # Run oled and car control on seperate threads
        car_control_thread  = threading.Thread(target=car_control_thread, args=(piracer, shanwan_gamepad))
        car_info_thread     = threading.Thread(target=display_carinfo_thread, args=(piracer,))
        car_control_thread.daemon = True
        car_info_thread.daemon = True
        car_control_thread.start()
        car_info_thread.start()

        while True:
            # Ensure the main thread doesn't exit and keeps running indefinitely
            time.sleep(1)

    except KeyboardInterrupt:
        # Cmd+C (MacOs😄)
        print("Programm stopped.")
