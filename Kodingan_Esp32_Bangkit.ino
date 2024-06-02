#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// Inisialisasi sensor MLX90614 dan MPU6050
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
Adafruit_MPU6050 mpu;

const int MQ7_PIN = 34; // Pin ADC untuk MQ-7
const int BUZZER_PIN = 27; // Pin untuk Buzzer

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // Inisialisasi MLX90614
  if (!mlx.begin()) {
    Serial.println("Gagal menemukan MLX90614. Periksa koneksi!");
    while (1);
  }
  
  // Inisialisasi MPU6050
  if (!mpu.begin()) {
    Serial.println("Gagal menemukan MPU6050. Periksa koneksi!");
    while (1);
  }
  
  pinMode(MQ7_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.println("Sistem Siap!");
}

void loop() {
  // Membaca suhu dari MLX90614
  double suhu = mlx.readObjectTempC();
  Serial.print("Suhu MLX90614: ");
  Serial.print(suhu);
  Serial.println(" °C");

  // Membaca nilai gas karbonmonoksida dari MQ-7
  int nilaiMQ7 = analogRead(MQ7_PIN);
  Serial.print("Nilai MQ-7: ");
  Serial.println(nilaiMQ7);

  // Menyalakan buzzer jika suhu melebihi 35°C
  if (suhu > 35.0) {
    digitalWrite(BUZZER_PIN, HIGH);
    Serial.println("Peringatan! Suhu melebihi 35°C, Buzzer ON!");
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }

  // Tunggu 1 detik sebelum pembacaan berikutnya
  delay(1000);
}
