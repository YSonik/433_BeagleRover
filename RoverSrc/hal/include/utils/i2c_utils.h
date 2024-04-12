#ifndef _I2C_UTILS_H_
#define _I2C_UTILS_H_

void configurePinI2c(const char *pin);

int initI2cBus(char *bus, int address);

void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value);

unsigned char readI2cReg(int i2cFileDesc, unsigned char regAddr);

void readI2cBlockData(int i2cFileDesc, unsigned char regAddr, unsigned char *data, int length);

#endif // _I2C_UTILS_H_