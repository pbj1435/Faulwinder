# Faulwinder

*The lazy watch winder.* (**"FOWL-vin-der"** — *faul* is German for "lazy," and a wink at the
**FAULHABER** motor inside. It deep-sleeps for years and wakes only to wind.)

A battery-powered automatic watch winder — a low-cost SwissKubik clone built on a Seeed
XIAO nRF52840, a FAULHABER coreless geared motor, and a DRV8833 driver, configured over
Bluetooth LE. Goal: **years of runtime on one 3.2V LiFePO4 cell.**

Full hardware spec, pin map, and gotchas live in **[CLAUDE.md](./CLAUDE.md)** — read that first.

## Open in Claude Code
1. Keep this whole folder together (don't move files out of it).
2. Open Claude Code pointed at this folder (desktop app → Code tab, or `claude` from the terminal here).
3. Claude reads `CLAUDE.md` automatically, so it already knows the wiring and goals.
4. Start with: **"Let's do slice 1 — motor spin and reverse."**

## Structure
```
faulwinder/
├── CLAUDE.md            ← project context (Claude reads this every session)
├── README.md            ← you are here
├── .gitignore
├── firmware/
│   ├── README.md        ← toolchain + flashing instructions
│   └── faulwinder/
│       └── faulwinder.ino   ← the sketch (skeleton; built up slice by slice)
└── webapp/
    ├── README.md        ← hosting the config page
    └── index.html       ← Web Bluetooth config page (skeleton)
```

## Build order (each slice: Claude writes → you flash & measure → refine)
1. Motor spin + reverse — verify the DRV8833 wiring
2. Bidirectional winding to a daily turn target (with calibration)
3. BLE config service — set turns/direction over Bluetooth, persist to flash
4. Deep sleep + button wake + nSLEEP control — **the battery-life core (measurement-driven)**
5. Battery sense → BLE Battery Service
6. Web Bluetooth config page (`webapp/`)

## Before you start
- **Set `GEAR_RATIO`** in `firmware/faulwinder/faulwinder.ino` to the motor you actually installed
  (22 / 76 / 221 / 262). It drives the run-time math.
- Install the Adafruit nRF52 Arduino core (see `firmware/README.md`).
- You are the hands + the meter: Claude writes and builds; you flash the XIAO and measure
  current. Paste your readings back during slice 4 — that's where it matters most.
