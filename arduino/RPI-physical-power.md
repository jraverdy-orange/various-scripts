# Prerequisite

## Material

* Arduino Pro mini 3.3V

## Software

* Arduino IDE

# Installing LowPower lib

* get the following archive

  `https://github.com/rocketscream/Low-Power/archive/master.zip`

* Add it to Arduino IDE

  `Sketch > Include Library > Add .ZIP Library...`

  **If you use an Atmega128's arduino, you have to patch the LowPower.cpp file after line #106 or you'll get compilation errors**

  ```cpp
  #if defined __AVR_ATmega168__
  #ifndef SLEEP_MODE_EXT_STANDBY
  #define SLEEP_MODE_EXT_STANDBY (0x07<<1)
  #endif
  #endif

  ```

* Get the ino file

* upload the sketch to the arduino

* ​

