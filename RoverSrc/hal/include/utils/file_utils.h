#ifndef _FILE_UTILS_H_
#define _FILE_UTILS_H_

#include <stddef.h>

void writeToFile(const char* fileName, const char* value);
int readLineFromFile(char* fileName, char* buff, size_t maxLength);
void runCommand(char* command);

#endif // _UTILS_H_
