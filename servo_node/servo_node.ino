#include <WiFi.h>
#include <ESP32Servo.h>
#include <WebServer.h>

const char* ssid = "Infinix HOT 30 5G";      // Wi-Fi SSID
const char* password = "harish@123";  // Wi-Fi Password

Servo myServo;
WebServer server(80); // HTTP server on port 80

void rotateServo() {
  myServo.write(90);  // Move to 90 degrees
  delay(1000);
  myServo.write(0);   // Back to 0 degrees
  server.send(200, "text/plain", "Servo rotated!");
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());

  myServo.attach(13); // Servo pin (change if needed)
  myServo.write(0);   // Initial position

  server.on("/rotate", rotateServo);
  server.begin();
}

void loop() {
  server.handleClient();
}
