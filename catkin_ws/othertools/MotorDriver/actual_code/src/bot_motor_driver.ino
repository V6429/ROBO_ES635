// PID motor position control.
// Thanks to Brett Beauregard for his nice PID library http://brettbeauregard.com/blog/2011/04/improving-the-beginners-pid-introduction/

#include <PinChangeInt.h>
#include <Wire.h>
#include <PID_v1.h>
#define encodPinA1 2 // Quadrature encoder A pin
#define encodPinB1 8 // Quadrature encoder B pin
#define M1 9         // PWM outputs to L298N H-Bridge motor driver module
#define M2 10
#define COUNTSPERREV 1401.0

String inString = "0";
volatile int isnegative = 0;
int setpoint_isupdated = 0,iszero=0;


double kp = 1, ki = 2, kd = 1; // modify for optimal performance
volatile double input = 0, output = 0, setpoint = 0;
unsigned long lastTime, now;
volatile int16_t encoderPos = 0, last_pos = 0, lastpos = 0;
PID myPID(&input, &output, &setpoint, kp, ki, kd, DIRECT);



void setup()
{
  pinMode(encodPinA1, INPUT_PULLUP);    // quadrature encoder input A
  pinMode(encodPinB1, INPUT_PULLUP);    // quadrature encoder input B
  attachInterrupt(0, encoder, FALLING); // update encoder position
  TCCR1B = TCCR1B & 0b11111000 | 1;     // To prevent Motor Noise
  myPID.SetMode(AUTOMATIC);
  myPID.SetSampleTime(1);
  myPID.SetOutputLimits(0, 255);
  Wire.begin(8);                // join i2c bus with address #8 for Right Motor
  Wire.onRequest(requestEvent); // register events
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
}

void loop()
{
  // reading uart
  while (Serial.available() > 0)
  {

    int inChar = Serial.read();
    inString += (char)inChar;
    

    if (inChar == 'e')
    { 
      Serial.print(String(encoderPos) + " \r\n"); // ENSURE THE SPACE in  [[[+ " \r\n"]]]
      inString = String(setpoint);
      setpoint_isupdated = 0;
      break;
    }

  setpoint_isupdated = 1;
  }


  // update setpoint if something received from uart
  if (setpoint_isupdated)
  {
   

    setpoint = inString.toDouble();
    if (setpoint > 0)
      isnegative = 0;
    else
    {
      isnegative = 1;
      setpoint = abs(setpoint);
    }

    inString = " ";
     setpoint_isupdated = 0;
  }


  // speed calculation for pid input
  now = millis();
  int timeChange = (now - lastTime);

  if (timeChange >= 100)
  {
    input = abs(360.0 * 1000 * (encoderPos - last_pos)) / (COUNTSPERREV * (now - lastTime));
    lastTime = now;
    last_pos = encoderPos;
  }

  // calling pid calculate function  using( PID myPID( &input, &output, &setpoint, kp, ki, kd, DIRECT);)
  myPID.Compute(); // calculate new output
  


  // safety to out 0
  if(setpoint)
  pwmOut(output);  // drive L298N H-Bridge module
  else
  pwmOut(0);  // drive L298N H-Bridge module




  // Serial.println(input);
  // Serial.println("Speed=" + String(input));

  delay(10);
}




void pwmOut(int out)
{ // to H-Bridge board
  if (!isnegative)
  {
    analogWrite(M1, out); // drive motor CW
    analogWrite(M2, 0);
  }
  else
  {
    analogWrite(M1, 0);
    analogWrite(M2, abs(out)); // drive motor CCW
  }
}

void encoder()
{ // pulse and direction, direct port reading to save cycles
  if (PINB & 0b00000001)
    encoderPos--; // if(digitalRead(encodPinB1)==HIGH)   count --;
  else
    encoderPos++; // if(digitalRead(encodPinB1)==LOW)   count ++;
}





void requestEvent()
{
  int8_t s;

  s = (360.0 * (encoderPos - lastpos)) / COUNTSPERREV; // change in position in degrees of the wheel
  lastpos = encoderPos;
  Wire.write(s); // respond with message of 6 bytes
}



// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  uint8_t a, b;
  a = Wire.read();
  b = Wire.read();
  setpoint = (double)((b << 8) | a);
}
