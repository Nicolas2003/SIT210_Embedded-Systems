#include <Arduino.h>
#include <ArduinoBLE.h>

byte trig = 7;
byte echo = 8;

BLEService parkDistanceService("a1b2c3d4-1234-4abc-8def-0123456789ab");
BLEStringCharacteristic outputDistance("b2c3d4e5-2345-4bcd-9f01-123456789abc", BLERead | BLENotify, 20);

void setup()
{
    Serial.begin(9600);
    while (!Serial);

    if (!BLE.begin())
    {
        Serial.println("BLE init failed!");
        while (1);
    }


    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);

    BLE.setLocalName("Nano33BLE");
    BLE.setAdvertisedService(parkDistanceService);
    parkDistanceService.addCharacteristic(outputDistance);
    BLE.addService(parkDistanceService);

    outputDistance.writeValue("0.0cm");

    BLE.advertise();
    Serial.println("BLE device is now advertising...");
}

float ultrasonic_distance(byte trigPin, byte echoPin)
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    int duration = pulseIn(echoPin, HIGH);
    float distance = duration * (0.034/2);
    Serial.println("Distance: " + String(distance));
    return distance;
}

void loop()
{
    BLE.poll();
    static unsigned long lastUpdate = 0;
    if (millis() - lastUpdate > 500)
    {
        lastUpdate = millis();
        float distance = ultrasonic_distance(trig, echo);
        outputDistance.writeValue(String(distance));
        Serial.println("Sent:");
        Serial.print(distance);
        Serial.println("\n");
    }
}
