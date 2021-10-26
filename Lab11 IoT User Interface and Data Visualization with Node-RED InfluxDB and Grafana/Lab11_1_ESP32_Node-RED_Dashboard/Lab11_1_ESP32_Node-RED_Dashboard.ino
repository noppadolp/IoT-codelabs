#include <PubSubClient.h>
#include <WiFi.h>
#include <Adafruit_HTS221.h>
#include <Adafruit_BMP280.h>

#define LED_PIN 2

Adafruit_HTS221 hts221;
Adafruit_BMP280 bmp280;

const char* ssid ="IEE-IoT";// your ssid
const char* password = "ieenopassword"; // your password

const char* mqtt_server = "192.168.1.100"; // your mqtt server IP 
const char* mqtt_username = ""; // your mqtt username
const char* mqtt_password = ""; // your mqtt password
const uint16_t mqtt_port = 1883;

WiFiClient tcpClient;// create tcp client object
PubSubClient mqttClient(tcpClient); // pass tcp client to mqtt client

// Variable field //
uint32_t lastTimestamp = millis();
sensors_event_t temp, humidity;
// Variable field //

void setupHardware(){
  while(!hts221.begin_I2C(0x5F)){
    Serial.println("HTS221 sensor not found!");
    delay(500);
  }
  Serial.println("HTS221 connected");
  while(!bmp280.begin(0x76)){
    Serial.println("BMP280 sensor not found!");
    delay(500);
  }
  Serial.println("BMP280 connected");
}

void setup_wifi(const char* ssid, const char* password){
  Serial.println("Connecting to: ");
  Serial.println(ssid);
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\r\nWiFi Connected");
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());
}

void reconnect(){
  while(!mqttClient.connected()){
    Serial.println("Attemping MQTT Connection !");
    String clientID = "ESP32Client-" + String(WiFi.macAddress());
    Serial.print("ClientID : ");
    Serial.println(clientID);
    if (mqttClient.connect(clientID.c_str(),mqtt_username,mqtt_password)){
      Serial.println("MQTT Broker Connected");
      String msg = "Hello from " + clientID;
      mqttClient.publish("ESP32/heartbeat",msg.c_str());
      mqttClient.subscribe("ESP32/command");
    }
    else{
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println("try again in 1 seconds");
      delay(1000);
    }
  }
}

void callBack(char* topic, byte* payload, uint16_t payload_length){
  String msg;
  String topic_str(topic);
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] :");
  for (uint16_t i = 0; i < payload_length; i++ ){
    char c = (char)payload[i];
    msg += c;
  }
  Serial.println(msg);

  if(topic_str == "ESP32/command"){
    uint8_t duty = uint8_t(msg.toInt());
    Serial.printf("Recieved command duty: %d\r\n",duty);
    ledcWrite(0, duty);
  }
}

void setup() {
  Serial.begin(115200);
  ledcSetup(0,5000,8);
  ledcAttachPin(LED_PIN,0);
  setupHardware();
  setup_wifi(ssid,password);
  mqttClient.setServer(mqtt_server,mqtt_port);
  mqttClient.setCallback(callBack);
}

void loop() {
  if(!mqttClient.connected()){
    reconnect();
  }
  mqttClient.loop();

  if(millis() - lastTimestamp > 1000){
    String msg("Timestamp from ESP32 [");
    msg += String(millis()) += String("]");
    Serial.print("Publish message: ");
    Serial.println(msg);
    mqttClient.publish("ESP32/heartbeat",msg.c_str());
    
    // Reading sensor data //
    hts221.getEvent(&humidity, &temp);
    String hts221_temperature(temp.temperature);
    String hts221_humidity(humidity.relative_humidity);
    String bmp280_temperature(bmp280.readTemperature());
    String bmp280_pressure(bmp280.readPressure());
    // Reading sensor data //

    Serial.println("HTS221 data: ");
    Serial.print("Temperature: ");
    Serial.println(hts221_temperature);
    Serial.print("Humidity: ");
    Serial.println(hts221_humidity);

    Serial.println("BMP280 data: ");
    Serial.print("Temperature: ");
    Serial.println(bmp280_temperature);
    Serial.print("Pressure: ");
    Serial.println(bmp280_pressure);

    mqttClient.publish("HTS221/Temperature", hts221_temperature.c_str());
    mqttClient.publish("HTS221/Humidity", hts221_humidity.c_str());
    mqttClient.publish("BMP280/Temperature", bmp280_temperature.c_str());
    mqttClient.publish("BMP280/Pressure", bmp280_pressure.c_str());
    
    lastTimestamp = millis();
  }
}
