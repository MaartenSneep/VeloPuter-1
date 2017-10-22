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
#include <Arduino.h>
#include <EEPROM.h>

#include "VeloPuterSettingsDefinitions.h"

// Write VeloSettings struct to EEPROM
int writeSettings(VeloSettings settings)
{
    int eeAddress = 0;
    pinMode(LED_BUILTIN, OUTPUT);
    
    // check that settings fit in EEPROM (1024 bytes)
    if (sizeof(settings) > EEPROM.length()) {
        blink(ERROR_SETTINGS_TOO_LARGE);
        return ERROR_SETTINGS_TOO_LARGE;
    } else {
        // write to EEPROM and turn on LED on Arduino for 2 seconds
        EEPROM.put(eeAddress, settings);
        return NO_ERROR;
    }
}


// Read VeloSettings struct from EEPROM
VeloSettings readSettings()
{
    VeloSettings settings;
    int eeAddress = 0;
    EEPROM.get(eeAddress, settings);
    
    return settings;
}


// Simple error communication (details via serial)
void blink(int error)
{
    pinMode(LED_BUILTIN, OUTPUT);
    switch(error)
    {
        case NO_ERROR: // on for 2 seconds
            digitalWrite(LED_BUILTIN, HIGH);
            delay(2000);
            digitalWrite(LED_BUILTIN, LOW);
            break;
        case ERROR_SETTINGS_TOO_LARGE: // blink 10 times (on: short, off: long)
            for (int ii=0; ii<10; ++ii) {
                digitalWrite(LED_BUILTIN, HIGH);
                delay(200);
                digitalWrite(LED_BUILTIN, LOW);
                delay(400); 
            }
            break;
        case ERROR_READING: // blink 10 times (on: long, off: short)
            for (int ii=0; ii<10; ++ii) {
                digitalWrite(LED_BUILTIN, HIGH);
                delay(400);
                digitalWrite(LED_BUILTIN, LOW);
                delay(200); 
            }
            break;
        default:
            break;
    }
}

