# Protocol

## Line coding: Manchester

Every bit is sent as two half-bit intervals, so every bit has an edge in the middle.

| Bit | First half | Second half | Mid-bit edge |
| --- | --- | --- | --- |
| 1 | LED on | LED off | falling |
| 0 | LED off | LED on | rising |

This keeps the signal balanced (equal on and off time on average) and lets the receiver recover its timing from the edges. Room light adds a steady offset to the sensor signal; a balanced signal is much less sensitive to it than plain on/off keying with long runs of identical bits.

## Frame

One frame carries one data byte in 25 bits. Every field is Manchester-coded.

| Field | Bits | Value | Purpose |
| --- | --- | --- | --- |
| Preamble | 8 | `10101010` (0xAA) | Lets the receiver lock its bit timing |
| Sync byte | 8 | `01111110` (0x7E) | Marks the start of the data |
| Data | 8 | one byte, MSB first | Payload |
| Parity | 1 | even parity of the data bits | Catches single-bit errors |

The LED is off between frames. The test message is `HELLO`: one frame per character, with a 1 s pause after each message.

## Timing at 200 ms per bit

| Quantity | Value |
| --- | --- |
| Bit period (`BIT_US`) | 200 ms |
| Half-bit interval | 100 ms |
| Line rate | 5 bit/s |
| Frame length | 25 bits = 5 s |
| Dark gap between frames (transmitter) | 3 bit periods = 600 ms |
| Time per character | 5.6 s |
| Time for `HELLO` | about 28 s |
| Payload rate | 8 bits per 5.6 s = about 1.4 bit/s |

## Receiver algorithm

1. **Wait for a long dark gap.** The line must stay dark for two bit periods (`IDLE_US`, 400 ms). Inside a frame the line is never dark for more than one bit period, so a long gap can only mean the space between frames. This stops a receiver that starts mid-frame from locking onto a wrong edge.
2. **First rising edge** after the gap = start of the preamble. The first mid-bit edge is expected half a bit later.
3. **Decode each bit** by waiting for the next edge. An edge within a quarter bit (`WINDOW_US`) of the expected time is the mid-bit edge: falling = 1, rising = 0. Any other edge is a bit-boundary edge and is ignored.
4. **Re-synchronize at every accepted edge:** the next mid-bit edge is expected one bit period after the last one. Clock differences between the two Arduinos therefore cannot accumulate over the frame.
5. **Shift bits in** until the last eight equal `0x7E` (sync found).
6. **Read the data byte** (8 bits, MSB first) and the parity bit.
7. **Check parity.** Print `Received: X` on success, or `no sync`, `frame error` or `parity error`. If the line stays high for several seconds it prints `Line stuck ON`.

## Parameters

| Parameter | File | Meaning |
| --- | --- | --- |
| `BIT_US` | both sketches | Bit period in microseconds. **Must match.** |
| `IDLE_US` | receiver | Dark time required before a frame can start (2 bit periods) |
| `WINDOW_US` | receiver | Tolerance around the expected mid-bit edge (a quarter bit) |
| `INVERT` | receiver | Set true if the line is high when the LED is off (left false: stage 2 restores the polarity) |
| gap delay | transmitter | `delay(3 * BIT_US / 1000)`: dark time between frames |

The receiver's `IDLE_US` must stay shorter than the transmitter's gap, with margin for the sensor's response time.
