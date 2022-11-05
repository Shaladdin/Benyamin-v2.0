int ledmerah = 8;
int ledhijau = 9;
int ledkuning = 10;

int trig_pin = 2;
int echo_pin = 3;
long echotime;
float distance;
void setup()
{
    Serial.begin(9600);
    pinMode(ledmerah, OUTPUT);
    pinMode(ledhijau, OUTPUT);
    pinMode(ledkuning, OUTPUT);
    pinMode(trig_pin, OUTPUT);
    pinMode(echo_pin, INPUT);
    digitalWrite(trig_pin, LOW);
}
void loop()
{
    digitalWrite(trig_pin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_pin, LOW);
    echotime = pulseIn(echo_pin, HIGH);
    distance = 0.0001 * ((float)echotime * 340.0) / 2.0;
    Serial.print(distance);

    if (distance < 10.0)
    {
        digitalWrite(ledmerah, HIGH);
    }
    else
    {
        digitalWrite(ledmerah, LOW);
    }
    if (distance > 10.0 && distance < 25.0)
    {
        digitalWrite(ledhijau, HIGH);
    }
    else
    {
        digitalWrite(ledhijau, LOW);
    }
    if (distance > 25.0 && distance < 40.0)
    {
        digitalWrite(ledkuning, HIGH);
    }
    else
    {
        digitalWrite(ledkuning, LOW);
    }
    if (distance > 40.0)
    {
        digitalWrite(ledmerah, LOW);
        digitalWrite(ledhijau, LOW);
        digitalWrite(ledkuning, LOW);
    }
}