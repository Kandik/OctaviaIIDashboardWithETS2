# OctaviaIIDashboardWithETS2
Backend for connecting Octavia II Dashboard to ETS2 using two Arduino Unos and ETS2 telemetry server


## Description

2020 project 

The dashboard in my car got broken (centre display blinking) and was changed. I did not have purpose for the broken one and I did not want to throw it away, so I took it apart and connected it to the computer using Arduino Uno. I also connected the Arduino Uno to ETS2 telemetry server to play ETS2/ATS with the dashboard.

The project uses two Arduino Unos with four stepper motor controllers. One Arduino (in Arduino/Arudino_main) is connected to the accelerometer and tachometer, and one (in Arduino/Arduino_tempfuel) to the coolant temperature and gas needle. The Arduinos are listening to the serial input, which is given via USB and a Python script - dashboard.py, which reads the data from ETS2 telemetry server (https://github.com/Funbit/ets2-telemetry-server) and sends it to Arduinos.


## Project video (in Slovak)
https://www.youtube.com/watch?v=4ZL53YbkdXY

[![Project video](https://img.youtube.com/vi/4ZL53YbkdXY/0.jpg)](https://www.youtube.com/watch?v=4ZL53YbkdXY)
