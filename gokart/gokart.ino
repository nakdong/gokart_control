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
  // 아날로그 값에 이동 평균 필터 적용
  total -= readings[readIndex];
  readings[readIndex] = analogRead(acceleratorPin);
  total += readings[readIndex];
  readIndex = (readIndex + 1) % numReadings;
  average = total / numReadings;

  // 아날로그 값 범위 제한
  int s = constrain(map(average, 180, 874, 0, 255), 0, 255);

  // 급격한 변화 감지
  if (s < 5) {
    s = 0; // 급격한 변화가 감지되면 속도를 0으로 설정
    error = true;
  } else {
    error = false;
  }

  // 플러그 뽑힘 감지
  if (digitalRead(errorPin) == HIGH) {
    error = true;
  }

  // 에러 상태에 따른 동작 결정
  if (error) {
    // 에러 상태에서는 정지
    digitalWrite(motorDirectionPin, HIGH);
    analogWrite(motorSpeedPin, 0);
  } else {
    // 정상 상태에서 토글 및 모터 제어
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
      analogWrite(motorSpeedPin, s);
      Serial.println(s);
    }
  }
}
