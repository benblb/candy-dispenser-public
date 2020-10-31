# Contactless candy dispenser

This is the code and 3D print files associated with a 3D-printed candy dispenser created during COVID-19 Halloween. The program uses a sonar sensor for contactless candy distribution. 

# Code

Designed to run on a Node MCU, the program distributes one piece of candy and send the admin a Telegram message when candy is running low. The program is provided here for anyone who would like to make a similar (or exactly the same) machine. 

1) User holds hand in front of sonar sensor
2) Candy is pushed out front slot of enclosure using a rack + pinion driven by a stepper motor. LEDs animate as candy dispenses. 
3) User takes candy and machine waits for next sonar sensing. LEDs flash to indicate "ready" status. 

By default the sonar range is 15 cm. Digital pins 5 and 6 are used for the sonar sensor. Digital pins 1-4 are used for the stepper motor. Digital pin 8 is used for the LEDs. 

Equipment needed:
Node MCU
Stepper motor + driver board
Sonar sensor
Wires
5v power supply (aim for >1A), I used a USB cable that I soldered some pin connectors onto to make power and ground nodes. 
