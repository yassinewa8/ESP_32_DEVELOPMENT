#include <esp_now.h>
#include <WiFi.h>

// Structure example to receive data
typedef struct struct_message {
    char text[200];
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Callback function that will be executed when data is received
void onReceive(const esp_now_recv_info_t *recvInfo, const uint8_t *incomingData, int len) {
  // Print MAC address of sender
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
           recvInfo->src_addr[0], recvInfo->src_addr[1], recvInfo->src_addr[2],
           recvInfo->src_addr[3], recvInfo->src_addr[4], recvInfo->src_addr[5]);
  Serial.print("Received from: ");
  Serial.println(macStr);

  // Copy the received data into the struct
  memcpy(&myData, incomingData, sizeof(myData));

  // Print the received message
  Serial.print("Received message: ");
  Serial.println(myData.text);
}

void printOwnMACAddress() {
  // Get the MAC address of the ESP32
  String macAddress = WiFi.macAddress();

  // Print the MAC address to the Serial Monitor
  Serial.print("ESP32 MAC Address: ");
  Serial.println(macAddress);
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Add a small delay to ensure Wi-Fi mode is initialized
  delay(100);

  // Print this ESP32's MAC address
  printOwnMACAddress();

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Register the receive callback function
  esp_now_register_recv_cb(onReceive);

  Serial.println("ESP-NOW Initialized and ready to receive messages");
}

void loop() {
  // Nothing to do here, just waiting for messages
}
