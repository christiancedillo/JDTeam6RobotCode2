Servo mastServo;
const int SERVO_PIN = 18;

// 5-turn dual mode servo (1800°): 500 µs = 0°, 2500 µs = 1800° (in servo mode)
const int SERVO_MIN_US = 500;
const int SERVO_MAX_US = 2500;
const int SERVO_MAX_DEG = 1800;

// Degrees moved per 15 ms tick while a button is held (10 = ~667°/s)
const int SERVO_STEP_DEG = 10;

int servoAngle = 900;  // mid-range (2.5 turns)

void writeMastServo(int angleDeg) {
  angleDeg = constrain(angleDeg, 0, SERVO_MAX_DEG);
  int pulseUs = map(angleDeg, 0, SERVO_MAX_DEG, SERVO_MIN_US, SERVO_MAX_US);
  mastServo.writeMicroseconds(pulseUs);
}

void setupMastServo() {
  ESP32PWM::allocateTimer(0);
  mastServo.setPeriodHertz(50);
  mastServo.attach(SERVO_PIN, SERVO_MIN_US, SERVO_MAX_US);
  writeMastServo(servoAngle);
}

void MastServoTask(void *pvParameters) {
  for (;;) {
    if (isConnected) {
      bool changed = false;

      if (r1Pressed) {
        servoAngle += SERVO_STEP_DEG;
        if (servoAngle > SERVO_MAX_DEG) servoAngle = SERVO_MAX_DEG;
        changed = true;
      }
      if (l1Pressed) {
        servoAngle -= SERVO_STEP_DEG;
        if (servoAngle < 0) servoAngle = 0;
        changed = true;
      }

      if (changed) writeMastServo(servoAngle);
    }
    vTaskDelay(15 / portTICK_PERIOD_MS);
  }
}
