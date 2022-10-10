class NXTMotor:
    def __init__(self, id, motionPinA, motionPinB, sensorPinA, sensorPinB):
        self.id = id
        self.motionPinA = motionPinA
        self.motionPinB = motionPinB
        self.sensorPinA = sensorPinA
        self.sensorPinB = sensorPinB
        
    def myfunc(self):
        print("Hello my name is " + self.name)