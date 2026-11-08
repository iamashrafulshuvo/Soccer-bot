#include "motor_control.h"

const int L_RPWM = 21;   // forward
const int L_LPWM = 19;   // backward

const int R_RPWM = 23;   // forward
const int R_LPWM = 22;   // backward

const int PWM_FREQ = 20000;   // 20 kHz
const int PWM_RES  = 8;       // 0-255

const int pwmPins[4] = { L_RPWM, L_LPWM, R_RPWM, R_LPWM };

#if ESP_ARDUINO_VERSION_MAJOR >= 3
  void pwmInit() {
    for (int i = 0; i < 4; i++) ledcAttach(pwmPins[i], PWM_FREQ, PWM_RES);
  }
  void pwmWrite(int i, int duty) { ledcWrite(pwmPins[i], duty); }
#else
  void pwmInit() {
    for (int i = 0; i < 4; i++) {
      ledcSetup(i, PWM_FREQ, PWM_RES);
      ledcAttachPin(pwmPins[i], i);
    }
  }
  void pwmWrite(int i, int duty) { ledcWrite(i, duty); }
#endif

void setLeft(int v) {
  v = constrain(v, -255, 255);
  if (v >= 0) { pwmWrite(0, v);  pwmWrite(1, 0);  }
  else        { pwmWrite(0, 0);  pwmWrite(1, -v); }
}

void setRight(int v) {
  v = constrain(v, -255, 255);
  if (v >= 0) { pwmWrite(2, v);  pwmWrite(3, 0);  }
  else        { pwmWrite(2, 0);  pwmWrite(3, -v); }
}

void drive(int left, int right) {
  setLeft(left);
  setRight(right);
}

void stopAll() { drive(0, 0); }
