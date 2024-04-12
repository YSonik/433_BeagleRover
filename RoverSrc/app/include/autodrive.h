#ifndef __AUTODRIVE_H
#define __AUTODRIVE_H

void AutoDrive_init();

void AutoDrive_start();
void AutoDrive_stop();

bool AutoDrive_isRunning();
void AutoDrive_cleanup();

#endif