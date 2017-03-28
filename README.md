# STM32Sleep
Helper library for using Stop &amp; Standby modes with [STM32duino](https://github.com/rogerclarkmelbourne/Arduino_STM32) 
on STM32F103Cx microcontrollers.

# Usage
1. Install [STM32duino](https://github.com/rogerclarkmelbourne/Arduino_STM32)
1. Create `STM32sleep` directory under Arduino IDE's library dir (`~Documents/Arduino/libraries` on OS X)
1. Clone all files in this repository to the created directory
1. Restart Arduino IDE if it was running
1. Include `STM32Sleep.h` and `RTClock.h` to your sketch
1. Code away!

# Examples

## Standby mode

The following code runs the microcontroller normally for 1 second and then sleeps 5 seconds in standby mode. When RTC alarm triggers, the device is reset and the sketch runs again from the beginning.

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

## Stop mode

The following sketch disables ADC & GPIO's to save power before entering stop mode. Device is then continuously woken up after 5 seconds by RTC.

    #include <STM32Sleep.h>
    #include <RTClock.h>
    
    RTClock rt(RTCSEL_LSE);
    long int alarmDelay = 5;
    
    void setup() {
      // Disable ADC to save power
      adc_disable_all();

      // Set all GPIO pins to Analog input to save power (this disables pretty 
      // much all I/O incl. Serial)
      setGPIOModeToAllPins(GPIO_INPUT_ANALOG);  

      sleepAndWakeUp(STOP, &rt, alarmDelay);
    }
    
    void loop() {
      // We have woken up from sleep! System clock is set to 8MHz HSI.
      
      // Optionally disable peripheral clocks to save power while in Run mode
      //disableAllPeripheralClocks();
    
      delay(100);  // As the main clock is only 8MHz instead of normal 72MHz this actually takes 900ms
    
      sleepAndWakeUp(STOP, &rt, alarmDelay);
    }

#Misc

Thanks for the hard work & inspiration for all the people working on STM32duino!

For more about low power on STM32 devices, see e.g. [Low power mode and wake from sleep.](http://www.stm32duino.com/viewtopic.php?f=3&t=658)
