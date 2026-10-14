// Replays wave.txt into the receiver sketch and prints what the receiver prints.
// usage: rx_sim <clock_scale> <start_offset_us> <rise_delay_us> <fall_delay_us> <duration_seconds>
//   clock_scale    receiver clock speed relative to the transmitter (1.005 = 0.5% fast)
//   start_offset   how far into the transmitter's stream the receiver starts
//   rise/fall      delay added to rising / falling edges (models a slow sensor such as an LDR)
#include "stub.h"
static double now_us = 0, limit_us = 60e6, clock_scale = 1.0, offset_us = 12345, rise_us = 0, fall_us = 0;
static std::vector<std::pair<double, int>> wave;
static size_t idx = 0;
SerialC Serial;

static void tick(double us) { now_us += us; if (now_us > limit_us) exit(0); }

unsigned long micros() { return (unsigned long)((now_us * clock_scale) / 4) * 4; }
unsigned long millis() { return (unsigned long)(now_us * clock_scale / 1000); }
void delay(unsigned long ms) { tick(ms * 1000.0 / clock_scale); }
void pinMode(int, int) {}
void digitalWrite(int, int) {}
int digitalRead(int) {
  tick(6);                                         // each read costs about 6 us of real time
  double t = now_us + offset_us;
  while (idx < wave.size() && wave[idx].first <= t) idx++;
  return idx == 0 ? 0 : wave[idx - 1].second;
}

int main(int argc, char** argv) {
  if (argc > 1) clock_scale = atof(argv[1]);
  if (argc > 2) offset_us = atof(argv[2]);
  if (argc > 3) rise_us = atof(argv[3]);
  if (argc > 4) fall_us = atof(argv[4]);
  if (argc > 5) limit_us = atof(argv[5]) * 1e6 - 1e6;
  FILE* f = fopen("wave.txt", "r");
  if (!f) { fprintf(stderr, "wave.txt not found: run tx_sim first\n"); return 1; }
  double t; int v;
  while (fscanf(f, "%lf %d", &t, &v) == 2) wave.push_back({t + (v ? rise_us : fall_us), v});
  fclose(f);
  setup();
  for (;;) loop();
}
