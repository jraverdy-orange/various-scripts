#include <LowPower.h>

//String str;
//String strlast;
int started;

int pushButton = 2;
int led_r = 3;
int led_g = 4;
int rpiPowerControl = 5;
int previousState = LOW;
//boolean stringComplete = false;

void wakeUp()
{
    // Just a handler for the pin interrupt.
}

void setup() {
  Serial.begin(57600);
//  pinMode(manualStartedButton, INPUT);
  pinMode(pushButton, INPUT_PULLUP);
  pinMode(led_r, OUTPUT);
  pinMode(led_g, OUTPUT);  
  pinMode(rpiPowerControl, OUTPUT);
  digitalWrite(led_r, LOW);
  digitalWrite(led_g, LOW);
  digitalWrite(rpiPowerControl, HIGH);
  /*started = 0;
  str.reserve(200);
  strlast.reserve(200);*/
}

void loop()
{
    attachInterrupt(0, wakeUp, LOW);
    int actionDone=0;

 /*  if (Serial.available() > 0)
  {
    strlast = str;
    str = Serial.readStringUntil('\n');

    if (started)
    {
      if (str.endsWith(" reboot: Power down\r"))
      {
        digitalWrite(led, LOW);
        digitalWrite(rpiPowerControl, HIGH);
        started = 0;
      }
    }
    else
    {
      if (str == "raspberrypi login: ")
      {
        digitalWrite(led, HIGH);
        started = 1;
      }
    }
  }*/

  int buttonState = digitalRead(pushButton);
  /*if (buttonState == 0)
  {
    if (started == 1)
    {
      //      Serial.println("1[" + strlast + "]");
      //      Serial.println("2[" + str + "]");
      //      delay(1000);
      Serial.print("pi\n");
      delay(1000);
      Serial.print("raspberry\n");
      delay(1000);
      Serial.print("sudo shutdown -h now\n");
      //when started
      //<raspberrypi login: >
      //when stopped
      //<[  390.039293] reboot: Power down>
    }
    else
    {
      digitalWrite(rpiPowerControl, LOW);
      delay(1000);
    }
  }*/
  if ( buttonState == 0 )
  {
    Serial.println("Button Pressed");
    if ( started == 0 && actionDone == 0 )
    {
       Serial.println("Starting the pi");
       // passing to low state for .5s then return to high state
       digitalWrite(rpiPowerControl, LOW);
       delay(500);
       digitalWrite(rpiPowerControl, HIGH);
       started=1;
       actionDone=1;
    }
    if ( started == 1 && actionDone == 0 )
     {
       // not implemented yet
       Serial.println("Shutdown the pi");
       started=0;
       actionDone=1;
     }
  }
  //wait until the button is released
  while (digitalRead(pushButton)==0)
  {
    //do nothing
  }
  delay(1000);  
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
  detachInterrupt(0);  
}
