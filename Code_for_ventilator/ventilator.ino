 
#include <Servo.h>
int val_back=0;
int dval=0;
int pumpin = 13;
int val=0;
int count=0;
int count1=0;
int count2=0;

void setup(){
  Serial.begin(9600);
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(13,OUTPUT);
}

void loop(){
  float val=0;
  float xval=0;
  float xval1=0;
  float xval2=0;
  float dval1=0;
  for (int j=0;j<5;j++){
      for (int i=0;i<30;i++){
        xval1=analogRead(A0);
        xval2=analogRead(A1);
        xval=xval1-xval2;
        val=val+xval;
        delay(2);
        }
        dval=val-val_back;
        val_back=val;
        dval1=dval1+dval;
  }
  
  val=dval1;

 if (val>600&&count2==0){
    digitalWrite(pumpin,LOW);
    delay(200);
    count=0;
    count1=0;
    count2=1;
  }
  if (val>200&&count2==1){
    digitalWrite(pumpin,LOW);
    delay(200);
    count=0;
    count1=0;
    count2=1;
  }
  if (val<200&&count2==1){
    digitalWrite(pumpin,LOW);
    delay(200);
    count=0;
    count1=0;
    count2=0;
  }
  else {
    count=count+1;
    if(count<50 && count1==0){
      digitalWrite(pumpin,LOW);
      delay(200);
    }
    else{
      digitalWrite(pumpin,HIGH);
      delay(1000);
      count=0;
      count1=1;
    }
  }
}

 
 
