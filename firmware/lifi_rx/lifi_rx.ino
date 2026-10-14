// Li-Fi receiver (Arduino #2)
// Reads the comparator output (through a divider) on RX_PIN and decodes
// Manchester frames: preamble 0xAA | sync 0x7E | data byte | even-parity bit.
// Prints decoded characters to the Serial Monitor (9600 baud).

const int  RX_PIN = 2;
const bool INVERT = false;                   // set true if the line is HIGH when the TX LED is OFF
const unsigned long BIT_US    = 200000;      // 200 ms per bit: slow enough for an LDR. Must match TX.
const unsigned long HALF_US   = BIT_US / 2;
const unsigned long WINDOW_US = BIT_US / 4;  // tolerance around the expected mid-bit edge
const unsigned long IDLE_US   = 2 * BIT_US;  // the line must stay dark this long before a frame can start

bool level;                                  // last known line level
unsigned long tNext;                         // expected time of the next mid-bit edge

inline bool lit() { return (digitalRead(RX_PIN) == HIGH) != INVERT; }

bool parityBit(uint8_t v) {
  bool p = false;
  for (int i = 0; i < 8; i++) p ^= (v >> i) & 1;
  return p;
}

// Returns 1 or 0, or -1 if the mid-bit edge never arrived.
// Every bit has a mid-bit edge: falling = 1, rising = 0. Edges at bit boundaries are ignored.
// Each mid-bit edge re-synchronises the clock, so small clock differences between the boards cancel out.
int readBit() {
  while (true) {
    unsigned long now = micros();
    bool cur = lit();
    if (cur != level) {
      long dt = (long)(now - tNext);
      level = cur;
      if (dt >= -(long)WINDOW_US && dt <= (long)WINDOW_US) {
        tNext = now + BIT_US;
        return cur ? 0 : 1;
      }
    } else if ((long)(now - tNext) > (long)WINDOW_US) {
      return -1;
    }
  }
}

void waitForStart() {
  // 1) Wait for a long dark gap. Inside a frame the line is never dark for more than one bit period,
  //    so this means a receiver that starts mid-frame simply waits for the next frame.
  unsigned long darkSince = micros();
  unsigned long litSince = millis();
  unsigned long lastWarn = 0;
  while ((micros() - darkSince) < IDLE_US) {
    if (lit()) {
      darkSince = micros();
      if (millis() - litSince > 3000 && millis() - lastWarn > 3000) {
        Serial.println("Line stuck ON: dim the room or lower receiver sensitivity");
        lastWarn = millis();
      }
    } else {
      litSince = millis();
    }
  }
  // 2) The first rising edge after the gap is the start of the preamble
  while (!lit()) { }
  level = true;
  tNext = micros() + HALF_US;                // first mid-bit edge expected half a bit later
}

void setup() {
  pinMode(RX_PIN, INPUT);
  Serial.begin(9600);
  Serial.println("Receiver ready");
}

void loop() {
  waitForStart();

  // 1) Decode bits until the last 8 equal the sync byte
  uint8_t window = 0;
  bool synced = false;
  for (int i = 0; i < 24 && !synced; i++) {
    int b = readBit();
    if (b < 0) break;
    window = (window << 1) | b;
    if (i >= 7 && window == 0x7E) synced = true;
  }
  if (!synced) { Serial.println("no sync"); return; }

  // 2) Data byte (MSB first), then parity
  uint8_t data = 0;
  for (int i = 0; i < 8; i++) {
    int b = readBit();
    if (b < 0) { Serial.println("frame error"); return; }
    data = (data << 1) | b;
  }
  int p = readBit();
  if (p < 0) { Serial.println("frame error"); return; }
  if (p != (int)parityBit(data)) { Serial.println("parity error"); return; }

  Serial.print("Received: ");
  Serial.println((char)data);
}
