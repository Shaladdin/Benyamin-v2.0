// Copied from MRC

#include <Arduino.h>
#include <ArduinoJson.h>
// #include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ArduinoWebsockets.h>
using namespace websockets;

// DEVICE INFO
#define DEVICEID F("c137-69420")




// outputs pin
#define LED D0
#define buzzer D8
void Buzzer(bool state, double freq = 400);
const int outputs[] = {LED, buzzer};

// // ir
// #define irDalam D6
// #define irLuar D5
const int inputs[] = {};

void pinInit();

// utils
void beep(int duration, int n, double freq = 400, bool includeLED = true);


// Websocket
int MaxMemory();
void sendWs(String &in);
void WebsocketInit();
void WebsocketRun();
extern bool activated;
extern bool connectedToServer;

// // Security
// extern bool securityMode;
// extern volatile int pendingNotif;
// void SecurityRun();

// // room detection
// extern volatile int jumlahOrang;
// extern volatile bool emptyRoom;
// extern bool notifSended;
// void RoomDetectionInit();
// void RoomDetectionRun();

// // Software serial
// extern SoftwareSerial nano;
// extern bool connectedToNano;
// void SerialInit();
// void SerialRun();

// // Sensor Data:

// // sensors reading
// extern float lpg, co, smoke,
//     temp, humidity, hIndex,
//     flame, light;

// // StreamSensor
// void StreamSensors();

// // smartHome
// extern bool controllMode;
// extern float maxBright, maxFlame, maxTemp, maxGas;
// void SmartHomeRun();
// void ManualRun();