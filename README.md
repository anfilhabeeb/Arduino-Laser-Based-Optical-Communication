# LaserLink

**Arduino Laser-Based Optical Communication**

LaserLink is a simple point-to-point optical communication experiment using two Arduino boards. The transmitter converts a character into serial bits and sends them through a laser. A light-sensitive receiver converts the received light variations back into digital data.

This project demonstrates the basic principle of data communication through light.

## Features

- Point-to-point optical data transmission
- Arduino-based transmitter and receiver
- 8-bit data framing
- Least-significant-bit-first transmission
- Configurable bit duration
- Serial Monitor output at the receiver
- No Wi-Fi, Bluetooth, or RF connection required

## System overview

```text
TRANSMITTER                                      RECEIVER
+------------------+                        +------------------+
| Arduino          |                        | Arduino          |
|                  |                        |                  |
| D13 ------------+--> Laser module         |                  |
|                  |          LIGHT ------->| Light receiver   |
|                  |                        | OUT ------------+--> D2
+------------------+                        |                  |
                                            | USB / Serial     |
                                            +--------+---------+
                                                     |
                                                Serial Monitor
```

The laser module and receiver must be aligned so that the receiver can detect the transmitted light. Use a compatible receiver module with a digital output.

## Hardware requirements

- 2 × Arduino Uno, Nano, or compatible boards
- 1 × laser diode module with a suitable driver
- 1 × compatible light receiver module with digital output
- Jumper wires
- USB cables for programming and serial monitoring
- Suitable power supplies

**Receiver note:** The original source code specifies a digital receiver output connected to Arduino D2. A bare photodiode or phototransistor may require an amplifier/comparator circuit; it cannot necessarily be connected directly and expected to produce a clean digital signal.

## Wiring

### Transmitter

| Arduino transmitter | Connection |
|---|---|
| D13 | Laser module driver signal input |
| GND | Laser module GND |
| Power | As required by the laser module |

### Receiver

| Arduino receiver | Connection |
|---|---|
| D2 | Receiver module digital output |
| 5V / 3.3V | According to receiver module specification |
| GND | Receiver module GND |

Check the voltage and pin requirements of your particular laser and receiver modules before connecting them. Do not power a laser diode directly from an Arduino I/O pin unless the module is specifically designed for that connection.

## Repository structure

```text
arduino-laserlink-optical-communication/
├── Tx/
│   └── Tx.ino
├── Rx/
│   └── Rx.ino
├── README.md
└── LICENSE
```

## Communication protocol

The transmitter sends one frame per character.

| Frame field | Value |
|---|---|
| Idle state | LOW |
| Start bit | HIGH |
| Data bits | 8 bits |
| Bit order | LSB first |
| Stop bit | LOW |
| Bit duration | 5 ms |
| Nominal bit rate | 200 bits/second |
| Receiver serial output | 9600 baud |

Example for character `A`:

- ASCII decimal: `65`
- Binary: `01000001`
- Transmission order (LSB first): `1 0 0 0 0 0 1 0`

The start and stop bits are framing signals and are not part of the character data.

## Getting started

### 1. Upload the transmitter

1. Connect the transmitter Arduino to your computer.
2. Open `Tx/Tx.ino` in the Arduino IDE.
3. Select the correct board and COM port.
4. Upload the sketch.
5. Connect the laser module to the configured transmitter output.

The transmitter repeatedly sends the character `A`, with a one-second pause between frames.

### 2. Upload the receiver

1. Connect the receiver Arduino to your computer.
2. Open `Rx/Rx.ino` in the Arduino IDE.
3. Select the correct board and COM port.
4. Upload the sketch.
5. Connect the digital output of the light receiver to D2.
6. Open Serial Monitor and select **9600 baud**.

When the receiver detects a valid frame, the output should look similar to:

```text
LaserLink receiver ready
RX: A  (ASCII: 65)
RX: A  (ASCII: 65)
RX: A  (ASCII: 65)
```

## Configuration

The transmitter and receiver must use the same bit duration.

```cpp
#define BIT_DELAY_MS 5
```

For example, `5` milliseconds gives a nominal bit rate of:

```text
1 / 0.005 = 200 bits per second
```

Changing the bit duration requires updating it in both sketches. The actual reliable data rate depends on the receiver response time, alignment, ambient light, and timing accuracy.

## Troubleshooting

| Problem | Things to check |
|---|---|
| No data received | Check laser alignment, receiver power, and D2 wiring |
| Random characters | Reduce ambient light, improve alignment, and check receiver output stability |
| Framing errors | Confirm both sketches use the same bit duration |
| Receiver always HIGH | Check receiver polarity, output wiring, and pull-up requirements |
| Very short range | Check laser module output, receiver sensitivity, and optical alignment |

Some receiver modules produce an inverted output. If yours does, the signal polarity and start/stop detection in the receiver sketch must be adapted.

## Safety

- Never look directly into the laser beam.
- Never point the laser at people, animals, vehicles, aircraft, or reflective surfaces.
- Use a low-power, appropriately certified module and follow its safety instructions.
- Keep the beam controlled and terminate it safely.

## Current limitations

This is a basic demonstration, not a high-speed or error-corrected communication system.

- One character is transmitted repeatedly.
- No checksum or acknowledgement is implemented.
- There is no retransmission mechanism.
- The timing is software-based and can be affected by execution delays.
- Reliable operation depends on the optical receiver and physical setup.

## Future improvements

- Send complete text messages
- Add packet length and checksum
- Add receiver acknowledgement and retransmission
- Add configurable transmission speed
- Add ambient-light filtering and signal conditioning
- Add two-way optical communication

## License

This project is released under the MIT License. See [LICENSE](LICENSE).
