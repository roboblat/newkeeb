# newKeeb — bench wiring reference

Both halves are wired **identically** to the XIAO. Mirroring is done in firmware,
not in the wiring. Format below: `Signal → nRF pin (XIAO pad)`.

This is a **3-controller dongle build**: both halves are peripherals (wired as
below), and the third XIAO is a USB dongle with **no wiring at all** (bare board,
optional reset button). Everything on this page applies to the two halves only.

Power the trackpad from **3V3, never 5V** (Pinnacle range 2.5–3.6 V, GPIO not 5 V
tolerant). VDD/GND are the XIAO's 3V3 and GND pads, not GPIO.

## Pin assignment (both halves wired IDENTICALLY)

Wire both hands the same, mirror-image. The right half reverses its column scan
order in firmware (see newkeeb_right.overlay), so you never wire left vs. right
differently. Wire to buses, not to "RC" labels.

### Matrix — rows (top of hand -> thumb)

```
Top row   → P1.11 (D6)
2nd row   → P1.12 (D7)
3rd row   → P1.13 (D8)
Thumb row → P0.09 (D14)   [back pad — needs nfct-pins-as-gpios]
```

### Matrix — columns (outer edge -> center gap)

```
Outer col → P0.02 (D0)
Col       → P0.03 (D1)
Col       → P0.28 (D2)
Col       → P0.29 (D3)
Inner col → P1.14 (D9)
```

### Trackpad (I2C)

```
SDA → P0.04 (D4)
SCL → P0.05 (D5)
DR  → P1.15 (D10)
VDD → 3V3 pad   [not GPIO]
GND → GND pad   [not GPIO]
```

### Spare

```
Free → P0.10 (D15)   [only unused castellation]
```

12 of 13 usable GPIO used. The single back-pad joint (P0.09/D14) is Row 3.

## Diode orientation — CONFIRM BEFORE SOLDERING

Firmware is set to `diode-direction = "col2row"`, which means:

```
Diode band (cathode) → toward the ROW line
Diode body (anode)   → toward the COLUMN line (through the switch)
```

Wire every diode the same way. If the matrix scans wrong after wiring (dead board,
or ghosting), the fix is usually flipping `diode-direction` to `"row2col"` in
`newkeeb.dtsi` — a one-line change, no rewiring — **provided all diodes point the
same direction**. Non-uniform diodes are the thing to rule out first.

## Trackpad breakout (12-pin FPC → 5 wired signals)

Only five pins are used in I2C mode. Leave the SPI pins unconnected.

```
SCL → P0.05 (D5)
SDA → P0.04 (D4)
DR  → P1.15 (D10)
VDD → 3V3
GND → GND
SCK → unconnected
SI  → unconnected
SO  → unconnected
SS  → unconnected
```

- **R1 must be removed** from each pad to select I2C mode (fixed address 0x2A).
- Add **4.7 kΩ pull-ups** on SDA and SCL to 3V3 — the XIAO has none. One pair per
  half (each half is its own I2C bus with one device).
- Keep the FPC tail short; if the bus is unreliable at 400 kHz, drop
  `clock-frequency` to `100000` in `newkeeb.dtsi`.
- Ignore the INVX / INVY / SWP jumpers on the pad PCB — orientation is handled in
  devicetree.

## Sanity checks before power-on

- 3V3 to VDD, **not** 5V.
- SDA/SCL not swapped; pull-ups present.
- All diode bands facing the row side, uniformly.
- Back-pad (D14) joint solid — reflow it if Row 3 reads dead.
