# Camera Track

This repository contains a simple Arduino sketch for the AI-THINKER ESP32-CAM board. The sketch uses computer vision to detect a person in the camera frame and blinks the onboard flash LED if the person appears to be moving toward the camera.

The detection logic relies on the [ESP-WHO](https://github.com/espressif/esp-who) library to obtain a bounding box of the detected person. When the area of the detected bounding box grows by more than 20% compared to the previous frame, the flash LED blinks briefly.

## File Structure

- `src/PersonTracker.ino` – main Arduino sketch implementing camera setup, person detection and LED control.

## Usage

1. Install the ESP32 board support in the Arduino IDE and the ESP-WHO library.
2. Open `src/PersonTracker.ino` in the Arduino IDE.
3. Select the `AI Thinker ESP32-CAM` board and flash the code to the module.
4. Mount the camera at a slight downward angle. When a person approaches and grows in the frame, the flash LED will blink.

This sketch is a minimal example and may require tuning of detection thresholds and camera parameters for your specific setup.
