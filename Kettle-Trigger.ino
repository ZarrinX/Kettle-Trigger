// microSD Interface
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#include <SPI.h>
#include "SdFat.h" //http://librarymanager/All#SdFat_exFat by Bill Greiman. We currently use v2.1.2
const int pin_microSD_CS = 9; //Default pin for the SparkFun MP3 Shield
SdFat sd;

// MP3 Shield Interface
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#include "vs1053_SdFat.h" //http://librarymanager/All#vs1053_sdFat by mpflaga
vs1053 MP3player; 

// Current monitor
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#include "EmonLib.h"             // Include Emon Library
double Irms;                     // Store current value
EnergyMonitor emon1;             // Create an instance

// Sketch Setup
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void setup()
{
  Serial.begin(9600); //Use serial for debugging

  if (sd.begin(pin_microSD_CS) == false)
  {
    Serial.println(F("SD init failed. Is card present? Formatted? Freezing..."));
    while (1);
  }

  MP3player.begin();
  MP3player.setVolume(10, 10); //(Left, Right) 40 is pretty good for ear buds, 10 for speakers

  emon1.current(1, 111.1);             // Current: input pin, calibration.
  Irms = emon1.calcIrms(1480);

  //Give the calcIrms time to settle to base
  while(Irms > 5){
    delay(5000);
    Serial.print("Calibrating:");
    Serial.print(" ");
    Serial.println(Irms);
    delay(100);
    Irms = emon1.calcIrms(1480);
  }
}
void loop()
{
  Irms = emon1.calcIrms(1480);
  if(Irms > 10.0) {
    Serial.println("Bean! Beans! Beans!");
    MP3player.playMP3("beans.mp3");
    Serial.println("Sleeping for 6 minutes.");
    delay(360000);
  }
  delay(1000);
  Serial.print("Idle:");
  Serial.print(" ");
  Serial.println(Irms);
}