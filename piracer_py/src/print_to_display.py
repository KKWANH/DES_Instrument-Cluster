import os
import pathlib
import time
import socket
from piracer.vehicles import PiRacerStandard

FILE_DIR = pathlib.Path(os.path.abspath(os.path.dirname(__file__)))

def print_battery_report(vehicle: PiRacerStandard):
    battery_voltage = vehicle.get_battery_voltage()
    battery_current = vehicle.get_battery_current()
    power_consumption = vehicle.get_power_consumption()

    display = vehicle.get_display()

    output_text = 'U={0:0>6.3f}V, I={1:0>8.3f}mA, P={2:0>6.3f}W'.format(battery_voltage, battery_current,power_consumption)

    display.fill(0)
    display.text(output_text, 0, 0, 'white', font_name=FILE_DI, 'fonts/font5x8.bin')
    display.show()

def print_ip_address(vehicle: PiRacerStandard):
    hostname=socket.gethostname()
    ipAddr=socket.gethostbyname(hostname)
    display = vehicle.get_display()

    output_text = f'IP:{ipAddr}'

    display.fill(0)
    display.text(output_text, 1, 0, 'white', font_name=FILE_DI, 'fonts/font5x8.bin')
    display.show()    

if __name__ == '__main__':

    piracer = PiRacerStandard()

    while True:
        print_battery_report(piracer)
        print_ip_address(piracer)
        time.sleep(0.5)
