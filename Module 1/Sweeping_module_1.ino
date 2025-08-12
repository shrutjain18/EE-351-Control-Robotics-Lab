#include <Servo.h>

const int trigPin = 9;
const int echoPin = 10;
const int servoPin = 6;

Servo lidServo;

long duration;
int distance;
bool lidOpen = false;
unsigned long lastOpenTime = 0;
const unsigned long openDuration = 1000; // Time to keep lid open (3 seconds)
const int maxRange = 50; // cm

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  lidServo.attach(servoPin);
  lidServo.write(0); // Initial position (closed)
  Serial.begin(9600);
}

void loop() {
  distance = getDistance();

  if (distance > 0 && distance <= maxRange) {
    // Map distance to servo angle using (30 - distance) * 15
    int angle = (maxRange - distance) * 3;

    // Constrain angle between 0° and 180°
    angle = constrain(angle, 0, 180);

    lidServo.write(angle);
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.print(" cm  ->  Servo Angle: ");
    Serial.println(angle);

    lidOpen = true;
    lastOpenTime = millis();
  }
  
  // Close lid after 3 seconds if no object in range
  if (lidOpen && (distance > maxRange || distance < 0) && millis() - lastOpenTime > openDuration) {
    lidServo.write(0);
    lidOpen = false;
    Serial.println("Lid Closed");
  }

  delay(100); // Prevent excessive sensor polling
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
