int pinSensor =A0;
int pinSensor1 =A1;
int val_back=0;
int dval=0;
int count=0;

int input1 = 5;
int input2 = 6; 
int eva = 11; 


void setup() {
  Serial.begin (9600);
  pinMode(input1,OUTPUT);
  pinMode(input2,OUTPUT);
  pinMode (pinSensor,INPUT);
  pinMode (pinSensor1,INPUT);
}

void loop() {
  float val=0;
  float xval=0;
  float xval1=0;
  float xval2=0;
  float dval1=0;
  for (int j=0;j<5;j++){
      for (int i=0;i<30;i++){
        xval1=analogRead(pinSensor);
        xval2=analogRead(pinSensor1);
        xval=xval1-xval2;
        val=val+xval;
        delay(2);
        }
        dval=val-val_back;
        val_back=val;
        dval1=dval1+dval;
  }
  Serial.println(dval1);
  if (dval1<200&&count==0){
      analogWrite (eva,255);
      //forward 
      digitalWrite(input1,HIGH); 
      digitalWrite(input2,LOW);  
      delay(1000);   
      count=1;
      analogWrite (eva,100);
      digitalWrite(input1,HIGH);
      digitalWrite(input2,LOW);
      delay(300);
  }
  if (dval1<200&&count==1){
    
      analogWrite (eva,0);
      count=1;
      digitalWrite(input1,HIGH);
      digitalWrite(input2,HIGH);  
      delay(300);  
      
  }
   if (dval1>200&&count==1){
     count=0;
     analogWrite (eva,100);
      //forward 
      digitalWrite(input1,LOW); 
      digitalWrite(input2,HIGH); 
      delay(100);   
  }
  if (dval1>200&&count==0){
     analogWrite (eva,0);
     digitalWrite(input1,LOW);
     digitalWrite(input2,LOW);  
     delay(500);  
  }
}
