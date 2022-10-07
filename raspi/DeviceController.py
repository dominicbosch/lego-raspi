# Control custom devices on a raspberry/arduino stack.

import sys
import smbus2 as smbus
import time

forever = True;             # Endless loop to catch commands

I2C_SLAVE_ADDRESS = 0x05    # i2c Bus Adress for Master and Slave

I2Cbus = smbus.SMBus(1)
with smbus.SMBus(1) as I2Cbus:
    
    while forever == True:
        #
        # INIT Device Message Format: 0[DeviceType][DeviceID][Pin1]...[PinN]
        # For DeviceType 0 (NXT Motor), four pins need to be defined: MotionA, MotionB, InterruptSensorA, InterruptSensorB.
        # There is also only three slots available for NXT Motors (DeviceID)
        # e.g.: 0|0|0|5|6|2|3

        cmd = raw_input("Enter command: ")
        arr = list(map(int, cmd.split("|")))

        print("Sent " + str(I2C_SLAVE_ADDRESS) + " the " + str(cmd) + " command.")
        print(arr)

        firstByte = arr.pop(0)
        I2Cbus.write_i2c_block_data(I2C_SLAVE_ADDRESS, firstByte, arr)
        time.sleep(0.5)
        data=I2Cbus.read_i2c_block_data(I2C_SLAVE_ADDRESS,0x00,16)
        print("recieve from slave:")
        print(data)

