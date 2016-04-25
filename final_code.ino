//=========================================
//=========================================
//=========================================
//=========================================
//=========================================
//=========================================
//=========================================
//=========================================





#include <L3G.h>

#include <ultrasonic.h>

#include <Servo.h>

#include<Wire.h>

#include<L3G.h>

#include <Encoder.h>

#define pi 3.1415926


//======================================
//==========Pin const===================
//======================================

const int botFlame = A0;
const int midFlame = A1;
const int topFlame = A2;

const int stopButton = 53;

const int stepdirPin = 49;//dir
const int stepPin = 25;//step

int stepCount1 = 0;
int stepCount2 = 150;

long timer = 0;
long timer1= 0;
float G_gain=.875;
float gyro_z;
float gyro_zold;
float gerrz;
float G_Dt=0.005;
//======================================
//============Modular defines===========
//======================================

Encoder myEncL(18, 19);
Encoder myEncR(2, 3);

ultrasonic sonicRight(11,10);
ultrasonic sonicMid(9,8);

Servo left;
Servo right;

L3G gyro;

//======================================
//===========Variables==================
//======================================

long oldPositionL  = -999;
long oldPositionR  = -999;
long newPositionL = myEncL.read();
long newPositionR = myEncR.read();

unsigned long timeBase = 0;

float disR = 0;
float disF = 0;

//======================================
//==========Setup=======================
//======================================


void setup(){  
  Serial.begin(115200);
  Serial.print("1");

  left.attach(11, 2000, 1000);
  right.attach(10, 2000, 1000);


  Serial.print("2");
  pinMode(botFlame, INPUT);
  pinMode(midFlame, INPUT);
  pinMode(topFlame, INPUT);
  pinMode(stopButton, INPUT);
  pinMode(stepdirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);  
  Serial.print("3");
  Wire.begin();
  timer = millis();
  gyro.enableDefault();
//  delay(1000);
//  for(int i = 0; i<100; i++){
//    gyro.read();
//    gerrz+=gyro.g.z;
//    delay(25);
//  }
  Serial.print("===");
  gerrz=gerrz/100;
}


//=====================================
//===========main loop=================
//=====================================


//  disR = sonicRight.distance();
//  disF = sonicMid.distance();

void loop(){
  //  double length = calInch((newPositionR+newPositionL)/2);
  //  if(length<70){
  //    //goWall();     
  //  }else{
  //    left.write(90);
  //    right.write(90);
  //  }
  rotateStep();

}



//======================================
//===========Helper functions===========
//======================================


void encoder() {

  newPositionL = myEncL.read();
  newPositionR = myEncR.read();
  if (newPositionL != oldPositionL) {
    oldPositionL = newPositionL;
  }
  if (newPositionR != oldPositionR) {
    oldPositionR = newPositionR;
  }

}

//======================================

double calInch(long tick){
  return (double)tick/3200*8.443;
}

//======================================

void rotateStep(){

  unsigned long Time = millis();
  if((Time-timeBase)>30){
    if((stepCount1 <150)&&(stepCount2 == 0)){      
      digitalWrite(stepdirPin, LOW);
      digitalWrite(stepPin,LOW);
      stepCount1++;
      stepCount2 = 0;
      digitalWrite(stepPin, HIGH);
      delay(10);
      digitalWrite(stepPin, LOW);
    }
    else if((stepCount1 == 150)&&(stepCount2 < 150)){
      digitalWrite(stepdirPin, HIGH);
      digitalWrite(stepPin,LOW);
      stepCount2++;
      digitalWrite(stepPin, HIGH);
      delay(10);
      digitalWrite(stepPin, LOW);
    }
    else if((stepCount1 == 150)&&(stepCount2 == 150)){
      stepCount1 = 0;
      stepCount2 = 0;
    }
    timeBase = Time;
  } 
}

//=========================================


int findZ(){

  int high = analogRead(topFlame);
  int mid = analogRead(midFlame);
  int low = analogRead(botFlame);

  if((high+mid+low)>3000){
    return 15;
  }

  if((high>mid)&&(mid>low)){
    return 5;    
  }
  else if((high<mid)&&(mid>low)){
    if(high>low){
      return 7;
    }
    else {
      return 9;
    }      
  }
  else if((high<mid)&&(mid<low)){
    return 11;
  } 
  else{
    return 0;
  } 
}

//=========================================

float readgyro (){
  if((millis()-timer)>=5){
    gyro.read();
    timer=millis();
    gyro_z=(float)(gyro.g.z-gerrz)*G_gain;
    gyro_z = gyro_z*G_Dt;
    gyro_z+=gyro_zold;
    gyro_zold=gyro_z;
  }
  return (gyro_z);
}




