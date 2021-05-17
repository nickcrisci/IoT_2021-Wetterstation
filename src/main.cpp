#include <Arduino.h>
#include "DHT.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <string>
#include <time.h>
#include "TM1637.h"


//Sensor einrichten
#define DHTPIN 14
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
int temperature = 0;
int humidity = 0;

//Display einrichten
#define CLK 33  
#define DIO 32
TM1637 tm1637(CLK,DIO);

const char* ssid = "WLAN-BEAB00";
const char* password = "0306665700636965";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

String serverName = "http://esp32.nick_crisci.repl.co/";

unsigned long lastTime = 0;
unsigned long timerDelay = 1000;

void initWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
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




double getTimeTillNextHour(){
  time_t now;
  time(&now);
  struct tm nextHour;

  time(&now); /* get current time */

  nextHour = *localtime(&now);
  nextHour.tm_hour ++;
  nextHour.tm_min = 0;
  nextHour.tm_sec = 0;

  double timeTillNextHour = difftime(mktime(&nextHour), now);
  return timeTillNextHour;
}

void goToDeepSleep(){
  double timeTillNextHour = getTimeTillNextHour();
  esp_sleep_enable_timer_wakeup(timeTillNextHour * 1000000);
  //esp_sleep_enable_timer_wakeup(10000000);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_26, 1);   //ESP wird bei Knopfdruck aufgeweckt
  Serial.println("Gehe fuer ");
  Serial.print(timeTillNextHour/60);
  Serial.print(" Minuten in den Deep Sleep!");
  delay(1000);
  Serial.flush();
  esp_deep_sleep_start();
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
    tm1637.init();
    tm1637.set(BRIGHTEST);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
    dht.begin();
    pinMode(26, INPUT);
    //attachInterrupt(26, ISRbuttonClicked, HIGH);
    if(esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0){
              menucycle = 0;
              displaymenu();
              delay(5000);
              menucycle++;
              displaymenu();
              delay(5000);
              menucycle++;
              displaymenu();
              delay(5000);
              tm1637.clearDisplay();
              tm1637.stop();
              goToDeepSleep();
            }
    initWifi();
}



void loop(){
            delay(3000);

            if(WiFi.status()==WL_CONNECTED){
                HTTPClient http;
                http.begin(serverName.c_str());
                //http.addHeader("Content-Type","text/plain");
                http.addHeader("Content-Type","application/json");
                
                temperature = dht.readTemperature();
                humidity = dht.readHumidity();

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
