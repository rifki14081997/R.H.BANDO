#include <Arduino.h>
#include <string.h>
// Device libraries (Arduino ESP32/ESP8266 Cores)
#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <WiFiClientSecure.h> // jika menggunakan ESP32
// Custom libraries
#include <utlgbotlib.h>
#include <UniversalTelegramBot.h> 
#include <Adafruit_Sensor.h>
#include "CTBot.h"
#include "DHT.h"
#define DHTPIN 15
#define DHTTYPE DHT22
CTBot myBot ; 
DHT dht(DHTPIN, DHTTYPE);
unsigned long currentTime = millis();
unsigned long BOT_MTBS = 1000; 
unsigned long bot_lasttime; 
float h, t;

#define WIFI_SSID "panici"
#define WIFI_PASS "14081997"

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

#define BOTtoken "5470158496:AAFBOFWs_0ACwPIDnWPE5fWKG9e1Z0ixMFY"  
#define CHAT_ID "5190726928"
#define DEBUG_LEVEL_UTLGBOT 0
//MQ-02
#define Sensor_Gas 26
int sensorValue;
float ratio; 
//esp32
const int ledPin = 2;
bool ledState = LOW;
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
//buzzer
int buz = 12;
int buzer = 13;
int buzzer = 14;
// Checks for new messages every 1 second.
boolean state_t, state_h;
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

//FLAME sensor
#define Pin_Api 25
 unsigned long previousMillis = 0; // millis sebagai pengganti delay 
 const long interval = 3000 ;//  3 detik

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "SELAMAT DATANG BOSKU!!, " + from_name + ".\n";
      welcome += "Gunakan perintah berikut untuk mengontrol output Anda.\n\n";
      welcome += "/led_on  MENGAKTIFKAN GPIO ON \n";
      welcome += "/led_off MENGAKTIFKAN GPIO OFF \n";
      welcome += "/Flame_Sensor Tampilkan Status Flame Sensor (Sensor Api). \n" ;
      welcome += "/Sensor_Mq02 Tampilkan Status Sensor MQ-02 (Sensor Asap dan Gas). \n" ;
      welcome += "/status_suhu Tampilkan status suhu aktual.\n" ;
      welcome += "/status_kelembaban Tampilkan status Kelembaban yang sebenarnya. \n";
      welcome += "/state  Status GPIO Saat Ini. \n";
      bot.sendMessage(chat_id, welcome, "");
      }
 if (text == "/Sensor_Mq02")
      {   String msg = "Status sensor MQ-02 saat ini adalah :  ";
          msg += msg.concat(ratio );
          msg += " PPM\n";
          bot.sendMessage(chat_id,msg, "");
    }
 if (text == "/status_suhu")
      {   String msg = "Suhu saat ini adalah :  ";
          msg += msg.concat(t);
          msg += " °C\n";
          bot.sendMessage(chat_id,msg, "");
      }
 if (text == "/status_kelembaban")
      {  
          String msg = "Kelembaban Saat ini adalah : ";
          msg += msg.concat(h);
          msg += " %Rh\n"; //Relatif Humadity 
          bot.sendMessage(chat_id,msg, ""); 
      }
 if (text == "/Flame_Sensor")
      {   String msg = "Status Flame Sensor saat ini adalah :  ";
          msg += msg.concat(Pin_Api);
          msg += " \n";
          bot.sendMessage(chat_id,msg, "");
      }
    
 if (text == "/led_on") {
      bot.sendMessage(chat_id, "LED state set to ON", "");
      ledState = HIGH;
      digitalWrite(ledPin, ledState);
    }
    
 if (text == "/led_off") {
      bot.sendMessage(chat_id, "LED state set to OFF", "");
      ledState = LOW;
      digitalWrite(ledPin, ledState);
    }
    
 if (text == "/state") {
 if (digitalRead(ledPin)){
        bot.sendMessage(chat_id, "LED is ON", "");
      }
 else{
        bot.sendMessage(chat_id, "LED is OFF", "");
      }
    }

  }
}

void setup(void)
{
  pinMode (buz, OUTPUT);
  pinMode (buzer, OUTPUT);
  pinMode (buzzer, OUTPUT);
  myBot.wifiConnect (WIFI_SSID, WIFI_PASS);
  myBot.setTelegramToken (BOTtoken) ; 
  pinMode (Sensor_Gas, INPUT);
pinMode(Pin_Api, INPUT);
  dht.begin();
 #ifdef ESP8266
    configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
    client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  #endif

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
  
  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID , WIFI_PASS);
  #ifdef ESP32
    client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  #endif
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

}

void loop()
{
  unsigned long currentMillis = millis (); //uji millis agar membaca setiap 3 detik 
//(baca millis pada waktu saat ini)
if (currentMillis - previousMillis >= interval)
  
   {

  TBMessage msg ;

   { 
{

  if (t > 37 && state_t == 0){
    state_t = 1;
    String msg = "Status Suhu :";
    msg += t;
    msg += " °C\n";
    msg += "Hati Hati Panas.";
digitalWrite (buz,HIGH);
digitalWrite (buzer,HIGH);
delay (500);
digitalWrite(buz,LOW);
delay(3000);
digitalWrite (buz,HIGH);
digitalWrite (buzer,HIGH);
delay (500);
digitalWrite(buz,LOW);
delay(1000);
digitalWrite (buz,HIGH);
digitalWrite (buzer,HIGH);
delay (200);
digitalWrite(buz,LOW);
delay(4000);
     myBot.sendMessage (5190726928,"Hati Hati Panas.");
   millis ();
  }
 
   else if (t < 37 && state_t == 0) { 
    state_t = 0;
     String msg = "Status Suhu :";
    msg += t;
    msg += " °C\n";
    msg += "Suhu Normal";
   digitalWrite (buz,LOW);
digitalWrite (buzer,LOW); ; //send tone
    delay (500);
  }

  if (h > 90 && state_h == 0) {
    state_h = 1;
    String msg = "Status Kelembaban :";
    msg += h;
    msg += " %Rh\n";
digitalWrite (buz,HIGH);
digitalWrite (buzer,HIGH);
delay (500);
digitalWrite(buz,LOW);
delay(3000);
digitalWrite (buz,HIGH);
digitalWrite (buzer,HIGH);
delay (500);
digitalWrite(buz,LOW);
delay(1000);
digitalWrite (buz,HIGH);
digitalWrite (buzer,HIGH);
delay (200);
digitalWrite(buz,LOW);
delay(4000);
  myBot.sendMessage (5190726928,"Hati Hati Kering.");
millis ();
  }
  else if (h < 90 && state_h == 0) {
    state_h = 0;
     String msg = "Status Kelembaban :";
    msg += h;
    msg += " %Rh\n";
    digitalWrite (buz, LOW) ;
    digitalWrite (buzer, LOW);
    delay(1000);
 }
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F(" %  Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));
  delay(1000);

   }
   }
{
  {
 sensorValue = analogRead(Sensor_Gas);
 ratio = sensorValue/3.7;
  Serial.print("Kondisi Sensor : ");
  Serial.print(ratio);
   if (ratio > 100) {
    Serial.println("Gas terdeteksi");
digitalWrite (buz,HIGH);
digitalWrite (buzer,HIGH);
delay (500);
digitalWrite(buz,LOW);
delay(3000);
digitalWrite (buz,HIGH);
digitalWrite (buzer,HIGH);
delay (500);
digitalWrite(buz,LOW);
delay(1000);
digitalWrite (buz,HIGH);
digitalWrite (buzer,HIGH);
delay (200);
digitalWrite(buz,LOW);
delay(4000);
    myBot.sendMessage (5190726928,"Gas Terdeteksi");
  }
  else {
     digitalWrite (buz, LOW) ;
     digitalWrite (buzer,LOW);
    Serial.println("Gas Tidak Terdeteksi");
  }
  delay (500);
}
}
{
//sensor api
 previousMillis = currentMillis ;
int sensor_api = digitalRead (Pin_Api);
 Serial.print("Kondisi : ");
 Serial.print (sensor_api);
 if (sensor_api < 1)
 {
    myBot.sendMessage (5190726928, "Ada Api Periksa Ruangan");

digitalWrite (buz,HIGH);
digitalWrite (buzer,HIGH);
delay (500);
digitalWrite(buz,LOW);
delay(3000);
digitalWrite (buz,HIGH);
digitalWrite (buzer,HIGH);
delay (500);
digitalWrite(buz,LOW);
delay(1000);
digitalWrite (buz,HIGH);
digitalWrite (buzer,HIGH);
delay (200);
digitalWrite(buz,LOW);
delay(4000);
 } 
 else 
 {
    digitalWrite (buz, LOW) ;
    digitalWrite (buzer, LOW) ; //send tone
    delay(1000);
 }
}


}


  if (millis() > lastTimeBotRan + botRequestDelay) 
   {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }

 h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  t  = dht.readTemperature();
if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
}
}
// Init WiFi interface
void wifi_init_stat(void)
{
  Serial.println("Initializing TCP-IP adapter...");
  Serial.print("Wifi connecting to SSID: ");
  Serial.println(WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  Serial.println("TCP-IP adapter successfuly initialized.");
}

bool wifi_handle_connection(void)
{
  static bool wifi_connected = false;

  // Device is not connected
  if (WiFi.status() != WL_CONNECTED)
  {
    // Was connected
    if (wifi_connected)
    {
      Serial.println("WiFi disconnected.");
      wifi_connected = false;
    }

    return false;
  }
  // Device connected
  else
  {
    // Wasn't connected
    if (!wifi_connected)
    {
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());

      wifi_connected = true;
    }

    return true;
  }
}
