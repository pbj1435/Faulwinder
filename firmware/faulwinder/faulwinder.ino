/*
  Faulwinder — the lazy watch winder — nRF52840 firmware
  Board:  Seeed XIAO nRF52840  (Adafruit nRF52 Arduino core)
  Spec:   see ../../CLAUDE.md for the full hardware map, gotchas, and build order.

  STATUS: SKELETON. Build it up in slices (see ../README.md).
          Slice 1 (motor spin + reverse) is roughly runnable as-is once GEAR_RATIO/pins are confirmed.
*/

// ---------------- Pin map (see CLAUDE.md) ----------------
const int PIN_AIN1   = D0;   // DRV8833 AIN1   — motor direction A (PWM-capable)
const int PIN_AIN2   = D1;   // DRV8833 AIN2   — motor direction B (PWM-capable)
const int PIN_NSLEEP = D2;   // DRV8833 nSLEEP — LOW = sleep driver, HIGH = run
const int PIN_BUTTON = D3;   // wake button -> GND (INPUT_PULLUP)
const int PIN_BATT_SENSE = A4;   // battery-sense divider node  (USE A4, NOT A0 — A0 == D0!)
                                 // NOTE: not PIN_VBAT — the XIAO variant.h already #defines that (GPIO32, internal batt).

// ---------------- Config (tune to your build) ----------------
const int   GEAR_RATIO    = 22;    // <-- SET to your installed gearhead: 22 / 76 / 221 / 262
const int   TURNS_PER_DAY = 650;   // daily winding target
const int   BURST_SECONDS = 20;    // motor run time per direction burst
const int   HALT_MS       = 1500;  // pause between reversals (let it coast to a stop)
const int   MOTOR_PWM     = 255;   // 0..255 duty; lower = slower holder (optional speed trim)
const float VBAT_SCALE    = 2.0;   // R1 == R2 divider -> multiply measured voltage by 2

// ---------------- Motor primitives (slice 1) ----------------
void driverWake()  { digitalWrite(PIN_NSLEEP, HIGH); }
void driverSleep() { digitalWrite(PIN_NSLEEP, LOW);  }   // <-- the battery-life lever

void windCW()  { driverWake(); analogWrite(PIN_AIN1, MOTOR_PWM); digitalWrite(PIN_AIN2, LOW); }
void windCCW() { driverWake(); digitalWrite(PIN_AIN1, LOW);  analogWrite(PIN_AIN2, MOTOR_PWM); }
void halt()    { digitalWrite(PIN_AIN1, LOW); digitalWrite(PIN_AIN2, LOW); }  // coast

// ---------------- TODO (later slices) ----------------
// 2: bidirectional scheduler -> hit TURNS_PER_DAY (run_seconds = TURNS_PER_DAY / outputRPM * 60); add calibration
// 3: BLE GATT config (turns_per_day, direction_mode, enabled, calibration) + LittleFS persistence
// 4: deep sleep between bursts; wake on RTC/timer; button (D3) opens ~2min BLE window; driverSleep() whenever idle
// 5: read A4, x VBAT_SCALE, -> % -> standard BLE Battery Service
// 6: config UI in ../../webapp/index.html (Web Bluetooth)

void setup() {
  pinMode(PIN_AIN1, OUTPUT);
  pinMode(PIN_AIN2, OUTPUT);
  pinMode(PIN_NSLEEP, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  driverSleep();                 // start with the driver asleep
  // TODO: Serial for bench testing; init BLE (Bluefruit); init LittleFS; set up RTC/timer wake
}

void loop() {
  // ---- Slice 1 smoke test: prove the wiring ----
  // Expect: motor turns one way ~20s, stops, turns the other way ~20s, stops, repeat.
  windCW();   delay((unsigned long)BURST_SECONDS * 1000);
  halt();     delay(HALT_MS);
  windCCW();  delay((unsigned long)BURST_SECONDS * 1000);
  halt();     delay(HALT_MS);
  driverSleep();
  // TODO (slice 2+): replace this loop with the turn-targeted scheduler + deep sleep.
}
