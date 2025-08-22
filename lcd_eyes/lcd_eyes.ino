#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();

// Inverted color for skyblue (so it shows correctly on your screen)
#define FAKE_SKYBLUE 0x7982  

// Eye geometry
int eyeRadius   = 50;   // outer radius (skyblue eyeball)
int pupilRadius = 20;   // pupil radius (black)
int centerY     = 120;  // vertical position
int leftX       = 80;   // left eye center
int rightX      = 240;  // right eye center

void setup() {
  tft.init();
  tft.setRotation(1);

  // Background → WHITE in code → BLACK on screen
  tft.fillScreen(TFT_WHITE);

  // Draw both eyes
  drawEye(leftX, centerY);
  drawEye(rightX, centerY);
}

void drawEye(int x, int y) {
  // Outer eyeball → FAKE_SKYBLUE in code → SKYBLUE on screen
  tft.fillCircle(x, y, eyeRadius, FAKE_SKYBLUE);

  // Pupil → WHITE in code → BLACK on screen
  tft.fillCircle(x, y, pupilRadius, TFT_WHITE);
}

void loop() {
  // nothing for now
}

