# Hardware connections

Values in square brackets are to be filled in from the as-built boards.

## Transmitter (Arduino #1)

| From | To | Purpose |
| --- | --- | --- |
| Arduino 5 V | Current-limit resistor [220 to 470 ohm], then LED anode | LED supply |
| LED cathode | BC547 collector | Switched path |
| BC547 emitter | Arduino GND | Return |
| Arduino pin D9 | 1 k resistor, then BC547 base | Control signal |

Notes:

- Arduino #1 only needs power after it is flashed. USB from a laptop, a power bank or a phone charger all work.
- The BC547 pin order differs between sources. If the LED never lights, swap the two outer legs; the circuit is current-limited, so reversing them does no damage. The middle leg is the base.
- LED: long leg = anode (toward the resistor and 5 V), short leg = cathode.

## Receiver (analog board)

Two TL072CP chips, each wired as an inverting stage, an LDR as the sensor, and two indicator LEDs. Powered from a bench supply with two outputs.

| Connection | Where | Purpose |
| --- | --- | --- |
| Bench supply 12 V output | V+ pin (pin 8) of both TL072CP chips | Power |
| Bench supply negative | V- pin (pin 4) of both chips and the receiver ground rail | Ground |
| Bench supply second output, 6 V | Bias (reference) input of stage 1 and stage 2 | Bias without a resistor divider |
| LDR | Input of stage 1 [add how it is connected; resistance in the dark: [add]; in light: [add]] | Sensor |
| Stage 1 | Inverting amplifier; feedback resistor [add value] | Turns the LDR change into a voltage, inverted |
| Indicator LED 1 | Stage 1 output through a series resistor [add value] | Visual check |
| Stage 2 | Inverting amplifier; input resistor [add value], feedback resistor [add value] | Restores the polarity |
| Indicator LED 2 | Stage 2 output through a series resistor [add value] | Visual check |

TL072 pinout (8-pin DIP, notch toward pin 1): 1 = Out1, 2 = IN1-, 3 = IN1+, 4 = V-, 5 = IN2+, 6 = IN2-, 7 = Out2, 8 = V+.

## Receiver to Arduino #2

| Connection | Where | Purpose |
| --- | --- | --- |
| Stage 2 output | Arduino pin D2, directly with no divider | Logic signal |
| Receiver negative rail | Arduino GND | Common ground |
| Level at D2 | [add measured high and low voltages] | Must stay at or below 5 V |

Arduino #2 is powered and read over USB by the second laptop.

> **Check the level at D2.** The Arduino pin tolerates about 5 V. The stage 2 output runs from a 12 V rail, so measure the voltage at D2 with a multimeter in both the lit and dark states before relying on a direct connection. If it exceeds 5 V, add a divider (for example 10 k in series from the output and 6.8 k from the pin to ground).

## Power-up checklist

1. Wire everything with the bench supply off.
2. Set the supply: output 1 at 12 V, output 2 at 6 V. Tie the supply negative to the receiver ground rail and to Arduino #2 GND.
3. Measure the voltage the stage 2 output presents at D2 before connecting the Arduino.
4. Power Arduino #1 and confirm the transmitter LED blinks.
5. Power Arduino #2 and open the Serial Monitor at 9600 baud: `Receiver ready`.
6. Point the transmitter LED at the LDR in a dim room.

## Photos

See [`../images/README.md`](../images/README.md) for the photos to add.
