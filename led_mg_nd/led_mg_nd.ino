#include <WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <WebServer.h>

const char* ssid = "Infinix HOT 30 5G";     // Your Wi-Fi SSID
const char* password = "harish@123";        // Your Wi-Fi password

#define LED_PIN 5       // GPIO pin connected to NeoPixel data in
#define LED_COUNT 10    // Number of LEDs in your strip

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
WebServer server(80);  // HTTP server on port 80

void turnOnLeds() {
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, strip.Color(255, 0, 0)); // Red color
  }
  strip.show();
  server.send(200, "text/plain", "LEDs turned ON");
}

void turnOffLeds() {
  strip.clear();
  strip.show();
  server.send(200, "text/plain", "LEDs turned OFF");
}

void setup() {
  Serial.begin(115200);

  // Initialize NeoPixel strip
  strip.begin();
  strip.show();  // Initialize all LEDs to off

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("ESP32 IP address: ");
  Serial.println(WiFi.localIP());

  // Define HTTP server routes
  server.on("/led/on", turnOnLeds);
  server.on("/led/off", turnOffLeds);

  server.begin();
}

void loop() {
  server.handleClient();
}
