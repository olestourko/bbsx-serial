## Example Read Output
```
--- Controller Info ---------
Manufacturer: SZBF
Model: BBS3
Hardware Version: 1.5
Firmware Version: 1.0.0.4
Voltage: 48V
Maximum Current: 28A

--- General Settings ---------
Low Voltage Cutoff: 40V
Current Limit: 20A
Assist Level 0 - Current Limit: 0%
Assist Level 1 - Current Limit: 10%
Assist Level 2 - Current Limit: 20%
Assist Level 3 - Current Limit: 30%
Assist Level 4 - Current Limit: 40%
Assist Level 5 - Current Limit: 50%
Assist Level 6 - Current Limit: 60%
Assist Level 7 - Current Limit: 70%
Assist Level 8 - Current Limit: 80%
Assist Level 9 - Current Limit: 100%
Assist Level 0 - Speed Limit: 100%
Assist Level 1 - Speed Limit: 100%
Assist Level 2 - Speed Limit: 100%
Assist Level 3 - Speed Limit: 100%
Assist Level 4 - Speed Limit: 100%
Assist Level 5 - Speed Limit: 100%
Assist Level 6 - Speed Limit: 100%
Assist Level 7 - Speed Limit: 100%
Assist Level 8 - Speed Limit: 100%
Assist Level 9 - Speed Limit: 100%
Wheel Diamater: 26"
Speedmeter Model: External
Speedmeter Signals: 01

--- Throttle Settings ---------
Start Voltage: 1100mV
End Voltage: 3500mV
Mode: Speed
Assist Level: 3
Speed Limit: 17km/h
Start Current: 20
```

## Terminal Requirements
- 1200 baud
- Raw mode; no data modification

## Protocol

### Basic Read Request
|Byte Number|Byte|Meaning|
|:---|:---|:---
|0|0x11|Read
|1|0x51|Basic Data
|2|0x04|Command Length
|3|0xB0|None?
|4|0x05|Sum(bytes 2..4) % 256

### General Read Request
|Byte Number|Byte|Meaning|
|:---|:---|:---
|0|0x11|Read
|1|0x52|General Settings

### Throttle Read Request
|Byte Number|Byte|Meaning|
|:---|:---|:---
|0|0x11|Read
|1|0x54|Throttle Settings

## TODO
- `pedal read` command
- `throttle read` command
- `general save` command
- `pedal save` command
- `throttle save` command
- Error handling
    - From controller responses
    - Device terminal setup and underlying read/write operations
- Allow specifing the `/dev/tty*` that USB-UART adapter is on
- Reading settings from a config file with `libconfig`
- (Maybe) `ncurses` for interaction