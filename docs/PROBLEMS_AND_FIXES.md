# Problems and fixes

| # | Problem | Symptom | Likely cause | Resolution |
| --- | --- | --- | --- | --- |
| 1 | Transmitter LED stayed dark | No light even with the sketch running and no compile errors | A far-too-large resistor in the LED path | Resistor values checked against the color code; the LED now blinks. |
| 2 | Uncertain BC547 pin order | Sources disagree on the collector/emitter order | Manufacturer and source variation | Safe swap test: the circuit is current-limited, so reversing the outer legs does no damage. |
| 3 | Chip seated along one column | No free holes for jumpers; pin mapping did not match the wiring tables | IC placed lengthwise instead of straddling the center gap | Chip reseated across the gap, one pin per column on each side. |
| 4 | Receiver LED stuck on | LED lit with the sensor removed | Both comparator inputs at the same reference, so the comparator sat at a knife edge | Threshold separated from the bias point. |
| 5 | Op-amp supply | Stage misbehaved on 5 V with a 2.5 V bias | The plain TL072CP needs its inputs several volts above ground | Moved to a 12 V supply with a 6 V bias. |
| 6 | Photodiode stage unusable | The photodiode responded alone, but the op-amp stage gave no usable output and room light shifted it | Photodiode resistance in the megaohm range, so the feedback resistance needed is also megohms | Multimeter checks led to replacing it with an LDR. |
| 7 | Output inverted | Signal fell when the transmitter LED turned on | A single inverting stage inverts the signal | Second inverting stage added. |
| 8 | Bursts of `no sync` after a restart | Several no-sync lines in a row | Receiver started in the middle of a frame and locked onto edges inside it | Receiver starts a frame only after a dark gap of two bit periods; transmitter leaves three. |
| 9 | Only the first letter received | Serial Monitor showed `H` repeatedly | New receiver paired with the old transmitter, whose short gap between letters was never recognized as a frame gap | Both boards re-flashed with matching versions. |
| 10 | Receiver output not visible | Nothing on the transmitter's laptop | The Serial Monitor shows only the board plugged into that laptop | Receiver Arduino connected to the second laptop; the transmitter only needs power. |
| 11 | Photodiode polarity uncertain | Sources disagree on which lead is the cathode | Marking differs between datasheet and distributor pages | LED-in-series test; no longer needed after the switch to the LDR. |

The pattern: most failures came from part values, supply limits and mismatched firmware versions, not from the protocol design.

## Quick checks

| Symptom | Check |
| --- | --- |
| No `Receiver ready` in the Serial Monitor | Correct port, baud 9600, press reset on the receiver Arduino |
| `Receiver ready` but nothing else | Run `firmware/tools/edge_monitor`; no edges means check the D2 wire, the ground wire and the supply |
| Only the first letter repeats | The two boards run different versions; re-flash both |
| Garbage or wrong letters | `BIT_US` differs between sketches, or the sensor is too slow: raise `BIT_US` in both |
| `Line stuck ON` | Dim the room or lower the receiver sensitivity |
| Nothing decodes but edges are seen | Set `INVERT = true` in `lifi_rx.ino` |
