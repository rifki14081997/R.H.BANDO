#include <Arduino.h>
#include <WiFi.h>
#include "ESPAsyncWebServer.h"
const char* ssid = "panici";
const char* password = "14081997";
WiFiServer server(80);
String header;
String Lamp1State = "off";
String Lamp2State = "off";
const int Lamp1 = 26;
const int Lamp2 = 27;
unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;
void setup() {
  Serial.begin(115200);
  pinMode(Lamp1, OUTPUT);
  pinMode(Lamp2, OUTPUT);
  digitalWrite(Lamp1, LOW);
  digitalWrite(Lamp2, LOW);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}
void loop(){
  WiFiClient client = server.available();   
  if (client) {                           
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          
    String currentLine = "";                
    while (client.connected() && currentTime - previousTime <= timeoutTime) { 
      currentTime = millis();
      if (client.available()) {            
        char c = client.read();           
        Serial.write(c);                
        header += c;
        if (c == '\n') {                   
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("GPIO 26 on");
              Lamp1State = "on";
              digitalWrite(Lamp1, HIGH);
            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");
              Lamp1State = "off";
              digitalWrite(Lamp1, LOW);
            } else if (header.indexOf("GET /27/on") >= 0) {
              Serial.println("GPIO 27 on");
              Lamp2State = "on";
              digitalWrite(Lamp2, HIGH);
            } else if (header.indexOf("GET /27/off") >= 0) {
              Serial.println("GPIO 27 off");
              Lamp2State = "off";
              digitalWrite(Lamp2, LOW);
            }
            
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #3DE31B; border: none; color: black; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #DB240F;}</style></head>");
            
            client.println("<body><h1>RIFKI HIDAYATULLAH</h1>");
            client.println("<body><h2>ESP32 IOT Web Server Control Lamp</h2>");
            client.println("<p>Lamp 1 - Current Condition <b>" + Lamp1State + "</b></p>");
            if (Lamp1State=="off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            client.println("<p>Lamp 2 - Current Condition <b>" + Lamp2State + "</b></p>");  
            if (Lamp2State=="off") {
              client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
            client.println();
            break;
          } else { 
            currentLine = "";
          }
        } else if (c != '\r') { 
          currentLine += c;  
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}