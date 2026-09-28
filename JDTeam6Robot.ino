#include "Globals.h"

int leftY = 0;
int rightY = 0;
int l2Trigger = 0;
int r2Trigger = 0;
bool r1Pressed = false;
bool l1Pressed = false;
bool isConnected = false;
volatile bool estop = false;  // true = all motors cut (toggled by Options button)

const int STBY1 = 32;  // TB6612FNG (1) - wheel motors
const int STBY2 = 33;  // TB6612FNG (2) - gripper motor

// STBY LOW puts the TB6612FNG in standby: all outputs go high-impedance,
// so no current can flow through the motors.
void setDriversEnabled(bool enabled) {
  digitalWrite(STBY1, enabled ? HIGH : LOW);
  digitalWrite(STBY2, enabled ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);

  // 1. Immediately ensure the driver is ASLEEP on boot
  pinMode(STBY1, OUTPUT);
  pinMode(STBY2, OUTPUT);
  setDriversEnabled(false);

  // 2. Initialize all motor pins to 0 (This runs the code in your other tabs)
  setupLeftMotor();
  setupRightMotor();
  setupGripperMotor();
  setupMastServo();

  // 3. Now that the pins are safely at 0, WAKE UP the drivers
  setDriversEnabled(true);

  // 4. Initialize Bluetooth (INSERT MAC ADDRESS)
  PS4.begin("DC:0C:2D:56:5C:9F"); 
  Serial.println("Ready. Press PS button to connect...");

  // Create 4 independent, concurrent tasks
  xTaskCreatePinnedToCore(LeftMotorTask, "LeftTask", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(RightMotorTask, "RightTask", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(GripperMotorTask, "GripperTask", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(MastServoTask, "ServoTask", 2048, NULL, 1, NULL, 0);
}

void loop() {
  static bool lastOptions = false;

  isConnected = PS4.isConnected();
  
  if (isConnected) {
    leftY = PS4.LStickY();
    rightY = PS4.RStickY();
    l2Trigger = PS4.L2Value();
    r2Trigger = PS4.R2Value();
    r1Pressed = PS4.R1();
    l1Pressed = PS4.L1();

    // Options button: toggle emergency stop on each new press (edge-detected)
    bool optionsNow = PS4.Options();
    if (optionsNow && !lastOptions) {
      if (!estop) {
        estop = true;                // tasks see this and zero their outputs
        setDriversEnabled(false);    // and the drivers go to standby
        Serial.println("E-STOP: all motors cut. Press Options again to re-enable.");
      } else {
        setDriversEnabled(true);     // outputs are already zeroed, so this is safe
        estop = false;
        Serial.println("E-STOP released: motors enabled.");
      }
    }
    lastOptions = optionsNow;
  } else {
    leftY = 0; rightY = 0;
    l2Trigger = 0; r2Trigger = 0;
    r1Pressed = false; l1Pressed = false;
    lastOptions = false;
  }
  
  delay(10); 
}
