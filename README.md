# Bin Status Monitoring System

## Overview
This project implements a **Bin Status Monitoring System** using an ESP32 microcontroller and an HC-SR04 ultrasonic sensor. The system measures the distance to the trash level in a bin and calculates the percentage of bin fullness. The data is sent to a server via Wi-Fi for monitoring and analysis. The system avoids sending values if the measured distance exceeds the bin height.

## Features
- **Distance Measurement:** Utilizes the HC-SR04 ultrasonic sensor to measure the distance to the trash level.
- **Bin Fullness Calculation:** Calculates the bin's fullness as a percentage relative to the bin height.
- **Wi-Fi Connectivity:** Connects to a Wi-Fi network to send data to a server.
- **Data Validation:** Ensures only valid and realistic measurements are sent to the server.
- **Error Handling:** Handles scenarios where the sensor reading exceeds the bin height or is invalid.

## Components Required
- ESP32 microcontroller
- HC-SR04 ultrasonic sensor
- Jumper wires
- Power source for ESP32

## Setup and Configuration

### Hardware Setup
1. **Connect the HC-SR04 sensor:**
   - **TRIG pin** to ESP32 GPIO 32
   - **ECHO pin** to ESP32 GPIO 33
   - VCC to 5V and GND to GND on the ESP32

### Software Setup
1. Install the Arduino IDE.
2. Add the ESP32 board to the Arduino IDE by following [this guide](https://github.com/espressif/arduino-esp32#installation-instructions).
3. Install the required libraries:
   - `WiFi.h`
   - `HTTPClient.h`
4. Update the following variables in the code:
   - `ssid` and `password`: Your Wi-Fi network credentials.
   - `serverName`: The URL of your server endpoint.
   - `binHeight`: The height of your bin in centimeters.
5. Upload the code to the ESP32 using the Arduino IDE.

## Code Explanation

### Key Variables
- `TRIG_PIN` and `ECHO_PIN`: Define the GPIO pins connected to the ultrasonic sensor.
- `binHeight`: The height of the bin, used to calculate the fullness percentage.
- `MIN_DISTANCE` and `MAX_DISTANCE`: Define the range of valid distance measurements for the sensor.

### Main Functions
- `setup()`: Initializes the ESP32 pins and connects to Wi-Fi.
- `loop()`: Measures the distance, calculates the bin fullness percentage, and sends the data to the server if valid.
- `measureDistance()`: Sends a trigger pulse to the sensor and measures the echo response time to calculate the distance.
- `sendDataToServer(float percentage)`: Sends the calculated percentage to the server as a JSON payload using an HTTP PATCH request.

### Example Output
1. **Valid Distance:**
    ```
    Distance: 37.08 cm, Bin Status: 25.85% full
    HTTP Response code: 200
    Response: {"id":1,"latitude":48.8988897,"longitude":2.3900410217200765,"status":25,"lastUpdated":"2025-01-09T21:42:08.7816478","sensorData":"{\"binStatusPercentage\":25.85}","locationName":"Paris"}
    ```
2. **Distance Exceeds Bin Height:**
    ```
    Distance exceeds bin height. Value will not be sent.
    ```
3. **Invalid Distance:**
    ```
    Invalid distance reading!
    ```

## Deployment
1. Power the ESP32 using a USB cable or battery.
2. Ensure the Wi-Fi network is active and reachable by the ESP32.
3. Monitor the output in the Serial Monitor for debugging or use the server to view real-time data.

## Troubleshooting
- **Wi-Fi Connection Issues:**
  - Ensure the Wi-Fi credentials are correct.
  - Check if the ESP32 is within range of the Wi-Fi network.
- **Incorrect Distance Measurements:**
  - Verify the sensor connections.
  - Ensure the sensor is not obstructed or placed incorrectly.
- **Server Communication Errors:**
  - Check the server URL and ensure it is accessible.
  - Verify the server supports HTTP PATCH requests and handles the JSON payload format.

## Future Enhancements
- Add MQTT protocol for real-time communication.
- Integrate a mobile app for monitoring bin status.
- Use a more robust distance sensor for higher accuracy.
- Implement power-saving modes for battery efficiency.

## License
This project is open-source and available under the MIT License.
