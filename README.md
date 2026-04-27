## ESP32 RC Car Camera

Remote-controlled car with ESP32-CAM that streams live video over WiFi and allows web-based control of movement and steering.

## Features
- Live video stream via HTTP
- Web interface for camera view (stream.html)
- Motor control (forward, backward, stop)
- Servo steering control (left/right/center)
- ESP32-CAM embedded server

## Tech Stack
- ESP32 (Arduino framework)
- C++ (firmware)
- HTML (web interface)
- HTTP server (ESP-IDF / Arduino libs)

## Project Structure
- src/ → firmware code
- web/ → web interface
- config/ → ESP32 configuration
- ci/ → build configuration

## Goal
This project demonstrates embedded systems programming, IoT communication, and real-time video streaming on a mobile robot platform.
