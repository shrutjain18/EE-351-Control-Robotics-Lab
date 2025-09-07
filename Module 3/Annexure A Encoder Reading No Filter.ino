// ===============================
// Show Encoder A & B signals in Serial Plotter
// ===============================

const int encoderPinA = 2;   // Encoder channel A
const int encoderPinB = 3;   // Encoder channel B

void setup() {
  Serial.begin(115200);  // Faster baud for better plotting
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);

  Serial.println("Encoder signal monitor started...");
}

void loop() {
  int signalA = digitalRead(encoderPinA);
  int signalB = digitalRead(encoderPinB);

  // Print two signals separated by space
  Serial.print(signalA);
  Serial.print(" ");
  Serial.println(signalB);

  delay(2);  // Small delay for readability in plot (adjust if needed)
}
