#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp280;
void setup() {
  Serial.begin(115200);
  while(!bmp280.begin(0x76)){
    Serial.println("BMP280 sensor not found, check pin and wiring!");
    delay(500);
  }
  Serial.println("BMP280 sensor connected");
  // bmp280 sampling rate and filter setting //
  bmp280.setSampling(Adafruit_BMP280::MODE_NORMAL,
                     Adafruit_BMP280::SAMPLING_X2,
                     Adafruit_BMP280::SAMPLING_X16,
                     Adafruit_BMP280::FILTER_X16,
                     Adafruit_BMP280::STANDBY_MS_500);
}

uint32_t time_ = millis();

void loop() {
  if(millis() - time_ > 1000){
    float temperature = bmp280.readTemperature();
    float pressure = bmp280.readPressure();
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" degC");
    Serial.print("Pressure: ");
    Serial.print(pressure);
    Serial.println(" Pa");
    time_ = millis();
  }
}
