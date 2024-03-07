#ifndef __DFROBOT_PIRATE_H
#define __DFROBOT_PIRATE_H

void DFRobotPirate_init(void);
void DFRobotPirate_cleanUp(void);

void DFRobotPirate_TurnLeft(void);
void DFRobotPirate_TurnRight(void);
void DFRobotPirate_MoveForward(void);
void DFRobotPirate_MoveBackward(void);

void DFRobotPirate_SetSpeed(int speed);

int DFRobotPirate_Stop(void);

#endif // __DFROBOT_PIRATE_H