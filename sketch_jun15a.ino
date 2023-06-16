#include <ESPAsyncWebServer.h>
#include <WiFi.h>


// LED digital pin number
#define LED 2

// Setup the server on port 80
AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);

  // Set the pinmode in the LED pin
  pinMode(LED, OUTPUT);

  // Start connecting to Wifi
  WiFi.begin("iPhone (3)", "Caralho123");

  // Connect to Wifi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wifi...");
  }
  Serial.print("Connected to Wifi with IP: ");
  Serial.println(WiFi.localIP());

  // Listen for requests in HTTP
  server.on("/blink", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(LED, HIGH);
  });

  server.on("/noblink", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(LED, LOW);
  });

  // Start the server
  server.begin();
}

void loop() {
}