#!/usr/bin/env node
// Faulwinder BLE contract generator.
// Reads shared/protocol.json (the ONE source of truth) and writes:
//   - firmware/faulwinder/ble_protocol.h  (Bluefruit-ready: LSB-first 128-bit UUID byte arrays + defaults/ranges)
//   - webapp/protocol.js                  (Web Bluetooth: lowercase UUID strings + defaults/ranges)
// Run:  node shared/gen.mjs   (or: npm run gen)
// Never hand-edit the generated files — change protocol.json and regenerate.

import { readFileSync, writeFileSync } from "node:fs";
import { fileURLToPath } from "node:url";
import { dirname, join } from "node:path";

const here = dirname(fileURLToPath(import.meta.url));
const root = join(here, "..");
const spec = JSON.parse(readFileSync(join(here, "protocol.json"), "utf8"));

const fullUuid = (short) => spec.baseUuid.replace("XXXX", short).toLowerCase();
const upper = (s) => s.toUpperCase();

// "9a8b0002-7c3e-4f1a-9b2d-1f6c0e5a7d10" -> LSB-first 16-byte C array literal for Bluefruit.
function lsbBytes(uuid) {
  const hex = uuid.replace(/-/g, "");
  const bytes = [];
  for (let i = 0; i < 16; i++) bytes.push("0x" + hex.slice(i * 2, i * 2 + 2));
  return bytes.reverse().join(", ");
}

const all = [spec.service, ...spec.characteristics];

// ---------- firmware header ----------
let h = `// AUTO-GENERATED from shared/protocol.json by shared/gen.mjs — DO NOT EDIT.\n`;
h += `// Regenerate with:  node shared/gen.mjs\n#pragma once\n#include <stdint.h>\n\n`;
h += `#define FW_DEVICE_NAME "${spec.deviceName}"\n\n`;
h += `// --- 128-bit UUIDs (LSB-first byte arrays, ready for Bluefruit BLEUuid/BLEService/BLECharacteristic) ---\n`;
for (const c of all) {
  h += `// ${fullUuid(c.short)}  (${c.key}: ${c.desc})\n`;
  h += `const uint8_t UUID128_${upper(c.key)}[16] = { ${lsbBytes(fullUuid(c.short))} };\n`;
}
h += `\n// --- Defaults & valid ranges (clamp every BLE write to these) ---\n`;
for (const c of spec.characteristics) {
  if (c.type === "uint16" || c.type === "uint8") {
    if (c.default !== undefined) h += `#define DEF_${upper(c.key)} ${c.default}\n`;
    if (c.min !== undefined) h += `#define MIN_${upper(c.key)} ${c.min}\n`;
    if (c.max !== undefined) h += `#define MAX_${upper(c.key)} ${c.max}\n`;
  } else if (c.type === "bool") {
    h += `#define DEF_${upper(c.key)} ${c.default ? 1 : 0}\n`;
  } else if (c.type === "enum") {
    c.values.forEach((v, i) => { h += `#define ${upper(c.key)}_${v} ${i}\n`; });
    h += `#define DEF_${upper(c.key)} ${upper(c.key)}_${c.default}\n`;
  }
}
h += `\n// --- Standard BLE services ---\n`;
for (const [k, v] of Object.entries(spec.standard)) {
  h += `// ${k}: service ${v.service}, characteristic ${v.characteristic} — ${v.desc}\n`;
}
writeFileSync(join(root, "firmware/faulwinder/ble_protocol.h"), h);

// ---------- webapp module ----------
let j = `// AUTO-GENERATED from shared/protocol.json by shared/gen.mjs — DO NOT EDIT.\n`;
j += `// Regenerate with:  node shared/gen.mjs\n\n`;
j += `export const DEVICE_NAME = ${JSON.stringify(spec.deviceName)};\n\n`;
j += `export const UUID = {\n`;
for (const c of all) j += `  ${c.key}: "${fullUuid(c.short)}", // ${c.desc}\n`;
j += `};\n\n`;
j += `// Defaults, ranges, types, and enum values — mirror of the firmware clamps.\n`;
j += `export const CHARS = ${JSON.stringify(spec.characteristics, null, 2)};\n\n`;
j += `export const STANDARD = ${JSON.stringify(spec.standard, null, 2)};\n`;
writeFileSync(join(root, "webapp/protocol.js"), j);

console.log("Generated:\n  firmware/faulwinder/ble_protocol.h\n  webapp/protocol.js");
