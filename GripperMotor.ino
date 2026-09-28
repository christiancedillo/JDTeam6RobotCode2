// TB6612FNG (2), channel A -> RackAPMotor
const int CIN1 = 22;      // AIN1 -> D22
const int CIN2 = 21;      // AIN2 -> D21
const int Gripper_PWM = 23;  // PWMA -> D23
const int TRIGGER_DEADZONE = 10;

void setupGripperMotor() {
  pinMode(CIN1, OUTPUT);
  pinMode(CIN2, OUTPUT);
  pinMode(Gripper_PWM, OUTPUT);
  digitalWrite(CIN1, LOW);
  digitalWrite(CIN2, LOW);
  analogWrite(Gripper_PWM, 0);
}

void GripperMotorTask(void *pvParameters) {
  for (;;) {
    if (!isConnected) {
      analogWrite(Gripper_PWM, 0);
      digitalWrite(CIN1, LOW);
      digitalWrite(CIN2, LOW);
    } else {
      if ((l2Trigger > TRIGGER_DEADZONE && r2Trigger > TRIGGER_DEADZONE) ||
          (l2Trigger < TRIGGER_DEADZONE && r2Trigger < TRIGGER_DEADZONE)) {
        analogWrite(Gripper_PWM, 0);
        digitalWrite(CIN1, LOW);
        digitalWrite(CIN2, LOW);
      } else if (r2Trigger >= TRIGGER_DEADZONE) {
        digitalWrite(CIN1, HIGH);
        digitalWrite(CIN2, LOW);
        analogWrite(Gripper_PWM, r2Trigger);
      } else if (l2Trigger >= TRIGGER_DEADZONE) {
        digitalWrite(CIN1, LOW);
        digitalWrite(CIN2, HIGH);
        analogWrite(Gripper_PWM, l2Trigger);
      }
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
