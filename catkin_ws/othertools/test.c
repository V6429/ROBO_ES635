#define encoder0PinA 2

volatile long encoder0Pos = 0;
long newposition;
long oldposition = 0;
unsigned long newtime;
unsigned long oldtime = 0;
long vel;

void setup()
{
    pinMode(encoder0PinA, INPUT);
    digitalWrite(encoder0PinA, HIGH);      // turn on pullup resistor
    attachInterrupt(0, doEncoder, RISING); // encoDER ON PIN 2
    Serial.begin(9600);
    Serial.println("start"); // a personal quirk
}

void loop()
{
    newposition = encoder0Pos;
    newtime = millis();
    vel = (newposition - oldposition) * 10000 / (newtime - oldtime); // 1000(to conver milli sec to sec)*60(to convert sec to minutes)/6(total number of pulses per rotation)=10000 (pulses per milli sec to rpm)
    Serial.print("speed = ");
    Serial.println(vel);
    oldposition = newposition;
    oldtime = newtime;
    delay(1000);
}

void doEncoder()
{
    encoder0Pos++;
    // Serial.println (encoder0Pos);
}