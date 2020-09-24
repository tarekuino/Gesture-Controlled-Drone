# Gesture-Controlled-Drone

Project Description:

Ability to control a drone using hand gestures.

The project has two main parts:

1) Glove Side (Transmitter):
  a) Responsible for translating hand movements to flight commmands.
  b) Flight Commands are transmitted over RF (XBEE) to the drone side
  
2) Drone Side (Receiver):
   a) Receives flight commands from XBEE module.
   b) Send flight commands to flight controller (DJI NAZA) as PWM pulses that mimic RC commands.
