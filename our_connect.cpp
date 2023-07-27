// MACROS FOR EXECUTION
#define BAUD_RATE       9600
#define SENSOR_PIN      2

void    setup(void)
{
    Serial.begin(BAUD_RATE);
    pinMode(SENSOR_PIN, INPUT);

    attachInterrupt(digitalPinToInterrupt(SENSOR_PIN), speedCalculate, RISING);
    delay(1000);
}

// MACROS FOR CALCULATION
#define PPR             2       // Purses Per Rotation
#define ZERO_TIMEOUT    100000  // Timeout period(microsecond) for RPM reset
#define NUM_READINGS    2       // Number of samples for moving average calculation 
#define PULSE_DISTANCE  6.8e-3  // Distance per pulse in m/s

// NUMBERS FOR CALCULATION
volatile unsigned long  prevTimeMeasured;
volatile unsigned long  purseInterval = ZERO_TIMEOUT + 1000;  // initial value must be bigger than timeout period
volatile unsigned long  intervalAvg = ZERO_TIMEOUT + 1000;
unsigned long           intervalSum;
unsigned int            purseCounter = 1;
unsigned long           frqRaw;
unsigned long           frqReal;
unsigned long           RPM;

unsigned long           prevCycleTime = prevTimeMeasured;
unsigned long           currMicroSec = micros();
unsigned int            zeroDebounce;
unsigned int            amountReadings = 1;
unsigned long           readings[NUM_READINGS];
unsigned long           readIndex;
unsigned long           total;
unsigned long           average;
unsigned long           speed;

void    loop(void)
{
    prevCycleTime = prevTimeMeasured;
    currMicroSec = micros();

    if (currMicroSec < prevCycleTime)
        prevCycleTime = currMicroSec;

    frqRaw = 10000000000 / intervalAvg;
    if ((purseInterval) > (ZERO_TIMEOUT - zeroDebounce) ||
        (currMicroSec - prevCycleTime) > ZERO_TIMEOUT - zeroDebounce)
    {
        frqRaw = 0; // set frequency as 0
        zeroDebounce = 2000;
    }
    else
        zeroDebounce = 0;
    
    frqReal = frqRaw / 10000;

    RPM = (frqRaw / PPR * 60) / 10000;
    speed = PULSE_DISTANCE * RPM;  // speed in m/s

    total = total - readings[readIndex];
    readings[readIndex] = speed;
    total = total + readings[readIndex];
    readIndex = readIndex + 1;

    if (readIndex >= NUM_READINGS)
        readIndex = 0;

    average = total / NUM_READINGS;

    Serial.print("Interval: [");
    Serial.print(purseInterval);
    Serial.print("]\tReadings: [");
    Serial.print(readingsCnt);
    Serial.print("]\tSpeed: [");
    Serial.print(speed);
    Serial.print("]\tAverage: [");
    Serial.print(average);
    Serial.println("]");
}

void    speedCalculate(void) {
    purseInterval = micros() - prevTimeMeasured;
    prevTimeMeasured = micros();

    if (purseCounter >= readingsCnt) {
        intervalAvg = intervalSum / readingsCnt;
        purseCounter = 1;
        intervalSum = purseInterval;

        int RemapedReadingsCnt = map(purseInterval, 40000, 5000, 1, 10);
        RemapedReadingsCnt = constrain(RemapedReadingsCnt, 1, 10);
        readingsCnt = RemapedReadingsCnt;
    } else {
        purseCounter++;
        intervalSum = intervalSum + purseInterval;
    }
}