Servo mastServo;
const int SERVO_PIN = 23;
int servoAngle = 90;

void setupMastServo() {
  ESP32PWM::allocateTimer(0);
  mastServo.setPeriodHertz(50);
  mastServo.attach(SERVO_PIN, 500, 2400);
  mastServo.write(servoAngle);
}

void MastServoTask(void *pvParameters) {
  for (;;) {
    if (isConnected) {
      if (r1Pressed) {
        servoAngle += 2;
        if (servoAngle > 180) servoAngle = 180;
        mastServo.write(servoAngle);
      }
      if (l1Pressed) {
        servoAngle -= 2;
        if (servoAngle < 0) servoAngle = 0;
        mastServo.write(servoAngle);
      }
    }
    vTaskDelay(15 / portTICK_PERIOD_MS); 
  }
}