#include <LowPower.h>

//String str;
//String strlast;
int started = 0; 
int firstBoot = 1; //on first boot the RPI start automaticaly on power plug

int pushButton = 2;
int led_r = 3;
int led_g = 4;
int rpiPowerUp = 5;
int rpiPowerDown = 6;
int rpiStat = 7;
//int previousState = LOW;
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
  pinMode(rpiPowerUp, OUTPUT);
  pinMode(rpiPowerDown, OUTPUT);  
  pinMode(rpiStat, INPUT);
  digitalWrite(led_r, LOW);
  digitalWrite(led_g, LOW);
  digitalWrite(rpiPowerUp, HIGH);
  digitalWrite(rpiPowerUp, HIGH);

}

void dotPrint()
{
   while (digitalRead(rpiStat) != LOW)
   {
    Serial.print(".");
    delay(1000);
   }
   Serial.println("");
}

void loop()
{
  attachInterrupt(0, wakeUp, LOW);
  int actionDone=0;
  
  int buttonState = digitalRead(pushButton);
  if ( buttonState == 0 || firstBoot == 1)
  {
    if ( firstBoot == 1 ) { Serial.println("Button Pressed"); } else { Serial.println("Plugged"); }
    if ( started == 0 && actionDone == 0 )
    {
       Serial.println("Starting the pi");
       if ( firstBoot != 1 )
       {
         // passing to low state for .2s then return to high state
         digitalWrite(rpiPowerUp, LOW);
         delay(200);
         digitalWrite(rpiPowerUp, HIGH);
       }
       // wait for RPI complete startup
       Serial.print("Waiting for RPI complete startup ");
       dotPrint;
       Serial.println("Startup complete");
       started=1;
       actionDone=1;
    }
    if ( started == 1 && actionDone == 0 )
     {
       // not implemented yet
       Serial.println("Shutdown the pi");
       digitalWrite(rpiPowerDown, LOW);
       // wait for RPI complete shutdown
       Serial.print("Waiting for RPI complete startup ");
       dotPrint;
       Serial.println("Shutdown complete");
       started=0;
       actionDone=1;
     }
  }
  /* //wait until the button is released
  while (digitalRead(pushButton)==0)
  {
    //do nothing
  }
  delay(1000);*/
  firstBoot=0;
  // go to sleep mode  
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
  detachInterrupt(0);  
}
