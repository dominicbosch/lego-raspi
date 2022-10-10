
import time
import smbus2

class I2CInterface:
    def __init__(self, slaveAddr, replyLength):
        self.slaveAddr = slaveAddr
        self.replyLength = replyLength
        self.i2cBus = smbus2.SMBus(1)
        
    def sendByteArray(self, arr):        
        print("sending to slave: ")
        print(arr)
        
        firstByte = arr.pop(0)
        self.i2cBus.write_i2c_block_data(self.slaveAddr, firstByte, arr)
        time.sleep(0.1)
        data = self.i2cBus.read_i2c_block_data(self.slaveAddr, 0x00, self.replyLength)
        print("got reply from slave: ")
        print(data)
        return data

        