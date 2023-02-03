#include "header.h"

// Automaticly initialize on utils:checkSensor()
Adafruit_MLX90614 termogun = Adafruit_MLX90614();
double totalTemp = 0;
unsigned int nTemp = 0;

void logTemp()
{
    Serial.println(F("current maximum:\t") + String(totalTemp));
    Serial.println(F("current n:\t") + String(nTemp));
    Serial.println(F("current average:\t") + String(totalTemp / nTemp));
}

double tempRun()
{
    double reading = termogun.readObjectTempC();
    if (isnan(reading))
    {
        Serial.println(F("FAILED READING:"));
        logTemp();
        return -69420;
    }
    totalTemp += reading;
    nTemp++;
    return reading;
}
double getTemp()
{
    logTemp();
    double total = totalTemp / nTemp;
    totalTemp = 0;
    nTemp = 0;
    return total;
}