#include <Servo.h>

// Pin connections
const int trigPin = 9;      // HC-SR04 TRIG
const int echoPin = 10;     // HC-SR04 ECHO
const int servoPin = 3; 
const int buzzerPin = 4;    // SG90 Servo signal (PWM pin)

Servo scanServo;

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  scanServo.attach(servoPin, 500, 2400);  // SG90 safe range
}

void loop() {
  // Sweep from left to right
  for (int angle = 15; angle <= 165; angle += 2) {
    scanServo.write(angle);
    delay(30); // allow servo to settle
    int dist = getDistance();
    Serial.print(angle);
    Serial.print(",");
    Serial.println(dist);
    alertWithBuzzer(dist);
  }

  // Sweep from right to left
  for (int angle = 165; angle >= 15; angle -= 2) {
    scanServo.write(angle);
    delay(30);
    int dist = getDistance();
    Serial.print(angle);
    Serial.print(",");
    Serial.println(dist);
    alertWithBuzzer(dist);
  }
}

// Function to read distance from HC-SR04
int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 20000L); // timeout 20ms
  if (duration == 0) return 200; // no echo

  int distance = duration * 0.034 / 2; // convert to cm
  if (distance < 2) distance = 2;      // HC-SR04 minimum
  if (distance > 200) distance = 200;  // cap for radar display
  return distance;
}

void alertWithBuzzer(int distance) {
  if (distance < 100) {
    tone(buzzerPin, 100);
  }
  else if (distance < 70){
    tone(buzzerPin, 700, 100);
    delay(100);
  }
  else if (distance < 30) {
    tone(buzzerPin, 500, 50);
      delay(50);
    }
    else{
      noTone(buzzerPin);
    }
  }

