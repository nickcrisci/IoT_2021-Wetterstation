#include <Arduino.h>
#include <time.h>

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