#ifndef _JOYSTICK_H_
#define _JOYSTICK_H_

typedef enum JoystickDirection
{
    JOYSTICK_UP,
    JOYSTICK_DOWN,
    JOYSTICK_LEFT,
    JOYSTICK_RIGHT,
    JOYSTICK_PRESSED,
    JOYSTICK_NONE
} direction;

typedef enum edge
{
    RISING,
    FALLING,
    BOTH
} edge;

void Joystick_init(void);
void Joystick_cleanup(void);

direction Joystick_waitForInput(int timeout_ms, edge e);
direction Joystick_read();
const char *Joystick_getDirectionString(direction dir);

#endif