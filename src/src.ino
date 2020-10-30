#include <WiFi.h>

char ssid[] = "network";
char pass[] = "password";
int keyIndex = 0;

int status = WL_IDLE_STATUS;
char server[] = "www.google.com";

WiFiClient client;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }

  String fv = WiFi.firmwareVersion();

  if (fv != "1.1.0") {
    Serial.println("Please upgrade the firmware");
  }
  
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }

  Serial.println("Connected to wifi");
  Serial.println("\nStarting connection to server...");
}

void loop() {
  if (client.connect(server, 80)) {
    Serial.println("connected to server");
    client.println("GET /search?q=arduino HTTP/1.1");
    client.println("Host: " + server);
    client.println("Connection: close");
    client.println();
  }
  delay(5000)
}
