// Mirror test (Arduino #2): copies the receiver input pin to the on-board LED.
// Use it to check the wiring before running the protocol. With the transmitter blinking,
// the small "L" LED on the receiver Arduino should blink in step with it.
const int RX_PIN = 2;

void setup() {
  pinMode(RX_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, digitalRead(RX_PIN));
}
