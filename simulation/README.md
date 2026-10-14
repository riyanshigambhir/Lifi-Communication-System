# Simulation

`run_simulation.sh` runs the real firmware sketches in simulated time, so the protocol can be checked without hardware.

How it works:

1. The sketches in `../firmware` are copied with the chosen `BIT_US` and compiled against `stub.h`, a small model of `micros()`, `delay()`, `digitalWrite()`, `digitalRead()` and `Serial`.
2. `tx_main.cpp` runs the transmitter sketch and records the LED waveform to `wave.txt`.
3. `rx_main.cpp` replays that waveform into the receiver sketch with a chosen clock speed, start offset and edge delays (to model a slow sensor such as an LDR), and prints what the receiver prints.

Run:

```bash
bash run_simulation.sh
```

Requires a C++ compiler (`g++` or `clang++`; set `CXX` to choose). Each scenario prints the text the receiver decoded. The frame that is in progress when the receiver starts is skipped by design, so the first letters of the message may be missing.

Limits: this checks the protocol logic and clock tolerance only. It does not model noise, LED rise time, amplifier settling or wiring problems.
