# Bill of materials

| Component | Part | Qty used | Qty bought | Role |
| --- | --- | --- | --- | --- |
| Microcontroller | Arduino Uno R3 (ATmega328P, 16 MHz, 5 V logic) | 2 | 2 | Transmitter and receiver, one each |
| Emitter | 5 mm high-brightness white LED | 1 | 10 | Transmitter light source |
| LED switch | BC547 NPN transistor | 1 | 5 | Carries the LED current |
| Light sensor | LDR (light-dependent resistor), [add part number] | 1 | [add] | Receiver sensor |
| Op-amps | TL072CP dual JFET op-amp | 2 chips | 2 | Stage 1 and stage 2, both inverting |
| Indicator LEDs | 5 mm LEDs | 2 | [add] | Visual check of the signal at the receiver |
| Passives | Resistor and capacitor assortment kits | n/a | 1 each | Gain, bias and current-limit resistors |
| Photodiode (not in the final receiver) | BPW34 PIN photodiode | 0 | 3 | First receiver attempt; planned photodiode stage |
| Prototyping | 830-point breadboards, jumper wires | 2, 1 set | 2, 1 set | One board per end |
| Test equipment | Multimeter | 1 | n/a | Debugging the receiver |
| Test equipment | USB logic analyzer, 24 MHz, 8 channels | 0 | 1 | Digital timing checks (planned) |
| Power | Laboratory DC bench supply, two outputs | 1 | n/a | 12 V rail and 6 V reference for the receiver |

Notes:

- The original cost estimate for the core set was Rs 2,200 to 2,600. Update this with the actual spend: [add].
- Buy spares of the cheap parts (BC547, LEDs, photodiodes); legs are easy to damage on a breadboard.
- Check marketplace listings carefully. One logic-analyzer listing showed the photo and description of a DC-DC converter under a logic-analyzer title.
- The plain TL072CP needs its inputs several volts above ground, so it runs from the 12 V supply, not from the Arduino's 5 V.
