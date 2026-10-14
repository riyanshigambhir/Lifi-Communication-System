# Roadmap

| Phase | Objective | Gate (exit criterion) | Status |
| --- | --- | --- | --- |
| 0 | Scope and procurement | BOM final, parts bought | Done |
| 1 | Transmitter bring-up | LED toggles at a code-set rate | Done |
| 2 | Receiver bring-up | Receiver output gives clean logic levels | Done |
| 3 | Optical link check | Receiver follows the transmitter pattern | Done |
| 4 | Protocol on hardware | `HELLO` decoded end to end | Done |
| 5 | Reliability, rate and range | Faster bits, longer range, under 1% BER | Next (5 to 6 days) |
| 6 | Python interface | Timestamped messages logged on a PC | Planned (3 days) |
| 7 | Characterization and report | Rate, range and BER curves; final report | Planned (4 to 5 days) |
| 8 | Stretch: photodiode stage | Kilobit-rate link with a photodiode | Optional |

## Revised targets for the LDR receiver (proposed)

The kilobit targets belong to a photodiode receiver. For the LDR link, Phase 5 aims to decode reliably at 100 ms per bit (10 bit/s) and at 1 m, with a bit error rate below 1% over a fixed number of frames. These are proposals, to be confirmed once the first measurements are in.

## Phase 5 steps

1. Record the distance at which the link works now, and measure the voltage at D2 with a multimeter; add a divider if it exceeds 5 V.
2. Measure the bit error rate at 200 ms per bit: send a fixed number of frames and count the errors.
3. Shorten the bit period in steps (150 ms, then 100 ms) and note where decoding starts to fail.
4. Increase the distance in steps toward 1 m and record the errors; add a lens, reflector or brighter LED if needed.
5. Repeat in different room lighting; add AC coupling if ambient light limits the result.

## Phases 6 to 8

- **Phase 6, Python interface.** A `pyserial` script on the PC logs the receiver's Serial output with timestamps. Python is used for logging only; bit timing stays on the Arduinos.
- **Phase 7, characterization and report.** Measure the maximum reliable bit rate, range and error rate, plot them against the targets, and write the final report.
- **Phase 8, photodiode stage (stretch).** Return to the BPW34 with a higher-gain, shielded transimpedance stage, or a ready-made photodiode amplifier, to reach kilobit rates with the same protocol.

## Risks and mitigations

| Risk | Effect | Mitigation |
| --- | --- | --- |
| The LDR is too slow for faster bit periods | The rate cannot be raised | Report the measured limit; use the photodiode stage for the kilobit targets |
| Range is limited by LED brightness and LDR sensitivity | The link fails beyond a certain distance | Brighter LED, a lens or reflector on the LED, and shading the LDR from room light |
| Room light holds the receiver on | No data | Dim room for tests, adjust the bias, add AC coupling |
| Op-amp output above 5 V at D2 | Damage to the Arduino pin | Measure first; add a divider (for example 10 k and 6.8 k) |
| The receiver depends on the lab supply | Not portable, tests only in the lab | Regulated 12 V and 6 V rails from a battery or adapter |
