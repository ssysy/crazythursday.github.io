#include <esp_now.h>
#include <WiFi.h>
#include <SoftwareSerial.h>
#include <stdint.h> // For fixed-width integers like int32_t
#include "time.h"

#define rxPin 17
#define txPin 18
#define GAME_DISTANCE 1.5 // distance between pitcher and swinger

//define command bits
// #define Enable 7;
// #define Player 0;


// Set up a new SoftwareSerial object
SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin);

// Structure to receive data (must match the sender structure)
typedef struct struct_message {
  int32_t id;   // Unique identifier for the device
  float vel;    // Velocity (float type)
  int acc;      // Acceleration (integer type)
  float Time; // Receiving time
} Struct_message;

// Create a struct_message to hold the incoming data
Struct_message myData;

// Create a structure to hold the readings from each board
Struct_message board1;
Struct_message board2;
Struct_message board3;

// Create an array with all the structures
Struct_message boardsStruct[3] = {board1, board2, board3};

// Whether a round finish
int Time_calFlag = 0;

// Cmd send to ATMega
uint8_t cmd = 0;

float adjust = 0.0;

// Callback function that will be executed when data is received
// Callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  // if (boardsStruct[0].Time == 0 || boardsStruct[1].Time == 0){
    char macStr[18];
    Serial.print("Packet received from: ");
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
            mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    Serial.println(macStr);

    // Copy the incoming data to the struct
    memcpy(&myData, incomingData, sizeof(myData));

    // Print the received data
    Serial.printf("Board ID %u: %u bytes\n", myData.id, len);

    // Update the structures with the new incoming data
    if (myData.id > 0 && myData.id <= 3) { // Ensure ID is within the bounds of the boardsStruct array
      boardsStruct[myData.id - 1].vel = myData.vel; // Store the velocity
      boardsStruct[myData.id - 1].acc = myData.acc; // Store the acceleration
    }

    // Print the velocity and acceleration of the corresponding board
    Serial.printf("Velocity: %.3f \n", boardsStruct[myData.id - 1].vel);
    Serial.printf("Acceleration: %d \n", boardsStruct[myData.id - 1].acc);
    Serial.println();

    // Record the time of the received data based on the board id
    struct tm timeinfo;
    struct timeval now;
    float value;
      if (getLocalTime(&timeinfo)) {
        gettimeofday(&now, NULL); // Get seconds and microseconds
      }
      
    // unsigned long currentTime = millis(); // Get the current time in milliseconds
    if (boardsStruct[myData.id - 1].vel > 0.1){
      if (myData.id == 1) {
        // Time_calFlag = 0;
        // board1.Time = currentTime; // Store the time for board 1
        if(boardsStruct[0].Time == 0){
          boardsStruct[0].Time = timeinfo.tm_sec + round((now.tv_usec / 1e6) * 1000) / 1000.0;
          Serial.printf("Board 1 Time: %.3f\n", boardsStruct[0].Time);
        }else{
          Serial.printf("Ball in the air");
          cmd |= (1<<5);
        }
      } else if (myData.id == 2) {
        // Time_calFlag = 1;
        // board2.Time = currentTime; // Store the time for board 2
        if(boardsStruct[0].Time != 0){
          boardsStruct[1].Time = timeinfo.tm_sec + round((now.tv_usec / 1e6) * 1000) / 1000.0;
          Serial.printf("Board 2 Time: %.3f\n", boardsStruct[1].Time);
          // air = false;
        }else{
            Serial.printf("Wait for ball to throw");
        }
      }
      // if(boardsStruct[0].Time != 0){
      //   air = false;
      // }else{
      //   air = true;
      // }
    }else{
        if (myData.id == 1 && boardsStruct[0].Time == 0) {
          cmd |= (1<<6);
        }
        Serial.printf("Not a acceptable input\n");
        Serial.printf("Velocity 1: %.3f \n", boardsStruct[0].vel);
        Serial.printf("Velocity 2: %.3f \n", boardsStruct[1].vel);
    }
  // }
}

void initail_board(Struct_message* sm){
  Serial.printf("Initail board");
  sm->acc =0;
  sm->id = 0;
  sm->Time = 0;
  sm->vel = 0;
}

void setup() {
  // Initialize the timestamps of received data for each board
  board1.Time = 0;
  board2.Time = 0;

  struct tm timeinfo;
  timeinfo.tm_year = 2024 - 1900;
  timeinfo.tm_mon = 0;           // January
  timeinfo.tm_mday = 1;          // Day of the month
  timeinfo.tm_hour = 1;          // Hour
  timeinfo.tm_min = 0;           // Minute: 0
  timeinfo.tm_sec = 0;            // Second: 0
  time_t t = mktime(&timeinfo);
    struct timeval now = {
        .tv_sec = t,               // Seconds
        .tv_usec = 0          // Microseconds (e.g., 123456us or 123.456ms)
    };
    settimeofday(&now, NULL);

  // Initialize Serial Monitor
  Serial.begin(115200);
  
  // Initialize Serial between MCU
  mySerial.begin(9600);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Register for received callback
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
  for (int i = 0; i < 3; i++) {
    initail_board(&boardsStruct[i]);
  }

}

void loop() {
  unsigned long T = 0;
  struct tm timeinfo;
  struct timeval now;
  float value;
    if (getLocalTime(&timeinfo)) {
      gettimeofday(&now, NULL); // Get seconds and microseconds
    }

  if(boardsStruct[0].Time != 0){
    float tc = timeinfo.tm_sec + round((now.tv_usec / 1e6) * 1000) / 1000.0;
    if((tc - boardsStruct[0].Time) > 4.0){
        Serial.println("Hitter didn't swing the bat!");
          boardsStruct[0].Time = 0;
          boardsStruct[1].Time = 0;
          cmd &= ~(1<<0); //ball win
          cmd |= (1<<7);
        if (boardsStruct[0].vel > 1.2){
          cmd |= (1<<1); //double score
        }
          // Serial.print("Passing command:");
          // Serial.println(cmd, BIN);
          // mySerial.write(cmd);
          // cmd = 0;
      }
  }
  // Calculate and print the time interval between the two boards (if both are received)
  //if (board1.vel > 0.1 && board2.vel > 0.1 && Time_calFlag == 1) {
  if (boardsStruct[0].Time != 0 && boardsStruct[1].Time != 0) {

    cmd |= (1<<7);
    Serial.printf("Velocity1: %.3f \n", boardsStruct[0].vel);
    float T = (GAME_DISTANCE/boardsStruct[0].vel);
    Serial.printf("T_calc: %.3f \n", T);
    float timeInterval = boardsStruct[1].Time - boardsStruct[0].Time;
    if(timeInterval<0){
      timeInterval += 60;
    }
    Serial.printf("T_actual: %.3f \n", timeInterval);
    // Catch detection & Score
    // if ((abs(T - timeInterval)/timeInterval < 1+adjust) && (boardsStruct[0].acc == boardsStruct[1].acc) )
    if ((abs(T - timeInterval)/timeInterval) < 1+adjust )
    {
      adjust = 0.0;
      cmd |= (1<<0); //hit win
      if (boardsStruct[0].vel > 0.7){
        cmd |= (1<<1); //double score
      }
      Serial.println("Hitter win!");
    }
    else{
      cmd &= ~(1<<0); //ball win
      adjust += 0.2;
      if (boardsStruct[1].vel > 1.2){
        cmd |= (1<<1); //double score
      }
      Serial.println("Ball win!");
    }
    boardsStruct[0].Time = 0;
    boardsStruct[1].Time = 0;
  }

  // if ((cmd & (1<<7))!= 0){
  if((cmd != 0)){
    Serial.print("Passing command:");
    Serial.println(cmd, BIN);
    mySerial.write(cmd);
  }

  cmd = 0;
}
