#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

const int SCREEN_W = 320;
const int SCREEN_H = 240;

struct Eye {
  int xc, yc;        // center coordinates
  int rx, ry;        // eye radii (horizontal/vertical)
  int irisR;         // iris radius
  int pupilR;        // pupil radius
};

Eye leftEye, rightEye;

void drawEye(const Eye &eye, int eyelid) {
  // eyelid: 0=open, up to ry=closed
  // Fill eye background (sclera)
  tft.fillEllipse(eye.xc, eye.yc, eye.rx, eye.ry, TFT_BLACK);
  tft.drawEllipse(eye.xc, eye.yc, eye.rx, eye.ry, TFT_DARKGREY);

  // Iris with glowing gradient effect
  for (int r = eye.irisR; r > 0; r -= 3) {
    uint8_t val = map(r, 0, eye.irisR, 120, 255);
    uint16_t color = tft.color565(0, val / 2, val); // Blue-green glow
    tft.fillCircle(eye.xc, eye.yc, r, color);
  }

  // Pupil (black)
  tft.fillCircle(eye.xc, eye.yc, eye.pupilR, TFT_BLACK);

  // Specular highlight
  tft.fillCircle(eye.xc - eye.pupilR/2, eye.yc - eye.pupilR/2, max(3, eye.pupilR/5), TFT_WHITE);

  // Eyelids: black bars closing from top and bottom around center
  if (eyelid > 0) {
    int topCover = eye.yc - eyelid;
    if (topCover > 0) tft.fillRect(eye.xc - eye.rx, 0, 2*eye.rx, topCover, TFT_BLACK);
    int bottomCover = eye.yc + eyelid;
    if (bottomCover < SCREEN_H) tft.fillRect(eye.xc - eye.rx, bottomCover, 2*eye.rx, SCREEN_H - bottomCover, TFT_BLACK);
  }
}

void drawPattern() {
  // Draw some cool futuristic circles and lines background pattern
  tft.drawCircle(SCREEN_W/4, SCREEN_H/2, 50, TFT_DARKCYAN);
  tft.drawCircle(3*SCREEN_W/4, SCREEN_H/2, 50, TFT_DARKCYAN);
  for(int i = 10; i < SCREEN_W/2; i += 15) {
    tft.drawLine(SCREEN_W/4 - i, SCREEN_H/2 - 60, SCREEN_W/4 - i, SCREEN_H/2 + 60, TFT_DARKGREY);
    tft.drawLine(3*SCREEN_W/4 + i, SCREEN_H/2 - 60, 3*SCREEN_W/4 + i, SCREEN_H/2 + 60, TFT_DARKGREY);
  }
}

void setup() {
  tft.begin();
  tft.setRotation(1);  // Landscape mode (320x240)
  tft.fillScreen(TFT_BLACK);

  // Configure eyes
  leftEye.xc = SCREEN_W / 4;
  leftEye.yc = SCREEN_H / 2;
  leftEye.rx = 55;
  leftEye.ry = 40;
  leftEye.irisR = 35;
  leftEye.pupilR = 18;

  rightEye.xc = 3 * SCREEN_W / 4;
  rightEye.yc = SCREEN_H / 2;
  rightEye.rx = 55;
  rightEye.ry = 40;
  rightEye.irisR = 35;
  rightEye.pupilR = 18;
}

void loop() {
  drawPattern();

  // Blink cycle: eyelids close and open from 0 to max (~ry)
  for (int e = 0; e <= leftEye.ry; e += 3) {
    drawEye(leftEye, e);
    drawEye(rightEye, e);
    delay(20);
  }
  delay(500);
  for (int e = leftEye.ry; e >= 0; e -= 3) {
    drawEye(leftEye, e);
    drawEye(rightEye, e);
    delay(20);
  }
  delay(1500);
}
