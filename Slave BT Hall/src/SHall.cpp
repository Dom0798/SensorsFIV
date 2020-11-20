#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

typedef struct Hall{
  int pos;
} Hall;

//Hall effect
int hallsensorpin1 = 23;
int hallsensorpin2 = 22;
int hallsensorpin3 = 21;
int hallsensorpin4 = 19;
//int ledpin4 = 22;
int state1 = 0;
int state2 = 0;
int state3 = 0;
int state4 = 0;

Hall Pos;
uint8_t bAddress[] = {0x7C, 0x9E, 0xBD, 0xF5, 0x88, 0x78};//IMU

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
   // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  pinMode(hallsensorpin1,INPUT); 
  pinMode(hallsensorpin2,INPUT);
  pinMode(hallsensorpin3,INPUT);
  pinMode(hallsensorpin4,INPUT);
  
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, bAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {

  state1 = digitalRead(hallsensorpin1);
  state2 = digitalRead(hallsensorpin2);
  state3 = digitalRead(hallsensorpin3);
  state4 = digitalRead(hallsensorpin4);
 
  if (state4 == LOW)
  {
    Serial.println("FOURTH POSITION");
    Pos.pos = 4;
    esp_err_t result = esp_now_send(bAddress, (uint8_t *) &Pos, sizeof(Pos));
    delay(20);
  }
  else if (state3 == LOW)
  {
    Serial.println("THIRD POSITION");
    Pos.pos = 3;
    esp_err_t result = esp_now_send(bAddress, (uint8_t *) &Pos, sizeof(Hall));
    delay(20);
  }
  else if (state2 == LOW)
  {
    Serial.println("SECOND POSITION");
    Pos.pos = 2;
    esp_err_t result = esp_now_send(bAddress, (uint8_t *) &Pos, sizeof(Hall));
    delay(20);
  }
  else if (state1 == LOW)
  {
    Serial.println("FIRST POSITION");
    Pos.pos = 1;
    esp_err_t result = esp_now_send(bAddress, (uint8_t *) &Pos, sizeof(Hall));
    delay(20);
  }
  else
  {
    Serial.println("NO POSITION");
    Pos.pos = 0;
    esp_err_t result = esp_now_send(bAddress, (uint8_t *) &Pos, sizeof(Hall));
    delay(20); 
  }
/*
if (state4 == HIGH)
{
    Serial.println("NO POSITION");
    Pos.pos = 0;
    esp_err_t result = esp_now_send(bAddress, (uint8_t *) &Pos, sizeof(Hall));
    delay(20); 
}*/
   delay(20);
}