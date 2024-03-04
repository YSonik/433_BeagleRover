#include <stdbool.h>
#include <time.h>
#include "hal/timeControl.h"

long long TC_get_current_time_ms(void)
{
    struct timespec curr_time;
    clock_gettime(CLOCK_REALTIME, &curr_time);
    long long seconds = curr_time.tv_sec;
    long long nano_seconds = curr_time.tv_nsec;
    long long milli_seconds = (seconds*1000) + (nano_seconds/1000000);
    return milli_seconds;
}

void TC_sleep_thread_ms(long long sleep_duration)
{
    long long start_time = TC_get_current_time_ms(); 
    long long current_time = TC_get_current_time_ms();
    long long elapsed_time = 0;

    while(true)
    {   
        current_time = TC_get_current_time_ms();
        elapsed_time = current_time - start_time;
        if(elapsed_time >= sleep_duration)
        {
            break;
        }
    }
    return;
}