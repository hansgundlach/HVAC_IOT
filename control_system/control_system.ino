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


void setup() {
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth);
  // put your setup code here, to run once:

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
