import dbus, dbus.mainloop.glib, sys
from gi.repository import GLib
import basic_control

import RPi.GPIO as GPIO
import time
import subprocess

GPIO.setmode(GPIO.BCM)

left = 2
center = 3
right = 4

# GPIO.setup(left, GPIO.IN, pull_up_down=GPIO.PUD_UP)
# GPIO.setup(center, GPIO.IN, pull_up_down=GPIO.PUD_UP)
# GPIO.setup(right, GPIO.IN, pull_up_down=GPIO.PUD_UP)

GPIO.setup(left,   GPIO.IN)
GPIO.setup(center, GPIO.IN)
GPIO.setup(right,  GPIO.IN)

def basic_control(bus):
    switchState = str(GPIO.input(left))+str(GPIO.input(center))+str(GPIO.input(right))
    #print(switchState)
    if switchState != "111":
        prevState = switchState
        #timer should start with 0
        start = time.time()
        #time should end with 1
        while True:
            #switchState = str(GPIO.input(left))+str(GPIO.input(center))+str(GPIO.input(right))
            #print(switchState)
            if prevState == "101" and GPIO.input(center) == 1:
                end = time.time()
                diff = end - start
                if diff >  3 :
                    print(" the center button was held for 3 seconds or greater")
                    subprocess.call(["/home/headgear/ofono-1.18/test/answer-calls"])
                else:
                    print(" the center button was held for less than 3 seconds")
                    subprocess.call(["/home/headgear/ofono-1.18/test/hangup-active"])
                    on_playback_control('play')
                    on_playback_control('pause')
                break
            
            elif prevState == "011" and GPIO.input(left) == 1:
                end = time.time()
                diff = end - start
                if diff >  3 :
                    print(" the left button was held for 3 seconds or greater")
                    print("decrease volume")
                if diff > 1:
                    on_playback_control('prev')
                    on_playback_control('prev')
                else:
                    print(" the left button was held for less than 3 seconds")
                    on_playback_control('prev')
                break
            
            elif prevState == "110" and GPIO.input(right) == 1:
                end = time.time()
                diff = end - start
                if diff >  3 :
                    print(" the right button was held for 3 seconds or greater")
                    print("increase volume")
                else:
                    print(" the right button was held for less than 3 seconds")
                    on_playback_control('next')
                break  
    #else:
        #subprocess.call(["/home/headgear/ofono-1.18/test/monitor-ofono"])
        #print("None of the buttons were called")
    time.sleep(0.1)

def on_property_changed(interface, changed, invalidated):
    if interface != 'org.bluez.MediaPlayer1':
        return
    for prop, value in changed.items():
        if prop == 'Status':
            print('Playback Status: {}'.format(value))
        elif prop == 'Track':
            print('Music Info:')
            for key in ('Title', 'Artist', 'Album'):
                print('   {}: {}'.format(key, value.get(key, '')))

def on_playback_control(fd):
    #str = fd.readline()
    str = fd
    if str.startswith('play'):
        player_iface.Play()
    elif str.startswith('pause'):
        player_iface.Pause()
    elif str.startswith('next'):
        player_iface.Next()
    elif str.startswith('prev'):
        player_iface.Previous()
    return True

def notifications(bus, message):
    global port

    args = message.get_args_list()

    title = ""
    body = ""
    level = ""

    cnt = 0
    for arg in args:
      if cnt == 3: title = str ( arg )
      elif cnt == 4: body = arg
      elif cnt == 6: level = str ( int ( arg [ 'urgency' ] ) )
      cnt += 1
      print(args)
      print(message)
    print( "C" + level + "\n" )
    print( "1" + title + "-" * 16 + "\n" )
    print( "2" + body + "-" * 16 + "\n" )

if  __name__ == "__main__":
  
    dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)
    bus = dbus.SystemBus()
    obj = bus.get_object('org.bluez', "/")
    mgr = dbus.Interface(obj, 'org.freedesktop.DBus.ObjectManager')
    player_iface = None
    transport_prop_iface = None
    for path, ifaces in mgr.GetManagedObjects().items():
        if 'org.bluez.MediaPlayer1' in ifaces:
            player_iface = dbus.Interface(
                    bus.get_object('org.bluez', path),
                    'org.bluez.MediaPlayer1')
        elif 'org.bluez.MediaTransport1' in ifaces:
            transport_prop_iface = dbus.Interface(
                    bus.get_object('org.bluez', path),
                    'org.freedesktop.DBus.Properties')
    if not player_iface:
        sys.exit('Error: Media Player not found.')
    if not transport_prop_iface:
        sys.exit('Error: DBus.Properties iface not found.')

    bus.add_signal_receiver(
            on_property_changed,
            bus_name='org.bluez',
            signal_name='PropertiesChanged',
            dbus_interface='org.freedesktop.DBus.Properties')
    bus_ = dbus.SessionBus()
    bus_.add_match_string_non_blocking("eavesdrop=true, interface='org.freedesktop.Notifications', member='Notify'")
    bus_.add_message_filter(notifications)
#     while True:
#         basic_control(bus)
    GLib.MainLoop().run()
