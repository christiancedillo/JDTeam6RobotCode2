const int BIN1 = 5;
const int BIN2 = 18;
// Reusing the same deadzone value logic, defining locally for this tab
const int R_DEADZONE = 15;

void setupRightMotor() {
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  analogWrite(BIN1, 0); 
  analogWrite(BIN2, 0);
}

void RightMotorTask(void *pvParameters) {
  for (;;) { 
    if (!isConnected || abs(rightY) < R_DEADZONE) {
      analogWrite(BIN1, 0); 
      analogWrite(BIN2, 0);
    } else {
      int speed = constrain(map(abs(rightY), R_DEADZONE, 127, 0, 255), 0, 255);
      if (rightY > 0) { 
        analogWrite(BIN1, speed); 
        analogWrite(BIN2, 0); 
      } else { 
        analogWrite(BIN1, 0); 
        analogWrite(BIN2, speed); 
      }
    }
    vTaskDelay(10 / portTICK_PERIOD_MS); 
  }
}