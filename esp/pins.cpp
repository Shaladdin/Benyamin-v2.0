#include "header.h"

void pinInit()
{
    // initialize outputs pins
    for (int i = 0; i < sizeof(outputs) / sizeof(outputs[0]); i++)
        pinMode(outputs[i], OUTPUT);
    // initialize inputs pins
    for (int i = 0; i < sizeof(inputs) / sizeof(inputs[0]); i++)
        pinMode(inputs[i], INPUT);
}