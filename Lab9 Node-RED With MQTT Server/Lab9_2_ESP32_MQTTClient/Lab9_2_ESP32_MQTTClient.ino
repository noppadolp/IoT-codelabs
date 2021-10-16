#include <PubSubClient.h>
#include <WiFi.h>

const char* ssid ="IEE-IoT";// your ssid
const char* password = "ieenopassword"; // your password

const char* mqtt_server = "192.168.1.100"; // your mqtt server IP 
const char* mqtt_username = ""; // your mqtt username
const char* mqtt_password = ""; // your mqtt password
const uint16_t mqtt_port = 1883;

WiFiClient tcpClient;// create tcp client object
PubSubClient mqttClient(tcpClient); // pass tcp client to mqtt client

uint32_t lastTimestamp = millis();

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

void callBack(char* topic, byte* payload, uint16_t payload_length){
  String msg;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] :");
  for (uint16_t i = 0; i < payload_length; i++ ){
    char c = (char)payload[i];
    msg += c;
  }
  Serial.println(msg);
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
      mqttClient.publish("mytopic/test",msg.c_str());
      mqttClient.subscribe("ESP32-Topic");
    }
    else{
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println("try again in 1 seconds");
      delay(1000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi(ssid,password);
  mqttClient.setServer(mqtt_server,mqtt_port);
  mqttClient.setCallback(callBack);
}

void loop() {
  if(!mqttClient.connected()){
    reconnect();
  }
  mqttClient.loop();

  if(millis() - lastTimestamp > 5000){
    String msg("Hello from ESP32 [");
    msg += String(millis()) += String("]");
    Serial.print("Publish message: ");
    Serial.println(msg);
    mqttClient.publish("mytopic/test",msg.c_str());
    lastTimestamp = millis();
  }
}
