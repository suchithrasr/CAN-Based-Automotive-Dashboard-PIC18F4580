# CAN-Based Automotive Dashboard — PIC18F4580

A 3-ECU automotive dashboard system where two sensor nodes read vehicle parameters (speed, RPM, gear, turn indicator) and transmit them over a shared CAN bus to a third node that displays everything live on a dashboard — modeling how real vehicles network multiple ECUs over just two wires instead of dedicated point-to-point links.

## Features

- 3-node CAN network using the PIC18F4580's built-in ECAN module 
- Real-time speed and RPM display, mapped from ADC readings to physical units
- Gear position (Neutral, 1–5, Reverse, Collision) shown as a 2-character code on CLCD
- Turn indicator status (Left / Right / Hazard / Off) shown on CLCD
- Indicator LED blink handled independently via Timer0 interrupt — blink timing isn't affected by how often CAN messages arrive
- Message-ID based routing — each parameter has a unique CAN ID so the receiving node knows exactly what each incoming frame contains

## My Implementation

### System Architecture

| Node | Reads | Sends via CAN |
|---|---|---|
| **ECU1** | Speed (ADC, channel 4) + Indicator (keypad: left/right/hazard/off) | `SPEED_MSG_ID` (0x10), `INDICATOR_MSG_ID` (0x50) |
| **ECU2** | RPM (ADC, channel 4) + Gear position (keypad: up/down/reverse) | `RPM_MSG_ID` (0x30), `GEAR_MSG_ID` (0x20) |
| **ECU3** | Receives all 4 parameters via CAN | Displays on 16x2 CLCD + drives indicator LED via Timer0 ISR |

ECU1 and ECU2 are pure transmitters — they never receive. ECU3 is a pure receiver — it never transmits. All three sit on the same physical CAN bus (CANH/CANL).

### CAN Initialization (identical structure across all 3 ECUs, in `can.c`)
1. RB2 set as CAN TX output, RB3 as CAN RX input
2. CAN module switched to **Configuration mode** via `CANCON` — required before bit-timing registers can be written; code waits on `CANSTAT` to confirm the mode switch completed
3. Bit timing set via `BRGCON1 = 0xE1`, `BRGCON2 = 0x1B`, `BRGCON3 = 0x03` — configured for an 8MHz system clock
4. `RXFCON0 = 0x00` disables all receive filters, with `RXM0`/`RXM1` set to accept every incoming frame regardless of filter match — filtering by relevance is instead done in software using the message ID
5. Module switched back to **Normal mode** to begin live bus communication

### Transmit / Receive Path
`can_transmit()` clears the extended ID registers (standard 11-bit frames only are used), loads the message ID into `TXB0SIDH`/`TXB0SIDL`, sets the data length in `TXB0DLC`, copies data bytes into the `TXB0Dn` buffer, and sets `TXB0REQ` to request transmission.

`can_receive()` checks the `RXB0FUL` flag for a new message, reconstructs the 11-bit ID from `RXB0SIDH`/`RXB0SIDL`, reads the data length and bytes from the receive buffer, then clears `RXB0FUL` and `RXB0IF` to free the buffer for the next frame.

### Example — Speed Data Path
1. ECU1 reads ADC channel 4, converts the raw value to a 0–100 km/h figure, formats it as a 3-character string
2. `can_transmit(SPEED_MSG_ID, buff, sizeof(buff))` sends it with message ID `0x10`
3. ECU3's `process_canbus_data()` receives the frame, matches ID `0x10`, and calls `handle_speed_data()` which writes the string directly to CLCD line 2

Gear and indicator are sent as single-byte index values rather than strings — ECU3's `handle_gear_data()` and `handle_indicator_data()` use a switch-case on the received byte to map it to the correct display string (e.g. index 6 → `"R"`, index 7 → `"C"`) and, for indicator, to set the LED state used by the Timer0 ISR.

### Indicator LED Timing
The LED blink state (`led_state`) is a variable shared between `message_handler.c` and the Timer0 ISR. The ISR toggles the LED on every timer overflow independent of the main loop, so blink timing stays consistent regardless of CAN traffic load.

## File Structure

```
CAN_BASED_MC_PROJECT1/
├── ECU1_PROJECT1.X/
│   ├── main.c                  — init + main loop (speed, indicator)
│   ├── ecu1_sensor.c/.h         — get_speed(), process_indicator()
│   ├── adc.c/.h                 — ADC read for speed sensor
│   ├── digital_keypad.c/.h      — indicator switch input
│   ├── can.c/.h                 — CAN init, transmit, receive
│   └── msg_id.h                 — shared CAN message ID definitions
│
├── ECU2_Project.X/
│   ├── main.c                   — init + main loop (RPM, gear)
│   ├── ecu2_sensor.c/.h         — get_rpm(), get_gear_pos()
│   ├── adc.c/.h, digital_keypad.c/.h
│   └── can.c/.h, msg_id.h
│
└── ECU3_project.X/
    ├── main.c                   — init + main loop (receive + display)
    ├── message_handler.c/.h     — routes CAN messages by ID to CLCD
    ├── timer0.c/.h, isr.c/.h    — Timer0 interrupt for indicator LED blink
    ├── clcd.c/.h                — character LCD driver
    └── can.c/.h, msg_id.h
```

## Tools Used

- MPLAB X IDE
- XC8 Compiler

## Demo

A working demo video of this project is available — [link to be added].

## Author

Suchithra S — B.E. Electronics and Communication Engineering
Built as part of the Advanced Embedded Systems Program at Emertxe Information Technologies
