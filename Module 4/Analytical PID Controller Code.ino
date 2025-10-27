#include <Arduino.h>

// Encoder pins
const int encA = 2;  // Interrupt
const int encB = 3;

// Motor driver pins (L293D)
const int in1 = 7;
const int in2 = 8;
const int enablePin = 9;  // PWM

// Potentiometer
const int potPin = A0;

// Encoder setup
const int pulsesPerRev = 312;
const float lidRangeDeg = 180.0;
const float degPerPulse = lidRangeDeg / pulsesPerRev;

volatile long encoderCount = 0;
long zeroCount = 0;

// PID constants (tune these)
const float Kp = 10;
const float Ki = 0.1;
const float Kd = 0.5;

// PID variables
float integral = 0;
float previousError = 0;
unsigned long lastTime = 0;

// Store last potentiometer value
int lastPotVal = -1;  // initialized to a value that won't match the first potentiometer read

void readEncoder() {
  int b = digitalRead(encB);
  if (b == HIGH)
    encoderCount++;
  else
    encoderCount--;
}

float getAngle() {
  noInterrupts();
  long count = encoderCount - zeroCount;
  interrupts();
  return count * degPerPulse;
}

void motorStop() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, HIGH);  // Brake mode
  analogWrite(enablePin, 0);
}

void motorForward(int pwm) {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enablePin, pwm);
}

void motorBackward(int pwm) {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enablePin, pwm);
}

void setup() {
  pinMode(encA, INPUT_PULLUP);
  pinMode(encB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encA), readEncoder, CHANGE);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enablePin, OUTPUT);
  motorStop();

  // Set current position as zero
  zeroCount = encoderCount;

  Serial.begin(9600);
  Serial.println("Motor controlled by potentiometer with PID");
  lastTime = millis();
}

void loop() {
  // Read potentiometer
  int potVal = analogRead(potPin);

  // Only proceed if potentiometer has changed significantly
  if (abs(potVal - lastPotVal) > 5) {  // Threshold to avoid tiny changes
    lastPotVal = potVal;  // Update last potentiometer value

    // Map potentiometer value (0-1023) to 0-180 degrees
    float targetAngle = map(potVal, 0, 1023, 0, 180);

    // Get the current angle from the encoder
    float actualAngle = getAngle();
    float error = targetAngle - actualAngle;

    // PID calculations
    unsigned long now = millis();
    float dt = (now - lastTime) / 1000.0;  // seconds
    lastTime = now;
    if (dt <= 0) dt = 0.001;  // avoid division by zero

    integral += error * dt;
    float derivative = (error - previousError) / dt;
    float output = Kp * error + Ki * integral + Kd * derivative;
    previousError = error;

    int pwm = constrain(abs((int)output), 0, 255);  // PWM must be positive

    // Control motor based on error direction
    if (error > 2) {
      motorForward(pwm);  // CW
    } else if (error < -2) {
      motorBackward(pwm); // CCW
    } else {
      motorStop();  // Stop if error is too small
      integral = 0;  // Prevent wind-up
    }

    // Print for serial plotter (for monitoring)
    Serial.print(targetAngle); Serial.print('\t');
    Serial.print(actualAngle); Serial.print('\t');
    Serial.print(error); Serial.print('\t');
    Serial.println(pwm);
  } else {
    motorStop();  // If potentiometer hasn't moved enough, stop motor
  }

  delay(50);  // Slight delay to prevent overloading the loop
}
