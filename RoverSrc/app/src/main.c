#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "hal/JS_input.h"
#include "hal/timeControl.h"



int main()
{
    printf("Rover project compiles correctly\n");
    
    JS_start();

    //Sleep for 30s
    TC_sleep_thread_ms(30000);

    JS_stop();

    return 0;
}