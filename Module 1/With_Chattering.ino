#include <Servo.h>

const int trigPin = 9;
const int echoPin = 10;
const int servoPin = 6;

Servo lidServo;

long duration;
int distance;
bool lidOpen = false;
unsigned long lastOpenTime = 0;
const unsigned long openDuration = 1000; // Time to keep lid open 

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  lidServo.attach(servoPin);
  lidServo.write(0); // Initial position (closed)
  Serial.begin(9600);
}

void loop() {
  distance = getDistance();

  // If hand is detected within 20 cm
  if (distance > 0 && distance < 20 && !lidOpen) {
    lidServo.write(150); // Open lid
    lidOpen = true;
    lastOpenTime = millis();
    Serial.println("Lid Opened");
  }

  // Close lid after 3 seconds
  if (lidOpen && millis() - lastOpenTime > openDuration) {
    lidServo.write(0); // Close lid
    lidOpen = false;
    Serial.println("Lid Closed");
  }

  delay(100); // Small delay to prevent excessive sensor readings
}

int getDistance() {
  // Send ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read echo time
  duration = pulseIn(echoPin, HIGH, 30000); // 30ms timeout
  if (duration == 0) return -1; // Timeout, no object detected

  // Calculate distance in cm
  return duration * 0.034 / 2;
}
