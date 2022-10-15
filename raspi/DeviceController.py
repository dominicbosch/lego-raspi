# Control custom devices on a raspberry/arduino stack.

import time
import pandas as pd
import I2CInterface
from devices.NXTMotor import NXTMotor

df = pd.read_csv('../error_codes.tsv', sep='\t')
print("Available error codes:")
print(df)
print("\n")

i2c = I2CInterface.I2CInterface(0x20, 16) # slave addr & reply length

motorA = NXTMotor(i2c, 0, 5, 6, 2, 3)
motorA.forward(100)
time.sleep(2)
motorA.stop(1) # apply breaks
time.sleep(1)
motorA.backward(255)
time.sleep(2)
motorA.forward(0)
time.sleep(1)
motorA.rotateTo(300, 1)
time.sleep(5)
motorA.stop(0) # no breaks

while True:
    #
    # INIT Device Message Format: 0[DeviceType][DeviceID][Pin1]...[PinN]
    # For DeviceType 0 (NXT Motor), four pins need to be defined: MotionA, MotionB, InterruptSensorA, InterruptSensorB.
    # There is also only three slots available for NXT Motors (DeviceID)
    # e.g.: 0 0 0 5 6 2 3
    
    cmd = input("Enter command: ")
    arr = list(map(int, cmd.split(" ")))

    print("Sent the " + str(cmd) + " command.")
    print(arr)
    data = i2c.sendByteArray(arr)
    print("recieve from slave:")
    print(data)

    # Handle device initialization answer
    if data[0] == 0:
        print("Device Type " + str(data[1]) + " successfully initialized.")
        print("DeviceID " + str(data[2]) + " on pins " + ", ".join(map(str, data[3:7])))

    # Handle error codes:
    if data[0] == 255:
        print(df.loc[df['code'] == data[1]])

