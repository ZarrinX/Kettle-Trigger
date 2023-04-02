//microSD Interface
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#include <SPI.h>
#include "SdFat.h" //http://librarymanager/All#SdFat_exFat by Bill Greiman. We currently use v2.1.2

const int pin_microSD_CS = 9; //Default pin for the SparkFun MP3 Shield
SdFat sd;
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//MP3 Shield Interface
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#include "vs1053_SdFat.h" //http://librarymanager/All#vs1053_sdFat by mpflaga

vs1053 MP3player; 
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

//Current monitor
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#include "EmonLib.h"             // Include Emon Library

EnergyMonitor emon1;             // Create an instance

void setup()
{
  if (sd.begin(pin_microSD_CS) == false)
  {
    Serial.println(F("SD init failed. Is card present? Formatted? Freezing..."));
    while (1);
  }

  MP3player.begin();
  MP3player.setVolume(10, 10); //(Left, Right) 40 is pretty good for ear buds

  emon1.current(1, 111.1);             // Current: input pin, calibration.
}

void loop()
{
  double Irms = emon1.calcIrms(1480);
  Serial.println(Irms);
  if(Irms > 10.0) {
    MP3player.playMP3("beans.mp3");
    delay(360000);
  }
  delay(1000);
}