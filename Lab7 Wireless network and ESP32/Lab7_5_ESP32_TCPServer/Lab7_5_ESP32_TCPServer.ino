#include <WiFi.h>
#define SERVER_PORT 9999

const char* ssid = "IEE-IoT";
const char* password = "ieenopassword";

WiFiServer my_server(SERVER_PORT);

void setup() {
  Serial.begin(115200);
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
        uint8_t data = my_client.read();
        Serial.write(data);
        my_client.write(data);
      }
      if(my_server.hasClient()){
        Serial.println("Client disconnected");
        return;
      }
    }
  } 
}
