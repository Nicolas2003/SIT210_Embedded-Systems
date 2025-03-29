#include <Wire.h>
#include "WiFiNINA.h"
#include "./wifiCredentials.h"

#define BH1750_ADDR 0x23
#define BEGINNING_TIME 0
#define THRESHOLD_LIGHT 40000
#define HOUR_IN_MILLISECONDS 3600000

String key = "X_U26dZJKzQWVkqabj4FL";
String sunbeam_active = "Sunlight_On";
String sunbeam_inactive = "Sunlight_Off";

char server[] = "maker.ifttt.com";

int total_light_sum = 0;
unsigned long start_time = millis();

bool sunlight_on = false;

void setup() 
{
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(BH1750_ADDR);
  Wire.write(0x10);
  Wire.endTransmission(); 

  WiFi.begin(SECRET_SSID, SECRET_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Connecting to the Wifi...");
    delay(1000);
  }

  Serial.println("Connected to the Wifi!");
  delay(500);
}

void send_IFTTT_requests(String event)
{
  WiFiClient client;
  if (client.connect(server, 80)) 
  {
    Serial.println("Connected to the IFTTT server!");

    String path = "/trigger/" + event + "/with/key/" + key;
    // Serial.println("Sending GET request to: " + path);
    client.println("GET " + path + " HTTP/1.1");

    client.println("Host: maker.ifttt.com");
    client.println("User-Agent: Arduino");
    client.println("Connection: close");
    client.println("");

    // Serial.println("Sent request to: " + path);

    unsigned long maxtime = millis() + 5000;
    while (client.available() == 0) {
      if (millis() > maxtime) {
        Serial.println("Timeout waiting for response");
        client.stop();
        return;
      }
    }
    
    while (client.available())
    {
      String line = client.readStringUntil('\r');
      // Serial.print(line);
    }

    client.println();
    client.stop();
  }
  else
  {
    Serial.println("Connection to IFTTT was unsuccessful");
  }
}

// void send_IFTTT_requests(String event)
// {
//   WiFiClient client;
//   if (client.connect(server, 80)) 
//   {
//     Serial.println("Connected to the IFTTT server!");

//     String path = "/trigger/" + event + "/with/key/" + key;
//     Serial.println("Sending GET request to: " + path);
//     client.println("GET " + path + " HTTP/1.1");
//     // GET /trigger/Sunlight_On/with/key/X_U26dZJKzQWVkqabj4FL HTTP/1.1

//     client.println("Host: maker.ifttt.com");
//     client.println("User-Agent: Arduino");
//     client.println("Connection: close");
//     client.println("");

//     Serial.println("Sent request to: " + path);

//     unsigned long maxtime = millis() + 5000;
//     while (client.available() == 0) {
//       if (millis() > maxtime) {
//         Serial.println("Timeout waiting for response");
//         client.stop();
//         return;
//       }
//     }
    
//     while (client.available())
//     {
//       String line = client.readStringUntil('\r');
//       Serial.print(line);
//     }
//     Serial.println("\nRequest sent.");

//     client.println();
//     client.stop();
//   }
//   else
//   {
//     Serial.println("Connection to IFTTT was unsuccessful");
//   }
// }

void sum_light_levels(int light)
{
  total_light_sum += light;
}

float calc_avg(int time)
{
  float average = (float)total_light_sum / time;
  return average;
}

// void elevated_Sunlight_Levels(float average)
// {
//   if (average >= THRESHOLD_LIGHT && !sunlight_on)
//   {
//     send_IFTTT_requests(sunbeam_active);
//     sunlight_on = true;
//     Serial.println("Elevated sun light");
//   }
// }

// void decreased_Sunlight_Levels(float average)
// {
//   if (average < THRESHOLD_LIGHT && sunlight_on)
//   {
//     send_IFTTT_requests(sunbeam_inactive);
//     sunlight_on = false;
//     Serial.println("Decreased sun light");
//   }
// }

uint16_t reading_light_levels()
{
  delay(500);
  Wire.requestFrom(BH1750_ADDR, 2);
  if (Wire.available() == 2)
  {
    uint16_t lux = (Wire.read() << 8) | Wire.read();
    return lux;
  }

}

int hour(int number)
{
  return HOUR_IN_MILLISECONDS * number;
}

void is_sunbeam_active(int lux)
{
  if (lux >= THRESHOLD_LIGHT && !sunlight_on)
  {
    send_IFTTT_requests(sunbeam_active);
    sunlight_on = true;
  }
  else if (lux < THRESHOLD_LIGHT && sunlight_on)
  {
    send_IFTTT_requests(sunbeam_inactive);
    sunlight_on = false;
  }
}

void loop() 
{
  uint16_t lux = reading_light_levels();

  Serial.println("Light Intensity: " + String(lux) + " lux");

  sum_light_levels(lux); // other stats of how much lumens has been absorbed

  is_sunbeam_active(lux);

  delay(1000);

}


