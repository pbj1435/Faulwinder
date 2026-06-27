// AUTO-GENERATED from shared/protocol.json by shared/gen.mjs — DO NOT EDIT.
// Regenerate with:  node shared/gen.mjs
#pragma once
#include <stdint.h>

#define FW_DEVICE_NAME "Faulwinder"

// --- 128-bit UUIDs (LSB-first byte arrays, ready for Bluefruit BLEUuid/BLEService/BLECharacteristic) ---
// 9a8b0001-7c3e-4f1a-9b2d-1f6c0e5a7d10  (config: Faulwinder configuration service)
const uint8_t UUID128_CONFIG[16] = { 0x10, 0x7d, 0x5a, 0x0e, 0x6c, 0x1f, 0x2d, 0x9b, 0x1a, 0x4f, 0x3e, 0x7c, 0x01, 0x00, 0x8b, 0x9a };
// 9a8b0002-7c3e-4f1a-9b2d-1f6c0e5a7d10  (turns_per_day: Daily winding target (turns))
const uint8_t UUID128_TURNS_PER_DAY[16] = { 0x10, 0x7d, 0x5a, 0x0e, 0x6c, 0x1f, 0x2d, 0x9b, 0x1a, 0x4f, 0x3e, 0x7c, 0x02, 0x00, 0x8b, 0x9a };
// 9a8b0003-7c3e-4f1a-9b2d-1f6c0e5a7d10  (direction_mode: Winding direction)
const uint8_t UUID128_DIRECTION_MODE[16] = { 0x10, 0x7d, 0x5a, 0x0e, 0x6c, 0x1f, 0x2d, 0x9b, 0x1a, 0x4f, 0x3e, 0x7c, 0x03, 0x00, 0x8b, 0x9a };
// 9a8b0004-7c3e-4f1a-9b2d-1f6c0e5a7d10  (speed: Motor PWM duty (%))
const uint8_t UUID128_SPEED[16] = { 0x10, 0x7d, 0x5a, 0x0e, 0x6c, 0x1f, 0x2d, 0x9b, 0x1a, 0x4f, 0x3e, 0x7c, 0x04, 0x00, 0x8b, 0x9a };
// 9a8b0005-7c3e-4f1a-9b2d-1f6c0e5a7d10  (enabled: Winding on/off)
const uint8_t UUID128_ENABLED[16] = { 0x10, 0x7d, 0x5a, 0x0e, 0x6c, 0x1f, 0x2d, 0x9b, 0x1a, 0x4f, 0x3e, 0x7c, 0x05, 0x00, 0x8b, 0x9a };
// 9a8b0006-7c3e-4f1a-9b2d-1f6c0e5a7d10  (calibration: Measured output speed in centi-RPM (RPM x100); 0 = use theoretical from GEAR_RATIO)
const uint8_t UUID128_CALIBRATION[16] = { 0x10, 0x7d, 0x5a, 0x0e, 0x6c, 0x1f, 0x2d, 0x9b, 0x1a, 0x4f, 0x3e, 0x7c, 0x06, 0x00, 0x8b, 0x9a };
// 9a8b0007-7c3e-4f1a-9b2d-1f6c0e5a7d10  (status: Runtime status code (0 idle, 1 winding, 2 sleeping, 3 fault))
const uint8_t UUID128_STATUS[16] = { 0x10, 0x7d, 0x5a, 0x0e, 0x6c, 0x1f, 0x2d, 0x9b, 0x1a, 0x4f, 0x3e, 0x7c, 0x07, 0x00, 0x8b, 0x9a };

// --- Defaults & valid ranges (clamp every BLE write to these) ---
#define DEF_TURNS_PER_DAY 650
#define MIN_TURNS_PER_DAY 0
#define MAX_TURNS_PER_DAY 2000
#define DIRECTION_MODE_BOTH 0
#define DIRECTION_MODE_CW 1
#define DIRECTION_MODE_CCW 2
#define DEF_DIRECTION_MODE DIRECTION_MODE_BOTH
#define DEF_SPEED 100
#define MIN_SPEED 10
#define MAX_SPEED 100
#define DEF_ENABLED 1
#define DEF_CALIBRATION 0
#define MIN_CALIBRATION 0
#define MAX_CALIBRATION 65000

// --- Standard BLE services ---
// battery: service 0x180F, characteristic 0x2A19 — Standard BLE Battery Service / Battery Level (uint8 %)
