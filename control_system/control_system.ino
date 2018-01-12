#define BLYNK_PRINT Serial
#include <Bridge.h>
#include <BlynkSimpleYun.h>
#include <math.h>
#include <AutoPID.h>
#include <TimeLib.h>
#include <WidgetRTC.h>

char auth[] = "4b0acc400cb5442fa471ab6de0db41d1";

//min_duration = ;
//thermistor setup
int ThermistorPin = 0;
int Vo;
float R1 = 1000;
float logR2, R2, T;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
//dailytemp contains the preffered temperature for each hour of the day 
float dailytemp = [];

BlynkTimer timer;//handles blynk events 
WidgetRTC rtc; // real time clock

// Digital clock recieves time over wifi
void clockDisplay()
{
  // You can call hour(), minute(), ... at any time
  // Please see Time library examples for details

  String currentTime = String(hour()) + ":" + minute() + ":" + second();
  String currentDate = String(day()) + " " + month() + " " + year();
  Serial.print("Current time: ");
  Serial.print(currentTime);
  Serial.print(" ");
  Serial.print(currentDate);
  Serial.println();

  // Send time to the App
  Blynk.virtualWrite(V1, currentTime);
  // Send date to the App
  Blynk.virtualWrite(V2, currentDate);
}

//reads temperature ad sends it off to blynk on Virtual Pin 6
void myTimerEvent(){
  Blynk.virtualWrite(V6,T);

}
//reads input from stepper on phone 
BLYNK_WRITE(V1)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  Serial.print("V1 Slider value is: ");
  Serial.println(pinValue);
}

//on-off button on app using virtual pin 2 
BLYNK_WRITE(V2)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("V2 button is on or off");
  Serial.println(pinValue);
}

void setup() {
// Debug console
 Serial.begin(9600);


//Blynk intro
Blynk.begin(auth);
// Begins real time clock
rtc.begin();
 // timer activates clock data and blynk sync every 1000 miliseconds
timer.setInterval(1000L,myTimerEvent);
timer.setInterval(1000L,clockDisplay);
//temp integration
myPID.setBangBang(5)
//change this if you want to chaneg update times 
myPID.setTimeStep(500000);

}

void loop() {  
//BLynk app integration
  Blynk.run();
//thermistor measurmets
//converts voltage measurment Vo to temperature T
 Vo = analogRead(ThermistorPin);
 R2 = R1 * (1023.0 / (float)Vo - 1.0);
 logR2 = log(R2);
 T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
 T = T - 273.15;
 T = (T * 9.0)/ 5.0 + 32.0; 

 Serial.print("Temperature: "); 
 Serial.print(T);
 Serial.println(" F"); 

 //runs timer to get hour and day data 
timer.run();
//==================================================================
//temperature controls 

//myPID.run();

//relay integration
if (T >= 150){
  //set relay output to LOW for off
    digitalWrite(pinOut, LOW);
  }
  else {
  //set relay ouput to HIGH for on 
    digitalWrite(pinOut, HIGH);
  }

 delay(500);
}
