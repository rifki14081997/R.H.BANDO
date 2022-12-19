#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <WiFi.h>
#include <utlgbotlib.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include "DHT.h"
#define DHTPIN 15
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
float h, t;

#define DEBUG_LEVEL_UTLGBOT 0

// Initialize Wifi connection to the router
char ssid[] = "panici";     // your network SSID (name)
char password[] = "14081997"; // your network key

// Initialize Telegram BOT
#define BOTtoken "5568609100:AAEsUOZfOx4u9G2Y9kpPYvPLjL_YRfC3I3E"  // your Bot Token (Get from Botfather)
#define chatid "5190726928"

uTLGBot Bot (BOTtoken);

const char TEXT_START[] =
  "Hallo, aku adalah bot yang akan membantu kamu.\n"
  "\n"
  "Ketik /help untuk melihat komen yang tersedia.";

char buff[100];
boolean state_t, state_h;

const char TEXT_HELP[] =
  "Komen yang tersedia:\n"
  "\n"
  "/statussuhu - Show actual temperature status.\n"
  "/statuskelembaban - Show actual Humidity status.\n";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dht.begin();

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Bot.getMe();
}

void loop() {
// Check for Bot received messages
  while (Bot.getUpdates())
  {
    Serial.println("Received message:");
    Serial.println(Bot.received_msg.text);
    Serial.println(Bot.received_msg.chat.id);

    if (strncmp(Bot.received_msg.text, "/start", strlen("/start")) == 0)
    {
      Bot.sendMessage(Bot.received_msg.chat.id, TEXT_START);
    }

    else if (strncmp(Bot.received_msg.text, "/help", strlen("/help")) == 0)
    {
      Bot.sendMessage(Bot.received_msg.chat.id, TEXT_HELP);
    }
    else if (strncmp(Bot.received_msg.text, "/statussuhu", strlen("/statussuhu")) == 0)
    {
      t = dht.readTemperature();
      if (isnan(h) || isnan(t)) {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
      }
      String msg = "Status Suhu :";
      msg += t;
      msg += " °C\n";

      msg.toCharArray(buff, 100);
      Bot.sendMessage(Bot.received_msg.chat.id, buff);
    }
    else if (strncmp(Bot.received_msg.text, "/statuskelembaban", strlen("/statuskelembaban")) == 0)
    {
      h = dht.readHumidity();
      if (isnan(h) || isnan(t)) {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
      }
      String msg = "Status Kelembaban :";
      msg += h;
      msg += " %Rh\n";

      msg.toCharArray(buff, 100);
      Bot.sendMessage(Bot.received_msg.chat.id, buff);
   }
  }
}