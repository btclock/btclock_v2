# BTClock v2

Improved code for BTClock with many features.
Don't forget to copy `config.h.example` to `config.h`. Not necessary to adapt it, since everything should be configurable through the WebUI.

## Software

- [PlatformIO](https://platformio.org/platformio-ide).
- [Node.js](https://nodejs.org/en) and [yarn](https://yarnpkg.com/).

## Hardware 

- MCU: ESP32 or ESP32-S3 ([Wemos S3 mini](https://www.wemos.cc/en/latest/s3/s3_mini.html) recommended)<br>
**The ESP32 S2 is not supported, it does not have enough RAM**
- Seven eInk displays
- [MCP23017](https://www.microchip.com/en-us/product/mcp23017) I2C Port Expander
- 4x WS2812B leds (optional)
- 5x push buttons (optional)

Recommended to use the BTClock PCB.

## Schematic
![Schematic](doc/schematic.png)
