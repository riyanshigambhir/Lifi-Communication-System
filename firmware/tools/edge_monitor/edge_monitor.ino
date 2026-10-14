// Edge monitor (Arduino #2): prints a line every time the receiver input changes.
// Open the Serial Monitor at 9600 baud. With lifi_tx running, you should see an edge
// about every half bit period at the start of each frame.
const int RX_PIN = 2;
bool last = false;
unsigned long t0;

void setup() {
  pinMode(RX_PIN, INPUT);
  Serial.begin(9600);
  Serial.println("Edge monitor ready");
  t0 = millis();
}

void loop() {
  bool now = digitalRead(RX_PIN);
  if (now != last) {
    last = now;
    Serial.print(millis() - t0);
    Serial.println(now ? " ms: HIGH" : " ms: LOW");
  }
}
