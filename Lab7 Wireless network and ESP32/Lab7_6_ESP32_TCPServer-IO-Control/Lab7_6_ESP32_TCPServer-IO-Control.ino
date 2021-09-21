#include <WiFi.h>
#define SERVER_PORT 9999
#define LED_PIN 2

const char* ssid = "IEE-IoT";
const char* password = "ieenopassword";

WiFiServer my_server(SERVER_PORT);

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN,OUTPUT);
  digitalWrite(LED_PIN,LOW);
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  my_server.begin();
  Serial.println("Server started");
}

void loop() {
  WiFiClient my_client = my_server.available();
  if(my_client){
    Serial.println("New client connected");
    while(true){
      while(my_client.available()){
        String msg = my_client.readString();
        if(msg == "ON"){
          digitalWrite(LED_PIN,LOW);
          my_client.print("\r\nOK\r\n");
          my_client.print("Set LED to ON\r\n");
        }
        else{
          digitalWrite(LED_PIN,HIGH);
          my_client.print("\r\nOK\r\n");
          my_client.print("Set LED to OFF\r\n");
        }
      }
      if(my_server.hasClient()){
        Serial.println("Client disconnected");
        return;
      }
    }
  } 
}
