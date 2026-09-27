#include "Globals.h"

int leftY = 0;
int rightY = 0;
int l2Trigger = 0;
int r2Trigger = 0;
bool r1Pressed = false;
bool l1Pressed = false;
bool isConnected = false;

const int STBY = 22; 

void setup() {
  Serial.begin(115200);

  // 1. Immediately ensure the driver is ASLEEP on boot
  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, LOW);

  // 2. Initialize all motor pins to 0 (This runs the code in your other tabs)
  setupLeftMotor();
  setupRightMotor();
  setupLiftMotor();
  setupMastServo();

  // 3. Now that the pins are safely at 0, WAKE UP the drivers
  digitalWrite(STBY, HIGH);

  // 4. Initialize Bluetooth (INSERT MAC ADDRESS)
  PS4.begin("4C:B9:9B:3A:43:B5"); 
  Serial.println("Ready. Press PS button to connect...");

  // Create 4 independent, concurrent tasks
  xTaskCreatePinnedToCore(LeftMotorTask, "LeftTask", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(RightMotorTask, "RightTask", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(LiftMotorTask, "LiftTask", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(MastServoTask, "ServoTask", 2048, NULL, 1, NULL, 0);
}

void loop() {
  isConnected = PS4.isConnected();
  
  if (isConnected) {
    leftY = PS4.LStickY();
    rightY = PS4.RStickY();
    l2Trigger = PS4.L2Value();
    r2Trigger = PS4.R2Value();
    r1Pressed = PS4.R1();
    l1Pressed = PS4.L1();
  } else {
    leftY = 0; rightY = 0;
    l2Trigger = 0; r2Trigger = 0;
    r1Pressed = false; l1Pressed = false;
  }
  
  delay(10); 
}