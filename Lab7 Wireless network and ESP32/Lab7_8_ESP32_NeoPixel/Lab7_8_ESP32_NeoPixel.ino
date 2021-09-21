#include <Adafruit_NeoPixel.h>
#define NeoPixel_PIN 18
#define NeoPixel_NUMBER 1

Adafruit_NeoPixel neopixel(NeoPixel_NUMBER, NeoPixel_PIN, NEO_GRB + NEO_KHZ800);
void setup() {
  Serial.begin(115200);
  neopixel.begin();
}

uint32_t time_ = millis();
uint16_t hue = 1;

void loop() {
  if(millis() - time_ > 10 ){
     hue+=100; // hue value ranging in 0 - 65535
     Serial.println(hue);
     neopixel.setPixelColor(0, neopixel.ColorHSV(hue)); // Set neopixel ID 0 with HSV color format
//     neopixel.setPixelColor(0, neopixel.Color(r, g, b))
     neopixel.show();
     time_ = millis();
  }
}
