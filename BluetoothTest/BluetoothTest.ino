#include <SoftwareSerial.h>
SoftwareSerial hc06(2,3);
String cmd="";
float sensor_val=0;
void setup(){
 //Initialize Bluetooth Serial Port
 hc06.begin(9600);
}
void loop(){
 //Read data from HC06
 while(hc06.available()>0){
   cmd+=(char)hc06.read();
 }
 //Select function with cmd
 if(cmd!=""){
   // We expect ON or OFF from bluetooth
   if(cmd=="ON"){
       hc06.println("Function is on");
   }else if(cmd=="OFF"){
       hc06.println("Function is off");
   }else{
       hc06.println("Function is off by default");
   }
   cmd=""; //reset cmd
 }

delay(100);
}
