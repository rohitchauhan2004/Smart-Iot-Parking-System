#include <Servo.h>

// Gate & Slot IR sensors
#define GATE_IR D1
#define SLOT1_IR D2
#define SLOT2_IR D3
#define SLOT3_IR D4

// LEDs for slot status
#define SLOT1_LED D5
#define SLOT2_LED D6
#define SLOT3_LED D7

Servo gateServo;

// Timer tracking
unsigned long slot1Start = 0, slot2Start = 0, slot3Start = 0;
bool slot1Occupied = false, slot2Occupied = false, slot3Occupied = false;

void setup() {
  Serial.begin(9600);

  // Set up pins
  pinMode(GATE_IR, INPUT);
  pinMode(SLOT1_IR, INPUT);
  pinMode(SLOT2_IR, INPUT);
  pinMode(SLOT3_IR, INPUT);

  pinMode(SLOT1_LED, OUTPUT);
  pinMode(SLOT2_LED, OUTPUT);
  pinMode(SLOT3_LED, OUTPUT);

  digitalWrite(SLOT1_LED, LOW);
  digitalWrite(SLOT2_LED, LOW);
  digitalWrite(SLOT3_LED, LOW);

  // Servo setup
  gateServo.attach(D8);  // Connect servo signal wire to D8
  gateServo.write(0);    // Keep gate closed initially
}

void loop() {
  // Entry gate detection
  if (digitalRead(GATE_IR) == LOW) {
    Serial.println("Vehicle at gate detected.");
    openGate();
    delay(3000);  // Simulate gate open duration
    closeGate();
    delay(1000);
  }

  // Slot 1
  handleSlot(SLOT1_IR, SLOT1_LED, slot1Start, slot1Occupied, 1);

  // Slot 2
  handleSlot(SLOT2_IR, SLOT2_LED, slot2Start, slot2Occupied, 2);

  // Slot 3
  handleSlot(SLOT3_IR, SLOT3_LED, slot3Start, slot3Occupied, 3);

  delay(300);  // Prevent sensor bouncing
}

// Gate control
void openGate() {
  Serial.println("Opening gate...");
  gateServo.write(90);  // Adjust for your servo's open angle
}

void closeGate() {
  Serial.println("Closing gate...");
  gateServo.write(0);   // Adjust for your servo's closed position
}

// Slot handling function
void handleSlot(int sensorPin, int ledPin, unsigned long &startTime, bool &occupied, int slotNumber) {
  int state = digitalRead(sensorPin);
  if (state == LOW && !occupied) {
    occupied = true;
    startTime = millis();
    digitalWrite(ledPin, HIGH);
    Serial.print("Slot ");
    Serial.print(slotNumber);
    Serial.println(" occupied.");
  } else if (state == HIGH && occupied) {
    occupied = false;
    digitalWrite(ledPin, LOW);
    unsigned long duration = (millis() - startTime) / 1000;
    Serial.print("Slot ");
    Serial.print(slotNumber);
    Serial.print(" freed. Time occupied: ");
    Serial.print(duration);
    Serial.println(" seconds.");
  }
}
