# RGB LED Information Panel

A convenient way to get important information at a glance from a desktop or wall mountable RGB LED panel to be used in conjunction with the API: ```https://github.com/sanjito31/tidbyt-clone-api```. Utilizes a standard 64x32 RGB LED panel and ESP32 Arduino microcontroller.

## Features
Can display a variety of information in full color configured from the API such as:
- **Time**
- **Weather Info**
- **Spotify Currently Playing**
- **NYC MTA Subway Wait Times**
- **F1 Championship Standings**

## Tech Stack
- Written in C++ 
- Developed with PlatformIO
- Implements Arduino Packackes
- Custom objects for each information type
- ESP32 HUB75 LED MATRIX PANEL DMA Display
- Adafruit GFX Library
- ArduinoJson

## Components
- 64x32 RGB LED panel using standard HUB75 connector
- ESP32 microcontroller with Wi-Fi
- 5V 15A DC power supply

## Future Developments
- Migrate to WEBP image/animation rendering direct from API so that display is content agnostic.
- Over the air updater module implementation


---
- Inspired by tidbyt.com
- Disclaimer: This project is not affiliated or endorsed by Tidbyt




