#include "header.h"

MFRC522 Reader;
bool cardDetectedBefore = false;
bool responded = false;
String UserName;

void rfidInit()
{

    // RFID initialization
    SPI.begin();
    Reader.PCD_Init(ssPin, resetPin);

    // // check if the pin connected properly
    // checkSensors();

    // Dump some information
    Serial.print(F(" initilised on pin ") +
                 String(ssPin) + F(". Antenna strength: ") +
                 String(Reader.PCD_GetAntennaGain()) +
                 F(". Version ."));
    Reader.PCD_DumpVersionToSerial();
}

void rfidRun()
{

    Reader.PCD_Init();
    bool cardDetected = Reader.PICC_IsNewCardPresent() && Reader.PICC_ReadCardSerial();
    bool newCard = cardDetected && !cardDetectedBefore;
    cardDetectedBefore = cardDetected;

    if (newCard)
    {
        String currentCard = ReadTag();
        String output;

        beep(500, 1, 1500);
        // TODO: read temprature
        DynamicJsonDocument doc = rawReq("absenRequest");
        doc["cardId"] = currentCard;
        doc.shrinkToFit();
        serializeJson(doc, output);

        // unsigned long int timer = millis() + 500;
        // while (millis() < timer)
        // {
        //     tempRun();
        //     lcd.print(F("JANGAN BERGERAK"), String(termogun.readObjectTempC()), false);
        // }
        // lcd.print(F("JANGAN BERGERAK"), String(getTemp()), false);

        lcd.print(F("JANGAN BERGERAK"));
        sendWs(output);

        // while waiting for the response
        responded = false;
        while (!responded)
        {
            Serial.println(totalTemp);
            lcd.print(F("JANGAN BERGERAK"), F("suhu:") + String(termogun.readObjectTempC()), false);
            tempRun();      // read temprature while waiting
            WebsocketRun(); // wait(poll) for response
        }
        delay(500);
        // contenoud to handler
    }
}

// handlers
void handleAbsen(DynamicJsonDocument &doc)
{
    // extraction
    responded = true;
    UserName = doc["name"].as<String>();
    Serial.println(F("absen data receaved, name: ") + UserName);

#define to_cold 2
#define to_high 1
#define normal 0
    // get temp
    double temp = getTemp();
    Serial.println("temp:");

    int bodyCondition = temp > TO_HIGH ? to_high : (temp < TO_COLD ? to_cold : normal);
    const String message[] = {F("normal O-o"), F("demam!"), F("menggigil!")};

    Serial.println(temp);
    Serial.println(bodyCondition);

    // if temp is abnormal
    if (bodyCondition != normal)
    {
        lcd.print(F("DILARANG MASUK"), F("Anda ") + message[bodyCondition]);
        beep(3 detik, 3);
        sendWs(cancelMsg);
        return;
    }

    // TODO: if already absen

    // if its normal
    // greet
    lcd.print(F("Selamat pagi"), UserName + F("!"));
    digitalWrite(LED, HIGH);
    beepInc(300, 3, 2000);
    digitalWrite(LED, LOW);

    // report body temp
    DynamicJsonDocument outDoc = rawReq("tempReport");
    outDoc["temp"] = temp;
    outDoc.shrinkToFit();
    String req;
    serializeJson(outDoc, req);
    sendWs(req);
    setHomeTimer();
}

void responeError()
{
    responded = true;
    beep(3 detik, 3);
    setHomeTimer();
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
