const int acceleratorPin = A0;
const int motorSpeedPin = 9;
const int motorDirectionPin = 8;
const int errorPin = A0;

bool toggle = false;
bool error = false;
unsigned long lastToggleTime = 0;
unsigned long toggleInterval = 5000;

const int numReadings = 10;
int readings[numReadings];
int readIndex = 0;
int total = 0;
int average = 0;

void setup() {
  pinMode(errorPin, INPUT);
  pinMode(acceleratorPin, INPUT);
  pinMode(motorDirectionPin, OUTPUT);
  pinMode(motorSpeedPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  total -= readings[readIndex];
  readings[readIndex] = analogRead(acceleratorPin);
  total += readings[readIndex];
  readIndex = (readIndex + 1) % numReadings;
  average = total / numReadings;

  int s = map(average, 180, 874, 0, 255);

  // 최솟값 및 최댓값 제한
  s = constrain(s, 0, 255);

  // 플러그가 뽑혔을 때를 감지
  if (s < 5) {
    s = 0; // 급격한 변화가 감지되면 속도를 0으로 설정
    error = true;
  } else {
    error = false;
  }

  // 플러그 뽑힘에 대한 긴급 정지
  if (digitalRead(errorPin) == HIGH) {
    error = true;
  }

  if (error) {
    // 에러 상태에서는 정지
    digitalWrite(motorDirectionPin, HIGH);
    analogWrite(motorSpeedPin, 0);
  } else {
    if (s == 255 && millis() - lastToggleTime >= toggleInterval) {
      toggle = !toggle;
      lastToggleTime = millis();
    }
    Serial.print(toggle);
    Serial.print(" ");

    if (toggle == false) {
      digitalWrite(motorDirectionPin, HIGH);
      analogWrite(motorSpeedPin, s);
      Serial.println(s);
    } else {
      digitalWrite(motorDirectionPin, LOW);
      analogWrite(motorSpeedPin, abs(s));
      Serial.println(abs(s));
    }
  }
}
