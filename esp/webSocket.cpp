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
#if WIFI == 6
const char *ssid = "Cak Mhd Fam";    // Enter SSID
const char *password = "MhdLiana123"; // Enter Password
#endif

#define WAIT true

const String host = "benyamin-v20.shaladddin.repl.co"; // Enter server adress
const String wsProtocol = "wss://";
const int port = 8080; // Enter server port
WebsocketsClient ws;
const char *deviceID = "smartHome"; // robotID (to identefied by server)

// constant message
String cancelMsg = "{\"message\":\"cancel\"}";

bool authenticated = false;
bool connectedToServer = false;
bool messageReceaved = false;
#define MemorySafetyGap 1024

unsigned long int authDuration;
unsigned long int connectDuration;

void onMessage(WebsocketsMessage rawMessage);
bool messageHandler(WebsocketsMessage rawMessage);

int MaxMemory()
{
    return (ESP.getMaxFreeBlockSize() - 512) - MemorySafetyGap;
}

bool deserialize(DynamicJsonDocument &doc, String input, bool shrink)
{
    DeserializationError err = deserializeJson(doc, input);

    if (err)
    {
        Serial.print(F("deserializeJson() failed with code "));
        Serial.println(err.f_str());
        return false;
    }
    Serial.print(F("avilable space: "));
    Serial.println(doc.capacity());

    if (shrink)
        doc.shrinkToFit();
    return true;
}

DynamicJsonDocument rawReq(String message, size_t startingByte)
{
    DynamicJsonDocument doc(startingByte);
    !deserialize(doc, (F("{\"message\" :\"") + message + F("\"}")));
    return doc;
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
    connectDuration = millis();

    // connect to wifi
    WiFi.begin(ssid, password);
    Serial.println(F("\nConnecting to ") + String(ssid));

#define tikDelay 500
    int tik = millis() + tikDelay, manyDots = 3;
    String bottom;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1);
        lcd.print(F("menghubungkan ke"), bottom, false);
        if (manyDots > 3)
        {
            manyDots = 0;
            bottom = String(ssid);
        }
        if (tik < millis())
        {
            manyDots++;
            bottom += F(".");
            tik = millis() + tikDelay;
        }

        if (WiFi.status() == WL_WRONG_PASSWORD)
        {
            beep(500, 3, 3000);
            lcd.print(F("pasword WiFi"), F("nya salah woe"), false);
            log(F("wrong pasword lol"));
        }
    }
    beep(500, 1, 500, true);
    lcd.print(F("menghubungkan ke"), F("server..."));
    Serial.println(F("\nConnected to Wifi, Connecting to server."));
    while (!connectedToServer)
    {
        connectedToServer = ws.connect(wsProtocol + host);
        Serial.println(F("Attemp to connect to Websockets server"));
    }

    lcd.print(F("Terhubung ke"), F("server!"));
    Serial.println(F("connected to websockets"));
    beep(1000, 2, 500);
    delay(500);

    // run callback when messages are received
    ws.onMessage(onMessage);
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

//
//
//
//
//
//

void onMessage(WebsocketsMessage rawMessage)
{
    if (messageHandler(rawMessage))
        messageReceaved = true;
}

bool messageHandler(WebsocketsMessage rawMessage)
{
    String msg = String(rawMessage.data());
    Serial.println(F("Got Message: ") + msg);

    DynamicJsonDocument doc(MaxMemory());
    if (!deserialize(doc, msg, true))
        return false;

    Serial.println(F("done!"));

    Serial.print(F("used capacity:"));
    Serial.println(doc.capacity());
    serializeJsonPretty(doc, Serial);

    //  msg handler
    String message = doc[F("message")].as<String>();
    if (message == F("auth is requested"))
    {
        authDuration = millis();
        // send ID
        DynamicJsonDocument authRequest(MaxMemory());
        String BenyaminRequest = F("{ \"message\": \"authRequest\", \"type\" : \"device\", \"deviceKind\" : \"Benyamin\" }");
        if (!deserialize(authRequest, BenyaminRequest))
            return false;
        authRequest["id"] = DEVICEID;
        authRequest.shrinkToFit();

        String authRequest_;
        serializeJson(authRequest, authRequest_);
        sendWs(authRequest_);
        return true;
    }
    if (message == F("absenResponse"))
    {
        handleAbsen(doc);
        return true;
    }
    if (message == F("error"))
    {
        Serial.println("Error receaved!");
        String error = doc[F("error")].as<String>();

        // switch Error
        if (error == F("this card is already present"))
            lcd.print(F("kamu sudah absen"), F("di sesi ini"));

        else if (error == F("card id not found"))
            lcd.print(F("this card is "), F("unrecognized"));

        else if (error == F("no sesion is currently running"))
            lcd.print(F("Tidak ada sesi"), F("absen saat ini:\\"));
        responeError();
        return false;
    }
    if (message.indexOf(F("connected as Benyamin")) != -1)
    {
        authenticated = true;
        Serial.println("Benyamin is authenticated");
        authDuration = millis() - authDuration;
        Serial.print("auth duration: ");
        Serial.println(authDuration);
        return true;
    }
    Serial.println("Unreqognize message!");
    return false;
}