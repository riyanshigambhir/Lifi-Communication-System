# Firmware

| Folder | Board | Purpose |
| --- | --- | --- |
| `lifi_tx/` | Arduino #1 | Transmitter: sends `HELLO` as Manchester-coded light frames on pin D9 |
| `lifi_rx/` | Arduino #2 | Receiver: decodes frames from pin D2 and prints them on the Serial Monitor |
| `tools/mirror_test/` | Arduino #2 | Copies pin D2 to the on-board LED, to check the wiring |
| `tools/edge_monitor/` | Arduino #2 | Prints every edge seen on pin D2 |

## Flashing

1. Install the [Arduino IDE](https://www.arduino.cc/en/software).
2. Open the `.ino` file (the folder name must match the file name, as it does here).
3. Tools > Board > *Arduino Uno*; Tools > Port > the board's port.
4. Upload. Repeat for the other board with the other sketch.

## Settings that must match

Both sketches define `BIT_US` (bit period in microseconds, 200000 = 200 ms). Change it in **both** files and re-flash **both** boards. Matching versions of the two sketches are also required: the receiver's dark-gap rule only works with a transmitter that leaves a long dark gap between frames.

## Serial Monitor

Open it for Arduino #2 at 9600 baud. Opening the monitor or pressing reset restarts the receiver; after a restart it waits for the next frame gap before the first letter appears.
