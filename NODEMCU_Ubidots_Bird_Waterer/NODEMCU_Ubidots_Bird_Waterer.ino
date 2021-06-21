#define waterlevelpin D7
#define waterthreshold 0
#define invalvepin D5
#define outvalvepin D6

#include "DHT.h" 
#define DHTPIN D2
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

#include "UbidotsMicroESP8266.h"
#define TOKEN  "A1E-DF9tgMDpWfZvZKXiSKr3cal5lj9Gip"  // Put here your Ubidots TOKEN
#define WIFISSID "Jalabulajangs"
#define PASSWORD "aaaaaaaaaa"

Ubidots client(TOKEN);
unsigned long lastMillis = 0;


int waterusage = 10;
int tank = 0, valve = 0;

void setup(){
    Serial.begin(115200);
    pinMode(waterlevelpin,INPUT);
    pinMode(invalvepin,OUTPUT);
    pinMode(LED_BUILTIN,OUTPUT);
    dht.begin();
    delay(10);
    client.wifiConnection(WIFISSID, PASSWORD);
}
void loop(){
    Serial.println(digitalRead(waterlevelpin));
   
    if (millis() - lastMillis > 5000) {

      float humedad = dht.readHumidity();
      float temperatura = dht.readTemperature();
      
      
       waterusage = waterusage*1.1;
            lastMillis = millis();
            client.add("Humidity", humedad);
            client.add("Temperature", temperatura);
            client.add("Valve", valve);
            client.add("Tank", tank);
            client.add("Water usage", waterusage);
            client.sendAll(true);
            
            }

            if(digitalRead(waterlevelpin) == waterthreshold){
         tank = 1;
         valve = 0;
         digitalWrite(LED_BUILTIN,LOW);
         digitalWrite(invalvepin,HIGH);
      }
      else {
         tank = 0;
         valve = 1;
         digitalWrite(LED_BUILTIN,HIGH);
         digitalWrite(invalvepin,LOW);

      }
   
}
