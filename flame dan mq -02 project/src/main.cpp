#include <Arduino.h>
#define RL 1
float Analog_value=0;
float VRL=0;
float Rs=0;
float Ro=0;
int Buzzer = 32;
int Gas =35;
void setup ()
{
  pinMode(Buzzer, OUTPUT);      
  pinMode(Gas, INPUT);
  Serial.begin(9600);
}
void loop ()
{
  Analog_value=analogRead(35);
  VRL=Analog_value*(3.3/4095.0);
  Rs=((3.3/VRL)-1)*RL;
  Ro= Rs/9.6;
  Serial.println("RIFKI HIDAYATULLAH");
  Serial.print("Ro di udara bersih =");
  Serial.println(Ro);
  delay (1000);
    if (Ro > 100) {
    Serial.println("Gas");
    digitalWrite (Buzzer, HIGH) ; //send tone
    delay(1000);
    digitalWrite (Buzzer, LOW) ;  //no tone
  }
  else {
    Serial.println("No Gas");
  }
  delay(100);
}
