<h1 align="center">ESP32 Bluetooth Soccer Robot</h1>

<p align="center">
  <b>A fast, high-torque soccer robot controlled over Bluetooth. Built with an ESP32, two BTS7960 motor drivers and a 3S LiPo battery.</b>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/MCU-ESP32-red?logo=espressif&logoColor=white" alt="MCU"/>
  <img src="https://img.shields.io/badge/Firmware-Arduino%20C%2B%2B-00979D?logo=arduino&logoColor=white" alt="Firmware"/>
  <img src="https://img.shields.io/badge/Control-Bluetooth%20Classic-0082FC?logo=bluetooth&logoColor=white" alt="Control"/>
  <img src="https://img.shields.io/badge/Drivers-2x%20BTS7960-2e7d32" alt="Drivers"/>
  <img src="https://img.shields.io/badge/Battery-3S%20LiPo%2011.1V-orange" alt="Battery"/>
  <img src="https://img.shields.io/badge/License-MIT-yellow" alt="License"/>
</p>

<p align="center">
  <img src="assets/media/bot_demo.gif" alt="Soccer Bot in action" width="600"/>
</p>

---

## Overview

This is a two-motor, tank-style robot made for robot soccer. In this game, strong pushing power and quick direction changes matter more than top speed.

An **ESP32** receives simple one-letter commands from an Android app over Bluetooth. It turns them into PWM signals for two **BTS7960** motor drivers. The motors run directly from an **11.1 V 3S LiPo** battery, and a separate **XL4015** buck converter gives the ESP32 a stable 5 V supply.

> **My contribution:** I did the **hardware side** of this team project: robot assembly, soldering and wiring of the power and driver connections, hardware testing and debugging. See [Team](#team).

---

## Key Features

- **Strong drive:** the BTS7960 drivers (43 A peak each) are powered straight from the battery for high torque and fast response.
- **Full movement control:** forward, backward, spin left, spin right, four curved moves and stop.
- **Adjustable speed:** ten speed levels that change instantly, even while the robot is moving.
- **Low-noise motors:** 20 kHz PWM keeps the motors free from audible whine.
- **Bluetooth failsafe:** the robot stops on its own if the phone disconnects.
- **Works on both ESP32 cores:** the code builds on Arduino core 2.x and 3.x.
- **Separate logic power:** the ESP32 runs from its own regulated 5 V supply.

---

## Table of Contents

- [Overview](#overview)
- [Key Features](#key-features)
- [Hardware](#hardware)
- [System Architecture](#system-architecture)
- [Wiring and Pin Map](#wiring-and-pin-map)
- [Firmware](#firmware)
- [Getting Started](#getting-started)
- [Gallery](#gallery)
- [Competitions](#competitions)
- [Future Improvements](#future-improvements)
- [License](#license)
- [Team](#team)

---

## Hardware

| Component | Specification | Role |
|:--|:--|:--|
| **Microcontroller** | ESP32 (WROOM-32), Wi-Fi + Bluetooth | Receives commands and creates PWM signals |
| **Motor drivers** | 2x BTS7960 (43 A peak) | Control motor speed and direction |
| **Motors** | 2x 37GB 800 RPM metal gear motors | Drive and pushing power |
| **Battery** | 11.1 V 3S LiPo (high C-rating) | Main power source |
| **Regulator** | XL4015 step-down buck module | Converts 11.1 V to 5 V for the ESP32 |
| **Controller** | Android Bluetooth serial app | Drives the robot from a phone |

---

## System Architecture

```
                     [ 11.1V 3S LiPo Battery ]
                                 │
           ┌─────────────────────┴─────────────────────┐
           ▼                                           ▼
[ BTS7960 Driver 1 & 2 ]                    [ XL4015 Buck Converter ]
(High-current power line)                   (Step-down to 5V DC)
           │                                           │
           ▼                                           ▼
[ 2x 37GB Motors ]                          [ ESP32 VIN & GND ]
 (800 RPM output)                           (Logic power line)
                                                       │
                                                       ▼
                                            [ Bluetooth Serial Interface ]
                                           (PWM controls -> BTS7960 inputs)
```

<p align="center">
  <img src="assets/hardware/Circuit_diagram.png" alt="Circuit Diagram" width="700"/>
</p>

---

## Wiring and Pin Map

| Driver side | Function | ESP32 GPIO | BTS7960 pin |
|:--|:--|:--:|:--|
| Left | Forward | 21 | RPWM |
| Left | Backward | 19 | LPWM |
| Right | Forward | 23 | RPWM |
| Right | Backward | 22 | LPWM |
| Both | Enable | n/a | R_EN and L_EN tied to 3V3 |

**Power connections**

- Battery (+/−) → both BTS7960 power inputs (B+ / B−) and the XL4015 input.
- XL4015 output (set to 5 V) → ESP32 VIN and GND.
- ESP32 GND must be shared with the BTS7960 logic GND.
- Each BTS7960 output → one 37GB motor.

---

## Firmware

The firmware is a single Arduino sketch: [`soccer_bot_control.ino`](soccer_bot_control.ino).

| Setting | Value |
|:--|:--|
| PWM frequency | 20 kHz |
| PWM resolution | 8-bit (0–255) |
| Bluetooth device name | `SoccerBot` |
| Library | `BluetoothSerial` (ESP32 Arduino core) |
| Failsafe | Both motors stop if the Bluetooth client disconnects |

**Commands**

| Key | Action |
|:--:|:--|
| `F` | Forward |
| `B` | Backward |
| `L` | Spin left |
| `R` | Spin right |
| `G` | Forward-left (left wheel at half speed) |
| `H` | Forward-right (right wheel at half speed) |
| `I` | Backward-left (left wheel at half speed) |
| `J` | Backward-right (right wheel at half speed) |
| `S` | Stop |
| `0`–`9`, `q` | Set speed (applies to the last movement command) |

**Speed levels**

| Key | `0` | `1` | `2` | `3` | `4` | `5` | `6` | `7` | `8` | `9` / `q` |
|:--|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|:--:|
| PWM duty | 0 | 100 | 120 | 140 | 160 | 180 | 200 | 220 | 240 | 255 |

Default speed at startup is level `5` (180).

---

## Getting Started

1. Install the **Arduino IDE** and add the **ESP32 board package**.
2. Open `soccer_bot_control.ino` and select your ESP32 board (e.g. *ESP32 Dev Module*).
3. Upload the sketch over USB.
4. Wire the robot as shown in [Wiring and Pin Map](#wiring-and-pin-map) and connect the battery.
5. On your Android phone, pair with **SoccerBot** in Bluetooth settings.
6. Open a Bluetooth serial / RC-car controller app that sends the letters above, connect to `SoccerBot`, and drive.

> ⚠️ Always lift the wheels off the ground for the first power-on test.

---

## Gallery

### Build and Hardware Testing

<p align="center">
  <img src="assets/hardware/soccer-bot.jpg" alt="Build progress" width="600"/>
</p>

<p align="center">
  <img src="assets/media/bot_showcase.gif" alt="Hardware showcase" width="600"/>
</p>

### Team

<p align="center">
  <img src="assets/media/Team%20Work.jpg" alt="Team work" width="600"/>
</p>

---

## Competitions

### ⚽ ROBO LEAGUE 2026 — BRAC University (ROBU)

| | |
|:--|:--|
| **Organizer** | Robotics Club of BRAC University (ROBU) |
| **Category** | Soccer Bot Competition |
| **Date** | November 2026 |
| **My role** | Hardware assembly, soldering and hardware debugging |
| **Status** | Participated and field tested |

The robot played in full-contact soccer matches. It accelerated and maneuvered well at first, but after a strong collision mid-game the Bluetooth connection became unstable and then dropped, and the ESP32 felt unusually hot. We stepped back from the match to inspect the hardware safely.

**Main takeaways**

- Loose jumper wiring is a weak point in a full-contact robot.
- Motor back-EMF and regulator stress are the leading suspects for the fault.
- Real matches expose problems that bench testing does not.

<p align="center">
  <img src="competitions/ROBO-LEAGUE-2026/Assets/Lineup.jpg" alt="Robot lineup" width="420"/>
  <img src="competitions/ROBO-LEAGUE-2026/Assets/COMPETITION_ARENA.jpg" alt="Competition arena" width="420"/>
</p>

👉 [**Full ROBO LEAGUE 2026 report, failure analysis and gallery ➔**](competitions/ROBO-LEAGUE-2026/README.md)

---

## Future Improvements

- [ ] Replace loose jumper wires with a soldered dot-board or custom PCB.
- [ ] Add optocoupler isolation between the ESP32 GPIOs and the motor driver logic pins.
- [ ] Add flyback protection against motor inductive spikes.
- [ ] Add shock-absorbing padding around the microcontroller and chassis.
- [ ] Stress-test under sustained high-load stall conditions.

---

## License

This project is released under the **MIT License**. Add a `LICENSE` file to the repository root to make it official.

---

## Team

This is a team project. I mainly worked on the hardware.

| Member | Contribution |
|:--|:--|
| [**Ashraful Shuvo**](https://github.com/iamashrafulshuvo) | Hardware assembly, soldering, wiring, hardware testing and debugging |
| [**Farhan**](https://github.com/iamfarhan99) | Original team repository, firmware and control |

> Original team repository: [iamfarhan99/ESP32-based-Soccer-Bot](https://github.com/iamfarhan99/ESP32-based-Soccer-Bot)
