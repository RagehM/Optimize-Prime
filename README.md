# Optmimize Prime

> Smart Self Transforming Robot

<div align="center">
    <img src="https://i.ibb.co/r5sXBBm/aue3999-Hec.png" alt="Optimus" width="300"/>
</div>

## 📑 Table of Contents

1. [About the Project](#about-the-project)
2. [Badges](#badges)
3. [Features](#features)
4. [Components](#Components)
5. [What is in the Repo](#repo)
6. [Currently Working on](#currently-working-on)
7. [The Team Behind this Project](#Team)
8. [Acknowledgements](#acknowledgements)
9. [Youtube Playlist](#Youtube)
10. [License](#license)

## 💡 About the Project <a name="about-the-project"></a>

This project features a unique robot capable of seamlessly transitioning between a humanoid form and a vehicle form. Standing 70 cm tall and weighing 3.25 kg, the robot combines entertainment and interactivity with practicality.

The robot is designed to provide a fun and engaging user experience, recognizing faces, waving hello, and responding to voice commands like “Transform” using TinyML technology. Entirely wireless, it can be controlled through a web interface hosted on a Pico W, making it adaptable to various environments and perfect for delivering entertainment with a practical twist.

## Badges <a name="badges"></a>

![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Raspberry Pi](https://img.shields.io/badge/Raspberry%20Pi-A22846?style=for-the-badge&logo=raspberry-pi&logoColor=white)
![Arduino](https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=arduino&logoColor=white)
![HTML5](https://img.shields.io/badge/HTML5-E34F26?style=for-the-badge&logo=html5&logoColor=white)
![CSS3](https://img.shields.io/badge/CSS3-1572B6?style=for-the-badge&logo=css3&logoColor=white)
![JavaScript](https://img.shields.io/badge/JavaScript-F7DF1E?style=for-the-badge&logo=javascript&logoColor=black)
![Python](https://img.shields.io/badge/Python-3776AB?style=for-the-badge&logo=python&logoColor=white)
![Edge Impulse](https://img.shields.io/badge/Edge%20Impulse-009FDF?style=for-the-badge&logo=edge-impulse&logoColor=white)
![TensorFlow](https://img.shields.io/badge/TensorFlow-FF6F00?style=for-the-badge&logo=tensorflow&logoColor=white)

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
