const int encoderPinA = 2;
const int encoderPinB = 3;
unsigned long lastChangeTime = 0;
const unsigned long debounceDelay = 500; // in microseconds
int filteredA = 0;
int lastStateA = 0;
int filteredB = 0;
int lastStateB = 0;

void setup(){
  Serial.begin(9600);  // Faster baud for better plotting
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);

  Serial.println("Encoder signal monitor started...");
  }
void loop() {
  int rawA = digitalRead(encoderPinA);
  int rawB = digitalRead(encoderPinB);
  unsigned long now = micros();

  if (rawA != lastStateA) {
    if (now - lastChangeTime > debounceDelay) {
      filteredA = rawA;             // accept new stable value
      lastChangeTime = now;
      filteredB = rawB;             // accept new stable value
      lastChangeTime = now;
    }
    lastStateA = rawA;
    lastStateB = rawB;
  }
  // Plot both raw and filtered signals
  Serial.print(rawA);
  Serial.print(" ");
  Serial.println(filteredA);
  delay(10);
}
