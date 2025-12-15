#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

SoftwareSerial fingerSerial(2, 3); // RX, TX
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&fingerSerial);

uint8_t id;

void setup() {
  Serial.begin(9600);
  delay(100);
  Serial.println("\nFingerprint Enrollment");

  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Fingerprint sensor found!");
  } else {
    Serial.println("Fingerprint sensor NOT found!");
    while (1);
  }

  Serial.println("Enter ID # (1 - 127) to enroll:");
}

void loop() {

  if (Serial.available()) {
    id = Serial.parseInt();

    if (id == 0) {
      Serial.println("Invalid ID");
      return;
    }

    Serial.print("Enrolling ID ");
    Serial.println(id);

    while (!enrollFingerprint(id));
    Serial.println("Enrollment Successful!");
    Serial.println("Enter next ID to enroll:");
  }
}

uint8_t enrollFingerprint(uint8_t id) {
  int p = -1;

  Serial.println("Place finger on sensor");

  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
  }

  p = finger.image2Tz(1);
  if (p != FINGERPRINT_OK) {
    Serial.println("Image error");
    return p;
  }

  Serial.println("Remove finger");
  delay(2000);

  while (finger.getImage() != FINGERPRINT_NOFINGER);

  Serial.println("Place SAME finger again");

  p = -1;
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
  }

  p = finger.image2Tz(2);
  if (p != FINGERPRINT_OK) {
    Serial.println("Image error");
    return p;
  }

  p = finger.createModel();
  if (p != FINGERPRINT_OK) {
    Serial.println("Finger mismatch");
    return p;
  }

  p = finger.storeModel(id);
  if (p != FINGERPRINT_OK) {
    Serial.println("Storage error");
    return p;
  }

  return true;
}
