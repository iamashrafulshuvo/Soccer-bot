#include "BluetoothSerial.h"
#include "motor_control.h"
#include "bluetooth_control.h"

BluetoothSerial SerialBT;

int Speed = 180;          // default speed
char lastCmd = 'S';       // current movement command
bool wasConnected = false;

void setup() {
  Serial.begin(115200);

  pwmInit();
  stopAll();

  SerialBT.begin("SoccerBot");
  Serial.println("SoccerBot ready");
}

void loop() {
  // Bluetooth disconnect hole auto stop
  bool connected = SerialBT.hasClient();
  if (wasConnected && !connected) { lastCmd = 'S'; stopAll(); }
  wasConnected = connected;

  while (SerialBT.available() > 0) {
    char c = SerialBT.read();
    Serial.println(c);

    switch (c) {
      // speed (app slider 0-9)
      case '0': Speed = 0;   applyMove(lastCmd, Speed); break;
      case '1': Speed = 100; applyMove(lastCmd, Speed); break;
      case '2': Speed = 120; applyMove(lastCmd, Speed); break;
      case '3': Speed = 140; applyMove(lastCmd, Speed); break;
      case '4': Speed = 160; applyMove(lastCmd, Speed); break;
      case '5': Speed = 180; applyMove(lastCmd, Speed); break;
      case '6': Speed = 200; applyMove(lastCmd, Speed); break;
      case '7': Speed = 220; applyMove(lastCmd, Speed); break;
      case '8': Speed = 240; applyMove(lastCmd, Speed); break;
      case '9':
      case 'q': Speed = 255; applyMove(lastCmd, Speed); break;

      // movement
      case 'F': case 'B': case 'L': case 'R':
      case 'G': case 'H': case 'I': case 'J':
      case 'S':
        lastCmd = c;
        applyMove(c, Speed);
        break;

      default: break;   // horn (Y), light (X/x)
    }
  }
}
