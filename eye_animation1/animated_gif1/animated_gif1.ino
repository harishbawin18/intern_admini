#include <SPI.h>
#include <SD.h>
#include <TFT_eSPI.h>
#include <AnimatedGIF.h>

#define SD_CS 7   // CS pin for your SD card module

TFT_eSPI tft = TFT_eSPI();
AnimatedGIF gif;
File gifFile;

// ---------- File Handlers ----------
void * GIFOpenFile(const char *fname, int32_t *pSize) {
  gifFile = SD.open(fname, FILE_READ);
  if (gifFile) { *pSize = gifFile.size(); return (void *)&gifFile; }
  return NULL;
}

void GIFCloseFile(void *pHandle) {
  File *f = (File*)pHandle;
  if (f) f->close();
}

int32_t GIFReadFile(GIFFILE *pFile, uint8_t *pBuf, int32_t len) {
  File *f = (File*)pFile->fHandle;
  if (!f) return 0;
  return f->read(pBuf, len);
}

int32_t GIFSeekFile(GIFFILE *pFile, int32_t position) {
  File *f = (File*)pFile->fHandle;
  if (!f) return 0;
  f->seek(position);
  return position;
}

// ---------- Fixed GIFDraw ----------
void GIFDraw(GIFDRAW *pDraw) {
  if (pDraw->y >= tft.height()) return;

  int16_t x = pDraw->iX;
  int16_t y = pDraw->iY;
  int16_t w = pDraw->iWidth;

  // Clip to screen
  if (x < 0) {
    w += x;
    x = 0;
  }
  if (x + w > tft.width()) {
    w = tft.width() - x;
  }
  if (w <= 0) return;

  static uint16_t lineBuffer[320]; // max screen width
  uint8_t *src = pDraw->pPixels;

  // Convert palette index -> RGB565
  for (int i = 0; i < w; i++) {
    uint8_t idx = src[i];
    if (pDraw->ucHasTransparency && idx == pDraw->ucTransparent) {
      // Transparent pixel → keep background
      lineBuffer[i] = tft.readPixel(x + i, y);
    } else {
      lineBuffer[i] = pDraw->pPalette[idx];
    }
  }

  // Push line
  tft.pushImage(x, y, w, 1, lineBuffer);
}

// ---------- Setup ----------
void setup() {
  Serial.begin(115200);
  delay(1000);

  tft.init();
  tft.setRotation(1);  // Landscape
  tft.fillScreen(TFT_BLACK);
  Serial.println("TFT initialized");

  // Quick color test
  tft.fillScreen(TFT_RED);   delay(200);
  tft.fillScreen(TFT_GREEN); delay(200);
  tft.fillScreen(TFT_BLUE);  delay(200);
  tft.fillScreen(TFT_BLACK);
  Serial.println("Color test complete");

  if (!SD.begin(SD_CS)) {
    Serial.println("SD card init failed!");
    tft.setTextColor(TFT_RED);
    tft.setCursor(10, 10);
    tft.println("SD CARD FAILED");
    while (1);
  }
  Serial.println("SD card OK");

  // List files
  File root = SD.open("/");
  Serial.println("Files on SD card:");
  while (true) {
    File entry = root.openNextFile();
    if (!entry) break;
    Serial.print("- ");
    Serial.println(entry.name());
    entry.close();
  }
  root.close();

  // Try opening GIF
  gif.begin(GIF_PALETTE_RGB565_LE);   // first try LE
  if (gif.open("/video.gif", GIFOpenFile, GIFCloseFile, GIFReadFile, GIFSeekFile, GIFDraw)) {
    Serial.println("GIF animation started (LE)!");
  } else {
    Serial.println("LE failed, trying BE...");
    gif.begin(GIF_PALETTE_RGB565_BE);
    if (gif.open("/video.gif", GIFOpenFile, GIFCloseFile, GIFReadFile, GIFSeekFile, GIFDraw)) {
      Serial.println("GIF animation started (BE)!");
    } else {
      Serial.println("GIF open failed!");
      while (1);
    }
  }
}

// ---------- Loop ----------
void loop() {
  int delayMS;
  if (gif.playFrame(true, &delayMS, NULL)) {
    Serial.printf("Frame drawn, delay=%d ms\n", delayMS);
    if (delayMS < 10) delayMS = 50; // safe minimum
    delay(delayMS);
  } else {
    Serial.println("GIF finished, restarting...");
    gif.close();
    delay(200);
    gif.open("/video.gif", GIFOpenFile, GIFCloseFile, GIFReadFile, GIFSeekFile, GIFDraw);
  }
}

