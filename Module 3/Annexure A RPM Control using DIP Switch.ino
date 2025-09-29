// Constants for motor control
const int motorPin1 = 10;  // PWM pin for motor control
const int motorPin2 = 9;  // PWM pin for motor control
const int encoderPinA = 2;  // Encoder A pin
const int encoderPinB = 3;  // Encoder B pin

// Constants for switches
const int switchPin1 = 4;  // Switch 1 pin
const int switchPin2 = 5;  // Switch 2 pin

// Variables for encoder and RPM
volatile int encoderTicks = 0;
int RPM = 0;
unsigned long lastEncoderTime = 0;

// Variables for switch states
int switchState1 = 0;
int switchState2 = 0;

// Variables for motor speed
int motorSpeed = 0;
int desiredRPM = 0;

void setup() {
  // Setup motor control
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  
  // Setup encoder
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);
  
  // Setup switches
  pinMode(switchPin1, INPUT_PULLUP);
  pinMode(switchPin2, INPUT_PULLUP);
  
  // Attach interrupt for encoder feedback
  attachInterrupt(digitalPinToInterrupt(encoderPinA), encoderInterrupt, RISING);
  
  // Initialize serial for debugging
  Serial.begin(9600);
}

void loop() {
  // Read switch states
  switchState1 = digitalRead(switchPin1);
  switchState2 = digitalRead(switchPin2);
  
  // Determine the state based on switch combinations
  int state = (switchState1 << 1) | switchState2;  // Create a 2-bit state
  
  // Set motor speed based on state
  switch(state) {
    case 0:  // Both switches OFF
      motorSpeed = 230;  // Low speed
      desiredRPM = 49;//55*(230/255);
      break;
    case 1:  // Switch 1 OFF, Switch 2 ON
      motorSpeed = 205;  // Medium speed
      desiredRPM = 44;//55*(205/255);
      break;
    case 2:  // Switch 1 ON, Switch 2 OFF
      motorSpeed = 180;  // Higher speed
      desiredRPM = 39;//55*(180/255);
      break;
    case 3:  // Both switches ON
      motorSpeed = 154;  // Maximum speed
      desiredRPM = 33;//55*(154/255);
      break;
    default:
      motorSpeed = 0;  // Default to zero if something goes wrong
      desiredRPM = 0;
      break;
  }
      analogWrite(motorPin1, motorSpeed);
      analogWrite(motorPin2, 0);
      delay(1000);
  
  // Calculate RPM from encoder feedback
  unsigned long currentTime = millis();
  if (currentTime - lastEncoderTime >= 1000) {
    RPM = encoderTicks * 60 / 312;  // Convert encoder ticks to RPM (assuming 360 ticks per revolution)
    encoderTicks = 0;  // Reset the tick count
    lastEncoderTime = currentTime;
    
    // Print the RPM value for debugging
    Serial.print("Desired RPM: ");
    Serial.print(desiredRPM);
    Serial.print(" Curent RPM: ");
    Serial.println(RPM);
  }
}

void encoderInterrupt() {
  // Increment the encoder ticks on each rising edge of the encoder signal
  encoderTicks++;
}
