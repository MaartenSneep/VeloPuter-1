/*********************************************************************
 *
 *  Settings module for the Velomobiel led driver board by Gert Beumer
 *
 **********************************************************************
 *
 * 2017
 * by Maarten Sneep
 *
 */

/*********************************************************************
 *
 * Store/Read settings in EEPROM
 * 
 * This is a slightly longer sub-project
 * Let's start with collecting the items that need to be stored in EEPROM
 */
 
#include <Arduino.h>
#include <EEPROM.h>

// Useful methods in EEPROM:
// * https://www.arduino.cc/en/Reference/EEPROMGet
// * https://www.arduino.cc/en/Reference/EEPROMPut

#include <U8g2lib.h>

// init to be added (Gert?)

#include "VeloPuterSettingsDefinitions.h"
#include "VeloPuterSettingsIO.cpp"

// Proposed control:
//
// * Enter settings mode by holding the settings button voor 3 seconds (shorter flicks toggles display intensity)
// * Up/down (intensity toggle) changes value
// * left/right (blinkers) select digit
// * alarm switch goes to next item in the list
// * holding settings for 3 seconds saves new settings
// * holding alarm for 3 seconds exits settings mode without saving.

VeloSettings settings;

// 
// as a sketch, including setting the built-in LED as output.
void setup(){
    // read settings from EEPROM
    settings = readSettings();
    
}

void loop(){
    /* empty loop */
}
