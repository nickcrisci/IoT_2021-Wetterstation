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
  int timeTillNextHour = getTimeTillNextHour();
  esp_sleep_enable_timer_wakeup(timeTillNextHour * 1000000);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_26, 1);   //ESP wird bei Knopfdruck aufgeweckt
  int minutes = timeTillNextHour / 60;
  int seconds = timeTillNextHour % 60;
  Serial.println("Gehe fuer " + String(minutes) + " Minuten und " + String(seconds) + " Sekunden in den Deep Sleep");
  delay(1000);
  Serial.flush();
  esp_deep_sleep_start();
}