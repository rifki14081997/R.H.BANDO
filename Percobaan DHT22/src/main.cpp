#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 22    
#define DHTTYPE    DHT22    

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  // Initialize device.
  dht.begin();
Serial.println(F("Rifki Hidayatullah"));
 Serial.println(F("DHT22 TES"));
}

void loop (){
  delay (2000) ;
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature (true);

  if (isnan(h)|| isnan(t)||isnan(f)) {

    Serial.println(F("DHT22 TIDAK TERDETEKSI !!"));
    return;
  }


  float hif = dht.computeHeatIndex(f,h);
  float hic = dht.computeHeatIndex (t,h,false);

  Serial.print(F("KELEMBABAN: "));
  Serial.print(h);
  Serial.println(F("%"));
Serial.print(F("SUHU: "));
  Serial.print (t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.println(F("°F"));
   Serial.print(F("INDEKS PANAS: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));

  
}
  