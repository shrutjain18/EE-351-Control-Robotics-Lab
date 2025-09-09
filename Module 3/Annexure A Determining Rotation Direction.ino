// ---------------- Encoder Pins ----------------
const int encoderChA = 2;  // Channel A connected to D2 (interrupt)
const int encoderChB = 3;  // Channel B connected to D3

volatile long encoderCount = 0;  
volatile int direction = 0;  // +1 = CW, -1 = CCW

void setup() {
  Serial.begin(9600);

  pinMode(encoderChA, INPUT);
  pinMode(encoderChB, INPUT);

  // Interrupt on rising edge of channel A
  attachInterrupt(digitalPinToInterrupt(encoderChA), encoderISR, RISING);

  Serial.println("Rotate the motor shaft in both directions to test...");
}

void loop() {
  // Show count and direction
  Serial.print("Encoder Count: ");
  Serial.print(encoderCount);
  Serial.print(" | Direction: ");
  
  if (direction == 1) {
    Serial.println("CW");
  } else if (direction == -1) {
    Serial.println("CCW");
  } else {
    Serial.println("Stopped");
  }

  delay(2);
}

void encoderISR() {
  // Direction detection using channel B
  if (digitalRead(encoderChB) == HIGH) {
    encoderCount++;     // One direction → CW
    direction = 1;
  } else {
    encoderCount--;     // Opposite direction → CCW
    direction = -1;
  }
}
