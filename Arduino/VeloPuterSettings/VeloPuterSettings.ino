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

// Battery type/Battery monitor
typedef enum { BATTERY_LiPo, BATTERY_LiFePO4, BATTERY_Unknown, BATTERY_not_monitored } BATTERY_TYPE;

// Intensity labels
typedef enum { INTENSITY_off, INTENSITY_low, INTENSITY_medium, INTENSITY_high, INTENSITY_max } INTENSITY_TYPE;

// Preferences collected in one handy structure
typedef struct {
    word wheelCircumference_mm;                 // For speed sensor
    word rearWheelCircumference_mm;             // To get sprocket counter correct
    byte numTimesToBlink;                       // Number of blinks for the auto-switchoff (0 to disable)
    byte mediumHeadlightIntensity;              // Set the mid-level intensity for headlights. 
                                                // Other default intensities are just fine and do not require 
                                                // a setting.
    INTENSITY_TYPE initialHeadlightIntensity;   // Startup intensity for the headlight.
    INTENSITY_TYPE initialRearlightIntensity;   // Startup intensity for the rearlight.
    byte teethOnChainring;                      // Number of teeth on the front chainring.
    boolean hasSchlumpf;                        // Is there a Schlumpf drive installed?
    byte numCogs;                               // Number of sprockets in the rear.
    byte TeethOnCassette[12];                   // teeth-count for the sprockets.
                                                // Wake me up when > 12 sprockets on the rear is an option 
                                                // on any velomobile.
    BATTERY_TYPE batteryType;                   // Type of battery used.
    float voltageOffset;                        // Offset for the voltage sensor.
    char veloName[50];                          // Model & serial number of the bike.
} VeloSettings;

// Proposed control:
//
// * Enter settings mode by holding the settings button voor 3 seconds (shorter flicks toggles display intensity)
// * Up/down (intensity toggle) changes value
// * left/right (blinkers) select digit
// * alarm switch goes to next item in the list
// * holding settings for 3 seconds saves new settings
// * holding alarm for 3 seconds exits settings mode without saving.


// Write VeloSettings struct to EEPROM
// 
void writeSettings(VeloSettings settings)
{
    int eeAddress = 0;
    pinMode(LED_BUILTIN, OUTPUT);
    
    // check that settings fit in EEPROM (1024 bytes)
    if (sizeof(settings) > EEPROM.length()) {
        // settings too large, quickly blink the LED on Arduino 10 times
        for (int ii=0; ii<10; ++ii) {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(100);
            digitalWrite(LED_BUILTIN, LOW);
            delay(200); 
        }
    } else {
        // write to EEPROM and turn on LED on Arduino for 2 seconds
        EEPROM.put(eeAddress, settings);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(2000);
        digitalWrite(LED_BUILTIN, LOW);
    }
}

// Read VeloSettings struct from EEPROM
//
VeloSettings readSettings()
{
    VeloSettings settings;
    int eeAddress = 0;
    EEPROM.get(eeAddress, settings);
    
    return settings;
}


// 
// as a sketch, including setting the built-in LED as output.
void setup(){
    // settings with defaults
    VeloSettings settings = {
        1450,
        1990,
        6,
        120,
        INTENSITY_low,
        INTENSITY_off,
        53,
        false,
        10,
        {11, 12, 13, 15, 17, 19, 22, 25, 28, 32},
        BATTERY_LiPo,
        0.00f,
        "IntercityBike DF 144"
    };

    writeSettings(settings);
}

void loop(){
    /* empty loop */
}
