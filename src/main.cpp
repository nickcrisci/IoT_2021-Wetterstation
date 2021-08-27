#include <Arduino.h>
#include "DHT.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <string>
#include "TM1637.h"
#include "deepsleep.h"

#include "wifi_secrets.h"

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

Adafruit_BMP280 bmp;

//Sensor einrichten
#define DHTPIN 14
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
int temperature = 0;
int humidity = 0;
float pressure = 0.0;

//Display einrichten
#define CLK 33  
#define DIO 32
TM1637 tm1637(CLK,DIO);

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

String serverName = "http://esp32.nick_crisci.repl.co/";

unsigned long lastTime = 0;
unsigned long timerDelay = 1000;

void displayError(int error[2]) {
  tm1637.display(0, 14); // Display "E" for "Error"
  tm1637.display(2, error[0]);
  tm1637.display(3, error[1]);
  delay(5 * 1000);
  tm1637.clearDisplay();
  tm1637.stop();
  esp_sleep_enable_timer_wakeup(300 * 1000000); // Retry after 5 Minutes
  esp_deep_sleep_start();
}

void initWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  Serial.print("Connecting to wifi...");
  delay(10 * 1000); // 10 Second Delay before Connection Error

  if (WiFi.status() != WL_CONNECTED) {
    int error [2] = {1, 1};
    displayError(error);
  }

  Serial.println(WiFi.localIP());
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void displayNumber(int num){   
    tm1637.display(1, num % 10);   
    tm1637.display(0, num / 10 % 10);   
    //tm1637.display(1, num / 100 % 10);   
    //tm1637.display(0, num / 1000 % 10);
}

int menucycle = 0;
void displaymenu(){
  switch(menucycle){
    case 0:
      displayNumber(dht.readTemperature());
      tm1637.display(3,12);
      break;
    case 1:
      displayNumber(dht.readHumidity());
      //int8_t LF[] = {0x0,0x0,B00111000, 0x0};
      tm1637.display(3,15);
      break;
    case 2:
      displayNumber(99);
      tm1637.display(2,9);
      tm1637.display(3,9);
      break;
  }
}

void ISRbuttonClicked(){
  if(menucycle < 2){
    menucycle++;
  }else {
    menucycle = 0;
  }
}

void setup(){
  Serial.begin(115200);

  if (!bmp.begin()) {
    int error [2] = {2, 1};
    displayError(error);
  }

  bmp.setSampling (
    Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
    Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
    Adafruit_BMP280::FILTER_X16,      /* Filtering. */
    Adafruit_BMP280::STANDBY_MS_500
  );

  tm1637.init();
  tm1637.set(BRIGHTEST);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  dht.begin();
  pinMode(26, INPUT);
  attachInterrupt(26, ISRbuttonClicked, HIGH);
  if(esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0){
    menucycle = 0;
    for (int i = 0; i < 3; i++) {
      displaymenu();
      delay(5000);
      menucycle++;
    }
    tm1637.clearDisplay();
    tm1637.stop();
    goToDeepSleep();
  }
  initWifi();
}

void loop(){
  if(WiFi.status()==WL_CONNECTED){
    HTTPClient http;
    http.begin(serverName.c_str());
    //http.addHeader("Content-Type","text/plain");
    http.addHeader("Content-Type","application/json");
    
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
    pressure = bmp.readPressure();
    
    char json[1024];
    sprintf(json, "{\"temperature\" :\"%i\", \"humidity\" :\"%i\" }",temperature,humidity);
    int httpResponseCode = http.POST(json);

    if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
    }
    else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
    http.end();
    goToDeepSleep();
    Serial.println("Ich werde nie geschrieben!");
  }
}
