# newKeeb

Wireless split ergo, ZMK (main / Zephyr 4.1). **3× Seeed XIAO nRF52840** — two
keyboard halves + a USB dongle. 4×5 hand-wired per half, dual Cirque Pinnacle
trackpads over I2C.

- **Dongle = central**, keyless, plugged into USB. Runs the keymap + both trackpad
  listeners (pointer + scroll).
- **Right = peripheral**, local trackpad = pointer (with acceleration), forwarded.
- **Left = peripheral**, local trackpad = scroll, forwarded.
- Board target: `xiao_ble//zmk` (the `//zmk` variant is required) for all three.

See [WIRING.md](WIRING.md) for the bench reference. The dongle itself has no
wiring — it's a bare XIAO on USB.

## Layout

```
config/
  west.yml                zmk + pointing-acceleration module
  newkeeb_dongle.conf     central: pointing, accel (staged off)
  newkeeb_left.conf       peripheral
  newkeeb_right.conf      peripheral
config/boards/shields/newkeeb/   (custom shields MUST live under config/boards)
  Kconfig.shield          SHIELD_NEWKEEB_LEFT / _RIGHT / _DONGLE
  Kconfig.defconfig       roles, peripheral count (=2), keyboard names
  newkeeb.dtsi            shared by ALL parts: transform + split-input proxies
  newkeeb_half.dtsi       halves only: matrix (kscan) + i2c0 + trackpad
  newkeeb_left.overlay    peripheral: forward pad -> scroll proxy
  newkeeb_right.overlay   peripheral: col-offset, reversed cols, pad -> pointer proxy
  newkeeb_dongle.overlay  central: mock kscan, enable both listeners, accel
  newkeeb_dongle.keymap   unified keymap (central only)
build.yaml                dongle + left + right + settings_reset
```

## Build order (do not skip ahead)

1. **Matrix.** Flash all three. Confirm every key scans through the dongle.
2. **Plain pointer.** Attach the right pad — cursor should move (accel is staged
   off in `newkeeb_dongle.overlay` / `.conf`).
3. **Scroll.** Attach the left pad, confirm forwarded scroll.
4. **Acceleration last.** Uncomment the `STEP 4` markers in
   `newkeeb_dongle.overlay` + `newkeeb_dongle.conf` and tune.

Pairing: flash `settings_reset` to all three if the halves won't bond to the
dongle, then reflash the real firmware.

## Things to settle on the bench

- **Diode direction** — `col2row` (band toward row). Uniform before soldering.
- **P0.09 back-pad wire** — one per half; solder it first on the bare board.
- **Trackpad axis orientation** — TODO block in each overlay (halves are mirrored).
- **Scroll speed** — divisor in `left_scroll_listener` (`&scroll_scaler 1 96`), 64–128.
- **Pointer accel** — factors in `&pointer_accel` (dongle overlay, STEP 4).

## Notes

- **Pin budget:** each half needs 12 signals but the XIAO has 11 castellated pins,
  so exactly one matrix row lands on the P0.09 back pad. The dongle does not change
  this — the pads and matrices are still on the hands.
- **Known ZMK issue #3156:** two-peripheral dongles can drop a half when battery
  fetching is on. Mitigation noted in `newkeeb_dongle.conf`.
- **ZMK Studio** is not wired up (would need physical-layouts + the studio snippet);
  ask if you want it added.
