// PID motor position control.
// Thanks to Brett Beauregard for his nice PID library http://brettbeauregard.com/blog/2011/04/improving-the-beginners-pid-introduction/

#include <PinChangeInt.h>
#include <Wire.h>
#include <PID_v1.h>
#define encodPinA1      2                       // Quadrature encoder A pin
#define encodPinB1      8                       // Quadrature encoder B pin
#define M1              9                       // PWM outputs to L298N H-Bridge motor driver module
#define M2              10
#define COUNTSPERREV    1401.0
String inString="0";
int flag=0;
double kp =10, ki =20 , kd =0;             // modify for optimal performance
double input = 0, output = 0, setpoint = 0;
unsigned long lastTime,now;
volatile long encoderPos = 0,last_pos=0,lastpos=0;
PID myPID(&input, &output, &setpoint, kp, ki, kd,DIRECT);  
void setup() {
  pinMode(encodPinA1, INPUT_PULLUP);                  // quadrature encoder input A
  pinMode(encodPinB1, INPUT_PULLUP);                  // quadrature encoder input B
  attachInterrupt(0, encoder, FALLING);               // update encoder position
  TCCR1B = TCCR1B & 0b11111000 | 1;                   // To prevent Motor Noise
  
  myPID.SetMode(AUTOMATIC);
  myPID.SetSampleTime(1);
  myPID.SetOutputLimits(-255, 255);
  
  Wire.begin(8);                // join i2c bus with address #8 for Right Motor
  Wire.onRequest(requestEvent); // register events
  Wire.onReceive(receiveEvent);
  Serial.begin (9600); 
  
}

void loop() {
while (Serial.available() > 0) {
    int inChar = Serial.read();
      // convert the incoming byte to a char and add it to the string:
      inString += (char)inChar;
      flag=1;
    }

  if(flag){flag=0;
    output=inString.toInt();
    inString =" ";
  }


   now = millis();
   int timeChange = (now - lastTime);
  if(timeChange>=250 )
  {
     input = (360.0*1000*(encoderPos-last_pos)) /(COUNTSPERREV*(now - lastTime));
     lastTime=now;
     last_pos=encoderPos;
     Serial.println("Current xxxxxxspeed="+String(input));
     
  }
  // Serial.println("Encoder POS="+ String(encoderPos));
  // Serial.print("   set point=");
  // Serial.print(setpoint);

  // myPID.Compute();                                    // calculate new output
  pwmOut(output);                                     // drive L298N H-Bridge module
  Serial.println("Current xxxxxxspeed="+String(input));
  Serial.println(" PWM val="+String(output));

  delay(200);
}

void pwmOut(int out) {                                // to H-Bridge board
  if (out > 0) {
    analogWrite(M1, out);                             // drive motor CW
    analogWrite(M2, 0);
  }
  else {
    analogWrite(M1, 0);
    analogWrite(M2, abs(out));                        // drive motor CCW
  }
}

void encoder()  {                                     // pulse and direction, direct port reading to save cycles  
  if (PINB & 0b00000001)    encoderPos--;             // if(digitalRead(encodPinB1)==HIGH)   count --;
  else                      encoderPos++;             // if(digitalRead(encodPinB1)==LOW)   count ++;
}

void requestEvent() {
  int8_t s;
  
  s= (360.0*(encoderPos-lastpos))/COUNTSPERREV; //change in position in degrees of the wheel
  lastpos=encoderPos;
  Wire.write(s); // respond with message of 6 bytes
}


// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  uint8_t a,b;
  a = Wire.read();
  b = Wire.read();
  setpoint= (double)((b<<8)|a);
  //Serial.println((uint8_t)a);
  //Serial.println((uint8_t)b);
  
  //Serial.println(setpoint);
}
