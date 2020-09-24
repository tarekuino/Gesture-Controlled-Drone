String inString = "";

String rollString = "";

String pitchString = "";

String yawString = "";

//THROTTLE (tu)
String tuString = "";

//PWM values to be applied to DJI NAZA control ports that mimic RC commands
int roll, pitch, yaw, t;

int flag;

#include <Servo.h>

Servo Roll; //Aieleron
Servo Pitch; //Elevator
Servo Throttle; //Throttle
Servo Yaw; //Rudder

void setup() {

  Serial.begin(115200); 

  //Attach ROLL control port from NAZA Flight Controller to pin '9' on the Arduino
  Roll.attach(9);
  
  //Attach PITCH control port from NAZA Flight Controller to pin '10' on the Arduino
  Pitch.attach(10);
  
  //Attach THROTTLE control port from NAZA Flight Controller to pin '11' on the Arduino
  Throttle.attach(11);
  
  //Attach YAW control port from NAZA Flight Controller to pin '12' on the Arduino
  Yaw.attach(12);
}

void loop() {
  
  while (Serial.available() > 0) {

    //Store received messages
    int inChar = Serial.read();

    //97,98,99,.. corresponds to 'a,b,c,..' from the ASCI table 

    //Check the flag for the desired control to be affected (PITCH,ROLL,pitchAW,THROTTLE)
    
    if (inChar == 97)  //97 -> Identifier for the ROLL parameter
      flag = 97;
    else if (inChar == 98)  //98 -> Identifier for the PITCH parameter
      flag = 98;
    else if (inChar == 99)  //99 -> Identifier for the YAW parameter
      flag = 99;
    else if (inChar == 100) //100 -> Identifier for the THROTTLE parameter
      flag = 100;
      
    if (isDigit(inChar) && id == 97) {
      // convert the incoming bpitchte to a char
      // and append it to the string:
      rollString += (char)inChar;

    }


    else if (isDigit(inChar) && flag == 98) {
      // convert the incoming bpitchte to a char
      // and append it to the string:
      pitchString += (char)inChar;

    }


    else if (isDigit(inChar) && flag == 99) {
      // convert the incoming bpitchte to a char
      // and append it to the string:
      yawString += (char)inChar;

    }

    else if (isDigit(inChar) && flag == 100) {
      // convert the incoming bpitchte to a char
      // and append it to the string:
      tuString += (char)inChar;

    }

    else if (inChar == '\n')
    {
      roll = rollString.toInt();
      pitch = pitchString.toInt();
      yaw = yawString.toInt();
      t = tuString.toInt();
      
      //WRITE THE PWM SIGNAL TO DJI NAZA (FLIGHT CONTROLLER)
      Roll.writeMicroseconds(roll);
      Pitch.writeMicroseconds(pitch);
      Throttle.writeMicroseconds(t);
      Yaw.writeMicroseconds(yaw);

      //Clear strings
      rollString = "";
      pitchString = "";
      yawString = "";
      tuString = "";
    }
  }
}
