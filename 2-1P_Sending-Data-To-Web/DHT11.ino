#include "DHT.h"
#include "WiFiNINA.h"
#include "ThingSpeak.h"
#include "secrets.h"

#define DHTPIN 4
#define DHTTYPE DHT11

const uint8_t max_temperature = 50;
const uint8_t max_humidity = 90;

DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;
unsigned long lastUpdate = 0;
const unsigned long updateInterval = 60000;

void setup()
{
    dht.begin();
    Serial.begin(9600);

    Serial.println("Connecting to the Wifi...");
    while (WiFi.begin(SECRET_SSID, SECRET_PASS) != WL_CONNECTED)
    {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("Wifi Connected √");

    ThingSpeak.begin(client);
}

void DHT(float humidity, float temperature, uint8_t result[2])
{
    checkingSensor(humidity, temperature);

    result[0] = (uint8_t)humidity;
    result[1] = (uint8_t)temperature;
}

void formatSensorValue(float temperature, float humidity)
{
    Serial.print("Humidity: " + String(humidity) + "% " + "Temperature: " + String(temperature) + "°C ");
}

void checkingSensor(float humidity, float temperature)
{
    if (isnan(humidity) && isnan(temperature))
    {
        Serial.println("DHT11 sensor is not getting any data: ");
        formatSensorValue(humidity, temperature);
    }

    if (humidity > max_humidity && temperature > max_temperature)
    {
        Serial.println("The sensor is getting exceeding values of humidity and temperature ");
        formatSensorValue(humidity, temperature);
    }

    else
    {
        formatSensorValue(humidity, temperature);
    }
}

void loop()
{
    unsigned long currentMillis = millis();
    if (currentMillis - lastUpdate >= updateInterval)
    {
        lastUpdate = currentMillis;

        float humidity = dht.readHumidity();
        float temperature = dht.readTemperature();
        uint8_t dhtData[2];
        DHT(humidity, temperature, dhtData);

        ThingSpeak.setField(1, temperature);
        ThingSpeak.setField(2, humidity);

        int response = ThingSpeak.writeFields(SECRET_CH_ID, SECRET_APIKEY);

        if (response == 200)
        {
            Serial.println("Data sent to ThingSpeak successfully!");
        }
        else
        {
            Serial.println("Failed to send data. Error: " + String(response));
        }
    }
}



