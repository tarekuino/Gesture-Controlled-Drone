#include <SoftwareSerial.h>
#include<Wire.h>

//Acceleration values from the accelerometer
int accx, accy, accz, gyrx, gyry, gyrz;

//Angle values
int xangle, yangle, zangle;

//Values to be trasmitted to the drone
int fakex, fakey, fakez;

//Flight Mode
bool mode=0;

bool canIcomein = 1;

//ROLL parameter flag
String rollFlag = "a";

//PITCH parameter flag
String pitchFlag = "b";

//YAW parameter flag
String yawFlag = "c";

//Throttle parameter flag
String throttleFlag = "d";

String gx,gy,gz,tu;

int t_up , yaw , t_up_temp , yaw_temp;

int g = 1;

//Use software serial to communicate with XBEE module on pins 11,9 from the ARDUINO
SoftwareSerial xbee(11, 9);

void setup() {

//SETUP Routine to Init. MPU6050
Wire.begin();

//Address of the MPU chip (0x68)
Wire.beginTransmission(0x68);

//Address of the required register
Wire.write(0x6B);

//Write required value to the register
Wire.write(00001000); //reset mpu6050 & disable temp.

Wire.endTransmission();
delay(200);

Wire.beginTransmission(0x68);
Wire.write(0x6C);
Wire.write(0);
Wire.endTransmission();


Wire.beginTransmission(0x68);
Wire.write(0x1C);
Wire.write(00011000); //set Acc. full scale to +- 4G, 8192 LSB/G
Wire.endTransmission();

Wire.beginTransmission(0x68);
Wire.write(0x1B);
Wire.write(00011000); //set gyro full scale to +- 2000 deg/sec
Wire.endTransmission();

xbee.begin(115200);

Serial.begin(9600);
}


void loop() {

//Determine flight mode
if(analogRead(A3) > 700)
    mode = !mode;

//This mode the values sent to the drone have a bigger range (PRO MODE)
if(!mode ) 
{
  
  //read analog voltage from the THROTTLE flex sensor
  t_up = analogRead(A2);

  //read analog voltage from the YAW flex sensor
  yaw = analogRead(A4); 
  
  digitalWrite(A5,LOW);

  //___reading ACCELEROMETER data
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.beginTransmission(0x68);

  //Read 6 bytes
  Wire.requestFrom(0x68,6 );
  
  accx  = Wire.read()<<8|Wire.read();

  accy  = Wire.read()<<8|Wire.read(); 

  accz  = Wire.read()<<8|Wire.read(); 
  
  //convert ACCELEROMETER data to angles  
  xangle = (180* (atan(accx / (sqrt(square(accy) + square(accz))))))/M_PI;
  
  yangle = (180*(atan(accy / (sqrt(square(accx) + square(accz))))))/M_PI;
  
  zangle = (180*(atan(sqrt(square(accx) + square(accy))/accz)))/M_PI;
  
  xangle = map(xangle ,-40,30 ,1000 , 2000);
  
  yangle = map(yangle ,-20,50 ,1000 , 2000);

  xangle = constrain(xangle,1000 , 2000);
  
  yangle = constrain(yangle,1000 , 2000);

  t_up = map(t_up , 380,200,1000,2000);
  t_up = constrain(t_up,1000,2000);

  yaw = map(yaw , 370,260,1000,2000);
  yaw = constrain(yaw,1000,2000);

  t_up_temp = t_up; yaw_temp = yaw;
  yaw = 1000;

  gx = String(xangle,DEC);
  gy = String(yangle,DEC);
  gz = String(yaw,DEC);
  tu = String(t_up,DEC);

  
  xbee.print(rollFlag); xbee.print(gx);
  xbee.print(pitchFlag); xbee.print(gy);
  xbee.print(yawFlag); xbee.print(gz);
  xbee.print(throttleFlag); xbee.print(tu);
  xbee.println();
}//end of zero

//////////////////////

//This mode the values sent to the drone are very limited (EASY TO FLY)
else if(mode)
  {
    analogWrite(A5,255);
    t_up = analogRead(A2);
    yaw = analogRead(A4);
  
//___reading ACCELEROMETER data
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.beginTransmission(0x68);

  //Read 6 bytes
  Wire.requestFrom(0x68,6 );
  
  accx  = Wire.read()<<8|Wire.read();

  accy  = Wire.read()<<8|Wire.read(); 

  accz  = Wire.read()<<8|Wire.read(); 
  
  //convert ACCELEROMETER data to angles  
  xangle = (180* (atan(accx / (sqrt(square(accy) + square(accz))))))/M_PI;
  
  yangle = (180*(atan(accy / (sqrt(square(accx) + square(accz))))))/M_PI;
  
  zangle = (180*(atan(sqrt(square(accx) + square(accy))/accz)))/M_PI;

  //convert control parameters to values suitable for PWM pulses 
  //which range between 1000 & 2000.
  //These values will be mimicing RC commands
  
  xangle = map(xangle ,-40,30 ,1000 , 2000);
  
  yangle = map(yangle ,-20,50 ,1000 , 2000);

  xangle = constrain(xangle,1000 , 2000);
  
  yangle = constrain(yangle,1000 , 2000);

  t_up = map(t_up , 380,200,1000,2000);
  t_up = constrain(t_up,1000,2000);

  yaw = map(yaw , 370,260,1000,2000);
  yaw = constrain(yaw,1000,2000);

  t_up_temp = t_up; yaw_temp = yaw;

  //Limit the controls to certain values 
  if(xangle < 1100)
    fakex = 1700;
  else if(xangle>1650)
    fakex = 1300;
  else 
    fakex = 1500;
  /////
  if(yangle < 1300)
    fakey = 1300;
  else if(yangle>1800)
    fakey = 1700;
  else 
    fakey = 1500;
  /////////////
  if(yaw < 1100)
    fakez = 1300;
  else if(yaw>1700)
    fakez = 1700;
  else 
    fakez = 1500;

  
  gx = String(fakex,DEC);
  gy = String(fakey,DEC); 
  gz = String(accz,DEC);
  tu = String(t_up,DEC);
  
  //Transmit control values to the drone xbee receiver
  xbee.print(rollFlag); xbee.print(gx);
  xbee.print(pitchFlag); xbee.print(gy);
  xbee.print(yawFlag); xbee.print(gz);
  xbee.print(throttleFlag); xbee.print(tu);
  xbee.println();
  
 }
}
