# Config app (Web Bluetooth)

A single static page that connects to the winder over BLE to set turns/day, direction, and
read battery level. Built in **slice 6**, after the firmware's GATT service exists (slice 3).

## Run it
Web Bluetooth needs a **secure context**, so use one of:
- Open `index.html` directly as `file://` in desktop Chrome/Edge (simplest).
- Serve locally: `python3 -m http.server` → `http://localhost:8000`.
- Host on **GitHub Pages** for a permanent HTTPS link you can bookmark on any device.

## Browser support
- ✅ Chrome / Edge on desktop and Android.
- ❌ iOS Safari (and all iOS browsers) lack Web Bluetooth — use the **Bluefy** app to load this page,
  or use **nRF Connect** to poke the characteristics directly.

## Implementation note
The UUIDs here must match the firmware's GATT service. Define them once in the sketch (slice 3),
then mirror them in `index.html` (slice 6). Battery level uses the standard service 0x180F / 0x2A19.
