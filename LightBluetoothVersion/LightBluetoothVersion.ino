#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial hc06(2,3);
Servo myservo;  // create servo object to control a servo
String cmd=""; //to store Bluetooth command
int IN1A = 5; // PWM speed control
int ENA = 4; //Low = disable, High = enabled
int IN1B = 6;
int ENB = 7;
int servoPin = 9; //D3
int IRSensorPin= A0; //A3
int IRval = 0;
bool isBallIn = false;

void setup()
{
  hc06.begin(9600); //Initialize Bluetooth Serial Port
  myservo.attach(servoPin);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);          //  setup serial
  pinMode (ENA, OUTPUT);
  pinMode (IN1A, OUTPUT);
  pinMode (ENB, OUTPUT);
  pinMode (IN1B, OUTPUT);
  myservo.write(0);                  // sets the servo position according to the scaled value
  delay(500);
  myservo.detach();
}

void loop()
{
  myservo.detach();
  IRval = analogRead(IRSensorPin);    // read the input pin
  Serial.println(IRval);             // debug value
  if (IRval < 300)
  {
    isBallIn = true;
    hc06.println("Ball Inserted");
  }
  else
  {
    isBallIn = false;
  }
  
  while(hc06.available()>0){
    cmd+=(char)hc06.read();
    delay(100);
  }
  if(cmd != ""){
    if(cmd == "GO")
    {
      cmd="";
      String enteredSpeed ="";
      hc06.println("Write speed");
      while(hc06.available()== 0){
      }
      while(hc06.available()>0){
        enteredSpeed+=(char)hc06.read();
        delay(100);
      }
      Serial.println("Speed set to: "+ enteredSpeed);
      digitalWrite(ENA,HIGH);
      digitalWrite(ENB,HIGH);
      analogWrite (IN1A, enteredSpeed.toInt());
      analogWrite (IN1B, enteredSpeed.toInt());  
      enteredSpeed ="";
      hc06.println("When ready to launch type: Y");
      while(hc06.available()== 0){
      }
      if(hc06.available()>0){
        cmd+=(char)hc06.read();
        if (cmd == "Y")
        {
          cmd="";
          myservo.attach(servoPin);
          myservo.write(90);
          delay(300);
          myservo.write(0);
          delay(500);
          myservo.detach();
        }
      }
    }
    else if(cmd == "STOP")
    {
      hc06.println("Take a break puppy");
      myservo.attach(servoPin);
      myservo.write(0);
      delay(500);
      myservo.detach();
      digitalWrite(ENA,LOW);
      digitalWrite(ENB,LOW);
      cmd=""; 
    }
    else
    {
      hc06.println("I don't get it :"+ cmd);
      cmd="";
    }
  }
  delay(1500);
}
