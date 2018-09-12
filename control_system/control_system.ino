#define RELAY1  6   
#define BLYNK_PRINT Serial
#include <Bridge.h>
#include <BlynkSimpleYun.h>
#include <math.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
//#include <AutoPID.h> //inlcude AutoPID for more accurate controls

//accutalry measure temperature
//day -night alterations
// schedule day to day

//blynk authentication token
char auth[] = "Add your own token here";
BlynkTimer timer;

//values for thermistor measurment equation
int ThermistorPin = 0;
int Vo;
//R1 is resistance of reference resistor
float R1 = 1000;
float logR2, R2, T;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
int pinValueON;

//setpoint is the goal temperature 
int setpoint;
//default setpoint is temperature set when there is no other setpoint
int default_setpoint = 65;
//day setpoint is the setpoint during the day between day_begin and day_end
int day_setpoint;
//night setpoint is the setpoint during the night ie between day_end and day_begin
int night_setpoint;
//delay between sucessive heater switches
int heatDelay = 500;
//width of bang-bang control 
//HVAC turn off +3 above setpoint
//HVAC turn on  -3 below setpoint
int width = 3;
//minTemp is lower bang-bang boundry
int minTemp ;
//maxTemp is upper bang-bang boundry
int maxTemp ;
//status monitors wether HVAC is on or off
String status;

String currentTime ;
String currentDate ;

//day begin is the hour at which day time starts in standard military time
int day_begin = 7;
int day_end = 19;

WidgetRTC rtc; // real time clock

//deal with time display 
void clockDisplay()
{
  // You can call hour(), minute(), ... at any time
  // Please see Time library examples for details

  currentTime = String(hour()) + ":" + minute() + ":" + second();
  currentDate = String(day()) + " " + month() + " " + year();
  //debug purpose of clock
  /*Serial.print("Current time: ");
  Serial.print(currentTime);
  Serial.print(" ");
  Serial.print(currentDate);
  Serial.println();*/

  // Send time to Blynk
  Blynk.virtualWrite(V1, currentTime);
  // Send date to Blynk
  Blynk.virtualWrite(V2, currentDate);
}


//reads temperature and sends it off to blynk on Virtual Pin 6
void myTimerEvent(){
  Blynk.virtualWrite(V6,T);

}

//read day setpoint 
BLYNK_WRITE(V5)
{
  day_setpoint = param.asInt();
  
// assigning incoming value from pin V1 to a variable
 //Serial.print("V5 Slider value in local: ");
 //Serial.println(pinValue);
}

BLYNK_WRITE(V9)
{
  night_setpoint = param.asInt();
  
// assigning incoming value from pin V1 to a variable
 //Serial.print("V5 Slider value in local: ");
 //Serial.println(pinValue);
}






BLYNK_WRITE(V7)
{
  // assigning incoming value from pin V1 to a variable
  pinValueON = param.asInt();
  //Serial.print("V7 button is on or off");
  //Serial.println(pinValueON);
}

 

void setup()
{    

// Initialise the Arduino data pins for OUTPUT
Blynk.begin(auth);
rtc.begin();
// timer activates clock data and blynk sync every 1000 miliseconds
timer.setInterval(1000L,myTimerEvent);
timer.setInterval(1000L,clockDisplay);

//pin 6 turns on and off relay
pinMode(RELAY1, OUTPUT);      
//pin 11 turns on an off LED 
pinMode(11, OUTPUT);

//possible PID setup
//myPID.setBangBang(5)
//change this if you want to chaneg update times 
//myPID.setTimeStep(500000);
}

 

 void loop()
{
  

//automatic temperature regualation throughout the day
Serial.print(hour());
if (hour() < day_end && hour() > day_begin) {
 Serial.print("day hours");
setpoint = day_setpoint; 
}else{
  Serial.print("night hours");
 setpoint = night_setpoint;
}
//Serial.println(setpoint);
maxTemp = setpoint + width;
minTemp = setpoint -width;

  
//initally turn on light
digitalWrite(11, HIGH);
//temperature measurments R2 is resistance of thermistor
 Vo = analogRead(ThermistorPin);
 R2 = R1 * (1023.0 / (float)Vo - 1.0);
 logR2 = log(R2);
 T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
 T = T - 273.15;
 //T is in degreees fahrenheit
 T = (T * 9.0)/ 5.0 + 32.0;
     
// debug purposes for temp 
 //Serial.println(T);
// Serial.print("yes temperature");
// Serial.println(pinValueON);

 
//BLynk app integration
 Blynk.run();
 timer.run();

//////////control system loop/////////////////////////////////////////////////////////////////////
//myPID.run();
//Serial.println(setpoint);
//first if statement deals with startup

status = digitalRead(RELAY1);
//delay control loop heatDelay milliseconds so you dont overstress heater
delay(heatDelay);
if ( T < (minTemp) || ( pinValueON == 1) ){
//RELAY on 
digitalWrite(RELAY1,HIGH);
delay(heatDelay);

}else if(T > (maxTemp)){
  //LED is off
  //Serial.print("REAY  IS OFF");
  // Turns Relay Off
  digitalWrite(RELAY1,LOW);  
  
 //if  temp is in intermediate zone and off keep it off otherwise keep it on   
}else if(T < maxTemp && T >minTemp && (status == LOW)){
//LED is off
  //Serial.print("REAY  IS OFF");
  // Turns Relay Off
  digitalWrite(RELAY1,LOW);  
  
}else{
  //LED is off
  //Serial.print("REAY  IS ON");
  // Turns Relay Off
  digitalWrite(RELAY1,HIGH);  
  
}
 
  //////////////end control system loop///////////////////////////////////                                   
  
 }
