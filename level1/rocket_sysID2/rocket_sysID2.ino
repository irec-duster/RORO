
#include <Servo.h>

//const int magSwitch = 2;
//const int ledStandby = 7;

// Offset to center servos about fins;
const int offset_s1 = 82;
const int offset_s2 = 99;
const int offset_s3 = 90;

const int offset_b1 = 57;
const int offset_b2 = 55;
const int offset_b3 = 62;

unsigned long Time, standbyTime;
//Assign signal application points


unsigned long stepSignal_time1 = 1500;
unsigned long stepSignal_time2 = 3500;
unsigned long brakeSignal_height = 300; 
float h0;
// brake angles
int bangle = 80;
int angle = 25;
Servo servo_1;
Servo servo_2;
Servo servo_3;
Servo servo_b1;
Servo servo_b2;
Servo servo_b3;
void setup(){
  //pinMode(7,OUTPUT);
  //pinMode(2,INPUT);
  ////Aelirons
  servo_1.attach(9);
  servo_2.attach(10);
  servo_3.attach(11);
  //brakes
  servo_b1.attach(3);
  servo_b2.attach(5);
  servo_b3.attach(6);
  Serial.begin(9600);
  //Time in millisecons
  
  h0 = 347; // what ever the preasure sensor gives 

  
  
  
}


void loop() {
  // put your main code here, to run repeatedly:
 

    // Standby mode
  //while(digitalRead(magSwitch) == HIGH){
      servo_1.write(offset_s1);
      servo_2.write(offset_s2);
      servo_3.write(offset_s3);
      
      servo_b1.write(offset_b1); //50 TO 70 G00D
      servo_b2.write(offset_b2);
      servo_b3.write(offset_b3);
      standbyTime = millis();
      Serial.print("State high");
      Serial.println(standbyTime);
      delay(500);
  //}
  //Launch
   /*while(digitalRead(magSwitch) == LOW){
     Serial.print("State low");
        delay(50);

     //time after launch
     Time = millis() -standbyTime;
     Serial.println(Time);
      if(Time < stepSignal_time1){
        servo_1.write(offset_s1);
        servo_2.write(offset_s2);
        servo_3.write(offset_s3);
        servo_b1.write(offset_s1);
        servo_b2.write(offset_s2);
        servo_b3.write(offset_s3);
        digitalWrite(ledStandby, LOW);
           Serial.print("motor fire");
     }
      else if( Time < stepSignal_time2){
      
        servo_1.write(offset_s1 + angle);
        servo_2.write(offset_s2 + angle);
        servo_3.write(offset_s3 + angle);
        servo_b1.write(offset_s1);
        servo_b2.write(offset_s2);
        servo_b3.write(offset_s3);
        Serial.print("flaps");

     }
          
      else if( Time < brakeSignal_time){
      
        servo_1.write(offset_s1 - angle);
        servo_2.write(offset_s2 - angle);
        servo_3.write(offset_s3 - angle);
        servo_b1.write(offset_s1);
        servo_b2.write(offset_s2);
        servo_b3.write(offset_s3);
        Serial.print("flaps2");
     }
      else if ( Time > brakeSignal_time) {
      
        servo_1.write(offset_s1 );
        servo_2.write(offset_s2 );
        servo_3.write(offset_s3 );
        servo_b1.write(offset_s1+bangle);
        servo_b2.write(offset_s2+bangle);
        servo_b3.write(offset_s3+bangle);
        
        Serial.print("break");
        
     }
   }*/

   
}
      
      

     
    
    
    
      
    
    
  
  
  
