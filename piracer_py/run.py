import  time
import  threading
from    piracer.vehicles import PiRacerStandard
from    piracer.gamepads import ShanWanGamepad
from    threads.display_carinfo_thread import display_carinfo_thread
from    threads.car_control_thread     import car_control_thread

if __name__ == '__main__':
    
    piracer = PiRacerStandard()             
    shanwan_gamepad = ShanWanGamepad()

    try:      

        # Run carinfo@oled thread
        car_control_thread          = threading.Thread(target=car_control_thread, args=(piracer, shanwan_gamepad))
        car_control_thread.daemon   = True
        car_control_thread.start()     

        # Run car control thread
        car_info_thread             = threading.Thread(target=display_carinfo_thread, args=(piracer,))
        car_info_thread.daemon      = True
        car_info_thread.start()

        while True:
            # Ensure the main thread doesn't exit and keeps running indefinitely
            time.sleep(1)

    except KeyboardInterrupt:

        # Cmd+C (MacOs😄)
        print("Programm stopped by KeyboardInterrupt.")

    except SystemExit: 
        print("Programm stopped by itself.")