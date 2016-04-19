#include <STM32Sleep.h>
#include <RTClock.h>

RTClock rt(RTCSEL_LSE);
long int alarmDelay = 5;

void setup() {
  // We have just started or woken up from sleep! System clock is set to 72MHz HSE.
  delay(1000);
  sleepAndWakeUp(STANDBY, &rt, alarmDelay);  
}

void loop() { }  // This is never run