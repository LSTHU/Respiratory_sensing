#include <FastLED.h>
#define LED_PIN 7
#define NUM_LEDS 1
CRGB leds [NUM_LEDS];
int pinBuzzer =13;
int pinBlue=8;
int pinRed=12;
int pinSensor =A0;
int pinSensor1 =A1;
long time1=0;
long time2=0;
int val_back=0;
int dval=0;
float breath=0;
int dtime=0;
int count;
int count1=0;
int count2=0;

void setup() {
  Serial.begin (9600);
  pinMode (pinBuzzer, OUTPUT);
  pinMode (pinBlue, OUTPUT);
  pinMode (pinRed, OUTPUT);
  pinMode (pinSensor,INPUT);
  pinMode (pinSensor1,INPUT);
  analogWrite (pinSensor,INTERNAL); 
  FastLED.addLeds<WS2812,LED_PIN,RGB>(leds,NUM_LEDS);
}

void loop() {
  float val=0;
  float xval=0;
  float xval1=0;
  float xval2=0;
  float dval1=0;
  // put your main code here, to run repeatedly:
  //val=analogRead(pinSensor);
  for (int j=0;j<2;j++){
      for (int i=0;i<20;i++){
        xval=analogRead(pinSensor);
        val=val+xval;
        delay(1);
        }
        dval=val-val_back;
        val_back=val;
        dval1=dval1+dval;
  }

  Serial.println(dval1);
 
  if (dval1>300){
    noTone(pinBuzzer);
    leds[0]=CRGB(255,0,0);
    FastLED.show();
    delay(200);
    count=0;
    count1=0;
    count2=1;
  }
  if (dval1<-200){
    noTone(pinBuzzer);
    leds[0]=CRGB(0,0,0);
    FastLED.show();
    delay(20);
    count=0;
    count1=0;
    count2=0;
  }
  else {
    count=count+1;
    Serial.println(count);
    if(count<100&&count1==0){
      leds[0]=CRGB(0,0,0);
    FastLED.show();
      noTone(pinBuzzer);
      delay(20);
    }
    else{
      leds[0]=CRGB(0,255,0);
      FastLED.show();
      tone(pinBuzzer,500);
      delay(500);
      count=0;
      count1=1;
    }
  }
}
