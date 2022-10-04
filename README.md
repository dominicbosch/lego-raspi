# lego-raspi
Commanding the NXT devices through Arduino &amp; RaspberryPi

# Raspberry
The raspberry should advertise itself as lego1 in the configured networks.
Login via `ssh pi@lego1`. pw is what we often used for supersonic dev devices!

# i2c communication protocol
The i2c interface allows communication through byte exchange. The first byte switches between different scenarios:

- `0` - Initialisation of devices. The byte array is interpreted as: `0[DeviceType][DeviceID][Pin1][Pin2]...[PinN]`
- `1` - Control the device: set run parameters or get sensor output. `1[DeviceID][Method][Param1]...[ParamN]`



# Devices
## Motors
The motors currently in use are the [lego NXT motors](https://www.philohome.com/nxtmotor/nxtmotor.htm) (or [here as pdf for eternity](documetnation/motorspecs.pdf)). Each motor needs a motor driver. we currently use the [DRV8833](https://www.ti.com/product/DRV8833).

Each of these setups occupy 4 pins:
- 2 pins for motion, one for each direction while the other one needs to be LOW.
- 2 pins for the optical sensor feedback that allows to monitor the movement of the motor. These pins need to be handled as interrupts and the time differential between the interrupt invocation determines the speed in one direction.
From the above linked source:
	
> Encoder wheel and enclosure back parts. We have 12 slits in encoder, and motor to encoder gear reduction is 10:32. So for 1 turn of output hub, encoders turns 48*10/32=15 turns, optical detectors sees 15*12=180 slits. Using both sides of slits gives nominal 360 ticks per turn resolution. Note that since we have a quadrature encoder, the maximum resolution is 720 ticks/turn, but this is not used by standard NXT firmware.


