
class NXTMotor:
    def __init__(self, i2c, id, motionPinA, motionPinB, sensorPinA, sensorPinB):
        self.i2c = i2c
        self.id = id
        self.motionPinA = motionPinA
        self.motionPinB = motionPinB
        self.sensorPinA = sensorPinA
        self.sensorPinB = sensorPinB
        self.i2c.sendByteArray([0, 0, id, motionPinA, motionPinB, sensorPinA, sensorPinB])

    def forward(self, speed):
        self.i2c.sendByteArray([1, 0, self.id, 0, speed])

    def backward(self, speed):
        self.i2c.sendByteArray([1, 0, self.id, 1, speed])