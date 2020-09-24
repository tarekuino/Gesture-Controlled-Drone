# Gesture-Controlled-Drone

Project Description:

The project has two main parts:

1) Glove Side (Transmitter):
   - Responsible for translating hand movements to flight commmands.
   - Flight Commands are transmitted over RF (XBEE) to the drone side
  
2) Drone Side (Receiver):
   - Receives flight commands from XBEE module.
   - Send flight commands to flight controller (DJI NAZA) as PWM pulses that mimic RC commands.

Hardware:

1) DJI NAZA Flight Controller
2) Arduino MEGA
3) Arduino LillyPad (GLOVE)
4) MPU6050
5) XBEE (RF Module)
