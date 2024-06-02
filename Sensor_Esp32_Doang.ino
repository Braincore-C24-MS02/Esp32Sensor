#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

// Replace with your network credentials
const char* ssid = "Cikua";
const char* password = "babanana";

// Replace with your Firebase project credentials
#define FIREBASE_HOST "firebasestorage.googleapis.com"
#define FIREBASE_AUTH "h0biMsta5bJEy4ninf3Q9wYcYxbXbO7yoE9DhvMK"

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
Adafruit_MPU6050 mpu;

const int MQ7_PIN = 34;
const int BUZZER_PIN = 27;

FirebaseData firebaseData;
FirebaseJson json;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to Wi-Fi");

  // Connect to Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  // Initialize MLX90614
  if (!mlx.begin()) {
    Serial.println("Gagal menemukan MLX90614. Periksa koneksi!");
    while (1);
  }

  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("Gagal menemukan MPU6050. Periksa koneksi!");
    while (1);
  }

  pinMode(MQ7_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.println("Sistem Siap!");
}

void loop() {
  // Read temperature from MLX90614
  double suhu = mlx.readObjectTempC();
  Serial.print("Suhu MLX90614: ");
  Serial.print(suhu);
  Serial.println(" °C");

  // Read MQ-7 sensor value
  int nilaiMQ7 = analogRead(MQ7_PIN);
  Serial.print("Nilai MQ-7: ");
  Serial.println(nilaiMQ7);

  // Send data to Firebase
  json.set("/suhu", suhu);
  json.set("/nilaiMQ7", nilaiMQ7);

  if (Firebase.updateNode(firebaseData, "/sensorData", json)) {
    Serial.println("Data updated successfully");
  } else {
    Serial.print("Error updating data: ");
    Serial.println(firebaseData.errorReason());
  }

  // Check temperature and activate buzzer if necessary
  if (suhu > 35.0) {
    digitalWrite(BUZZER_PIN, HIGH);
    Serial.println("Peringatan! Suhu melebihi 35°C, Buzzer ON!");
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }

  delay(1000);
}