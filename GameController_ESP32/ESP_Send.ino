/*********
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp-now-many-to-one-esp32/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*********/

// This code can make ESP32 read from ATmega via UART1 communication port
// same for all slave devices !!!


#include <esp_now.h>
#include <WiFi.h>
#include <SoftwareSerial.h>
#include <stdint.h> // For fixed-width integers like int32_t

#define rxPin 17
#define txPin 18

// RECEIVER'S MAC Address
uint8_t broadcastAddress[] = {0x84, 0xF7, 0x03, 0x73, 0xC3, 0x2A};

// Define the updated struct
typedef struct struct_message {
  int32_t id;  // Unique device ID
  float vel;   // Velocity as a float
  int acc;     // Acceleration as an integer
} struct_message;

// Create a serial object
SoftwareSerial mySerial(rxPin, txPin);

// Create a peer interface
esp_now_peer_info_t peerInfo;

// Global buffer and variables for received data
char buffer[80]; // Buffer for storing received data
int bufferIndex = 0; // Current buffer index

// Callback for data sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// Function to process received serial data
void processReceivedData(char *data) {
    Serial.print("Raw Data Received: ");
    Serial.println(data);

    // Parse data into the struct_message format
    struct_message packet;

    if (sscanf(data, "Id:%d, Vel:%f, Acc:%d", &packet.id, &packet.vel, &packet.acc) == 3) {
        // Validate ID range
        if (packet.id < 1 || packet.id > 3) {
            Serial.println("Invalid device ID");
            return;
        }

        // Send the structured data via ESP-NOW
        esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&packet, sizeof(packet));
        if (result == ESP_OK) {
            Serial.println("Data sent successfully.");
        } else {
            Serial.println("Failed to send data.");
        }
    } else {
        Serial.println("Error parsing received data.");
    }
}

void setup() {
  // Define pin modes for TX and RX
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);

  // Initialize serial interfaces
  mySerial.begin(9600);
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register callback for sent data
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // Check if data is available on the serial port
  while (mySerial.available() > 0) {
    char c = mySerial.read(); // Read one character
    if (c == '\n' || c == '\r') { // If a newline or carriage return is detected
      buffer[bufferIndex] = '\0'; // Null-terminate the string
      if (bufferIndex > 0) { // Ensure buffer is not empty
        processReceivedData(buffer); // Process the data
      }
      bufferIndex = 0; // Reset buffer index
    } else {
      if (bufferIndex < sizeof(buffer) - 1) { // Prevent buffer overflow
        buffer[bufferIndex++] = c; // Append character to buffer
      }
    }
  }
}
