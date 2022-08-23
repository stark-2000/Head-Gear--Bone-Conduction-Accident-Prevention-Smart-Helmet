import RPi.GPIO as GPIO
import time
import subprocess
#import mediacontrol

def GPIO_init():
    GPIO.setmode(GPIO.BCM)

    left = 2
    center = 3
    right = 4

    # GPIO.setup(left, GPIO.IN, pull_up_down=GPIO.PUD_UP)
    # GPIO.setup(center, GPIO.IN, pull_up_down=GPIO.PUD_UP)
    # GPIO.setup(right, GPIO.IN, pull_up_down=GPIO.PUD_UP)

    GPIO.setup(left, GPIO.IN)
    GPIO.setup(center, GPIO.IN)
    GPIO.setup(right, GPIO.IN)

def basic_control():
    switchState = str(GPIO.input(left))+str(GPIO.input(center))+str(GPIO.input(right))
    print(switchState)
    if switchState != "111":
        #timer should start with 0
        start = time.time()
        #time should end with 1
        while True:
            if GPIO.input(center) == 1:
                end = time.time()
                diff = end - start
                if diff >  3 :
                    print(" the left button was held for 3 seconds or greater")
                    subprocess.call(["/home/headgear/ofono-1.18/test/answer-calls"])
                else:
                    print(" the left button was held for less than 3 seconds")
                    subprocess.call(["/home/headgear/ofono-1.18/test/hangup-active"])                 
                break                
    else:
        mediacontrol.on_playback_control('play')
        #subprocess.call(["/home/headgear/ofono-1.18/test/monitor-ofono"])
        print("the left button wasnt called")
    time.sleep(0.1)