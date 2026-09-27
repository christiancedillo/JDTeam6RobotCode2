const int CIN1 = 19;
const int CIN2 = 21;
const int TRIGGER_DEADZONE = 10;

void setupLiftMotor() {
  pinMode(CIN1, OUTPUT);
  pinMode(CIN2, OUTPUT);
  analogWrite(CIN1, 0); 
  analogWrite(CIN2, 0);
}

void LiftMotorTask(void *pvParameters) {
  for (;;) {
    if (!isConnected) {
      analogWrite(CIN1, 0); 
      analogWrite(CIN2, 0);
    } else {
      if ((l2Trigger > TRIGGER_DEADZONE && r2Trigger > TRIGGER_DEADZONE) || 
          (l2Trigger < TRIGGER_DEADZONE && r2Trigger < TRIGGER_DEADZONE)) {
        analogWrite(CIN1, 0); 
        analogWrite(CIN2, 0);
      } else if (r2Trigger >= TRIGGER_DEADZONE) {
        analogWrite(CIN1, r2Trigger); 
        analogWrite(CIN2, 0);
      } else if (l2Trigger >= TRIGGER_DEADZONE) {
        analogWrite(CIN1, 0); 
        analogWrite(CIN2, l2Trigger);
      }
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}