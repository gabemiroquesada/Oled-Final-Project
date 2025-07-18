#  OLED Pong Game on TM4C123 Tiva Launchpad

A fun, embedded systems project written in Embedded C for the TM4C123 Tiva Launchpad, featuring a classic Pong game on a 128x64 OLED screen via I²C communication.

##  Project Overview

This project showcases a simple yet engaging implementation of the Pong game using:
- **Embedded C**
- **I²C communication protocol**
- **OLED 128x64 display**
- **Push-button input for player control**

Originally inspired by a previous challenge with OLED displays on an FPGA, this project successfully demonstrates OLED control using the Tiva Launchpad and includes both **player-vs-player** and **player-vs-CPU** modes.

##  Hardware Requirements

- TM4C123 Tiva Launchpad
- 128x64 OLED display (I²C interface)
- Breadboard
- Push buttons (configured with negative logic)
- Resistors and jumper wires

##  Key Features

-  OLED initialization and display control via I²C
-  Pong gameplay with ball physics and scoring logic
-  Player-vs-player mode with dual button input
-  Player-vs-CPU mode with basic AI paddle behavior
-  Bonus: In-progress Galaga-like game 

## 📷 Gameplay Preview

Player paddle and CPU paddle bounce the ball across the screen. First to 3 points wins. Game displays on OLED with real-time updates.

## Code Highlights

- **I²C Setup**: Custom implementation of I²C3 write and control functions
- **OLED Driver**: Initialization sequence, buffer updates, pixel drawing
- **Game Logic**: Ball physics, paddle movement, collision detection, scoring

## Credits

Developed by Gabe Miroquesada  
University of Tennessee, Knoxville  
ECE 455 Final Project  
Mentored by Dr. Himanshu Thapliyal

## 📚 References

- [Microcontrollers Lab – OLED Interfacing with TM4C123G](https://microcontrollerslab.com/oled-interfacing-with-tm4c123g-display-texts-and-graphics/)
- [Hackster.io – OLED Pong Game for Arduino UNO](https://www.hackster.io/kelly77/oled-0-96-inch-display-pong-game-for-arduino-uno-r3-02fdfb)
- [Embedded Systems – Shape The World (UT Austin)](https://users.ece.utexas.edu/~valvano/Volume1/E-Book/)

---
