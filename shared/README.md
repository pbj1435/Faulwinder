# shared/ — the BLE contract (single source of truth)

The firmware and the web app must agree, byte-for-byte, on the Bluetooth GATT layout. If they
drift, the app silently fails to connect or reads garbage. To make drift **impossible**, both
sides are *generated* from one file.

## Files
- **`protocol.json`** — the canonical spec. UUIDs, characteristic types, defaults, valid ranges,
  enum values. **This is the only file you edit by hand.**
- **`gen.mjs`** — generator. Reads `protocol.json`, writes the two derived files below.

## Generated (do NOT hand-edit — they say so at the top)
- `firmware/faulwinder/ble_protocol.h` — Bluefruit-ready: LSB-first 128-bit UUID byte arrays plus
  `DEF_*` / `MIN_*` / `MAX_*` macros for clamping every BLE write.
- `webapp/protocol.js` — Web Bluetooth: lowercase UUID strings plus defaults/ranges/enums.

## Workflow
1. Edit `shared/protocol.json`.
2. Run `node shared/gen.mjs`  (or `npm run gen`).
3. Commit `protocol.json` **and** both generated files together, so a checkout always builds.

Rule of thumb: if you ever find yourself typing a UUID or a default value into `.ino` or
`index.html`, stop — it belongs in `protocol.json`.
