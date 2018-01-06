#define BLYNK_PRINT Serial
#include <Bridge.h>
#include <BlynkSimpleYun.h>
#include <math.h>


char auth[] = "4b0acc400cb5442fa471ab6de0db41d1";

//min_duration = ;
//thermistor setup
int ThermistorPin = 0;
int Vo;
float R1 = 1000;
float logR2, R2, T;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

BlynkTimer timer;

//reads temperature ad sends it off to blynk
void myTimerEvent(){
  Blynk.virtualWrite(V6,T);

}
//reads input from slider 
BLYNK_WRITE(V1)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  Serial.print("V1 Slider value is: ");
  Serial.println(pinValue);
}


void setup() {
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth);
  // put your setup code here, to run once:
  timer.setInterval(1000L,myTimerEvent);
}

void loop() {
  Blynk.run();
//thermistor measurmets
 Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  T = T - 273.15;
  T = (T * 9.0)/ 5.0 + 32.0; 

  Serial.print("Temperature: "); 
  Serial.print(T);
  Serial.println(" F"); 

  delay(500);
timer.run();

  /*temp_real
  temp_set
  // put your main code here, to run repeatedly:

//PID control 
if(temp_real < temp_set){
  turn_on 
}else{
  turn off
}*/
 

}
