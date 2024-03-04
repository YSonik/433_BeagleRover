#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "hal/JS_input.h"

//Joystick GPIO directories.
#define UP_DIR "/sys/class/gpio/gpio26"
#define DOWN_DIR "/sys/class/gpio/gpio46"
#define LEFT_DIR "/sys/class/gpio/gpio65"
#define RIGHT_DIR "/sys/class/gpio/gpio47"

//Joystick GPIO pins.
#define UP_PIN "p8.14"
#define DOWN_PIN "p8.16"
#define LEFT_PIN "p8.18"
#define RIGHT_PIN "p8.15"

//Config-pin command
#define NUM_PINS 4
#define CMD_SIZE 51
#define PATH_SIZE 101
#define VALUE_SIZE 2
#define CONFIG_CMD "config-pin"
static char CMD_string[CMD_SIZE];
static char FILE_path[PATH_SIZE];
static char READ_value[VALUE_SIZE];

//Array of joystick-direction GPIO directories.
static char* JS_gpio[NUM_PINS] = {UP_DIR, DOWN_DIR, LEFT_DIR, RIGHT_DIR};

//Array of joystick-direction pins.
static char* JS_pins[NUM_PINS] = {UP_PIN, DOWN_PIN, LEFT_PIN, RIGHT_PIN};

//JS reader thread
static pthread_t readerThreadID;
static bool stopThread = false;


//Helper Functions
static void JS_joinReaderThread(void)
{
    if(stopThread)
    {
        //Block the mainThread to Join the readerThread.
        pthread_join(readerThreadID, NULL);
    }
    return;
}

static void JS_executeCMD(void)
{
    //Execute the command.
    FILE* pipe = popen(CMD_string, "r");

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

    return;
}

static void JS_writeFile(char* new_value)
{
    FILE* file_ptr = fopen(FILE_path, "w");
    if(file_ptr == NULL)
    {
        printf("Could not open file: %s\n",FILE_path);
        exit(1);
    }

    int charWritten = fprintf(file_ptr, new_value);
    if(charWritten <= 0)
    {
        printf("Could not write to file: %s\n",FILE_path);
        exit(1);
    }

    fclose(file_ptr);
    return;
}

static void JS_readFile(void)
{
    FILE* file_ptr = fopen(FILE_path, "r");
    if(file_ptr == NULL)
    {
        printf("Could not open file: %s\n",FILE_path);
        exit(1);
    }

    fgets(READ_value, VALUE_SIZE, file_ptr);
    fclose(file_ptr);

    return;
}

static void* JS_readerThreadExecute(void* arg)
{
    //Configure the JS pins for gpio.
    for(int i=0; i<NUM_PINS; i++)
    {
        //Create the command string.
        memset(&CMD_string, "\0", CMD_SIZE*sizeof(char));
        snprintf(CMD_string, CMD_SIZE-1, "%s %s gpio",CONFIG_CMD, JS_pins[i]);
        JS_executeCMD();
    }

    //Set the gpio direction to "in".
    for(int i=0; i<NUM_PINS; i++)
    {
        memset(&FILE_path, "\0", PATH_SIZE*sizeof(char));
        snprintf(FILE_path, PATH_SIZE-1, "%s/direction",JS_gpio[i]);
        JS_writeFile("in");
    }

    //Create the value path strings.
    char up_value_path[PATH_SIZE];
    snprintf(up_value_path, PATH_SIZE-1, "%s/value",UP_DIR);
    
    char down_value_path[PATH_SIZE];
    snprintf(down_value_path, PATH_SIZE-1, "%s/value",DOWN_DIR);

    char left_value_path[PATH_SIZE];
    snprintf(left_value_path, PATH_SIZE-1, "%s/value",LEFT_DIR);

    char right_value_path[PATH_SIZE];
    snprintf(right_value_path, PATH_SIZE-1, "%s/value",RIGHT_DIR);

    char* value_paths[] = {up_value_path,
                           down_value_path,
                           left_value_path,
                           right_value_path};


    //Continously monitor the JS pins for a movement.
    while(true)
    {
        if(stopThread)
        {
            break;
        }
        
        //Check the current value for each direction.
        for(int i=0; i<NUM_PINS; i++)
        {
            memset(&FILE_path, "\0", PATH_SIZE*sizeof(char));
            snprintf(FILE_path, PATH_SIZE-1, value_paths[i]);
            JS_readFile();

            //Value=1 means not pressed.
            //Value=0 means pressed. 
            if(strcmp(READ_value, "0") == 0)
            {
                printf("JS pressed %s\n",i);
                //Send a UDP packet to the Rover.
            }
        }
    }
    
    return arg;
}

//Interface functions
void JS_start(void)
{
    //Create the JS reader thread
    pthread_create(&readerThreadID, NULL, JS_readerThreadExecute, NULL);
    JS_joinReaderThread();
    return;
}


void JS_stop(void)
{
    stopThread = true;
    JS_joinReaderThread();
    return; 
}