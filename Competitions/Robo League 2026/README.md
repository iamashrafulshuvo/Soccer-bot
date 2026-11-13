<h1 align="center">🏟️ ROBO LEAGUE 2026</h1>

<p align="center">
  <b>Match report and technical post-mortem of our ESP32 Bluetooth soccer robot</b><br/>
  Organized by the Robotics Club of BRAC University (ROBU)
</p>

<p align="center">
  <a href="../../README.md">← Back to main project</a>
</p>

---

## Contents

- [At a Glance](#at-a-glance)
- [Summary](#summary)
- [Robot Configuration](#robot-configuration)
- [Match Timeline](#match-timeline)
- [Gallery](#gallery)
- [Failure Analysis](#failure-analysis)
- [How We Will Isolate the Fault](#how-we-will-isolate-the-fault)
- [Lessons Learned](#lessons-learned)
- [Next Revision](#next-revision)
- [Pre-Match Checklist for Future Events](#pre-match-checklist-for-future-events)

---

## At a Glance

| | |
|:--|:--|
| **Event** | ROBO LEAGUE 2026 |
| **Organizer** | Robotics Club of BRAC University (ROBU) |
| **Category** | Soccer Bot Competition |
| **Date** | November 2026 |
| **Control** | Bluetooth, Android serial app |
| **Power** | 11.1 V 3S LiPo |
| **My role** | Hardware assembly, soldering, hardware testing and debugging |
| **Result** | Participated and field tested; withdrew mid-match after a hardware fault |

---

## Summary

This was the first time the robot played in full-contact matches. Speed, acceleration and handling were good in the opening phase of the match. After a hard collision, the Bluetooth link became unstable and then dropped, and the ESP32 was noticeably hot afterwards. With no control left, we pulled the robot out of the match rather than risk damaging the electronics or the battery.

The most important outcome is a clear picture of how the robot behaves under real impact, and a concrete list of hardware changes for the next revision. The control firmware behaved as designed: the drive commands worked until the link was lost.

---

## Robot Configuration

| Part | Specification | Role in the match |
|:--|:--|:--|
| **Controller** | ESP32 (WROOM-32) | Receives Bluetooth commands and generates 20 kHz PWM |
| **Motor drivers** | 2x BTS7960 (43 A peak) | Drive the left and right motors |
| **Motors** | 2x 37GB 800 RPM metal gear motors | Movement and pushing the ball |
| **Battery** | 11.1 V 3S LiPo | Powers drivers and motors directly |
| **Regulator** | XL4015 buck converter | Supplies 5 V to the ESP32 |
| **Wiring** | Jumper wires and soldered power joints | Logic connections between ESP32 and drivers |

The wiring and pin map are in the [main README](../../README.md#wiring-and-pin-map). The system diagram is in [System Architecture](../../README.md#system-architecture).

---

## Match Timeline

| Phase | What happened |
|:--|:--|
| **1. Before the match** | Robot assembled, powered up and paired over Bluetooth with the controller phone. |
| **2. Opening play** | The robot responded quickly to commands, accelerated well and turned sharply. |
| **3. Collision** | A strong physical impact mid-game. |
| **4. Link instability** | Bluetooth became unreliable, with delayed or missed commands. |
| **5. Disconnection** | The Bluetooth link dropped completely and the robot stopped responding. |
| **6. Withdrawal** | The ESP32 felt unusually hot, so we stepped back from the match to inspect the hardware safely. |

The firmware stops both motors when the Bluetooth client disconnects, so losing the link did not leave the robot driving uncontrolled.

---

## Gallery

<p align="center">
  <img src="Assets/Lineup.jpg" alt="Robot lineup" width="650"/><br/>
  <sub>Robot lineup</sub>
</p>

<p align="center">
  <img src="Assets/COMPETITION_ARENA.jpg" alt="Competition arena" width="650"/><br/>
  <sub>Competition arena</sub>
</p>

<p align="center">
  <img src="Assets/ROBOT_ARENA.jpg" alt="Robot in the arena" width="650"/><br/>
  <sub>Robot in the arena</sub>
</p>

---

## Failure Analysis

### Observed symptoms

| # | Symptom | When |
|:--:|:--|:--|
| 1 | Bluetooth became unstable, then disconnected | After the collision |
| 2 | ESP32 unusually hot | Found after the match |
| 3 | No response to any command | After the disconnection |

### Suspected causes

The exact fault is not confirmed yet. These are the leading suspects, in the order we plan to check them.

**1. Loose jumper wires**

The logic side is connected with jumper wires, which can slip or shift under impact. A wire moving on the board could briefly short two logic pins or touch a power pin, which would explain both the sudden reset or dropout and the heat.

**2. Motor back-EMF and current spikes**

When the motors are suddenly stalled or reversed by a collision, they push inductive spikes back into the power system. Without isolation between the motor side and the ESP32, those spikes can reach the logic supply or the GPIO pins.

**3. Supply dip or regulator stress**

A stalled motor draws a very large current. A sag on the battery rail can pull the 5 V rail down with it and cause the ESP32 to brown out and reset, which would also drop the Bluetooth link. A regulator working harder than expected, either the XL4015 or the ESP32 board's own regulator, would also explain the heat. Since the XL4015 output is set with a small adjustment screw, we will also check that vibration did not move it.

### Which symptoms each cause explains

| Suspected cause | Bluetooth drop | ESP32 heat |
|:--|:--:|:--:|
| Loose jumper wires | ✅ | ✅ |
| Motor back-EMF / spikes | ✅ | ➖ |
| Supply dip / regulator stress | ✅ | ✅ |

Wiring and regulator stress both fit the full picture, which is why they come first in the checks below.

---

## How We Will Isolate the Fault

Steps for the post-event inspection, in order:

1. **Visual inspection:** look for burn marks, bent pins, loose or shifted jumpers, and any solder joint that moved.
2. **Check the XL4015 output** with a multimeter, with no load and then under load. It should read a steady 5 V.
3. **Check for shorts** between 5 V, 3V3, GND and the GPIO pins with the power off.
4. **Bench test the ESP32 alone** on USB power to see whether the board and its Bluetooth still work.
5. **Run the robot on the bench** with the wheels lifted and watch the ESP32 temperature.
6. **Shake and tap test:** gently move the wiring while the robot runs to see whether the connection drops.
7. **Stall test:** hold a wheel briefly and watch the 5 V rail for dips.

---

## Lessons Learned

- **Loose wiring does not survive full contact.** Connections that work on the bench can fail in a real match.
- **Bench tests are not match tests.** Impact and stall conditions show faults that normal testing never does.
- **Motors and logic need protection between them.** Spikes and supply dips from the motor side have to be kept away from the ESP32.
- **Test for noisy wireless environments.** Many Bluetooth devices run in a competition hall, so link stability matters.
- **A failsafe is worth having.** Stopping the motors on disconnect kept the robot from driving blind.

---

## Next Revision

| Problem | Planned fix | Why it helps |
|:--|:--|:--|
| Jumper wires can shift on impact | Move the logic wiring to a soldered dot-board or custom PCB | Connections cannot move or short |
| Motor spikes can reach the ESP32 | Add optocoupler isolation on the driver control lines, using fast parts that suit 20 kHz PWM | Keeps motor-side noise away from the GPIO pins |
| Inductive spikes from the motors | Add flyback protection | Absorbs the voltage spike when current is cut or reversed |
| Supply dips during stalls | Add bulk capacitance near the ESP32 supply and near the drivers | Smooths short voltage sags |
| Board damaged by impact | Mount the ESP32 on shock-absorbing padding and add strain relief to cables | Reduces shock reaching the electronics |
| Not tested under worst-case load | Run sustained stall and collision tests before the next event | Finds weak points before the match |

---

## Pre-Match Checklist for Future Events

1. Check that every connector and jumper is seated and secured.
2. Measure the XL4015 output at 5 V before connecting the ESP32.
3. Confirm the battery is fully charged and the wiring is dry and undamaged.
4. Pair the controller phone and test every command with the wheels lifted.
5. Check the ESP32 temperature after a few minutes of running.
6. Keep a spare ESP32, spare wires and a multimeter in the toolbox.

---

## Certificate

<!-- Add the participation certificate here once you have it:
<p align="center"><img src="Assets/certificate.jpg" alt="Participation certificate" width="600"/></p>
-->
