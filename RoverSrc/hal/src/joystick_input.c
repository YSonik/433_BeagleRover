#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "hal/joystick_input.h"

//Array of joystick-direction GPIO directories.
char* js_gpio[] = {UP_DIR, DOWN_DIR, LEFT_DIR, RIGHT_DIR};

//Array of joystick-direction pins.
char* js_pin[] = {UP_PIN, DOWN_PIN, LEFT_PIN, RIGHT_PIN};

void init_js()
{
    //Configure the js pins for gpio.
    for(int i=0; i<4; i++)
    {
        configure_pin(js_pin[i]);
    }

    //Set the gpio direction to "in".
    for(int i=0; i<4; i++)
    {
        set_gpio_direction(js_gpio[i]);
    }
}


void configure_pin(char* pin_number)
{
    //Create the command string.
    char config_cmd[50];
    strcpy(config_cmd, "config-pin ");
    strcat(config_cmd, pin_number);
    strcat(config_cmd, " gpio");

    //Execute the command.
    FILE* pipe = popen(config_cmd, "r");

    char cmd_output[1024];
    while(!feof(pipe) && !ferror(pipe))
    {
        if( fgets(cmd_output, sizeof(cmd_output), pipe) == NULL )
        {
            break;
        }
    }

    int exit_code = WEXITSTATUS(pclose(pipe));
    if(exit_code != 0)
    {
        printf("Unable to execute command: %s\n",config_cmd);
        exit(1);
    }
}


void set_gpio_direction(char* gpio_directory)
{
    char direction_file_path[100];
    strcpy(direction_file_path, gpio_directory);
    strcat(direction_file_path, "/direction");

    FILE* direction_file = fopen(direction_file_path, "w");
    if(direction_file == NULL)
    {
        printf("Could not open file: %s\n",direction_file_path);
        exit(1);
    }

    int charWritten = fprintf(direction_file, "in");
    if(charWritten <= 0)
    {
        printf("Could not write to file: %s\n",direction_file_path);
        exit(1);
    }

    fclose(direction_file);
}


long long get_current_time_ms()
{
    struct timespec curr_time;
    clock_gettime(CLOCK_REALTIME, &curr_time);
    long long seconds = curr_time.tv_sec;
    long long nano_seconds = curr_time.tv_nsec;
    long long milli_seconds = (seconds * 1000) + (nano_seconds / 1000000);
    return milli_seconds;
}


int detect_movement(long duration_ms)
{
    //Create the value path strings.
    char up_value_path[100];
    strcpy(up_value_path, UP_DIR);
    strcat(up_value_path, "/value");

    char down_value_path[100];
    strcpy(down_value_path, DOWN_DIR);
    strcat(down_value_path, "/value");

    char left_value_path[100];
    strcpy(left_value_path, LEFT_DIR);
    strcat(left_value_path, "/value");

    char right_value_path[100];
    strcpy(right_value_path, RIGHT_DIR);
    strcat(right_value_path, "/value");

    char* value_paths[] = {up_value_path,
                           down_value_path,
                           left_value_path,
                           right_value_path};

    //Record start time.
    long long start_time = get_current_time_ms();
    long long curr_time = get_current_time_ms();

    //Continuously check the JS values for the specified duration.
    while(true)
    {
        //Check the current value for each direction.
        for(int i=0; i<4; i++)
        {
            FILE* value_file = fopen(value_paths[i], "r");
            if(value_file == NULL)
            {
                printf("Could not open file: %s\n",value_paths[i]);
                exit(1);
            }

            char value_read[2]; //value and null character
            fgets(value_read, 2, value_file);
            fclose(value_file);

            //Value=1 means not pressed.
            //Value=0 means pressed. 
            if(strcmp(value_read, "0") == 0)
            {
                return i;
            }
        }

        //If the specified duration has elapsed, exit the loop.
        curr_time = get_current_time_ms();
        long long elapsed_time = curr_time - start_time;

        if(elapsed_time >= duration_ms)
        {
            break;
        }
    }
    
    return -1;
}