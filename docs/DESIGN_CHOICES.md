# Design choices

Each choice answers a specific problem. Four of them changed during the build (marked **changed**).

## Modulation and line coding

**On/off keying with Manchester coding.** Plain on/off keying (LED on = 1, off = 0) has two weaknesses: the receiver cannot tell where a bit starts, and a long run of identical bits shifts the average signal level, which is what a fixed threshold depends on. Manchester coding puts an edge in the middle of every bit, so the signal is balanced and self-clocking.

## Framing and synchronization

**Preamble, sync byte, data byte, parity bit.** A receiver cannot "sample the middle of a bit" without knowing where bits begin, and two Arduinos have independent clocks. The preamble gives the receiver an alternating pattern to lock onto, the sync byte marks the start of the data, and parity catches single-bit errors. The receiver also re-synchronizes at every mid-bit edge, so clock differences cancel instead of accumulating.

**Start-of-frame rule (changed).** The first receiver locked onto the first rising edge it saw. After a restart it started in the middle of a frame and locked onto a wrong edge, printing bursts of `no sync`. The receiver now starts a frame only after the line has been dark for two bit periods; the transmitter leaves three dark bit periods between frames. Inside a frame the line is never dark for more than one bit period, so a long dark gap can only mean "between frames".

## Sensor: LDR instead of a photodiode (changed)

The first receiver used a BPW34 photodiode feeding a transimpedance stage and a comparator.

1. **The photodiode measured in the megaohm range.** A reverse-biased photodiode behaves as a current source with very high resistance, and its photocurrent is only nanoamps to microamps. Turning that into volts needs a feedback resistance that is also in the megaohm range.
2. **A megaohm stage is hard to use on a breadboard.** It is sensitive to noise and stray leakage, it is slow, and room light shifts its output by volts. The photodiode responded to light on its own, but the op-amp stage gave no usable output.
3. **An LDR avoids the problem.** Its resistance changes by a large factor with light, so an op-amp stage of modest gain produces a swing of volts.

| | BPW34 photodiode | LDR (cadmium sulfide) |
| --- | --- | --- |
| Output | Current of nanoamps to microamps | Resistance, from megohms in the dark to kilohms in light |
| Amplification needed | Megohm-range transimpedance gain | Modest voltage gain |
| Sensitivity to noise and leakage on a breadboard | High | Low |
| Speed | Nanoseconds | Tens of milliseconds |
| What it allows | Kilobit-rate links | A few bits per second |

**The trade is speed for robustness.** Datasheets for common cadmium-sulfide LDRs list rise and fall times in the tens of milliseconds, so the bit period went from the 416 us (about 2.4 kbps) of the original plan to 200 ms (about 5 bit/s). The protocol itself is unchanged and was checked in simulation up to 2.4 kbps, so a photodiode stage remains the route back to kilobit rates (stretch phase in the roadmap).

## Receiver electronics

**Two inverting op-amp stages (changed).** An inverting stage makes its output move opposite to its input, so after the first stage the signal fell when the LED turned on. A second inverting stage flips it back, so the final output is high when the transmitter LED is on. The receiver sketch has an `INVERT` switch for this, but fixing the polarity in hardware keeps the firmware simple.

**12 V rail and 6 V reference from the bench supply (changed).** The plain TL072CP is not meant for a single 5 V supply: its inputs need to sit several volts above ground, so a 5 V supply with a 2.5 V bias does not suit it. The bench supply has two outputs, so both stages run from the 12 V output and the second output, set to 6 V, is the bias for both stages. No resistor divider is needed, and the bias is exact and adjustable.

**TL072 instead of LM358.** The LM358 has a gain-bandwidth product of about 1 MHz and a slew rate of about 0.3 V/us; the TL072 has about 3 to 4 MHz and about 13 V/us in the same pin-compatible package. The speed is not the bottleneck with an LDR, but the TL072 keeps the front end ready for the photodiode stage.

**Indicator LEDs.** Two LEDs show the signal at the receiver for visual debugging. They are not needed for operation.

## Transmitter electronics

**BC547 switch.** An Arduino pin is rated for about 20 mA, so a BC547 NPN transistor carries the LED current. Pin D9 drives its base through a 1 kohm resistor, and a series resistor limits the LED current.

## Microcontroller

**Arduino Uno R3 at both ends.** Its 16 MHz clock gives timing margin far beyond what the bit rates need, and its 5 V logic matches the transistor stage. An ESP32 was considered and left as a later extension.

## Bit period

**200 ms per bit.** Each half-bit lasts 100 ms, several times longer than the tens of milliseconds the LDR needs to respond, so the receiver sees clean edges. Faster bit periods have not been tried yet; finding the fastest reliable one is part of Phase 5.

## What we would change next

- Measure the bit error rate, range and the fastest bit period the LDR can follow.
- Add a divider or protection at pin D2 if the stage 2 output exceeds 5 V.
- Add AC coupling between the stages to reject room light.
- Return to a photodiode with a higher-gain, shielded transimpedance stage for kilobit rates.
