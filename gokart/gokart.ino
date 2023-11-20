const int acceleratorPin = A0;
const int motorSpeedPin = 9;
const int motorDirectionPin = 8;
const int errorPin = A1;

bool toggle = false;
bool error = false;

void setup() {
  pinMode(errorPin, INPUT);
  pinMode(acceleratorPin, INPUT);
  pinMode(motorDirectionPin, OUTPUT);
  pinMode(motorSpeedPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int s = map(analogRead(acceleratorPin), 180, 874, 0, 255);
  if (s < 0) {
    s = 0;
    Serial.print("ERROR");
    Serial.print(" ");
  } else if (s > 256) {
    s = 0;
    Serial.print("ERROR");
    Serial.print(" ");
  }

  // if (s > 22 && s < 35) {
  //   s = 0;
  //   Serial.print("ERROR");
  //   Serial.print(" ");
  // }

  if (s == 255) {
    toggle = !toggle; // toggle을 반전시킴
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
