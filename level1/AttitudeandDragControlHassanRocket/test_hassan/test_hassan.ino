
String inString = "";    // string to hold input
#include <Servo.h>

Servo myservo1;  // create servo object to control a servo
Servo myservo2;  // create servo object to control a servo
Servo myservo3;  // create servo object to control a servo

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(56700);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  myservo1.attach(3);  // attaches the servo on pin 9 to the servo object
  myservo2.attach(4);  // attaches the servo on pin 9 to the servo object
  myservo3.attach(5);  // attaches the servo on pin 9 to the servo object


  // send an intro:
}

void loop() {
  // Read serial input:
  while (Serial.available() > 0) {
    int inChar = Serial.read();

    if (inChar != '\n') { 

      // As long as the incoming byte
      // is not a newline,
      // convert the incoming byte to a char
      // and add it to the string
      inString += (char)inChar;
    }
    // if you get a newline, print the string,
    // then the string's value as a float:
    else {
      Serial.print("Input string: ");
      Serial.print(inString);
      Serial.print("\tAfter conversion to float:");
      float altitude = inString.toFloat();
      Serial.println(altitude);
      if (altitude > 331){
        myservo1.write(30);
        myservo2.write(30);
        myservo3.write(30);
        
      }else{
        myservo1.write(60);
        myservo2.write(60);
        myservo3.write(60);
        }
      // clear the string for new input:
      inString = "";
    }
  }
}
