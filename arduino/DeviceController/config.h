// Some important constants for reuse and maintenance

#define BAUD_RATE 115200                    // should be fast enough and still allow serial to work
#define I2C_ADDRESS 0x20                    // I2C Address
#define STATIC_MSG_LENGTH 16                // comm protocol: 16 bytes exchanged back and forth
#define INTERVAL_DEVICE_UPDATE 100          // The interval for the devices to update themselves
#define INTERVAL_PRINT 5000                 // The print interval
#define NXTMOTOR_TICKS_PER_ROTATION 720     // number of interrupts on the sensors per rotation on the NXT Motor