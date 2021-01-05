### Terminal Requirements
- 1200 baud
- Raw mode; no data modification

### Basic Read
|Byte Number|Byte|Meaning|
|:---|:---|:---
|0|0x11|Read
|1|0x51|Basic Data
|2|0x04|Command Length
|3|0xB0|None?
|4|0x05|Sum(bytes 2..4) % 256
