const int AIN1 = 2;
const int AIN2 = 4;
const int DEADZONE = 15;

void setupLeftMotor() {
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  analogWrite(AIN1, 0); 
  analogWrite(AIN2, 0);
}

void LeftMotorTask(void *pvParameters) {
  for (;;) { 
    if (!isConnected || abs(leftY) < DEADZONE) {
      analogWrite(AIN1, 0); 
      analogWrite(AIN2, 0);
    } else {
      int speed = constrain(map(abs(leftY), DEADZONE, 127, 0, 255), 0, 255);
      if (leftY > 0) { 
        analogWrite(AIN1, speed); 
        analogWrite(AIN2, 0); 
      } else { 
        analogWrite(AIN1, 0); 
        analogWrite(AIN2, speed); 
      }
    }
    vTaskDelay(10 / portTICK_PERIOD_MS); 
  }
}