#include "UbidotsESPMQTT.h"
#include "DHT.h"
#define TOKEN "xxxx" 
#define WIFINAME "xxxx" 
#define WIFIPASS "xxxx"
#define DEVICE_LABEL "esp8266";

#define DHTPIN D1 
#define RELAY D3    
#define DHTTYPE DHT11   

Ubidots ubiClient(TOKEN);

WiFiClient client;

DHT dht(DHTPIN, DHTTYPE);


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i
Serial.print((char)payload[i]);
}

  if ((char)payload[0]=='1'){

digitalWrite(RELAY, HIGH);

  }

  else{

digitalWrite(RELAY, LOW);

  }

  Serial.println();

}


void setup() {


  ubiClient.ubidotsSetBroker("app.ubidots.com"); 

  ubiClient.setDebug(true); 

  Serial.begin(115200);

  pinMode(RELAY, OUTPUT);

  dht.begin();

  ubiClient.wifiConnection(WIFINAME, WIFIPASS);

  ubiClient.begin(callback);

  if(!ubiClient.connected()){

  ubiClient.reconnect();

  }

  char* deviceStatus = getUbidotsDevice(DEVICE_LABEL);

  if (strcmp(deviceStatus, "404") == 0) {

ubiClient.add("light", 0); 

ubiClient.ubidotsPublish(DEVICE_LABEL);

ubiClient.loop();

  }

  ubiClient.ubidotsSubscribe(DEVICE_LABEL,"light"); 

}

void loop() {

  if(!ubiClient.connected()){

  ubiClient.reconnect();

  ubiClient.ubidotsSubscribe(DEVICE_LABEL,"light"); //Insert the Device and Variable's Labels

  }

  // Read humidity

  float humValue = dht.readHumidity();

  // Read temperature as Celsius (the default)

  float tempCValue = dht.readTemperature();

  // Read temperature as Fahrenheit (isFahrenheit = true)

  float tempFValue = dht.readTemperature(true);

  ubiClient.add("temperature-c", tempCValue); //Insert your variable Labels and the value to be sent

  ubiClient.ubidotsPublish(DEVICE_LABEL);

  ubiClient.add("temperature-f", tempFValue); //Insert your variable Labels and the value to be sent

  ubiClient.ubidotsPublish(DEVICE_LABEL);

  ubiClient.add("humidity", humValue); //Insert your variable Labels and the value to be sent

  ubiClient.ubidotsPublish(DEVICE_LABEL);

  ubiClient.loop();

 

  delay(1000);

}

char* getUbidotsDevice(char* deviceLabel) {

  char* data = (char *) malloc(sizeof(char) * 700);

  char* response = (char *) malloc(sizeof(char) * 400);

  sprintf(data, "GET /api/v1.6/devices/%s/", deviceLabel);

  sprintf(data, "%s HTTP/1.1\r\n", data);

  sprintf(data, "%sHost: industrial.api.ubidots.com\r\nUser-Agent:esp8266/1.0\r\n", data);

  sprintf(data, "%sX-Auth-Token: %s\r\nConnection: close\r\n\r\n", data, TOKEN);

  if (client.connect("industrial.api.ubidots.com", 80)) {

client.println(data);

  } else {

return "e";

  }

  free(data);

  int timeout = 0;

  while(!client.available() && timeout < 5000) {

timeout++;

if (timeout >= 4999){

    return "e";

}

delay(1);

  }

  int i = 0;

  while (client.available()) {

response[i++] = (char)client.read();

if (i >= 399){

  break;

}

  }

  char * pch;

  char * statusCode;

  int j = 0;

  pch = strtok (response, " ");

  while (pch != NULL) {

if (j == 1 ) {

  statusCode = pch;

}

pch = strtok (NULL, " ");

j++;

  }

  free(response);

  return statusCode;

}
