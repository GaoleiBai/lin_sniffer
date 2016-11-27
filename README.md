# lin_sniffer
Lin bus sniffer, based on stm32f103c8t6
## Features
### Verson 0.1
#### Firmware
- Support LIN v2.0
- PC communication
- Show full header, real ID, data QTY, CRC

#### Software

### PC communications
You need STM32 VCP driver for connect sniffer to PC<p>
Actual link:  <a>http://www.st.com/en/development-tools/stsw-stm32102.html</a>

### Mesage format
- Byte[0] - Full header
- Byte[1] - Real message ID
- Byte[2] - Data lenght (QTY)
- Byte[3..10] - Data
- Byte[11] - CRC
