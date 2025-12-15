Code
int flame = 2; // Flame sensor DO pin
int relay = 8; // Relay input pin
void setup() {
pinMode(flame, INPUT);
pinMode(relay, OUTPUT);
digitalWrite(relay, HIGH); // Pump OFF initially
}
void loop() {
int sensorValue = digitalRead(flame);
if (sensorValue == LOW) { // Flame detected
digitalWrite(relay, LOW); // Pump ON
} else { // No hand
digitalWrite(relay, HIGH); // Pump OFF
}
}