# Firmware

nRF52840 sketch for Faulwinder, the lazy watch winder. Open `faulwinder/faulwinder.ino`.

## Toolchain — Adafruit nRF52 core
1. Arduino IDE → Preferences → Additional Board Manager URLs, add:
   `https://adafruit.github.io/arduino-board-index/package_adafruit_index.json`
2. Tools → Board → Boards Manager → install **Adafruit nRF52**.
3. Select board: **Seeed XIAO nRF52840** (not the Sense variant unless that's what you have).
4. (PlatformIO is fine too if you prefer — board `xiaoblesense` / `xiao_nrf52840`.)

### CLI (used for the pre-flash compile check)
This repo also compiles headlessly with `arduino-cli`, so we can catch build errors before you flash:
```
arduino-cli config set board_manager.additional_urls https://adafruit.github.io/arduino-board-index/package_adafruit_index.json
arduino-cli core update-index
arduino-cli core install adafruit:nrf52
npm run build:firmware     # compiles firmware/faulwinder without flashing
```

## Libraries
- **Bluefruit** — bundled with the Adafruit nRF52 core (BLE, incl. Battery Service).
- **Adafruit_LittleFS / InternalFS** — bundled; used for settings persistence (slice 3).

## Flashing
- Double-tap the XIAO **RESET** button → it mounts as a USB drive → drag the compiled `.uf2` on,
  OR just hit Upload in the Arduino IDE.
- For bench testing, power from **USB**. Keep the 3V3-to-battery jumper OUT while USB is connected
  (never USB + battery on 3V3 at once — see CLAUDE.md).

## First run (slice 1)
1. Confirm `GEAR_RATIO` and the pin constants match your build.
2. Wire per the breadboard layout; nSLEEP on D2.
3. Upload. The motor should turn one way ~20s, stop, reverse ~20s, stop, repeat.
4. If a direction doesn't move: check AIN1/AIN2 wiring and that nSLEEP (D2) is going HIGH.

Then move to slice 2 with Claude Code.
