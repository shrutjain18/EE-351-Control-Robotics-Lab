const int switchPin1 = 4;  // Switch 1 pin
const int switchPin2 = 5;  // Switch 2 pin
// Variables for switch states
int switchState1 = 0;
int switchState2 = 0;

void setup() {
  // put your setup code here, to run once:
  // Setup switches
  Serial.begin(9600);
  pinMode(switchPin1, INPUT_PULLUP);
  pinMode(switchPin2, INPUT_PULLUP);
  Serial.println("Starting System");
}

void loop() {
  // put your main code here, to run repeatedly:
  switchState1 = digitalRead(switchPin1);
  switchState2 = digitalRead(switchPin2);
  Serial.print("Switch 1: ");
  Serial.print(switchState1);
  Serial.print(" Switch 2: ");
  Serial.println(switchState2);
}
