# lego-raspi
Commanding the NXT devices through Arduino &amp; RaspberryPi

# Raspberry
The raspberry should advertise itself as lego1 in the configured networks.
Login via `ssh pi@lego1`. pw is what we often used for supersonic dev devices!
Install pandas: `pip install pandas`

# i2c communication protocol
The i2c interface allows communication through byte exchange. Using SMBUS we are limited to 32 bytes at once. We decided to go for 16 bytes as more than enough for now for receiving back data from the slave. However, the sending of bytes allows for dynamic length. The first byte switches between different scenarios:

- `0` - Initialisation of devices
- `1` - Control the device

## `0` - Initialisation of devices
Each device type expects a different number of initialization bytes.  

### DeviceType `0` - NXTMotor

The motors currently in use are the [lego NXT motors](https://www.philohome.com/nxtmotor/nxtmotor.htm) (or [here as pdf for eternity](documentation/motorspecs.pdf)). Each motor needs a motor driver. we currently use the [DRV8833](https://www.ti.com/product/DRV8833).

Byte Array sent from Master to Slave: `00[DeviceID][MotionPinA][MotionPinB][SensorInterruptPinA][SensorInterruptPinB]`

`[DeviceID]` maps like this:

- `0`: MotorA
- `1`: MotorB
- `2`: MotorC

Each motor requires 4 pins:

- 2 pins for motion, one for each direction while the other one needs to be `LOW`, unless to enable breaking, where both pins are `HIGH`.
- 2 pins for the optical sensor feedback that allows to monitor the movement of the motor. These pins need to be handled as interrupts and the time differential between the interrupt invocation determines the speed in one direction and also the angle so far.

Regarding sensors from the above linked source:
	
> Encoder wheel and enclosure back parts. We have 12 slits in encoder, and motor to encoder gear reduction is 10:32. So for 1 turn of output hub, encoders turns 48*10/32=15 turns, optical detectors sees 15*12=180 slits. Using both sides of slits gives nominal 360 ticks per turn resolution. Note that since we have a quadrature encoder, the maximum resolution is 720 ticks/turn, but this is not used by standard NXT firmware.

:point_right: **Example**
Byte array sent from master to slave: `0 0 0 5 6 2 3` initializes a NXTMotor, identified with `[DeviceID] 0` (MotorA), with `[MotionPinA] 5`, `[MotionPinB] 6`, `[SensorInterruptPinA] 2` & `[SensorInterruptPinB] 3`


## `1` - Control the device
Set run parameters or get sensor feedback. Generic byte array format: `1[DeviceID][Method][Param1]...[ParamN]`
Depending on the Device ID (associated with Device Type), the methods and parameters will be different.


### NXTMotor 
`[DeviceIDs]`: `0`, `1` & `2`, if initialized previously

#### `0` - setForwardSpeed(speed)
`1[DeviceID]0[speed]`

:point_right: **Example**
Byte array sent from master to slave: `1 2 0 255` sets forward speed to `255` on MotorC (`[DeviceID] 2`)


#### `1` - setBackwardSpeed(speed)
`1[DeviceID]1[speed]`

:point_right: **Example**
Byte array sent from master to slave: `1 0 1 100` sets backward speed to `100` on MotorA (`[DeviceID] 0`)

#### `2` - stop(applyBreaks)
`1[DeviceID]2[applyBreaks]`
applyBreaks will be checked for `> 0` to enable breaking mode.

:point_right: **Example**
Byte array sent from master to slave: `1 0 1 50` sets speed to `0` on MotorB (`[DeviceID] 1`) but enables breaking mode as well (`50 > 0`)

#### `3` - rotateTo(angle)






# Ideas for future implementation:
 
Methods inherited from interface lejos.robotics.DCMotor
getPower, setPower
 
Methods inherited from interface lejos.robotics.BaseMotor
backward, flt, forward, isMoving, stop
 
Methods inherited from interface lejos.robotics.Encoder
getTachoCount, resetTachoCount

void	backward()
          Causes motor to rotate backwards.
 void	flt()
          Causes motor to float.
 void	forward()
          Causes motor to rotate forward.
 int	getPower()
          Returns the current motor power setting.
 boolean	isMoving()
          Returns true iff the motor is in motion.
 void	setPower(int power)
          Set the power level 0%-100% to be applied to the motor
 void	stop()
          Causes motor to stop, pretty much instantaneously.
protected  void	updateState(int newMode)
          Update the internal state tracking the motor direction

https://lejos.sourceforge.io/nxt/nxj/api/lejos/nxt/NXTRegulatedMotor.html
          Class NXTRegulatedMotor
java.lang.Object
  extended by lejos.nxt.NXTRegulatedMotor
All Implemented Interfaces:
BaseMotor, Encoder, RegulatedMotor, Tachometer

Abstraction for a NXT motor. The basic control methods are: forward, backward, reverseDirection, stop and flt. To set each motor's velocity, use setSpeed . The maximum velocity of the motor is limited by the battery voltage and load. With no load, the maximum degrees per second is about 100 times the voltage.
The velocity is regulated by comparing the tacho count with velocity times elapsed time, and adjusting motor power to keep these closely matched. Changes in velocity will be made at the rate specified via the setAcceleration(int acceleration) method. The methods rotate(int angle) and rotateTo(int ange) use the tachometer to control the position at which the motor stops, usually within 1 degree or 2.

Listeners. An object implementing the RegulatedMotorListener interface may register with this class. It will be informed each time the motor starts or stops.
Stall detection If a stall is detected or if for some other reason the speed regulation fails, the motor will stop, and isStalled() returns true.
Motors will hold thier position when stopped. If this is not what you require use the flt() method instead of stop().

Method Summary
 void	addListener(RegulatedMotorListener listener)
          Add a motor listener.
 void	backward()
          Causes motor to rotate backwards until stop() or flt() is called.
 void	flt()
          Set the motor into float mode.
 void	flt(boolean immediateReturn)
          Set the motor into float mode.
 void	forward()
          Causes motor to rotate forward until stop() or flt() is called.
 int	getAcceleration()
          returns acceleration in degrees/second/second
 int	getLimitAngle()
          Return the angle that this Motor is rotating to.
 float	getMaxSpeed()
          Returns the maximim speed of the motor.
 int	getPosition()
          Returns the current position that the motor regulator is trying to maintain.
 int	getRotationSpeed()
          Return the current velocity.
 int	getSpeed()
          Return the current target speed.
 int	getTachoCount()
          Returns the tachometer count.
 boolean	isMoving()
          This method returns true if the motor is attempting to rotate.
 boolean	isStalled()
          Return true if the motor is currently stalled.
 void	lock(int power)
          Deprecated. The regulator will always try to hold position unless the motor is set into float mode using flt().
 RegulatedMotorListener	removeListener()
          Removes the RegulatedMotorListener from this class.
 void	resetTachoCount()
          Reset the tachometer associated with this motor.
 void	rotate(int angle)
          Rotate by the requested number of degrees.
 void	rotate(int angle, boolean immediateReturn)
          Rotate by the request number of degrees.
 void	rotateTo(int limitAngle)
          Rotate to the target angle.
 void	rotateTo(int limitAngle, boolean immediateReturn)
          causes motor to rotate to limitAngle;
if immediateReturn is true, method returns immediately and the motor stops by itself
and getTachoCount should be within +- 2 degrees if the limit angle If any motor method is called before the limit is reached, the rotation is canceled.
 void	setAcceleration(int acceleration)
          sets the acceleration rate of this motor in degrees/sec/sec
The default value is 6000; Smaller values will make speeding up.
 void	setSpeed(float speed)
          Sets desired motor speed , in degrees per second; The maximum reliably sustainable velocity is 100 x battery voltage under moderate load, such as a direct drive robot on the level.
 void	setSpeed(int speed)
          Sets desired motor speed , in degrees per second; The maximum reliably sustainable velocity is 100 x battery voltage under moderate load, such as a direct drive robot on the level.
 void	setStallThreshold(int error, int time)
          Set the parameters for detecting a stalled motor.
 void	stop()
          Causes motor to stop, pretty much instantaneously.
 void	stop(boolean immediateReturn)
          Causes motor to stop, pretty much instantaneously.
 boolean	suspendRegulation()
          Removes this motor from the motor regulation system.
 void	waitComplete()
          Wait until the current movement operation is complete (this can include the motor stalling).