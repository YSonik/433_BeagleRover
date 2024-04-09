#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <pthread.h>
#include <stdbool.h>

#include "helper.h"

#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"

#define I2C_DEVICE_ADDRESS 0x18 //0X18   

#define CTRL_REG1 0x20  //0X20

static bool stopping = false;

static int i2cFileDesc;

char accelValues[7];
int16_t xAccl;
int16_t yAccl;
int16_t zAccl;


// Initialize the accelerometer
void Accel_init(void)
{
   runCommand("config-pin P9_18 i2c");
   runCommand("config-pin P9_17 i2c");


   //open the bus
   i2cFileDesc = openI2CBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);

   //set the control register for the accelerometer to ACTIVE
   writeI2cReg(i2cFileDesc, CTRL_REG1, 0x27);   //0X20

}

void Accel_cleanup(void)
{
   stopping = true;
   close(i2cFileDesc);
}

void getAccelerometerValues(int16_t *xAccl, int16_t *yAccl)
{
    int x = 0X28 + 0X80;
   // To read a register, must first write the address
   int res = write(i2cFileDesc, &x, sizeof(x));
   if (res != sizeof(x)) {
        printf("Size of regAddr: %zu\n", sizeof(x));
        perror("I2C: Unable to write to i2c register. error is here");
        exit(1);
   }
   // Now read the value and return it
   int size = 7;
   res = read(i2cFileDesc, &accelValues, sizeof(char)*size);
   if (res != sizeof(accelValues)) {
       perror("I2C: Unable to read from i2c register");
       exit(1);
   }

   *xAccl = (accelValues[0] << 8) | (accelValues[1]) ;
   *xAccl = *xAccl >> 4;  

   *yAccl = (accelValues[2] << 8) | (accelValues[3]) ;
   *yAccl = *yAccl >> 4;

   zAccl = (accelValues[4] << 8) | (accelValues[5]);
   zAccl = zAccl >> 4;
}   