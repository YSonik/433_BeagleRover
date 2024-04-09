#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <time.h>

#include "helper.h"

// helper function to run commands for our c program
void runCommand(char* command)
{
        // Execute the shell command (output into pipe)
        FILE *pipe = popen(command, "r");
        // Ignore output of the command; but consume it
        // so we don't get an error when closing the pipe.
        char buffer[1024];
        while (!feof(pipe) && !ferror(pipe)) {
            if (fgets(buffer, sizeof(buffer), pipe) == NULL)
                break;
            // printf("--> %s", buffer); // Uncomment for debugging
        }
        // Get the exit code from the pipe; non-zero is an error:
        int exitCode = WEXITSTATUS(pclose(pipe));
        if (exitCode != 0) {
            perror("Unable to execute command:");
            printf(" command: %s\n", command);
            printf(" exit code: %d\n", exitCode);
        }
}

int readValueFromFile(char *fileName)
{
    // open the file in read mode "r"
    FILE *pFile = fopen(fileName, "r");
    // if pFile is empty, print an error message and exit
    if (pFile == NULL) {
        printf("ERROR: Unable to open file (%s) for read\n", fileName);
        exit(-1);
    }
    // Read single character value
    int value = fgetc(pFile);
    // check if we've reach EOF or if value is not 0 or 1
    if (value == EOF || (value != '0' && value != '1')) {
        // print an erorr message and close the file and proceed to exit
        printf("ERROR: Unable to read a valid value from the file (%s)\n", fileName);
        fclose(pFile);
        return -1;
    }
    // close the file and return value
    fclose(pFile);
    return value - '0';
}

void sleepForMs (long long delayInMs)
{
    const long long NS_PER_MS = 1000000;
    const long long NS_PER_SECOND = 1000000000;

    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;

    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}

// Writes to the I2C register.
// Pulled from Dr. Brian's example code.
void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value)
{
    unsigned char buff[2];
    buff[0] = regAddr;
    buff[1] = value;
    int result = write(i2cFileDesc, buff, 2);
    if(result != 2){
        perror("I2C: Unable to write i2c register.");
        exit(1);
    }
}

// Opens the I2C bus so that we can write to the register
int openI2CBus(char* bus, int address)
{
    int i2cFileDesc = open(bus, O_RDWR);

    int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
    if(result < 0){
        perror("I2C: Unable to set I2C device to slave address.");
        exit(1);
    }

    return i2cFileDesc;

}

unsigned char readI2cReg(int i2cFileDesc, unsigned char regAddr)
{
    // To read a register, must first write the address
    int res = write(i2cFileDesc, &regAddr, sizeof(regAddr));
    if (res != sizeof(regAddr)) {
        perror("I2C: Unable to write to i2c register.");
        exit(1);
    }
    // Now read the value and return it
    char value = 0;
    res = read(i2cFileDesc, &value, sizeof(value));
    if (res != sizeof(value)) {
        perror("I2C: Unable to read from i2c register");
        exit(1);
    }
    return value;
}

void writeToFile(const char *fileName, const char *value)
{
    // Use fopen() to open the file for write access.
    FILE *pFile = fopen(fileName, "w");
    if (pFile == NULL)
    {
        printf("ERROR: Unable to open file (%s)\n", fileName);
        exit(1);
    }
    // Write to data to the file using fprintf():
    fprintf(pFile, "%s", value);

#ifdef DEBUG
    printf("Wrote to file: %s\n", fileName);
    printf("Value: %s\n\n", value);
#endif

    // Close the file using fclose():
    fclose(pFile);
}