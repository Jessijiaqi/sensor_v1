#include <Arduino.h>
#include "esp_camera.h"
#include <WiFi.h>
#include <SPI.h>

// Define the HC-SR501 motion sensor pin
#define PIR_PIN 13 // Modify according to the actual connected pin
// Set your WiFi information
const char* ssid = "jqqn";
const char* password = "570882590";

void startCameraServer();

// ESP32-CAM camera configuration, adjust according to your camera model
camera_config_t camera_config = {
  .pin_pwdn  = -1,
  .pin_reset = -1,
  .pin_xclk = 0,
  .pin_sccb_sda = 26,
  .pin_sccb_scl = 27,
  .pin_d7 = 35,
  .pin_d6 = 34,
  .pin_d5 = 39,
  .pin_d4 = 36,
  .pin_d3 = 21,
  .pin_d2 = 19,
  .pin_d1 = 18,
  .pin_d0 = 5,
  .pin_vsync = 25,
  .pin_href = 23,
  .pin_pclk = 22,
  .xclk_freq_hz = 20000000,
  .ledc_timer = LEDC_TIMER_0,
  .ledc_channel = LEDC_CHANNEL_0,
  .pixel_format = PIXFORMAT_JPEG,
  .frame_size = FRAMESIZE_SVGA,
  .jpeg_quality = 12,
  .fb_count = 1
};

void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT); // Set the PIR motion sensor pin as input
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi Connected. IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize the camera
  esp_err_t err = esp_camera_init(&camera_config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  // Start the camera server
  startCameraServer();
  Serial.println("Camera Ready!");
}

void loop() {
  static unsigned long lastMotionDetected = 0; // Record the last time motion was detected
  bool motionDetected = digitalRead(PIR_PIN) == HIGH;

  if (motionDetected) {
    if (lastMotionDetected == 0) { // If it's the first time motion was detected
      lastMotionDetected = millis();
    } else if ((millis() - lastMotionDetected) > 5000) { // If motion continues for more than 5 seconds
      Serial.println("Motion detected continuously for 5 seconds. Taking photo...");
      
      camera_fb_t * fb = esp_camera_fb_get(); // Get a frame
      if (!fb) {
        Serial.println("Camera capture failed");
      } else {
        // Here, process the captured photo, such as saving or uploading
        // Note: The specific processing logic needs to be implemented according to your needs

        esp_camera_fb_return(fb); // Return the frame buffer
      }

      lastMotionDetected = 0; // Reset detection time
    }
  } else {
    lastMotionDetected = 0; // If no motion is detected, reset the time
  }
}

// Note: This startCameraServer function is a placeholder, in reality, you need to use the function provided by the ESP32-CAM library to start the web server
void startCameraServer() {
  // Your code to start the camera server, this usually depends on the library you are using
  Serial.println("Camera Web Server Started.");
}
