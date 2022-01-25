#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial hc06(2,3);
Servo myservo;  // create servo object to control a servo
String cmd=""; //to store Bluetooth command
int IN1A = 5; //PWM speed control
int ENA = 4; //Low = disable, High = enabled
int IN1B = 6; //PWM speed control
int ENB = 7; //Low = disable, High = enabled 
int servoPin = 9; //D9
int BallSensorPin= 8; //D3
bool manualMode = false; // Is the machine in manual mode
int speed = 0; // Global speed variable for manual mode

String readBluetoothSerial()
{
  while(hc06.available()>0){
    cmd+=(char)hc06.read();
    delay(100);
  }
  if(cmd != ""){
    hc06.println("bluetooth received: " + cmd);
    return cmd;
  }
  else{
    return "";
  }
}

bool isManualMode()
{
  String bluetoothCommand = readBluetoothSerial();
  cmd = "";
  if(bluetoothCommand == "M"){
    manualMode = true;
    return manualMode;
  }
  else if(bluetoothCommand == "A"){
    manualMode = false;
    return manualMode;
  }
  else{
    return manualMode;
  }
}

bool isBallIn()
{
  int BallSensorVal = LOW; // reset Ball Sensor value to 0 
  BallSensorVal = digitalRead(BallSensorPin);
  Serial.println(" Ball Sensor value: " + String(BallSensorVal));             // debug value
  if (BallSensorVal == HIGH)
  {
    delay(400);
    BallSensorVal = digitalRead(BallSensorPin);
    if (BallSensorVal == HIGH)
    {
      return true;
      BallSensorVal = LOW;
    }
  }
  else
  {
    return false;
  }
}

void bluetoothManual(){
  hc06.println("Entering Manual Mode");
  String localCommand = readBluetoothSerial();
  cmd = "";
  hc06.println("local commmand: " + localCommand);
  while (localCommand != "STOP")
  {
    if(localCommand == ""){
      localCommand = readBluetoothSerial();
      cmd = "";
    }
    if(localCommand != ""){
      hc06.println("local commmand inside loop: " + localCommand);  
      if(localCommand[0] == 'P')
      {
        String enteredSpeed = localCommand.substring(1);
        speed = map(enteredSpeed.toInt(), 0, 100, 40, 255);
        hc06.println("Speed set to: "+ enteredSpeed);

        //clear command
        localCommand = "";
      }
      else if(localCommand == "E"){
        hc06.println("Motors start");
        analogWrite (IN1A, speed);
        analogWrite (IN1B, speed);
        digitalWrite(ENA,HIGH);
        digitalWrite(ENB,HIGH);
        
        //clear command
        localCommand = "";      
      }
      else if(localCommand == "G"){
        hc06.println("shot fired");
      
        //Shoot the ball
        myservo.attach(servoPin);
        myservo.write(90);
        delay(135);
        myservo.write(0);
        delay(500);
        myservo.detach();

        //Wait for the ball to shoot
        delay(1000);

        //Stop the wheels
        digitalWrite(ENA,LOW);
        digitalWrite(ENB,LOW);
        analogWrite (IN1A, 0);
        analogWrite (IN1B, 0);

        //clear command
        localCommand = "";
      }
      else if(localCommand == "STOP"){
        localCommand = localCommand;
      }
      else{
        //clear command
        localCommand = "";
      }
    }
  }
  //Quitting Manual Mode
  //clear command
  localCommand = "";
  
  //Stop the wheels
  digitalWrite(ENA,LOW);
  digitalWrite(ENB,LOW);
  analogWrite (IN1A, 0);
  analogWrite (IN1B, 0);
  speed = 0;
  manualMode = false;
  hc06.println("Leaving Manual Mode");
}

void randomAutomatic()
{
  if(isBallIn())
  {
    int randomSpeed = random(40, 255);
    int randomDelayBeforeShot = random(1500, 5000);
    Serial.println("Speed: " + String(randomSpeed));
    Serial.println("Delay: " + String(randomDelayBeforeShot));
    delay(1000); // 1 sec delay before starting the wheels
    
    //Start the wheels
    digitalWrite(ENA,HIGH);
    digitalWrite(ENB,HIGH);
    analogWrite (IN1A, randomSpeed);
    analogWrite (IN1B, randomSpeed);

    delay(randomDelayBeforeShot); // random delay before the shot

    //Shoot the ball
    myservo.attach(servoPin);
    myservo.write(90);
    delay(135);
    myservo.write(0);
    delay(500);
    myservo.detach();

    //Stop the wheels
    digitalWrite(ENA,LOW);
    digitalWrite(ENB,LOW);
    analogWrite (IN1A, 0);
    analogWrite (IN1B, 0);
  }
}

void setup()
{
  hc06.begin(9600); //Initialize Bluetooth Serial Port
  myservo.attach(servoPin);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);          //  setup serial
  pinMode (ENA, OUTPUT);
  pinMode (IN1A, OUTPUT);
  pinMode (ENB, OUTPUT);
  pinMode (IN1B, OUTPUT);
  pinMode(BallSensorPin, INPUT);
  myservo.write(0);                  // sets the servo position according to the scaled value
  delay(500);
  myservo.detach();
}

void loop()
{
  bool inManualMode = isManualMode();
  if(inManualMode){
    bluetoothManual(); 
  }
  else{
    randomAutomatic();
  }
}
