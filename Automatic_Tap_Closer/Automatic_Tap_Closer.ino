int ir = 2;       // IR sensor OUT pin
int relay = 8;    // Relay input pin

void setup() {
  pinMode(ir, INPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);   // Pump OFF initially
}

void loop() {

  int sensorValue = digitalRead(ir);

  if (sensorValue == LOW) {      // Hand detected
    digitalWrite(relay, LOW);    // Pump ON
  } else {                       // No hand
    digitalWrite(relay, HIGH);   // Pump OFF
  }
}