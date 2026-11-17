/*
  LaserLink - Arduino Optical Communication Transmitter
  Board: Arduino Uno/Nano (or compatible)
  Output: D13 -> laser module driver input

  Protocol:
    Idle: LOW
    Start: HIGH for one bit period
    Data: 8 bits, least-significant bit first
    Stop: LOW for one bit period

  IMPORTANT:
  Use a suitable laser module/driver and never point a laser at eyes,
  people, animals, vehicles, aircraft, or reflective surfaces.
*/

#define LASER_PIN 13
#define BIT_DELAY_MS 5

void sendBit(bool bitValue) {
  digitalWrite(LASER_PIN, bitValue ? HIGH : LOW);
  delay(BIT_DELAY_MS);
}

void sendByte(byte data) {
  sendBit(HIGH);  // Start bit

  for (byte bitIndex = 0; bitIndex < 8; bitIndex++) {
    sendBit(bitRead(data, bitIndex));  // LSB first
  }

  sendBit(LOW);  // Stop bit
}

void setup() {
  pinMode(LASER_PIN, OUTPUT);
  digitalWrite(LASER_PIN, LOW);  // Keep laser off while idle
}

void loop() {
  sendByte('A');
  delay(1000);
}
