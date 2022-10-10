# Control custom devices on a raspberry/arduino stack.

import pandas as pd
import I2CInterface
import devices.NXTMotor as NXTMotor

i2c = I2CInterface.I2CInterface(0x20, 16) # slave addr & reply length

motorA = NXTMotor(i2c, 0, 5, 6, 2, 3)
motorA.forward(100)

df = pd.read_csv('../error_codes.tsv', sep='\t')
print(df)

while True:
    #
    # INIT Device Message Format: 0[DeviceType][DeviceID][Pin1]...[PinN]
    # For DeviceType 0 (NXT Motor), four pins need to be defined: MotionA, MotionB, InterruptSensorA, InterruptSensorB.
    # There is also only three slots available for NXT Motors (DeviceID)
    # e.g.: 0 0 0 5 6 2 3
    
    cmd = raw_input("Enter command: ")
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

