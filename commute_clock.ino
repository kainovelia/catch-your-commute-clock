// Libraries to include
#include <TFT_eSPI.h>
#include <SPI.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "time.h"
#include <ESP32Time.h>

// Network variables
const char* ssid = "Kibel";
const char* password = "Stella@1";
TFT_eSPI tft = TFT_eSPI();
HTTPClient http;

// World timezone API url variables
String timeBase = "https://worldtimeapi.org/api/timezone/";
String timezone = "America"; 
String city = "New_York";

// Clock variables
ESP32Time rtc(0);
String curTime;
volatile bool timeSet = false;
int mode = 1;
int lastMode = 1;
int menuCursor = 1;
String joystickDirection;
int alarmMillis = 600000; // 10 mins
bool alarmOn = false;
bool flash = true;

// Button variables
#define BUTTON_TOP 13
#define BUTTON_BOTTOM 15
#define X_PIN 36
#define Y_PIN 37
#define Z_PIN 38

bool topButtonPressed = false;
bool bottomButtonPressed = false;
bool joystickMoved = false;

String route = "Q";
String direction = "DOWN";
const int timeToStation = 10; //in minutes
bool firstGet = true;
bool refresh = false;
int nextArr;

void setup() {
  Serial.begin(115200);

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  tft.fillScreen(TFT_BLUE);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.setTextFont(2);
  tft.setTextSize(4);

  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.print("Connected to WiFi Network with IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println(timeSet);

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("WiFi connected!");
    drawClock();
  } else {
    Serial.println("WiFi not connected.");
  }

  pinMode(BUTTON_TOP, INPUT_PULLUP);
  pinMode(BUTTON_BOTTOM, INPUT_PULLUP);
  pinMode(Z_PIN, INPUT_PULLUP);
}

void loop() {  

  int topState = digitalRead(BUTTON_TOP);
  int bottomState = digitalRead(BUTTON_BOTTOM);
  int menuState = digitalRead(Z_PIN);
  Serial.printf("%d, %d, %d", topState, bottomState, menuState);

  if(menuState==1){
    mode = 0;
  }

  if(topState==0) {
    if (mode == 0) {
      mode = menuCursor;
    }
  } else if (bottomState==0) {
    if(mode == 0){
      mode == lastMode;
    }
  }
  
  if(mode == 0) {
    Serial.println("Menu called");
    menuMode();
  }

  if(mode == 1){
    lastMode = 1;
    drawClock();
  } else if(mode == 2) {
    lastMode = 2;
    getGTFS();
    delay(5000);
  } else if(mode == 3) {
    lastMode = 3;
    alarmOn = true;
    getGTFS();
  }
}

void drawClock() {
  Serial.print("Time set: ");
  Serial.println(timeSet);
  if(timeSet == false) {
    String url = timeBase + timezone + "/" + city;
    Serial.println(url);
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    int timeResponse = http.GET();

    if (timeResponse > 0) {
      String startTime = http.getString();
      JSONVar timeJSON = JSON.parse(startTime);

      if (JSON.typeof(timeJSON) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }

      JSONVar timeKeys = timeJSON.keys();
      int unixtime = timeJSON[timeKeys[6]];
      int timeoffset = timeJSON[timeKeys[7]];

      rtc.offset = timeoffset;
      rtc.setTime(unixtime);
      timeSet = true;
    }
  }
    curTime = rtc.getTime();

    tft.drawString(curTime, 10, 10);
}

void getGTFS() {
  tft.fillScreen(TFT_BLUE);

  if(refresh || firstGet){
    Serial.println("R");
    
    if(Serial.available() > 0) {
      // A unix timestamp is 10 digits + null terminating char
      char recvdChar[11];

      Serial.readBytes(recvdChar, 10);
      recvdChar[10] = '\0';
      String recvdString = String(recvdChar);

      int nowTime = rtc.getLocalEpoch();
      int time = recvdString.toInt();
      nextArr = time;
      int diff = nextArr - nowTime;
      int mins = diff / 60;
      String train_info1 = "<" + route + "> " + direction;
      String train_info2 = String(mins) + "mins";
      tft.setTextSize(2);
      tft.drawString(train_info1, 10, 10);
      tft.drawString(train_info2, 10, 40);

      refresh = false;
      firstGet = false;
    }
  } else {
    int nowTime = rtc.getLocalEpoch();
    int diff = (nextArr - nowTime);
    int mins = diff / 60;
    String train_info1 = "<" + route + "> " + direction;
    String train_info2 = String(mins) + "mins";
    tft.setTextSize(2);
    tft.drawString(train_info1, 10, 10);
    tft.drawString(train_info2, 10, 40);
    if (alarmOn==true) {
      if(mins > 10) {
        tft.drawString("You got time", 10, 70);
      } else if(mins <= 10) {
        tft.drawString("You better hurry...", 10, 70);
      } else if(mins <= 6) {
        if(flash){
          flash = false,
          tft.setTextColor(TFT_RED, TFT_BLUE);
          tft.drawString("Run!!!", 10, 70);
        } else {
          flash = true,
          tft.setTextColor(TFT_RED, TFT_BLUE);
          tft.drawString("Run!!!", 10, 70);
        }
      } else if(mins < 5) {
        tft.setTextColor(TFT_WHITE, TFT_BLUE);
        tft.drawString("Wait for the next one...", 10, 70);
      }
    } 
    if(mins == 0){
      refresh = true;
      alarmOn = false;
    }
  }
}

void menuMode() {
  int xVal = analogRead(X_PIN);
  if(menuCursor == 0){
    menuCursor == 3;
  }

  if(menuCursor == 4) {
    menuCursor == 1;
  }

  if(menuCursor == 1) {
    tft.drawString("RT CLOCK", 10, 10);
    tft.drawString("MODE", 25, 70);
  } else if(menuCursor == 2) {
    tft.drawString("TRAIN", 10, 10);
    tft.drawString("SCHED", 10, 70);
  } else if(menuCursor == 3) {
    tft.drawString("COMMUTE", 10, 10);
    tft.drawString("ALARM", 10, 70);
  } 

  if (xVal < 100) {
    tft.fillScreen(TFT_BLUE);
    menuCursor--;
  } else if (xVal > 4000) {
    tft.fillScreen(TFT_BLUE);
    menuCursor++;
  }
}

