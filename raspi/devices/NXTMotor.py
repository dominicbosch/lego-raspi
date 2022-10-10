
class NXTMotor:
    def __init__(self, i2c, id, motionPinA, motionPinB, sensorPinA, sensorPinB):
        self.i2c = i2c
        self.id = id
        self.motionPinA = motionPinA
        self.motionPinB = motionPinB
        self.sensorPinA = sensorPinA
        self.sensorPinB = sensorPinB
        data = self.i2c.sendByteArray([0, 0, id, motionPinA, motionPinB, sensorPinA, sensorPinB])
        print(data)

    def forward(self, speed):
        data = self.i2c.sendByteArray([1, 0, id, 0, speed])
        print(data)