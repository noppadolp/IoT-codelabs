#include <WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "IEE-IoT";
const char *password = "ieenopassword";

// Broadcast port
const uint16_t udpPort = 10000;
char packetBuffer[255]; // buffer to hold incoming packet

WiFiUDP udp;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  // Beginning UDP!
  udp.begin(udpPort);
}

void loop() {
  udp.beginPacket(udpDestination, udpPort);
  uint16_t packetSize = udp.parsePacket();

  if(packetSize){
    Serial.print("Recieved packet of size: ");
    Serial.println(packetSize);
    Serial.print("From: ");
    IPAddress remoteIp = udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port: ");
    Serial.println(udp.remotePort());

    uint16_t len = udp.read(packetBuffer, 255);
    if(len > 0){
      packetBuffer[len] = 0; 
      // Assign ending of the packet -> \0 for c string
    }
    Serial.println("Contents: ");
    Serial.println(packetBuffer);

    //Send a reply to the IP address and port that sent us the packet
    udp.beginPacket(udp.remoteIP(), udp.remotePort());
//    char *replyBuffer = "Ok, I got it\n";
    // this line below use to echo data back to sender
    char *replyBuffer = strdup(packetBuffer);
    udp.print(replyBuffer);
    udp.endPacket();
  }
}
