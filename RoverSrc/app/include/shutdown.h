#ifndef _SHUTDOWN_H_
#define _SHUTDOWN_H_

#include <stdbool.h>

void Shutdown_init();
void Shutdown_cleanup();

void Shutdown_request();
bool Shutdown_isRequested();

#endif