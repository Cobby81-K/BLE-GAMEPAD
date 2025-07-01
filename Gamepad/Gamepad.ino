//#include <esp32_gamepad.h>

#include <BleGamepad.h>/home/cobby/Downloads/ESP32_Gamepad_Project/ESP32_Gamepad.ino

// Name, manufacturer, battery level (100%)
BleGamepad bleGamepad("ESP32 Gamepad", "OpenAI", 100);

// Button pin assignments (PlayStation style)
const int buttonPins[] = {
  2,   // Cross (✖️)
  4,   // Circle (🟥)
  5,   // Square (⬜)
  18,  // Triangle (🔺)
  19,  // L1
  21,  // R1
  22,  // L2
  23,  // R2
  13,  // Start
  12,  // Select
  27,  // Home
  14,  // L3 (press left stick)
  26   // R3 (press right stick)
};

const int numButtons = sizeof(buttonPins) / sizeof(buttonPins[0]);

// D-pad buttons (hat switch)
const int dpadUp     = 32;
const int dpadDown   = 33;
const int dpadLeft   = 34;
const int dpadRight  = 35;

// Analog joystick pins
const int leftJoyX   = 36;
const int leftJoyY   = 39;
const int rightJoyX  = 25;
const int rightJoyY  = 15;

void setup() {
  Serial.begin(115200);

  // Setup buttons
  for (int i = 0; i < numButtons; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  // Setup D-pad buttons
  pinMode(dpadUp, INPUT_PULLUP);
  pinMode(dpadDown, INPUT_PULLUP);
  pinMode(dpadLeft, INPUT_PULLUP);
  pinMode(dpadRight, INPUT_PULLUP);

  bleGamepad.begin();
}

void loop() {
  if (bleGamepad.isConnected()) {

    // Read and update buttons (active LOW)
    for (int i = 0; i < numButtons; i++) {
      bool pressed = !digitalRead(buttonPins[i]);
      bleGamepad.setButton(i + 1, pressed);  // Button 1 to 13
    }

    // D-pad Hat switch logic
    int hat = DPAD_CENTERED;

    if (!digitalRead(dpadUp)) {
      if (!digitalRead(dpadRight)) hat = DPAD_UP_RIGHT;
      else if (!digitalRead(dpadLeft)) hat = DPAD_UP_LEFT;
      else hat = DPAD_UP;
    } else if (!digitalRead(dpadDown)) {
      if (!digitalRead(dpadRight)) hat = DPAD_DOWN_RIGHT;
      else if (!digitalRead(dpadLeft)) hat = DPAD_DOWN_LEFT;
      else hat = DPAD_DOWN;
    } else if (!digitalRead(dpadLeft)) {
      hat = DPAD_LEFT;
    } else if (!digitalRead(dpadRight)) {
      hat = DPAD_RIGHT;
    }

    bleGamepad.setHat(hat);

    // Joystick analog readings
    int lx = map(analogRead(leftJoyX), 0, 4095, -32767, 32767);
    int ly = map(analogRead(leftJoyY), 0, 4095, -32767, 32767);
    int rx = map(analogRead(rightJoyX), 0, 4095, -32767, 32767);
    int ry = map(analogRead(rightJoyY), 0, 4095, -32767, 32767);

    bleGamepad.setLeftThumb(lx, ly);
    bleGamepad.setRightThumb(rx, ry);

    bleGamepad.sendReport();
  }

  delay(10);
}
