#define RELAY1  6   
#define BLYNK_PRINT Serial
#include <Bridge.h>
#include <BlynkSimpleYun.h>
#include <math.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
//#include <AutoPID.h>

char auth[] = "4b0acc400cb5442fa471ab6de0db41d1";
//char auth[] = "721137cc6fe24899816ee75ebac3611b";
BlynkTimer timer;

int ThermistorPin = 0;
int Vo;
float R1 = 1000;
float logR2, R2, T;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
int pinValueON;
int pinValue;


WidgetRTC rtc; // real time clock
//deal with time display 
void clockDisplay()
{
  // You can call hour(), minute(), ... at any time
  // Please see Time library examples for details

  String currentTime = String(hour()) + ":" + minute() + ":" + second();
  String currentDate = String(day()) + " " + month() + " " + year();
  /*Serial.print("Current time: ");
  Serial.print(currentTime);
  Serial.print(" ");
  Serial.print(currentDate);
  Serial.println();*/

  // Send time to the App
 Blynk.virtualWrite(V1, currentTime);
  // Send date to the App
  Blynk.virtualWrite(V2, currentDate);
}


//reads temperature ad sends it off to blynk on Virtual Pin 6
void myTimerEvent(){
  Blynk.virtualWrite(V6,T);

}

BLYNK_WRITE(V5)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  Serial.print("V5 Slider value is: ");
  Serial.println(pinValue);
}



BLYNK_WRITE(V7)
{
  pinValueON = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.print("V7 button is on or off");
  Serial.println(pinValueON);
}

 

void setup()
{    

// Initialise the Arduino data pins for OUTPUT
Blynk.begin(auth);
rtc.begin();
// timer activates clock data and blynk sync every 1000 miliseconds
timer.setInterval(1000L,myTimerEvent);
timer.setInterval(1000L,clockDisplay);


  pinMode(RELAY1, OUTPUT);       
  pinMode(11, OUTPUT);

//myPID.setBangBang(5)
//change this if you want to chaneg update times 
//myPID.setTimeStep(500000);
}

 

 void loop()

{

  digitalWrite(11, HIGH);

//temperature measurments
 Vo = analogRead(ThermistorPin);
 R2 = R1 * (1023.0 / (float)Vo - 1.0);
 logR2 = log(R2);
 T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
 T = T - 273.15;
 T = (T * 9.0)/ 5.0 + 32.0;    
// delay(500);
 Serial.println(T);
// Serial.print("yes temperature");
 Serial.println(pinValueON);

timer.run();
 
//BLynk app integration
  Blynk.run();


  /*digitalWrite(RELAY1,LOW);           // Turns ON Relays 1

   delay(2000);                                      // Wait 2 seconds

   digitalWrite(RELAY1,HIGH); */        // Turns Relay Off

 //control system loop
 //myPID.run();
  
  if ( T > pinValue || ( pinValueON == 1) ){
 
  
//this is light on config
  
    digitalWrite(RELAY1,HIGH);
      delay(500);
 
  }else{
     
    //Serial.print("REAY  IS OFF");
   // Turns Relay Off
   digitalWrite(RELAY1,LOW); 
   // Turns ON Relays 1 
   delay(500);  
   
  
  }
 
  // delay(2000);                                      // Wait 2 seconds
   

 }
