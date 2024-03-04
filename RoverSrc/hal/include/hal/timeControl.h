#ifndef timeControl
#define timeControl

//(1) Return the current time in milliseconds.
long long TC_get_current_time_ms(void);

//(2) Sleep the thread for the specified number of milliseconds.
void TC_sleep_thread_ms(long long sleep_duration);

#endif