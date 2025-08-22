#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>

// ==== EDIT THESE ====
const char* ssid = "Infinix HOT 30 5G";
const char* password = "harish@123";

AsyncWebServer server(80);
Servo myServo;

const int servoPin = 13; // change to your pin

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());

  myServo.attach(servoPin);

  // HTTP endpoint to rotate servo
  server.on("/rotate", HTTP_GET, [](AsyncWebServerRequest *request) {
    myServo.write(90); // rotate
    delay(1000);
    myServo.write(0);  // back to start
    request->send(200, "text/plain", "Servo rotated!");
  });

  server.begin();
}

void loop() {}
