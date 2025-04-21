#include "./wifiCred.h"
#include "WiFiNINA.h"
#include <SPI.h>

byte greenLEDPin = 4;
byte yellowLEDPin = 3;
byte redLEDPin = 2;

const int pathStart = 4;

WiFiServer server(80);

void setup() 
{
  Serial.begin(9600);

  pinMode(redLEDPin, OUTPUT);
  pinMode(yellowLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);

  connectingWifi();
}

void connectingWifi()
{
  WiFi.begin(SECRET_SSID, SECRET_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Connecting to the Wifi...");
    delay(1000);
  }

  Serial.println("Connected to the Wifi!");
  Serial.println(WiFi.localIP());
  server.begin();

  delay(500);  
}

void trafficLight(WiFiClient client)
{
  if (client) 
  {
    String request = client.readStringUntil('\r');
    client.read(); 
    request.trim();

    
    if (request.startsWith("GET "))
    {
      int pathEnd = request.indexOf(' ', pathStart);
      String path = request.substring(pathStart, pathEnd);

      redLight(path);
      yellowLight(path);
      greenLight(path);
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println("Access-Control-Allow-Origin: *");
    client.println("Connection: close");
    client.println();
    client.println("Command received.");
    delay(1);
    client.stop();
  }
}

void redLight(String request)
{
  if (request.indexOf("/red/on") != -1) 
    digitalWrite(redLEDPin, HIGH); 

  else if (request.indexOf("/red/off") != -1) 
    digitalWrite(redLEDPin, LOW);
}

void yellowLight(String request)
{
  if (request.indexOf("/yellow/on") != -1) 
    digitalWrite(yellowLEDPin, HIGH); 

  else if (request.indexOf("/yellow/off") != -1) 
    digitalWrite(yellowLEDPin, LOW);
}

void greenLight(String request)
{
  if (request.indexOf("/green/on") != -1) 
    digitalWrite(greenLEDPin, HIGH); 

  else if (request.indexOf("/green/off") != -1) 
    digitalWrite(greenLEDPin, LOW);
}

void loop() 
{
  WiFiClient client = server.available();
  trafficLight(client);
}

