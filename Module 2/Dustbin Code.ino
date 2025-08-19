// ------------------ Ultrasonic Pins ------------------
const int trigPin = 11;
const int echoPin = 12;

// ------------------ Motor Control Pins ----------------
const int motorIn1 = 8;
const int motorIn2 = 9;
const int motorEnable = 10;

// ------------------ Variables ------------------------
long duration;
int distance;
bool lidOpen = false;
unsigned long lastOpenTime = 0;
const unsigned long openDuration = 2000; // Lid stays open 2 sec
const unsigned long motorRunTime = 700;  // Motor run time to fully open/close lid (tune this!)

void setup() {
  // Ultrasonic setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Motor setup
  pinMode(motorIn1, OUTPUT);
  pinMode(motorIn2, OUTPUT);
  pinMode(motorEnable, OUTPUT);

  Serial.begin(9600);

  // Motor off initially
  stopMotor();
}

void loop() {
  distance = getDistance();

  // If hand detected within 20 cm and lid not open
  if (distance > 0 && distance < 20 && !lidOpen) {
    Serial.println("Opening Lid...");
    openLid();
    lidOpen = true;
    lastOpenTime = millis();
  }

  // Close lid after timeout
  if (lidOpen && millis() - lastOpenTime > openDuration) {
    Serial.println("Closing Lid...");
    closeLid();
    lidOpen = false;
  }

  delay(100); // prevent excessive ultrasonic readings
}

// ------------------ Functions ------------------

// Open lid (CW rotation for fixed time)
void openLid() {
  digitalWrite(motorIn1, HIGH);
  digitalWrite(motorIn2, LOW);
  analogWrite(motorEnable, 200); // Speed control (0–255)
  delay(motorRunTime);           // Run motor long enough to open lid
  stopMotor();
}

// Close lid (CCW rotation for fixed time)
void closeLid() {
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, HIGH);
  analogWrite(motorEnable, 200);
  delay(motorRunTime);           // Run motor long enough to close lid
  stopMotor();
}

// Stop motor
void stopMotor() {
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, LOW);
  analogWrite(motorEnable, 0);
}

// Ultrasonic distance (in cm)
int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 30000);
  if (duration == 0) return -1;

  return duration * 0.034 / 2; 
}
