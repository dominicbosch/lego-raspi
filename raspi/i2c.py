# Raspberry Pi to Arduino I2C Communication
# Initial Version
# PYTHON

# PEB 28.7.2021
#
# Challenges : Libraray smbus2 not available for Python 3 / Input not available in 2.7 (raw_input used instead)

import sys
import smbus2 as smbus#,smbus2
import time

forever = True;             # Endless loop to catch commands

I2C_SLAVE_ADDRESS = 0x05    # i2c Bus Adress for Master and Slave

# Converting a string to an array of bytes.
def ConvertStringsToBytes(src):
  converted = [];
  for b in src:
    converted.append(ord(b));
  return converted

I2Cbus = smbus.SMBus(1);
with smbus.SMBus(1) as I2Cbus:
    
    while forever == True:
    
        cmd = raw_input("Enter command: ");
        BytesToSend = ConvertStringsToBytes(cmd);

        print("Sent " + str(I2C_SLAVE_ADDRESS) + " the " + str(cmd) + " command.");
        print(BytesToSend );

        I2Cbus.write_i2c_block_data(I2C_SLAVE_ADDRESS, 0x00, BytesToSend);
        
        data=I2Cbus.read_i2c_block_data(I2C_SLAVE_ADDRESS,0x00,16)
        print("recieve from slave:")
        print(data)

