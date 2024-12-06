# crazythursday.github.io


    * Team Name: Crazy Thursday
    * Team Members: Zheyu Li, Shun Yao, Zhuozi Xie 
    * Github Repository URL: https://github.com/upenn-embedded/final-project-crazy-thursday.git
    * Github Pages Website URL: https://github.com/ssysy/crazythursday.github.io
    * Description of hardware: (embedded hardware, laptop, etc) 

## Final Project Report

Don't forget to make the GitHub pages public website!
If you’ve never made a Github pages website before, you can follow this webpage (though, substitute your final project repository for the Github username one in the quickstart guide):  <https://docs.github.com/en/pages/quickstart>

### 1. Video

<video src="Final demo.mov" controls width="640" height="360">
Final Demo
</video>

### 2. Images

[Insert final project images here]

### 3. Results

What were your results? Namely, what was the final solution/design to your problem?

#### 3.1 Software Requirements Specification (SRS) Results

Based on your quantified system performance, comment on how you achieved or fell short of your expected software requirements. You should be quantifying this, using measurement tools to collect data.

SRS 01 – In a single peer-to-peer setup, ESP device communicates directly with another ESP device by specifying its MAC address.

SRS 02 – Devices use MAC addresses to identify specific peers, allowing a single device to send data to multiple others, either as broadcast or to specific groups.

SRS 03 - Upon the signal send to computer, the system will generate the appropriate animation: bat swing, followed by the ball landing or catched animation.

Due to the limitaion of LCD, instead of ball landing or cathed animation we developed a show each stage with words on the screen with a ball flying animation.

SRS 04 - With the xyz acceleration and spatial angle acquired by the sensor, the trajectory and landing position of the baseball are calculated, and the result is judged by comparing the swing time and catch time.

#### 3.2 Hardware Requirements Specification (HRS) Results

Based on your quantified system performance, comment on how you achieved or fell short of your expected hardware requirements. You should be quantifying this, using measurement tools to collect data.

HRS 01 – Project shall be based on ATmega microcontroller using baremetal C

HRS 02 - Project wireless communication parts will be handelled by ESP

HRS 06 - One button switch shall be used at the Pitcher Glove to detect whether the 'ball' has been thrown out or not.
Modification & Improvement:

HRS 03 – A 6-DOF Orientation IMU shall be used for movement detection. The sensor shall detect the acceleration and angular velocity at the Pitcher Glove and the Hit Bat.

HRS 04 – An LCD used for user interface. The display shall communicate with the microcontroller via UART bus.

HRS 05 - A 5V and 3.3V power supply to power up the ESP and ATmega.

### 4. Conclusion

Reflect on your project. Some questions to consider: What did you learn from it? What went well? What accomplishments are you proud of? What did you learn/gain from this experience? Did you have to change your approach? What could have been done differently? Did you encounter obstacles that you didn’t anticipate? What could be a next step for this project?
