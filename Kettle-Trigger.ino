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
double kettleIrms;                     // Store current value
EnergyMonitor kettleSense;             // Create an instance
double grinderIrms;                     // Store current value
EnergyMonitor grinderSense;             // Create an instance

// These are the pins used for current sensors
#define KETTLE_PIN 1
#define GRINDER_PIN 2

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

  kettleSense.current(KETTLE_PIN, 111.1);             // Current: input pin, calibration.
  kettleIrms = kettleSense.calcIrms(1480);

  grinderSense.current(KETTLE_PIN, 111.1);             // Current: input pin, calibration.
  grinderIrms = grinderSense.calcIrms(1480);

  //Give the calcIrms time to settle to base
  while(kettleIrms > 5 || grinderIrms > 5){
    delay(5000);
    Serial.print("Calibrating Kettle:");
    Serial.print(" ");
    Serial.println(kettleIrms);
    delay(100);
    kettleIrms = kettleSense.calcIrms(1480);

    Serial.print("Calibrating Grinder:");
    Serial.print(" ");
    Serial.println(grinderIrms);
    delay(100);
    grinderIrms = grinderSense.calcIrms(1480);
  }
}
void loop()
{
  kettleIrms = kettleSense.calcIrms(1480);
  if(kettleIrms > 10.0) {
    Serial.println("Bean! Beans! Beans!");
    MP3player.playMP3("beans.mp3");
    Serial.println("Sleeping for 6 minutes.");
    delay(360000);
  }
  delay(1000);
  Serial.print("Kettle Idle:");
  Serial.print(" ");
  Serial.println(kettleIrms);
  Serial.print("Grinder Idle:");
  Serial.print(" ");
  Serial.println(grinderIrms);
}