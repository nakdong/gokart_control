const int acceleratorPin = A0;
const int motorSpeedPin = 9;
const int motorDirectionPin = 8;

bool toggle = false;

void setup() {
  pinMode(acceleratorPin, INPUT);
  pinMode(motorDirectionPin, OUTPUT);
  pinMode(motorSpeedPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int s = map(analogRead(acceleratorPin), 182, 877, 0, 255);
  Serial.println(s);

  if (s == 0) {
    toggle = !toggle; // toggle을 반전시킴
  }

  if (toggle == false) {
    digitalWrite(motorDirectionPin, HIGH);
    analogWrite(motorSpeedPin, s);
  } else {
    digitalWrite(motorDirectionPin, LOW);
    analogWrite(motorSpeedPin, abs(s));
  }
}
