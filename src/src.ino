#include <ESP8266HTTPClient.h>

#include <BearSSLHelpers.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureAxTLS.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiServerSecureAxTLS.h>
#include <WiFiServerSecureBearSSL.h>
#include <WiFiUdp.h>

#include <Servo.h>
#include <esp8266wifi.h>
#include <esp8266httpclient.h>

const char* ssid = "network name";
const char* pass = "network password";
char classcode[6];

Servo servo;
HTTPClient http;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, pass);
  Serial.print("Starting...");
  while (WiFi.status() != WL_CONNECTED) {

    delay(1000);
    Serial.print("Connecting..");

  }
  servo.attach(5); //D1
  servo.write(0);

  http.begin("http://scarsdale-raise-hand.herokuapp.com/api/generateId");  //Specify request destination
  int httpCode = http.GET();                                                                  //Send the request
  if (httpCode > 0) { //Check the returning code
    DynamicJsonDocument doc(2048);
    deserializeJson(doc, http.getStream());
    strncpy(classcode, doc["id"], 6);
  }
  http.end();   //Close connection  

  delay(2000);

}

void loop() {
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    http.begin("http://scarsdale-raise-hand.herokuapp.com/api/handRaised/" + classcode);  //Specify request destination
    int httpCode = http.GET();                                                                  //Send the request
    if (httpCode > 0) { //Check the returning code
      String payload = http.getString();   //Get the request response payload
      if (payload.substring(25, 26) == "f") {
        Serial.print("false");
        servo.write(0);

      } else {
        Serial.print("true");
        servo.write(90);

      }
    }
    http.end();   //Close connection
  }
  delay(50);    //Send a request every 0.05 seconds

}
