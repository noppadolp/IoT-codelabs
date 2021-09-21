#include <WiFi.h>
#define SERVER_PORT 8888

// TCP server ip address
// Change it to be your ipaddress
IPAddress server_ip(192,168,1,100); 
const char* ssid     = "IEE-IoT";
const char* password = "ieenopassword";

WiFiClient my_client;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid,password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());
  while(!my_client.connect(server_ip, SERVER_PORT)){
    Serial.print(".");
    delay(100);
  }
  Serial.println("\r\nServer connection establish");
}

void loop() {
  while(my_client.available()){
    uint8_t data = my_client.read();
    Serial.write(data);
    my_client.write(data);
  }
}
