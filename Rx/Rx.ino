/*
  LaserLink - Arduino Optical Communication Receiver
  Board: Arduino Uno/Nano (or compatible)
  Input: D2 <- digital output of a compatible light receiver module

  Protocol:
    Idle: LOW
    Start: HIGH for one bit period
    Data: 8 bits, least-significant bit first
    Stop: LOW for one bit period

  The receiver waits 1.5 bit periods from start detection before
  sampling data bit 0, placing the sample near the bit centre.
*/

#define RX_PIN 2
#define BIT_DELAY_MS 5

void setup() {
  pinMode(RX_PIN, INPUT);
  Serial.begin(9600);
  Serial.println("LaserLink receiver ready");
}

void loop() {
  // Wait for a HIGH start bit.
  if (digitalRead(RX_PIN) == HIGH) {
    // Skip the start bit and sample near the centre of data bit 0.
    delay(BIT_DELAY_MS + (BIT_DELAY_MS / 2));

    byte receivedData = 0;

    // Read 8 data bits, LSB first.
    for (byte bitIndex = 0; bitIndex < 8; bitIndex++) {
      bitWrite(receivedData, bitIndex, digitalRead(RX_PIN));

      if (bitIndex < 7) {
        delay(BIT_DELAY_MS);
      }
    }

    // Move to the centre of the stop bit and validate it.
    delay(BIT_DELAY_MS);
    bool stopBitValid = (digitalRead(RX_PIN) == LOW);

    if (stopBitValid) {
      Serial.print("RX: ");
      Serial.print((char)receivedData);
      Serial.print("  (ASCII: ");
      Serial.print(receivedData);
      Serial.println(")");
    } else {
      Serial.println("Framing error: invalid stop bit");
    }

    // Do not treat a sustained HIGH level as another start bit.
    while (digitalRead(RX_PIN) == HIGH) {
      delay(1);
    }
  }
}
