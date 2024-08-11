#include <esp_now.h>
#include <WiFi.h>

// MAC address of the receiver ESP32
uint8_t receiverAddress[] = {0x10, 0x06, 0x1C, 0x82, 0xCD, 0xC4};

typedef struct struct_message {
    char text[200];
} struct_message;

struct_message myData;

void onSent(const uint8_t *macAddr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  
  // Add a small delay to ensure Wi-Fi mode is initialized
  delay(100);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Register for Send CB to get the status of transmission
  esp_now_register_send_cb(onSent);
  
  // Register peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 1;  // Set to 1 or the channel your network is on
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  Serial.println("Peer added successfully.");
}

void loop() {
  // Ask user for input
  Serial.print("Enter text to send: ");
  while (Serial.available() == 0) {}  // Wait until user enters something
  
  // Read the user input
  String input = Serial.readString();
  
  // Convert input to char array and copy to struct
  input.toCharArray(myData.text, 200);
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(receiverAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }
}
