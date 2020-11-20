#include <Arduino.h>
#include <Wire.h>
#include "SparkFun_BNO080_Arduino_Library.h"
#include <BluetoothSerial.h>
#include <esp_now.h>
#include <WiFi.h>

typedef struct Hall{
  int pos;
} Hall;

BNO080 myIMU2; //Closed I2C ADR jumper - address 0x4A
Hall Pos;
BluetoothSerial SerialBT;
String payload1;
String payload2;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&Pos, incomingData, sizeof(Pos));
}

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  SerialBT.begin("VenusIMU");
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  Wire.begin();
  Wire.setClock(10000);

  if (myIMU2.begin(0x4A) == false)
  {
    Serial.println("Segundo BNO080 no conectado...");
  }

  //myIMU1.enableRotationVector(50); //Send data update every 50ms
  myIMU2.enableRotationVector(50);
}

void loop()
{
  if (myIMU2.dataAvailable() == true)
  {
    float roll2 = (myIMU2.getRoll()) * 180.0 / PI; // Convert roll to degrees
    float pitch2 = (myIMU2.getPitch()) * 180.0 / PI; // Convert pitch to degrees
    float yaw2 = (myIMU2.getYaw()) * 180.0 / PI; // Convert yaw to degrees
    int r2 = round(roll2);
    int p2 = round(pitch2);
    int y2 = round(yaw2);

    payload2 = -r2;
    payload2 += ";";
    payload2 += -p2;
    payload2 += ";";
    payload2 += -y2;
    payload2 += ";";
    payload2 += Pos.pos;

    Serial.println(payload2);
    SerialBT.println(payload2);
    SerialBT.flush();
    Wire.endTransmission();
  }
}