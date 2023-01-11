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

extern WebsocketsClient ws;

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
void beepInc(int duration, int n, double statingFreq = 500, int inc = 500);
void space(int len = 3);
void checkSensors();

// Websocket
int MaxMemory();
void sendWs(String &in);
void WebsocketInit();
void WebsocketRun();
bool deserialize(DynamicJsonDocument &doc, String input, bool shrink = false);
DynamicJsonDocument rawReq(String message, size_t startingByte = 1024);
extern bool authenticated;
extern bool connectedToServer;

// constants message
extern String cancelMsg;

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

// RFID
extern MFRC522 Reader;
#define resetPin D3
#define ssPin D4
extern bool responded;
void rfidInit();
void rfidRun();
void handleAbsen(DynamicJsonDocument &doc);
void responeError();
String ReadTag();

// GY-906
extern Adafruit_MLX90614 termogun;
double tempRun();
double getTemp();

#define TO_HIGH 40.0
#define TO_COLD 30.0

#define detik *1000

unsigned long int homeTimer;
void setHomeTimer();
void home();
