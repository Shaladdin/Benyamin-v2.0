// Library

#include "header.h"

// display home screen
unsigned long int homeTimer;
void setHomeTimer()
{
    homeTimer = millis() + 5 detik;
}
void home(bool force)
{
    lcd.print(F("Tempelkan kartu"), F("mu disini"),force);
}

void setup()
{
    Serial.begin(115200);
    Serial.println();
    pinInit();
    rfidInit();

    // starting sound
    lcd.print(F("memulai system"), F("Benyamin!"));
    beepInc(1 detik, 3, 500, 1000);
    checkSensors();
    WebsocketInit();
    checkSensors();

    lcd.print(F("Benyamin siap"), F("bertugas!  :D"));
    // finishing sound c:
    beepInc(300, 3, 1500, -500);
    delay(1 detik);

    Serial.println("==== End Setup ====");
    home();
}

void loop()
{
    checkSensors();
    WebsocketRun();
    rfidRun();

    // homescreen run
    if (homeTimer < millis())
        home();
}
