// Runs the transmitter sketch in simulated time and records the LED waveform to wave.txt.
// usage: tx_sim <duration_seconds>
#include "stub.h"
static double now_us = 0, limit_us = 60e6;
static std::vector<std::pair<double, int>> wave;
SerialC Serial;

static void finish() {
  FILE* f = fopen("wave.txt", "w");
  for (auto& e : wave) fprintf(f, "%.1f %d\n", e.first, e.second);
  fclose(f);
  exit(0);
}
static void tick(double us) { now_us += us; if (now_us > limit_us) finish(); }

unsigned long micros() { tick(4); return (unsigned long)(now_us / 4) * 4; }
unsigned long millis() { return (unsigned long)(now_us / 1000); }
void delay(unsigned long ms) { tick(ms * 1000.0); }
void pinMode(int, int) {}
void digitalWrite(int, int v) { tick(5); wave.push_back({now_us, v}); }
int digitalRead(int) { return 0; }

int main(int argc, char** argv) {
  if (argc > 1) limit_us = atof(argv[1]) * 1e6;
  setup();
  for (;;) loop();
}
