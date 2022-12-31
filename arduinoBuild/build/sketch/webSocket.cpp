#line 1 "d:\\Shaladdin\\Robotik\\Project\\buat sekoalah\\Benyamin v2.0\\arduino\\webSocket.cpp"
#include "header.h"

#define WIFI 1

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
                     String msgType, details, error;
                     JsonObject data;
                     // a block scope so the doc gonna be deleted after extraction
                     {
                         DynamicJsonDocument doc(ESP.getMaxFreeBlockSize() - 512);
                         DeserializationError err = deserializeJson(doc, msg);
                         while(err)
                         {
                             Serial.print(F("deserializeJson() failed with code "));
                             Serial.println(err.f_str());
                             err = deserializeJson(doc, msg);
                             return;
                         }
                         msgType = doc[F("msg")].as<String>();
                         details = doc[F("details")].as<String>();
                         error = doc[F("err")].as<String>();
                         data = doc[F("data")];
                     }
                     Serial.println(F("type:") + msgType);
                     Serial.println(F("detail:") + details);
                     Serial.println(F("error:") + error);

                        // setting up connection
                     if (msgType == F("identity requested"))
                     {
                         // self identify to server
                         String identifyMsg;
                         StaticJsonDocument<64> identifyDoc;
                         identifyDoc[F("msg")] = F("identity");
                         identifyDoc[F("device")] = F("smartHome");
                         serializeJson(identifyDoc, identifyMsg);
                         sendWs(identifyMsg);
                         return;
                     }
                     if (msgType == F("connected"))
                     {
                         String loadReq;
                         StaticJsonDocument<32> doc;
                         doc[F("type")] = F("load");
                         serializeJson(doc, loadReq);
                         Serial.println(loadReq);
                         delay(500);
                         sendWs(loadReq);
                         return;
                     }
                     if (msgType == F("loadUp"))
                     {
                         activated = true;
                         securityMode = data[F("security")].as<bool>();
                         controllMode = data[F("controllMode")].as<String>() == F("Manual");
                         maxBright = data[F("maxBright")].as<float>();
                         maxFlame = data[F("maxFlame")].as<float>();
                         maxTemp = data[F("maxTemp")].as<float>();
                         maxGas = data[F("maxGas")].as<float>();

                         Serial.println(String(F("\ncontrollMode:\t")) + (controllMode ? "manual" : "otomatis") +
                                        F("\nsecurity:\t") + (securityMode ? "true" : "false") +
                                        F("\nmaxBright:\t") + String(maxBright) +
                                        F("\nmaxFlame:\t") + String(smoke) +
                                        F("\nmaxTemp:\t") + String(maxTemp) +
                                        F("\nmaxGas:\t") + String(maxGas));
                         return;
                     }

                    //  notification system
                     if (msgType == F("security mode"))
                     {
                         securityMode = details == F("1");
                         Serial.println(String(F("Security mode is ")) + (securityMode ? F("On") : F("Off")));
                         return;
                     }
                     if (msgType == F("ok"))
                     {
                         notifSended = false;
                         pendingNotif = false;
                         noTone(buzzer);
                         Serial.println(F("notif turned off"));
                         return;
                     } 

                    //  Manual System
                     if(msgType == F("change mode")){
                        controllMode = details == F("Manual");
                        Serial.println(String(F("changing controll mode into ")) + details);
                        Serial.println(controllMode);
                        return;
                     }

                     if(msgType == F("manual contoll"))
                     {
                        if(!controllMode){
                            Serial.println(F("current controll mode is otomatis"));
                            return;
                        }
                        int pin = data[F("aplience")] == F("fan")?fan:lamp;
                        bool state = data[F("state")].as<bool>();
                        Serial.println(data[F("aplience")] == F("fan"));
                        Serial.println(data[F("state")].as<bool>());
                        Serial.println(String(F("turning ")) + (pin == fan?F("fan"):F("lamp")) + " " + (state?F("on"):F("off")));
                        digitalWrite(pin,state  );
                        return;
                     } });
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