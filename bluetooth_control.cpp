#include "bluetooth_control.h"
#include "motor_control.h"

void applyMove(char c, int Speed) {
  switch (c) {
    case 'F': drive( Speed,  Speed); break;         // forward
    case 'B': drive(-Speed, -Speed); break;         // backward
    case 'L': drive(-Speed,  Speed); break;         // spin left
    case 'R': drive( Speed, -Speed); break;         // spin right
    case 'G': drive( Speed / 2,  Speed); break;     // forward left
    case 'H': drive( Speed,  Speed / 2); break;     // forward right
    case 'I': drive(-Speed / 2, -Speed); break;     // backward left
    case 'J': drive(-Speed, -Speed / 2); break;     // backward right
    case 'S': stopAll(); break;                     // stop
  }
}
