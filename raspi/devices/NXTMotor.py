
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
        if speed > 255:
            speed = 255
        self.i2c.sendByteArray([1, self.id, 0, speed])

    def backward(self, speed):
        if speed > 255:
            speed = 255
        self.i2c.sendByteArray([1, self.id, 1, speed])

    def stop(self, applyBreaks):
        if applyBreaks > 0:
            applyBreaks = 1
        self.i2c.sendByteArray([1, self.id, 2, applyBreaks])

    def rotateTo(self, angle, direction):
        if angle > 32767:
            angle = 32767
        if direction > 0:
            direction = 1
        else:
            direction = -1
        arrAngle = angle.to_bytes(2, 'big')
        self.i2c.sendByteArray([1, self.id, 3, arrAngle[0], arrAngle[1], direction])