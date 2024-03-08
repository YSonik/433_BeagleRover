#define _POSIX_C_SOURCE 200809L
#include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <errno.h> // Errors
#include <string.h>

#include <sys/epoll.h> // for epoll()
#include <fcntl.h>     // for open()
#include <unistd.h>    // for close()

#include "utils/file_utils.h"


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

int readLineFromFile(char *fileName, char *buff, size_t maxLength)
{
    FILE *file = fopen(fileName, "r");
    int bytes_read = getline(&buff, &maxLength, file);
    fclose(file);

#ifdef DEBUG
    printf("Read from file: %s\n", fileName);
#endif

    return bytes_read;
}

void runCommand(char *command)
{
// Execute the shell command (output into pipe)
#include <stdio.h> // Add the necessary header file

    FILE *pipe = popen(command, "r");
    // Ignore output of the command; but consume it
    // so we don't get an error when closing the pipe.
    char buffer[1024];
    while (!feof(pipe) && !ferror(pipe))
    {
        if (fgets(buffer, sizeof(buffer), pipe) == NULL)
            break;
        // printf("--> %s", buffer); // Uncomment for debugging
    }
    // Get the exit code from the pipe; non-zero is an error:
    int exitCode = WEXITSTATUS(pclose(pipe));
    if (exitCode != 0)
    {
        perror("Unable to execute command:");
        printf(" command: %s\n", command);
        printf(" exit code: %d\n", exitCode);
    }

#ifdef DEBUG
    printf("Ran command: %s\n", command);
#endif
}