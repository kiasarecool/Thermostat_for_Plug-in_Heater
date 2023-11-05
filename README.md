# Thermostat for Plug-in Heater

This repository contains the code and documentation for a custom-built thermostat designed to control plug-in heaters. The thermostat uses a dual 10A relays setup in parallel to handle up to 20A of current, making it suitable for high-power applications such as heating a home greenhouse.

## Features

- **Temperature Control**: Utilizes a BME280 sensor for accurate temperature readings.
- **Relay Handling**: Controls dual 2-10A relays in parallel for high-current handling.
- **Safety**: Designed with safety in mind to handle the electrical loads of household heating appliances.
- **Ease of Use**: Simple button interface for adjusting temperature setpoints.
- **Display**: Incorporates an SH1106 OLED display for real-time temperature, humidity, and pressure readouts.
- **OTA Updates**: Supports Over-The-Air (OTA) updates for easy firmware upgrades.

## Hardware Components

- ESP32-S2 Mini for the main microcontroller and WiFi capabilities
- BME280 sensor for temperature, humidity, and atmospheric pressure measurements
- SH1106 OLED display for real-time feedback
- HiLetGo 4-channel relay module, 2 channels in use
- Standard 20A outlet to plug in the heater
- 120V to 5V converter (retrieved from a USB charging block) to power the ESP32-S2 Mini
- 12V to 12V, 60W transformer providing constant 12V output
- DC barrel plug for the 12V output connection
- Buttons for user input to adjust the temperature setpoint
- LEDs to indicate the status of the relay

## Wiring Diagram

Please refer to the `wiring_diagram.png` file for details on how to wire the components together.

## Setup and Usage

1. **Flash the Firmware**: Upload the code from this repository to the ESP32-S2 using the Arduino IDE.
2. **Configure WiFi**: Set your WiFi credentials in the code to enable OTA updates.
3. **Adjust Setpoint**: Use the button interface to adjust the temperature setpoint to your desired level.
4. **Monitor**: The OLED display will show the current temperature, setpoint, humidity, and pressure.
5. **OTA Updates**: When updates are available, use the Arduino IDE to upload new firmware wirelessly.

## OTA Updates

The ESP32-S2 Mini supports OTA updates. To update your thermostat firmware:

1. Choose the network port associated with the device in the Arduino IDE.
2. Upload the sketch normally; the IDE will handle the OTA update process.

Ensure that the device is on and connected to the same network as your computer during the update process.

## Contributing

Contributions to the `Thermostat_for_Plug-in_Heater` project are welcome. Please fork this repository and submit pull requests with your improvements.


## Acknowledgments

- Thanks to the open-source community for the support and tools that made this project possible.
- Special thanks to Adafruit for providing the libraries for the ESP32, BME280 sensor, and SH1106 display.
