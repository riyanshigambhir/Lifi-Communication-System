// Li-Fi transmitter (Arduino #1)
// Frame: preamble 0xAA | sync 0x7E | data byte | even-parity bit
// Every bit is Manchester-coded: 1 = LED HIGH then LOW, 0 = LED LOW then HIGH.

const int LED_PIN = 9;                       // pin 9 -> 1k -> BC547 base
const unsigned long BIT_US  = 200000;        // 200 ms per bit: slow enough for an LDR. Must match RX.
const unsigned long HALF_US = BIT_US / 2;

unsigned long tNext;

void sendHalf(bool level) {
  digitalWrite(LED_PIN, level ? HIGH : LOW);
  tNext += HALF_US;
  while ((long)(micros() - tNext) < 0) { }   // wait until this half-bit ends
}

void sendBit(bool b) {
  sendHalf(b);
  sendHalf(!b);
}

void sendByte(uint8_t v) {
  for (int i = 7; i >= 0; i--) sendBit((v >> i) & 1);
}

bool parityBit(uint8_t v) {                  // even parity: total number of 1s becomes even
  bool p = false;
  for (int i = 0; i < 8; i++) p ^= (v >> i) & 1;
  return p;
}

void sendFrame(uint8_t data) {
  tNext = micros();
  sendByte(0xAA);                            // preamble 10101010
  sendByte(0x7E);                            // sync 01111110
  sendByte(data);
  sendBit(parityBit(data));
  digitalWrite(LED_PIN, LOW);                // idle = LED off
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  const char *msg = "HELLO";
  for (const char *c = msg; *c; c++) {
    sendFrame(*c);
    delay(3 * BIT_US / 1000);                // dark gap between frames: 3 bit periods
  }
  delay(1000);
}
