// AUTO-GENERATED from shared/protocol.json by shared/gen.mjs — DO NOT EDIT.
// Regenerate with:  node shared/gen.mjs

export const DEVICE_NAME = "Faulwinder";

export const UUID = {
  config: "9a8b0001-7c3e-4f1a-9b2d-1f6c0e5a7d10", // Faulwinder configuration service
  turns_per_day: "9a8b0002-7c3e-4f1a-9b2d-1f6c0e5a7d10", // Daily winding target (turns)
  direction_mode: "9a8b0003-7c3e-4f1a-9b2d-1f6c0e5a7d10", // Winding direction
  speed: "9a8b0004-7c3e-4f1a-9b2d-1f6c0e5a7d10", // Motor PWM duty (%)
  enabled: "9a8b0005-7c3e-4f1a-9b2d-1f6c0e5a7d10", // Winding on/off
  calibration: "9a8b0006-7c3e-4f1a-9b2d-1f6c0e5a7d10", // Measured output speed in centi-RPM (RPM x100); 0 = use theoretical from GEAR_RATIO
  status: "9a8b0007-7c3e-4f1a-9b2d-1f6c0e5a7d10", // Runtime status code (0 idle, 1 winding, 2 sleeping, 3 fault)
};

// Defaults, ranges, types, and enum values — mirror of the firmware clamps.
export const CHARS = [
  {
    "key": "turns_per_day",
    "short": "0002",
    "type": "uint16",
    "props": [
      "read",
      "write"
    ],
    "default": 650,
    "min": 0,
    "max": 2000,
    "desc": "Daily winding target (turns)"
  },
  {
    "key": "direction_mode",
    "short": "0003",
    "type": "enum",
    "props": [
      "read",
      "write"
    ],
    "default": "BOTH",
    "values": [
      "BOTH",
      "CW",
      "CCW"
    ],
    "desc": "Winding direction"
  },
  {
    "key": "speed",
    "short": "0004",
    "type": "uint8",
    "props": [
      "read",
      "write"
    ],
    "default": 100,
    "min": 10,
    "max": 100,
    "desc": "Motor PWM duty (%)"
  },
  {
    "key": "enabled",
    "short": "0005",
    "type": "bool",
    "props": [
      "read",
      "write"
    ],
    "default": true,
    "desc": "Winding on/off"
  },
  {
    "key": "calibration",
    "short": "0006",
    "type": "uint16",
    "props": [
      "read",
      "write"
    ],
    "default": 0,
    "min": 0,
    "max": 65000,
    "desc": "Measured output speed in centi-RPM (RPM x100); 0 = use theoretical from GEAR_RATIO"
  },
  {
    "key": "status",
    "short": "0007",
    "type": "uint8",
    "props": [
      "read",
      "notify"
    ],
    "readOnly": true,
    "desc": "Runtime status code (0 idle, 1 winding, 2 sleeping, 3 fault)"
  }
];

export const STANDARD = {
  "battery": {
    "service": "0x180F",
    "characteristic": "0x2A19",
    "desc": "Standard BLE Battery Service / Battery Level (uint8 %)"
  }
};
