#include <Arduino.h>
#include "esp_camera.h"
#include "human_detection.h" // Provided by ESP-WHO library

#define FLASH_LED_PIN 4 // Flash GPIO for AI-THINKER ESP32-CAM

// Threshold to decide if the object is moving closer
// Ratio > 1.2 means the detected bounding box grew 20%
static const float APPROACH_THRESHOLD = 1.2f;

// Handle to the human detector (implementation dependent)
human_detection_t detector;

void setup() {
  Serial.begin(115200);
  pinMode(FLASH_LED_PIN, OUTPUT);
  digitalWrite(FLASH_LED_PIN, LOW);

  // Camera configuration for AI-THINKER ESP32-CAM
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = 5;
  config.pin_d1 = 18;
  config.pin_d2 = 19;
  config.pin_d3 = 21;
  config.pin_d4 = 36;
  config.pin_d5 = 39;
  config.pin_d6 = 34;
  config.pin_d7 = 35;
  config.pin_xclk = 0;
  config.pin_pclk = 22;
  config.pin_vsync = 25;
  config.pin_href = 23;
  config.pin_sccb_sda = 26;
  config.pin_sccb_scl = 27;
  config.pin_pwdn = 32;
  config.pin_reset = -1;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_RGB565;
  config.frame_size = FRAMESIZE_QVGA; // 320x240
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Camera init failed");
    return;
  }

  // Initialize the human detector
  human_detection_config_t hd_config;
  hd_config.frame_size = config.frame_size;
  human_detection_init(&detector, &hd_config);
}

void loop() {
  static float prevArea = 0.0f;
  bool approaching = false;

  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  detection_result_t result;
  bool person_found = human_detection_detect(&detector, fb->buf, fb->len, fb->width, fb->height, &result);

  if (person_found) {
    int w = result.box.right - result.box.left;
    int h = result.box.bottom - result.box.top;
    float area = (float)w * (float)h;

    if (prevArea > 0.0f && area > prevArea * APPROACH_THRESHOLD) {
      approaching = true;
    }
    prevArea = area;
  } else {
    prevArea = 0.0f;
  }

  if (approaching) {
    digitalWrite(FLASH_LED_PIN, HIGH);
    delay(100);
    digitalWrite(FLASH_LED_PIN, LOW);
  } else {
    digitalWrite(FLASH_LED_PIN, LOW);
    delay(20);
  }

  esp_camera_fb_return(fb);
}
