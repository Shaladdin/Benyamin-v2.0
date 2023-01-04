// Library

#include "header.h"

// RFID Variables
#define resetPin D3
#define ssPin D4
MFRC522 Reader;

void setup()
{
    Serial.begin(115200);
    pinInit();

    // starting sound
    for (int i = 0; i < 3; i++)
    {
        tone(buzzer, 500 + i * 500);
        delay(300);
    }
    noTone(buzzer);

    WebsocketInit();

    //     // RFID initialization
    // SPI.begin();
    // Reader.PCD_Init(ssPin, resetPin);

    // // check if the pin connected properly
    // checkSensors();

    // // Dump some information
    // Serial.print(F(" initilised on pin ") +
    //              String(ssPin) + F(". Antenna strength: ") +
    //              String(Reader.PCD_GetAntennaGain()) +
    //              F(". Version ."));
    // Reader.PCD_DumpVersionToSerial();




//     WiFi.begin(wifiSSID, wifiPassword);
//     log(F("\n\n"));
//     Serial.println(F("connecting to ") + wifiSSID);

// #define tikDelay 500
//     int tik = millis() + tikDelay, manyDots = 3;
//     String bottom;
//     while (WiFi.status() != WL_CONNECTED)
//     {
//         Serial.println(manyDots);
//         if (manyDots > 3)
//         {
//             manyDots = 0;
//             bottom = wifiSSID;
//         }
//         if (tik < millis())
//         {
//             manyDots++;
//             bottom += F(".");
//             tik = millis() + tikDelay;
//         }

//         lcd.print(F("menghubungkan ke"), bottom, false);

//         if (wifi.status() == WL_WRONG_PASSWORD)
//         {
//             beep(500, 3000, 3, redLED);
//             lcd.print(F("pasword wifi"), F("nya salah woe"), false);
//             log(F("wrong pasword lol"));
//         }
//     }

//     lcd.print(F("Terhubung ke"), wifiSSID);
//     Serial.print(F("Connected to WiFi network with IP Address: "));
//     Serial.println(WiFi.localIP());




    // lcd.print(F("menghubungkan ke"), F("server..."));

    // lcd.print(F("Terhubung ke"), F("server!"));
    // beep(1000, 500, 2, greenLED, true);
    // delay(500);

    // lcd.print(F("Benyamin siap"), F("bertugas!  :D"));
    // // finishing sound c:
    // for (int i = 0; i < 3; i++)
    // {
    //     tone(buzzer, 1500 - i * 500);
    //     delay(100);
    // }
    // noTone(buzzer);
    // delay(1000);

    // Serial.println("==== End Setup ====");
}

void loop()
{
    WebsocketRun();
}
// RFID functions
String ReadTag()
{
    byte *buffer = Reader.uid.uidByte;
    byte bufferSize = Reader.uid.size;
    String output;
    for (byte i = 0; i < bufferSize; i++)
    {
        output += buffer[i] < 0x10 ? F("0") : F("");
        output += String(buffer[i]);
    }
    return output;
}

// void checkSensors()
// {
//     int antennaStrength = Reader.PCD_GetAntennaGain();
//     while (antennaStrength < 64)
//     {
//         Serial.println(F("Reader is not conected properly\tantena strength: ") + String(antennaStrength));
//         lcd.print(F("RFID reader"), F("tidak terhubung!"), false);
//         antennaStrength = Reader.PCD_GetAntennaGain();
//         delay(500);
//     }

//     // mlx init
//     while (!termogun.begin())
//     {
//         Serial.println(F("Error connecting to MLX sensor. Check wiring."));
//         lcd.print(F("Sensor suhu"), F("tidak terhubung"), false);
//     };
// }
