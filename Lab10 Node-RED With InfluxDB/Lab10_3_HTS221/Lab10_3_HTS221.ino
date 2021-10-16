#include <Adafruit_HTS221.h>

Adafruit_HTS221 hts221; // create hts221 object

void setup() {
  Serial.begin(115200);
  while(!hts221.begin_I2C(0x5F)){ // start connection with hts221 sensor
    Serial.println("HTS221 sensor not found");
    delay(500);
  }
  Serial.println("HTS221 sensor connected");
  hts221.setDataRate(HTS221_RATE_1_HZ); // set hts211 datarate
}

uint32_t time_ = millis();
sensors_event_t temp, humidity;

void loop() {
  if(millis() - time_ > 1000){
    hts221.getEvent(&humidity, &temp);
    Serial.print("Temperature: ");
    Serial.println(temp.temperature);
    Serial.print("Humidity: ");
    Serial.println(humidity.relative_humidity);
    time_ = millis();   
  }
}
