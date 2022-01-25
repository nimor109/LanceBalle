#include <Servo.h>

Servo myservo;  // create servo object to control a servo
int analogPin = 3;
int servoPin = 9;
int val = 0;           // variable to store the value read



void setup()
{
  myservo.attach(servoPin);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);          //  setup serial
  myservo.write(0);                  // sets the servo position according to the scaled value
}


void loop()
{
  myservo.detach();
  val = analogRead(analogPin);    // read the input pin
  Serial.println(val);             // debug value
  if(val < 300)
  {
    delay(3000);
    myservo.attach(servoPin);
    myservo.write(90);                  // sets the servo position according to the scaled value
    delay(150);
    myservo.detach();
  }
  else
  {
    myservo.attach(servoPin);
    myservo.write(0);                  // sets the servo position according to the scaled value
    delay(150);
  }
  
}
