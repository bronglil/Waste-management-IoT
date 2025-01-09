#include <WiFi.h>
#include <HTTPClient.h>

// Pin definitions for HC-SR04
#define TRIG_PIN 32
#define ECHO_PIN 33

// Wi-Fi credentials
const char* ssid = "Irffy";
const char* password = "12345678";

// Server details
const char* serverName = "http://192.168.112.113:8080/api/bins/1";

// Constants for distance measurement
const float SPEED_OF_SOUND = 0.034;  // cm per microsecond
const float MAX_DISTANCE = 400.0;    // cm
const float MIN_DISTANCE = 2.0;      // cm
const float binHeight = 50.0;        // Height of the bin in cm (set as per your requirement)

void setup() {
  Serial.begin(115200);
  
  // Initialize pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);  // Ensure trigger is LOW initially
  
  connectToWiFi();
}

void loop() {
  float distance = measureDistance();
  
  if (distance >= MIN_DISTANCE && distance <= binHeight) {
    float percentage = ((binHeight - distance) / binHeight) * 100;
    Serial.printf("Distance: %.2f cm, Bin Status: %.2f%% full\n", distance, percentage);
    sendDataToServer(percentage);
  } else if (distance > binHeight) {
    Serial.println("Distance exceeds bin height. Value will not be sent.");
  } else {
    Serial.println("Invalid distance reading!");
  }
  
  delay(10000);
}

void connectToWiFi() {
  Serial.print("Connecting to Wi-Fi");
  
  WiFi.begin(ssid, password);
  
  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 20000) {
    delay(500);
    Serial.print(".");
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("\nConnected to Wi-Fi! IP: %s\n", WiFi.localIP().toString().c_str());
  } else {
    Serial.println("\nFailed to connect to Wi-Fi!");
  }
}

float measureDistance() {
  // Clear the trigger pin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  // Send the pulse
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Measure the response
  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout
  
  if (duration == 0) {
    return -1;  // Indicate timeout/error
  }
  
  return (duration * SPEED_OF_SOUND) / 2.0;
}

void sendDataToServer(float percentage) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi disconnected. Reconnecting...");
    connectToWiFi();
    if (WiFi.status() != WL_CONNECTED) {
      return;  // Exit if reconnection failed
    }
  }
  
  HTTPClient http;
  
  // Create JSON payload
  String payload = "{\"sensorData\":{\"binStatusPercentage\":" + String(percentage, 2) + "},\"status\":" + String(static_cast<int>(percentage)) + "}";

  http.begin(serverName);
  http.addHeader("Content-Type", "application/json");
  
  int httpResponseCode = http.PATCH(payload);
  
  if (httpResponseCode > 0) {
    Serial.printf("HTTP Response code: %d\n", httpResponseCode);
    String response = http.getString();
    Serial.println("Response: " + response);
  } else {
    Serial.printf("Error in HTTP request: %d\n", httpResponseCode);
  }
  
  http.end();
}
