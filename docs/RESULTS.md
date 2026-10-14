# Results

## Hardware tests

| # | Test | Setup | Result | Status |
| --- | --- | --- | --- | --- |
| 1 | Transmitter blink | D9 to BC547 to LED, 1 Hz sketch | LED blinks at the programmed rate | Pass |
| 2 | Photodiode and op-amp checks | Multimeter on the BPW34 and the op-amp stage | Photodiode resistance in the megaohm range; no usable output from the op-amp stage | Led to the LDR |
| 3 | Receiver follows transmitter | LDR receiver, transmitter blinking, indicator LEDs | The indicator LED at the final stage blinks in step with the transmitter LED | Pass |
| 4 | Signal polarity | One stage, then two stages | One stage inverts the signal; the second restores it | Pass |
| 5 | Protocol on hardware | `lifi_tx` and `lifi_rx` at 200 ms per bit, receiver on the second laptop | Serial Monitor prints H, E, L, L, O | Pass |
| 6 | Distance | LED to LDR distance of [add distance] cm | [add result] | [add status] |

## Timing of the working link

| Quantity | Value |
| --- | --- |
| Bit period | 200 ms |
| Line rate | 5 bit/s |
| Frame | 25 bits, 5 s |
| Dark gap between frames | 600 ms |
| Time per character | 5.6 s |
| Time for `HELLO` | about 28 s, plus 1 s before it repeats |
| Payload rate | about 1.4 bit/s (8 data bits per 5.6 s) |

## Simulation results

Produced by `simulation/run_simulation.sh` on the final sketches. The frame that is in progress when the receiver starts is skipped by design.

| Bit period | Bit rate | Condition | Result |
| --- | --- | --- | --- |
| 10 ms | 100 bps | Clock offset 0.5% either way | `HELLO` decoded |
| 2 ms | 500 bps | Clock offset 0.5% either way | `HELLO` decoded |
| 416 us | about 2.4 kbps | Clock offset 0% and 0.5% either way | `HELLO` decoded |
| 200 ms | 5 bps | LDR-like delays (rising edges 20 ms late, falling edges 40 ms late), clock offset 0.5%, receiver started at several different points | `HELLO` decoded, no false starts |

The simulation checks the protocol logic only, not noise, LED rise time, amplifier settling or breadboard wiring.

## Not yet measured

- Bit error rate over many frames
- Maximum range
- The fastest bit period the LDR can follow
- Voltage levels at Arduino pin D2: [add]

## Reproduce the hardware result

1. Build the hardware ([`HARDWARE_CONNECTIONS.md`](HARDWARE_CONNECTIONS.md)).
2. Flash `firmware/lifi_tx/lifi_tx.ino` to Arduino #1 and `firmware/lifi_rx/lifi_rx.ino` to Arduino #2.
3. Open the Serial Monitor of Arduino #2 at 9600 baud, aim the LED at the LDR in a dim room, and wait for the first full frame.

Add screenshots to `images/` and link them here.
