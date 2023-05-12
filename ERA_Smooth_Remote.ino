#include <esp_now.h>
#include <WiFi.h>

const int xPin1 = 4;
const int yPin1 = 5;
const int xPin2 = 6;
const int yPin2 = 7;

float x = 400;
float y = 0;

unsigned long previousMillis = 0;

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x68, 0xB6, 0xB3, 0x2C, 0x11, 0x70}; //MAC: 68:B6:B3:2C:11:70

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  float x;
  float y;
  float b; //Base
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;
 
void setup() {

  // Init Serial Monitor
  Serial.begin(115200);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {

  float yVal = mapToValue(xPin1, 0, 500); //(pin, min, max). Orientation swapped, wires downward
  float xVal = mapToValue(yPin1, 0, 500); //(pin, min, max)
  //int b = mapToValue(xPin1, 0, 500); //(pin, min, max)

  
  unsigned long currentMillis = millis();

  /*if (currentMillis - previousMillis >= 15)
  {
    previousMillis = currentMillis;
    if (xVal > 400)
      x++;
    else if (xVal < 100)
      x--;

    if (yVal > 400)
      y++;
    else if (yVal < 100)
      y--;
  }*/


  /*Serial.print(x);
  Serial.print("    ");
  Serial.println(y);*/

  // Set values to send
  myData.x = xVal;
  myData.y = yVal;
  //myData.b = mapToValue(xPin2, 0, 500);
  
  /*Serial.print(mapToValue(xPin1, 0, 500));
  Serial.print("    ");
  Serial.print(-mapToValue(yPin1, -250, 250));
  Serial.print("    ");
  Serial.print(mapToValue(xPin2, 0, 500));
  Serial.print("    ");
  Serial.println(mapToValue(yPin2, -250, 250));*/
  
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  
  /*if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }*/
  
}

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  //Serial.print("\r\nLast Packet Send Status:\t");
  //Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

float mapToValue(int pin, int min, int max)
{
  float value = analogRead(pin);
  value = map(value, 0, 4095, max, min);
  value = constrain(value, min, max);
  return value;
}