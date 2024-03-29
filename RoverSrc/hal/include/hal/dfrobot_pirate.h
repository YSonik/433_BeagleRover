#ifndef __DFROBOT_PIRATE_H
#define __DFROBOT_PIRATE_H

void DFRobotPirate_init(void);
void DFRobotPirate_cleanup(void);

void DFRobotPirate_turnLeft(void);
void DFRobotPirate_turnRight(void);
void DFRobotPirate_moveForward(void);
void DFRobotPirate_moveBackward(void);

int DFRobotPirate_getSpeed(void);
void DFRobotPirate_setSpeed(int speed);

void DFRobotPirate_stop(void);

#endif // __DFROBOT_PIRATE_H