// Copied from MRC
#include <SPI.h>
#include <Wire.h>
#include <Arduino.h>
#include <MFRC522.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_MLX90614.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoWebsockets.h>
using namespace websockets;

// DEVICE INFO
#define DEVICEID F("c137-69420")


#define log Serial.println

// outputs pin
#define LED D0
#define buzzer D8
const int outputs[] = {LED, buzzer};

const int inputs[] = {};

void pinInit();

// utils
void Buzzer(bool state, double freq = 400);
void beep(int duration, int n, double freq = 400, bool includeLED = true);
void space(int len = 3)



// Websocket
int MaxMemory();
void sendWs(String &in);
void WebsocketInit();
void WebsocketRun();
extern bool authenticated;
extern bool connectedToServer;

// LCD
#define LCD_ROW 2
#define LCD_COLMN 16

struct LCD
{
    String currentText[2];
    bool comingUpdate = false;

    LCD();
    // replace all text with this one
    void print(String top, String bottom = "", bool force = true);
};
extern LCD lcd;