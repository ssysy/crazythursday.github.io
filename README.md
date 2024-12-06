# Developed by Crazy Thursday

- Team Name: Crazy Thursday
- Team Members: Zheyu Li, Shun Yao, Zhuozi Xie
- Github Repository URL: https://github.com/upenn-embedded/final-project-crazy-thursday.git
- Github Pages Website URL: https://github.com/ssysy/crazythursday.github.io
- Description of hardware: ATmega328PB, ESP32 S2 Feather, 6-DOF IMU, LCD, LED, Buzzer

## Introduction

Swingtronics is a **motion sensing game controller** that can be held in one hand. By detecting the player's movement as an input, it is converted into a digital signal and transmitted wirelessly to a portable processor. All devices are **portable** and **do not require WiFi**. Just have batteries ready to play in **indoor/outdoor environments**!

Minimum of just one game controller and one portable processor to play the game (Multiplayer/multi-input device is also supported). Currently, a baseball simulation game has been developed, which requires 2 players with 1 processor. More game (tree-cutting, sword battle etc.) is on the way!

### 1. Video

[Youtube Demo Video](https://www.youtube.com/watch?v=_IhdffkUVsc)

### 2. Images

#### Game controller 1 (Input device/Pitcher)

![alt text](images/Pitcher_case.jpg)

#### Game controller 2 (Input device/Hit Bat)

![alt text](images/HitBat_case.jpg)

#### Portable Central Processor

![alt text](images/Processor.jpg)

### 3. Results

We finally have three devices, Pitcher, Hit Bat, and Score board. Each device have a Atmaga328Pb, ESP32, level shifter, and buck converter. Pitcher and Hit bat have an IMU to detect the motion of the device, which will be used to calculate the winner of the game. Score board have a LCD screen, buzzer, and two light bulbs. The Score board will receive data from Pitcher and Hit bat to determine who win the game, and display the corresponding anime on LCD and light it bulb. The buzzer is a notification that if the Pitcher failed to throw the ball and asked for retry. The game will end if a player get 5 score.

### 4 Software Requirements Specification (SRS) Validation

- SRS 01 – Two microcontroller needs to be able to communicate with each other without external WiFi connection. Communication delay should be less than 10ms. In our project, ESP32 device communicates directly with another ESP32 by specifying its MAC address.

- SRS 02 – Mutiple MCU devices required to be able to identify others, allowing one Master device able to communicate with mutiple slave diveces. This project use ESP32 MAC addresses to identify specific peers, allowing a single device to receive data from multiple others.

- SRS 03 - The Master MCU device needs to be able to generate a appropriate animation or score board on an LCD or equivlent display based on the signal send from slave devices.

- SRS 04 - With the acceleration on X and Y axis of IMU, one calculation algorithm that can calculate the trajectory and timing slot between the Pither and HitBall needs to be programmed.

- SRS 05 - The timing and predicted trajectory are used to determine which player win the points, also the gaming logic will be programmed within one project.

### 5 Hardware Requirements Specification (HRS) Validation

- HRS 01 – Microcontroller with enough communication ports to establish at least three communicaiton channels. This project is based on ATmega328PB microcontroller coded with baremetal C.

- HRS 02 - Microcontroller with WiFi communication ability or equivlent. Here wireless communication parts will be handelled by ESP32 S2 Feather through ESP-NOW protocol.

- HRS 03 – A 6-DOF orientation IMU shall be used for movement detection. The sensor shall measure acceleration and angular velocity on the Pitcher Glove and the Hit Bat, with a detection range of at least -4g to 4g and a resolution of 2^16 steps.

- HRS 04 – An LCD used for user interface. The display shall be able to communicate with the microcontroller via UART, I2C or equivalent communication protocol.

- HRS 05 - A power supply that can provide rated power for the used mircocontrollers, in this case: 3.3V for ESP32 S2 Feather and 5V for ATmega328PB.

- HRS 06 - One push button (momentary switch) shall be used at the Pitcher Glove to detect whether the 'ball' has been thrown out or not.

- HRS 07 - Actuators: LED, Buzzer, etc. could be added to any socre board devices as player interfaces. This project used two LEDs and one buzzer on socre board.

### 4. Conclusion

### Current Outcomes

What did you learn from it? 
What went well? 
What accomplishments are you proud of? 
What did you learn/gain from this experience?

### Encountered Issues
- High Complexity of calculating velocity and angle (Kalman filtering and quaternions can be used to improve detection accuracy)
- Short Battery life (Final demo: Playable for only 10-15 mins per battery cycle)
- Conflicts caused by multiplexing of communication protocol pin (UART1 and SPI0, we therefore change to SPI1)
- Game logics can be complicated and cause some bugs.

### Next steps

- More Reliable, Longer Lasting, Rechargeable Power Supply Solutions
- Better visualization (Larger LCD etc.)
- More Robust Hardware Connection (PCB, More fitted cases)
- More Available Games 
