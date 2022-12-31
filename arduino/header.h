// Copied from MRC

#include <Arduino.h>
#include <ArduinoJson.h>
// #include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ArduinoWebsockets.h>
using namespace websockets;

// // outputs pin
// #define lamp D1
// #define fan D2
// #define buzzer D0
// const int outputs[] = {lamp, fan, buzzer};

// // ir
// #define irDalam D6
// #define irLuar D5
// const int inputs[] = {irDalam, irLuar};

// void pinInit();

// Websocket
void sendWs(String &in);
void WebsocketInit();
void WebsocketRun();
extern bool activated;

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