#ifndef _HELPER_H_
#define _HELPER_H_

// helper function to run commands for our c program
void runCommand(char* command);

int readValueFromFile(char *fileName);

void sleepForMs (long long delayInMs);

void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value);

//Opens the I2C bus for usage
int openI2CBus(char* bus, int address);

//Reads an I2C register
unsigned char readI2cReg(int i2cFileDesc, unsigned char regAddr);

void writeToFile(const char *fileName, const char *value);

#endif