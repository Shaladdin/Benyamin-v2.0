// Library
#include <SPI.h>
#include <Wire.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Adafruit_MLX90614.h>
#include <LiquidCrystal_I2C.h>
#include "header.h"

// RFID Variables
// #define resetPin D3
// #define ssPin D8
// MFRC522 Reader;
// String lastCard;
// unsigned long int lastReading;

void setup()
{
    WebsocketInit();
}

void loop()
{
    WebsocketRun();
}