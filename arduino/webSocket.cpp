#include "header.h"

#define WIFI 3

// IOT var
#if WIFI == 1
const char *ssid = "Adzka";         // Enter SSID
const char *password = "123456789"; // Enter Password
#endif
#if WIFI == 2
const char *ssid = "R.Guru New";    // Enter SSID
const char *password = "B15m1ll4h"; // Enter Password
#endif
#if WIFI == 3
const char *ssid = "Mas MHD";                // Enter SSID
const char *password = "Fahri_Ali-17082016"; // Enter Password
#endif
#if WIFI == 4
const char *ssid = "Ga duku";         // Enter SSID
const char *password = "ASDFGHJKL;'"; // Enter Password
#endif

#define WAIT true

const char *host = "Si-Santui-M4RC.shaladddin.repl.co"; // Enter server adress
const char *wsProtocol = "wss://";
const uint16_t port = 8080; // Enter server port
WebsocketsClient ws;
const char *deviceID = "smartHome"; // robotID (to identefied by server)

bool activated = false;

// Server function
void sendWs(String &in)
{
    Serial.println(F("Sending: ") + in);
    ws.send(in);
}

void WebsocketInit()
{
    delay(1000);
    // connect to wifi
    WiFi.begin(ssid, password);
    Serial.println(F("\nConnecting to ") + String(ssid));
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(F("."));
        delay(500);
    }
    Serial.println(F("\nConnected to Wifi, Connecting to server."));
    bool connectedToServer = false;
    while (!connectedToServer)
    {
        connectedToServer = ws.connect(wsProtocol + String(host));
        Serial.println(F("Attemp to connect to Websockets server"));
    }
    Serial.println(F("connected to websockets"));

    // run callback when messages are received
    ws.onMessage([&](WebsocketsMessage message)
                 {
                     String msg = String(message.data());
                     Serial.println(F("Got Message: ") + msg);
                    //  String msgType, details, error;
                    //  JsonObject data;
                    //  // a block scope so the doc gonna be deleted after extraction
                    //  {
                    //      DynamicJsonDocument doc(ESP.getMaxFreeBlockSize() - 512);
                    //      DeserializationError err = deserializeJson(doc, msg);
                    //      while (err)
                    //      {
                    //          Serial.print(F("deserializeJson() failed with code "));
                    //          Serial.println(err.f_str());
                    //          err = deserializeJson(doc, msg);
                    //          return;
                    //      }
                    //      msgType = doc[F("msg")].as<String>();
                    //      details = doc[F("details")].as<String>();
                    //      error = doc[F("err")].as<String>();
                    //      data = doc[F("data")];
                    //  }
                    //  Serial.println(F("type:") + msgType);
                    //  Serial.println(F("detail:") + details);
                    //  Serial.println(F("error:") + error);
                     ; });
#if WAIT
    while (!activated)
        WebsocketRun();
#endif
}
void WebsocketRun()
{
    // check websocket
    if (ws.available())
        ws.poll();
}