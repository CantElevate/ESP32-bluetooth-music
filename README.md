# ESP32 Bluetooth A2DP Music Sink

This project allows you to turn your ESP32 board into a Bluetooth A2DP music sink, which means that you can play music wirelessly from a phone or computer and have the audio outputted through the ESP32's built-in DAC. The OLED display will show the track name and artist information for the currently playing song.

## Hardware Requirements

- ESP32 board
- OLED display

## Library Requirements
- https://github.com/pschatzmann/ESP32-A2DP
- https://github.com/adafruit/Adafruit_SSD1306

## Features

- Connects to phone or computer as a Bluetooth A2DP sink
- Outputs audio through built-in DAC on ESP32
- Displays track and artist information on OLED display
- Shuts down after 5 minutes of inactivity

## Usage

1. Connect the OLED display to your ESP32 board according to the manufacturer's instructions. SDA/SCL on d22 and d21
2. Upload the code to your ESP32 board using the Arduino IDE.
3. On your phone or computer, search for Bluetooth devices and connect to "MyMusic".
4. Start playing music and the track and artist information should be displayed on the OLED display. The device will shut down after 5 minutes of inactivity.

## Customization
