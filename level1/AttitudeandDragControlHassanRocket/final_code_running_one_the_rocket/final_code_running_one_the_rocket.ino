#include <Servo.h>

String inString = "";    // string to hold input
const int offset_b1 = 57;
const int offset_b2 = 55;
const int offset_b3 = 62;

const int offset_atti_s1 = 82;
const int offset_atti_s2 = 90;
const int offset_atti_s3 = 90;

 
unsigned long brakeSignal_time = 5000; 

// brake angles
int bangle = 80;
Servo servo_b1;
Servo servo_b2;
Servo servo_b3;

Servo servo_a1;
Servo servo_a2;
Servo servo_a3;


int inChar;
float altitude_readout = 0;
float mean_altitude = 0;
bool start_flag = 0;
double start_time = 0;
float get_altitude(){
  inString = "";
  while(1){
    if (Serial.available() > 0){
       inChar = Serial.read();
      if (inChar != '\n') {
        inString += (char)inChar;
      } else {
        return inString.toFloat();
             } 
    }
  }
}
  
void setup(){
 
  Serial.begin(57600);
  Serial.println("START");
  servo_b1.attach(3);
  servo_b2.attach(5);
  servo_b3.attach(6);

  servo_a1.attach(9);
  servo_a2.attach(10);
  servo_a3.attach(11);

  servo_b1.write(offset_b1); //50 TO 70 G00D
  servo_b2.write(offset_b2);
  servo_b3.write(offset_b3);

  servo_a1.write(offset_atti_s1); //50 TO 70 G00D
  servo_a2.write(offset_atti_s2);
  servo_a3.write(offset_atti_s3);

  while(1){
    if (Serial.available() > 0){
       inChar = Serial.read();
      if (inChar != '\n') {
        break;
      }
    }
  }
  int i;
  float sum = 0;
  for (i = 0; i < 100; i++)
  {
    sum = sum + get_altitude();
  }
  
  mean_altitude = sum/100;
  
  Serial.println("Calibration. Mean Altitude! ");
  Serial.println(mean_altitude);
  Serial.println();


  
  Serial.println("Servo calibrated at offset ");
}

void loop() {
  float altitude_readout = get_altitude();
  
  if (altitude_readout - mean_altitude >= 15 && start_flag == 0){
        start_time = millis();
        Serial.println("Set start flag");
        start_flag = 1;
      }
  
  Serial.println(start_time);
  double time_difference;
  time_difference = millis() - start_time;
  Serial.println(time_difference);
  if (start_flag == 1 && time_difference > brakeSignal_time){
    
    servo_b1.write(offset_b1+bangle);
    servo_b2.write(offset_b2+bangle);
    servo_b3.write(offset_b3+bangle);

  }

  //servo_a1.write(offset_atti_s1); //50 TO 70 G00D
  //servo_a2.write(offset_atti_s2);
  //servo_a3.write(offset_atti_s3);


   
}
      
      

     
    
    
    
      
    
    
  
  
  
