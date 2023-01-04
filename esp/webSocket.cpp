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
#if WIFI == 5
const char *ssid = "BukanAdzka";    // Enter SSID
const char *password = "123456789"; // Enter Password
#endif

#define WAIT true

const char *host = "benyamin-v20.shaladddin.repl.co"; // Enter server adress
const char *wsProtocol = "wss://";
const uint16_t port = 8080; // Enter server port
WebsocketsClient ws;
const char *deviceID = "smartHome"; // robotID (to identefied by server)

bool authenticated = false;
bool connectedToServer = false;
#define MemorySafetyGap 1024

int MaxMemory()
{
    return (ESP.getMaxFreeBlockSize() - 512) - MemorySafetyGap;
}

bool deserialize(DynamicJsonDocument &doc, String input, bool shrink = false)
{
    DeserializationError err = deserializeJson(doc, input);

    if (err)
    {
        Serial.print(F("deserializeJson() failed with code "));
        Serial.println(err.f_str());
        return false;
    }
    if (shrink)
        doc.shrinkToFit();
    return true;
}

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
    while (!connectedToServer)
    {
        connectedToServer = ws.connect(wsProtocol + String(host));
        Serial.println(F("Attemp to connect to Websockets server"));
    }
    Serial.println(F("connected to websockets"));

    // run callback when messages are received
    ws.onMessage([&](WebsocketsMessage rawMessage)
                 {
    String msg = String(rawMessage.data());
    Serial.println(F("Got Message: ") + msg);

    DynamicJsonDocument doc(MaxMemory());
    if (!deserialize(doc, msg, true))
        return;

    Serial.println(F("done!"));
    Serial.print(F("current capacity: "));
    Serial.println(doc.capacity());

    serializeJsonPretty(doc, Serial);

    //  msg handler
    String message = doc[F("message")].as<String>();
    if (message == F("auth is requested"))
    {
        // send ID
        DynamicJsonDocument authRequest(MaxMemory());
        String BenyaminRequest = F("{ \"message\": \"authRequest\", \"type\" : \"device\", \"deviceKind\" : \"Benyamin\" }");
        if (!deserialize(authRequest, BenyaminRequest))
            return;
        authRequest["id"] = DEVICEID;
        authRequest.shrinkToFit();

        String authRequest_;
        serializeJson(authRequest, authRequest_);
        sendWs(authRequest_);
        return;
    }
    if (message.indexOf(F("connected as Benyamin")) != -1)
    {
        authenticated = true;
        Serial.println("Benyamin is authenticated");
        return;
    }

    ; });
#if WAIT
    while (!authenticated)
        WebsocketRun();
#endif
}
void WebsocketRun()
{
    // check websocket
    if (ws.available())
        ws.poll();
}
