#ifndef joystick_input
#define joystick_input

//Joystick GPIO directories.
#define UP_DIR "/sys/class/gpio/gpio26"
#define DOWN_DIR "/sys/class/gpio/gpio46"
#define LEFT_DIR "/sys/class/gpio/gpio65"
#define RIGHT_DIR "/sys/class/gpio/gpio47"

#define UP_PIN "p8.14"
#define DOWN_PIN "p8.16"
#define LEFT_PIN "p8.18"
#define RIGHT_PIN "p8.15"

//Initialize the joystick for the game.
void init_js();

//Configure joystick pin for GPIO.
void configure_pin(char* pin_number);

//Set the direction of GPIO for a pin.
void set_gpio_direction(char* gpio_directory);

//Return the current time in milliseconds.
long long get_current_time_ms();

//Return the direction that the joystick was moved.
int detect_movement(long duration_ms);

#endif