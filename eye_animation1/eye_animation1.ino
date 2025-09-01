#include <TFT_eSPI.h> // Include the TFT_eSPI library
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI(); // Create TFT object

// Define colors (RGB565 format)
#define BLACK 0x0000
#define BLUE  0x07FF
#define WHITE 0xFFFF

// Global flag for inversion
bool inverted = true;  // set true if your display is inverted

// Eye positions and sizes
int eye1X = 80;   // X position of left eye
int eye1Y = 120;  // Y position of left eye
int eye1Width = 60;   // Width of left eye
int eye1Height = 80;  // Height of left eye
int eye2X = 160;  // X position of right eye
int eye2Y = 120;  // Y position of right eye
int eye2Width = 40;   // Width of right eye
int eye2Height = 60;  // Height of right eye

// Function to get correct color considering inversion
uint16_t fixColor(uint16_t c) {
  if (!inverted) return c;  // normal mode
  // Manual inversion (approximation)
  return ~c;  // bitwise NOT inverts the 16-bit RGB565 value
}

void setup() {
  tft.init();           
  tft.setRotation(1);   
  tft.fillScreen(fixColor(BLACK)); // Clear screen with proper bg
}

void drawEye(int x, int y, int w, int h, bool visible) {
  if (visible) {
    for (int i = 0; i < h; i += 5) {
      int lineY = y - h / 2 + i;
      int lineWidth = w * sqrt(1 - pow((2.0 * i / h - 1), 2)); 
      tft.drawLine(
        x - lineWidth / 2, lineY, 
        x + lineWidth / 2, lineY, 
        fixColor(BLUE)   // use fixed color
      );
    }
  } else {
    tft.fillRect(
      x - w / 2 - 2, y - h / 2 - 2, 
      w + 4, h + 4, 
      fixColor(BLACK)   // clear with fixed background
    );
  }
}

void loop() {
  // Draw both eyes
  drawEye(eye1X, eye1Y, eye1Width, eye1Height, true);
  drawEye(eye2X, eye2Y, eye2Width, eye2Height, true);
  delay(1000);

  // Hide both eyes
  drawEye(eye1X, eye1Y, eye1Width, eye1Height, false);
  drawEye(eye2X, eye2Y, eye2Width, eye2Height, false);
  delay(500);
}
