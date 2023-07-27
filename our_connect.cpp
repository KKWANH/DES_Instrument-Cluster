const int   sensorPin = 2;

void        rpmCounter(void)
{
    Serial.println("detect");
}

void        setup(void)
{
    Serial.begin(9600);
    Serial.pinMode(sensorPin, INPUT);
    attatchInterrupt((sensorPin), rpmCounter, RISING);
}

void        loop(void)
{
    delay(1000);
}