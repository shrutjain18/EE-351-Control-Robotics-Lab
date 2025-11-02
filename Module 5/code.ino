const int trigPin = 9;
const int echoPin = 10;
// defines variables
long duration;
int distance;
// Include the Servo library 
#include <Servo.h> 
// Declare the Servo pin 
int servoPin = 3; 
// Create a servo object 
Servo servo; 
unsigned long lastEncoderTime = 0;
unsigned long currentTime = 0;
float Kp = 5; 
int error = 0;
int ref = 20;
int ang = 0 ;
void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  servo.attach(servoPin);
  Serial.begin(9600); // Starts the serial communication
  servo.write(90);
}
void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  error = distance - ref;
  ang = 90 + Kp*error;
  servo.write(ang);
}
