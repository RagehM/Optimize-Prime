# Optmimize Prime

> Smart Self Transforming Robot

<div align="center">
    <img src="https://i.imghippo.com/files/aue3999Hec.png" alt="Optimus" width="300"/>
</div>

## 📑 Table of Contents

1. [About the Project](#about-the-project)
2. [Features](#features)
3. [Components](#Components)
4. [What is in the Repo](#repo)
5. [Currently Working on](#currently-working-on)
6. [The Team Behind this Project](#Team)
7. [Acknowledgements](#acknowledgements)
8. [Youtube Playlist](#Youtube)
9. [License](#license)

## 💡 About the Project <a name="about-the-project"></a>

This project features a unique robot capable of seamlessly transitioning between a humanoid form and a vehicle form. Standing 70 cm tall and weighing 3.25 kg, the robot combines entertainment and interactivity with practicality.

The robot is designed to provide a fun and engaging user experience, recognizing faces, waving hello, and responding to voice commands like “Transform” using TinyML technology. Entirely wireless, it can be controlled through a web interface hosted on a Pico W, making it adaptable to various environments and perfect for delivering entertainment with a practical twist.

## 🚀 Features <a name="features"></a>

- Transformation Functionality from Humanoid to Car and Vice Versa
- IoT integration and I2C communication between boards are successfully implemented.
- TinyML has been successfully implemented on the microphone to recgonize The word "Transform"
- Pico W Web server that controls the robot wirelessly and shows if there is a human in from of him real-time

<div align="center">
    <img src="https://i.imghippo.com/files/STi5659ys.png" alt="" border="0">
</div>

## 🗂️ What is in the Repo <a name="repo"></a>

- **Bottom: Arduino Nano RP 2040; contains logic for the bottom half of the robot**
  - Servo Cluster: code to control multiple servos simultaneously
  - Servo.h, Servo.cpp: main code to initialize the servos
- **Microphone: Mic Driver for Arduino Nano RP 2040 in a separate project**
  - Microphone.h, Microphone.c: Arduino Nano RP 2040 Mic Module Driver
- **IOT: RP Pico W Project Directory; contains logic for the top half of the robot and IOT integration**
  - Camera.h, Camera.cpp: Face Detection Module Driver
  - WIFI: driver to enable the WIFI connection to control all operations for transformations and camera detection
- **Top (included in IOT Folder): Raspberry Pi Pico Project Directory; contains logic for the top half of the robot**
  - Servo Cluster: code to control multiple servos simultaneously
  - Servo.h Servo.cpp: the main code to initialize the servos
  - DC Motor Control (included in IOT Folder): Rasperry Pi Pico; contains the logic for the vehicle form movement
  - DcMotor.h DcMotor.cpp: DC Motor Driver
- **Communication and I2C**
  - Communication: sends the WIFI transformation signal to the Arduino Nano RP2040
  - I2C: get a message from the Master (Arduino Nano RP2040) to perform some operation
- **Face Recognition Module: RP 3 Model B**
  - The code in the Raspberry PI 3 that uses OpenCV for facial recognition

## 🚧 Currently Working On <a name="currently-working-on"></a>

- **Enable I2C on a read req from the master.**
- **Solving the RUN TIME ERROR after waving.**
- **Migrating to FreeRTOS.**
- **Conduct more trials and adjust mechanisms or control algorithms for better fluidity in transformations.**
- **Neural Network Classifier**
  - Expand the dataset to improve the NN classifier’s reliability.
  - Optimize and tweak it in a real-world environment for robustness.
- **Facial Recognition System**
  - Further fine-tune to ensure consistent performance under varying conditions.
    Next Steps
  - Schedule systematic testing of each functionality under real-world conditions.
  - Monitor performance to identify weak spots and refine both hardware and software aspects

## 🤝 The team <a name="Team"></a>

Marawan Abdelrahman

Mohamed Rageh

Omar Riad

Mohamed Gamal

Omar Farouk

Mahmoud Hegazy

## 🎥 Youtube Playlist <a name="Youtube">

[Click Here](https://youtube.com/playlist?list=PLkCAPNfoQVagmsrvh149anhgtYPH4tno4&si=05gJKN6DMOV7_OrY)

## ⚖️ License <a name="license">

The Project is following [MIT License](https://opensource.org/license/mit)
