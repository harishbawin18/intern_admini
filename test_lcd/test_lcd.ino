#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>

#define TFT_CS    10
#define TFT_DC     9
#define TFT_RST   14
#define TFT_MOSI  11
#define TFT_SCLK  12
#define TFT_MISO  13  // optional

SPIClass spi = SPIClass(FSPI);
Adafruit_ILI9341 tft = Adafruit_ILI9341(&spi, TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(115200);
  spi.begin(TFT_SCLK, TFT_MISO, TFT_MOSI);
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(3);
  tft.setCursor(40, 100);
  tft.println("Harish Bawin");
}

void loop() {
  // Nothing yet
}
