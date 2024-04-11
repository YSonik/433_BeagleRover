#ifndef _TIME_UTILS_H_
#define _TIME_UTILS_H_

#include <stddef.h>

long long getTimeInMs(void);
long long getTimeInUs(void);
void sleepForMs(long long delayInMs);
void sleepForNs(long long delayInNs);

#endif // _UTILS_H_
