#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

static const int RXPin = 3, TXPin = 2;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

void setup() {
  Serial.begin(9600);
  ss.begin(GPSBaud);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  display.clearDisplay();
}

void loop() {
  while (ss.available() > 0) {
    if (gps.encode(ss.read())) {
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);

      if (gps.location.isValid()) {
        display.print(F("Latitude:"));
        display.println(gps.location.lat(), 6);
        display.print(F("Longitude:"));
        display.println(gps.location.lng(), 6);
      } else {
        display.println(F("INVALID LOCATION"));
      }

      display.println(F("Date/Time:"));
      if (gps.date.isValid()) {
        display.print(gps.date.month());
        display.print(F("/"));
        display.print(gps.date.day());
        display.print(F("/"));
        display.print(gps.date.year());
        display.print(F(" "));

        if (gps.time.isValid()) {
          display.print(gps.time.hour());
          display.print(F(":"));
          display.print(gps.time.minute());
          display.print(F(":"));
          display.print(gps.time.second());
        } else {
          display.println(F("INVALID TIME"));
        }
      } else {
        display.println(F("INVALID DATE"));
      }

      display.display();
    }
  }

  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println(F("No GPS detected: check wiring."));
    while (true)
      ;
  }
}
