# CLAUDE.md — Faulwinder (DIY Watch Winder Firmware)

Context for Claude Code. Read this every session before touching code.

## What we're building
A battery-powered automatic watch winder — a low-cost SwissKubik clone. A Swiss coreless
geared motor winds the watch a set number of turns per day, then everything deep-sleeps.
Configured over Bluetooth LE from a phone/laptop. Target: **years of runtime on one
3.2V LiFePO4 cell**. The firmware's hard job is not the winding — it's the low-power sleep.

## Repo layout
- `firmware/` — nRF52 Arduino sketch (the winder controller)
- `webapp/`   — Web Bluetooth config page (static HTML/JS, single file)

## Hardware
- **MCU:** Seeed XIAO nRF52840 (nRF52840, BLE, Arduino-compatible)
- **Driver:** DRV8833 carrier (Pololu #2130) — H-bridge, has nSLEEP
- **Motor:** FAULHABER 1624E024S + 16/5 spur gearhead (coreless, brushed DC, 2-wire)
- **Power:** ONE 26650 LiFePO4 cell, 3.2V nominal, feeding the rail directly
- **Button:** panel-mount momentary, normally-open
- **Battery sense:** 2× ~1MΩ resistors as a divider → analog pin

## Pin map  (XIAO → target)
| XIAO pin | Net | Notes |
|---|---|---|
| 3V3  | +3.2V rail | Power IN via 3V3 (NOT BAT). Also feeds DRV8833 VIN. |
| GND  | GND rail | Common ground with DRV8833, button, divider. |
| D0   | DRV8833 AIN1 | motor direction A (PWM-capable) |
| D1   | DRV8833 AIN2 | motor direction B (PWM-capable) |
| D2   | DRV8833 nSLEEP | **drive LOW to sleep the driver, HIGH to run** |
| D3   | wake button | other leg → GND; use INPUT_PULLUP; wake-on-press |
| D4 / A4 | battery-sense node | analog read of ½·Vbatt. **Use A4, NOT A0** (A0==D0, already taken) |

DRV8833 outputs: AOUT1 → motor M1, AOUT2 → motor M2 (channel A only; channel B unused).

## Power architecture
- Single LiFePO4 cell (3.2V) → +3.2V rail → XIAO 3V3 + DRV8833 VIN.
- XIAO runs 1.7–3.6V (fine). DRV8833 needs **≥2.7V** — below that the motor browns out.
- The XIAO's GPIO highs sit at the same ~3.2V rail as the DRV8833 VIN, so logic levels
  match with no level shifting.

## Motor / ratio model
Gear ratio is a config constant — set it to the unit actually installed.
- Available ratios on hand: **22:1** (output ~75 RPM @3V), **76:1** (~24 RPM), **221:1 / 262:1** (~7–8 RPM).
- Bare motor ≈ ~1650 RPM @3V; **output RPM = bare ÷ ratio**.
- Daily run time to hit the turn target: `run_seconds = (TURNS_PER_DAY / output_RPM) * 60`.
- Don't trust theory blindly — calibrate: run a known duration, count actual holder turns,
  adjust. Expose the calibration as a setting.

## Firmware behaviour (target)
1. **Winding:** bidirectional. Drive AIN1/AIN2 for a burst one way, brief halt, reverse,
   repeat, spreading bursts across the day until TURNS_PER_DAY is reached, then sleep.
   - Halt (~1–2s, both inputs LOW) between reversals — no slamming into reverse.
   - Optional PWM speed control (LEDC / analogWrite on the inputs) — PWM BOTH inputs for
     balanced bidirectional drive per the DRV8833 note.
2. **Sleep (the whole point):** between bursts and overnight, put the nRF52 in deep sleep
   and pull D2 (nSLEEP) LOW so the driver draws ~0. Wake on an RTC/timer for the next burst.
3. **Button wake:** D3 press wakes the MCU and opens a BLE advertising window (~2 min),
   then it stops advertising and returns to sleep. Don't advertise continuously.
4. **Battery report:** sample A4, convert (×2, scale to %), expose via the standard BLE
   **Battery Service**. Sampling is brief and on-demand only.
5. **Persistence:** store settings in flash (LittleFS) so they survive power cycles.

## BLE config (GATT) — settings to expose
- `turns_per_day`   (uint, default 650)
- `direction_mode`  (CW / CCW / BOTH)
- `speed`           (PWM duty %, optional)
- `enabled`         (on/off)
- `calibration`     (run-seconds-per-turn or measured output RPM)
- (read-only) `battery_level` %, `status`
- Optional: a daily start-time / schedule window.

## Toolchain
- Adafruit nRF52 Arduino core (Bluefruit BLE library), or PlatformIO if preferred.
- Flash via UF2 bootloader: double-tap RESET → drag-drop, or upload from Arduino IDE.
- LittleFS (Adafruit_LittleFS / InternalFS) for settings.
- Web Bluetooth page: vanilla HTML/JS, no framework, must be served over HTTPS or file://.

## Critical gotchas (don't regress these)
- **Battery sense is on A4, not A0** — A0 is D0 (AIN1).
- **nSLEEP (D2) is the battery-life lever** — must go LOW whenever not actively winding,
  including during MCU sleep. If the driver never sleeps, the build fails its whole purpose.
- **Power via 3V3 pin, never BAT; never USB + battery at once.** During dev, run on USB and
  leave the 3V3-to-rail jumper out; for battery runs, USB out first, then connect 3V3.
- **DRV8833 < 2.7V = brownout.** LiFePO4 holds above this for nearly its whole charge.
- **XIAO board quiescent is the wildcard.** The dev board's regulator/charge-IC/LED can draw
  tens of µA and cap battery life short of ideal. Measure deep-sleep current with a µA meter;
  disable the user LED and anything unneeded. Target single-digit µA; accept tens of µA.
- Coreless motor draws only ~2.5–5 mA running — current headroom is huge, not a concern.

## Build order (slices — write, user flashes & measures, refine)
1. Motor spin + reverse (verify DRV8833 wiring; nSLEEP HIGH to enable)
2. Bidirectional winding to a daily turn target (with calibration)
3. BLE config service (set turns/direction over Bluetooth, persist to flash)
4. Deep sleep + button wake + nSLEEP control  ← the hard, measurement-driven part
5. Battery-sense → BLE Battery Service
6. Web Bluetooth config page

## Working conventions (how we build this)
- **BLE contract = one source of truth.** `shared/protocol.json` defines every UUID, type,
  default, and range. Run `node shared/gen.mjs` (`npm run gen`) to regenerate
  `firmware/faulwinder/ble_protocol.h` and `webapp/protocol.js`. NEVER type a UUID or default
  into the `.ino` or `index.html` — change `protocol.json` and regenerate. Commit the json and
  both generated files together. See `shared/README.md`.
- **Compile before the user flashes.** After any firmware change, run `npm run build:firmware`
  (`arduino-cli compile --fqbn Seeeduino:nrf52:xiaonRF52840 firmware/faulwinder`). Never hand
  the user a sketch that hasn't compiled clean — every flash is a manual ritual for them.
- **Defensive firmware, always:** clamp every BLE-writable value to its `MIN_*`/`MAX_*`;
  put a hard timeout on any motor run so a bad command can't spin forever; drive `nSLEEP` (D2)
  LOW on every code path that exits winding, including faults; version the flash settings struct
  so an old layout is detected and reset to defaults, never read as garbage.
- **Per-slice git.** Commit at the end of each working slice with the measured result in the
  message (e.g. "slice 4: deep sleep, measured 12µA"). One slice = one revertible step.
- **Toolchain note:** the XIAO nRF52840 builds under the **Seeed** core (`Seeeduino:nrf52`,
  FQBN `Seeeduino:nrf52:xiaonRF52840`), which layers on Adafruit's Bluefruit. The Adafruit
  core alone does NOT list the XIAO. Board index URLs are in `firmware/README.md`.

## Feedback loop
The user is the hands and the meter. Treat their multimeter / µA-meter readings as ground
truth, especially for sleep current and turn calibration. Adjust code against measured
numbers, not theory.
