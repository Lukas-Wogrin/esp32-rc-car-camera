#include "esp_camera.h"
#include <WiFi.h>
#include <ESP32Servo.h>
#include "esp_http_server.h"
#include "board_config.h"
#include "config.h"

// WLAN
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

// Motor Pins
#define MOTOR_FWD 12
#define MOTOR_BWD 13

// Servo Pin
#define SERVO_PIN 14

Servo steering;

// HTTP Server
httpd_handle_t server = NULL;

// ================= MOTOR / SERVO =================
void stopAll() {
  digitalWrite(MOTOR_FWD, LOW);
  digitalWrite(MOTOR_BWD, LOW);
}

void forward() {
  digitalWrite(MOTOR_FWD, HIGH);
  digitalWrite(MOTOR_BWD, LOW);
  Serial.println("F");
}

void backward() {
  digitalWrite(MOTOR_FWD, LOW);
  digitalWrite(MOTOR_BWD, HIGH);
  Serial.println("B");
}

void left() {
  steering.write(60);
  Serial.println("L");   // links
}

void right() {
  steering.write(120);
  Serial.println("R");  // rechts
}

void center() {
  steering.write(90);   // gerade
}

// ================= HTTP HANDLER =================
static esp_err_t move_handler(httpd_req_t *req) {
    char buf[50];
    if (httpd_req_get_url_query_str(req, buf, sizeof(buf)) == ESP_OK) {

        char dir[10];
        if (httpd_query_key_value(buf, "dir", dir, sizeof(dir)) == ESP_OK) {

            if (!strcmp(dir, "w")) forward();
            else if (!strcmp(dir, "s")) backward();
            else if (!strcmp(dir, "a")) left();
            else if (!strcmp(dir, "d")) right();
            else if (!strcmp(dir, "stop")) {
                stopAll();
                center();
            }
        }
    }

    httpd_resp_send(req, "OK", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

// ================= CAMERA SERVER =================
void startCameraServer();

// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  // Motor Pins
  pinMode(MOTOR_FWD, OUTPUT);
  pinMode(MOTOR_BWD, OUTPUT);

  // Servo
  steering.attach(SERVO_PIN);
  center();

  // Kamera Config (SCHNELL)
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;

  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  // ⚡ SPEED SETTINGS
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 20;
  config.fb_count = 2;
  config.grab_mode = CAMERA_GRAB_LATEST;

  esp_camera_init(&config);

  // WLAN
  WiFi.begin(ssid, password);
  WiFi.setSleep(false);

  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.println(WiFi.localIP());

  startCameraServer();
}

// ================= LOOP =================
void loop() {
  delay(10000);
}
