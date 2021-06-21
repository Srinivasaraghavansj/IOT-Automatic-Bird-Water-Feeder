#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h> 
#include <ESP8266WiFi.h>

#define Relay1            D6
#define Relay2            D5
#define led               LED_BUILTIN
#define WLAN_SSID       "Jalabulajangs"     
#define WLAN_PASS       "aaaaaaaaaa"        
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   
#define AIO_USERNAME    "SRinivasaRaghavan"           
#define AIO_KEY         "cbc9783f3209470088774eaaf9a55029"   


WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe Light1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/flush-cleanse-and-re-fill"); // FeedName
//Adafruit_MQTT_Publish temp = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");
//Adafruit_MQTT_Publish hum = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity");
//Adafruit_MQTT_Publish wf = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/water-flow");
void MQTT_connect();

void setup() {
  Serial.begin(115200);
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(led,OUTPUT);
  digitalWrite(led,HIGH);
  Serial.println("\n");
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); } Serial.println();
  digitalWrite(led,LOW);

  Serial.println("WiFi connected\nIP address:");
  Serial.println(WiFi.localIP());
  mqtt.subscribe(&Light1);
}

void loop() {
 
  MQTT_connect();

  Adafruit_MQTT_Subscribe *subscription;
  
  while ((subscription = mqtt.readSubscription(20000))) {
    if (subscription == &Light1) {
      Serial.print("Water refresher ");
      Serial.print(F("is: "));
      Serial.println((char *)Light1.lastread);
      byte Light1_State = atoi((char *)Light1.lastread);
      digitalWrite(Relay1, !Light1_State);
    }
   
  }
   /*// Now we can publish stuff!
  uint16_t temperature = 37;
    Serial.print(F("Sending pot val "));
    Serial.print(temperature);
    Serial.print("...");
    if (! temp.publish(potAdcValue)) {
      Serial.println(F("Failed"));
    } else {
      Serial.println(F("OK!"));
      delay(2000);
    }*/
  }
   

void MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) {return;}
   digitalWrite(led,HIGH);
  Serial.print("Connecting to MQTT... ");
   uint8_t retries = 3;
  
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      //while (1);
      Serial.println("Wait 10 min to reconnect");
      delay(600000);
    }
  }
  Serial.println("MQTT Connected!");
  digitalWrite(led,LOW);
}
