#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <Arduino.h>

extern const int L_RPWM;
extern const int L_LPWM;
extern const int R_RPWM;
extern const int R_LPWM;

void pwmInit();
void pwmWrite(int i, int duty);
void setLeft(int v);
void setRight(int v);
void drive(int left, int right);
void stopAll();

#endif
