#include "header.h"

void beep(int duration, int n, double freq, bool includeLED)
{
    n *= 2;
    bool state = true;
    for (int i = 0; i < n; i++)
    {
        digitalWrite(LED, state);
        Buzzer(state);
        state = !state;
        delay(duration / n);
    }
    digitalWrite(LED, LOW);
    Buzzer(0);
}

void Buzzer(bool state, double freq)
{
    if (state)
        tone(buzzer, freq);
    else
        noTone(buzzer);
}