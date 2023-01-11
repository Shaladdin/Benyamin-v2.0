#include "header.h"

void beep(int duration, int n, double freq, bool includeLED)
{
    n *= 2;
    bool state = true;
    for (int i = 0; i < n; i++)
    {
        digitalWrite(LED, state);
        Buzzer(state, freq);
        state = !state;
        delay(duration / n);
    }
    digitalWrite(LED, LOW);
    Buzzer(0);
}
void beepInc(int duration, int n, double startingFreq, int inc)
{
    for (int i = 0; i < n; i++)
    {
        tone(buzzer, startingFreq + i * inc);
        delay(duration / n);
    }
    noTone(buzzer);
}

void Buzzer(bool state, double freq)
{
    if (state)
        tone(buzzer, freq);
    else
        noTone(buzzer);
}

void space(int len)
{
    for (int i = 0; i < len; i++)
        Serial.println();
}


void checkSensors()
{
    int antennaStrength = Reader.PCD_GetAntennaGain();
    while (antennaStrength < 64)
    {
        Serial.println(F("Reader is not conected properly\tantena strength: ") + String(antennaStrength));
        lcd.print(F("RFID reader"), F("tidak terhubung!"), false);
        antennaStrength = Reader.PCD_GetAntennaGain();
        delay(500);
    }

    // mlx init
    while (!termogun.begin())
    {
        Serial.println(F("Error connecting to MLX sensor. Check wiring."));
        lcd.print(F("Sensor suhu"), F("tidak terhubung"), false);
    };
}
