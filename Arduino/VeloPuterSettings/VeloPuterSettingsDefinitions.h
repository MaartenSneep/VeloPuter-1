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

// Battery type/Battery monitor
typedef enum { BATTERY_LiPo, BATTERY_LiFePO4, BATTERY_Unknown, BATTERY_not_monitored } BATTERY_TYPE;

// Intensity labels
typedef enum { INTENSITY_off, INTENSITY_low, INTENSITY_medium, INTENSITY_high, INTENSITY_max } INTENSITY_TYPE;

const int MAXIMUM_NUMBER_OF_SPROCKETS = 12;
const int MAXIMUM_LENGTH_VELONAME = 50;

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
    byte TeethOnCassette[MAXIMUM_NUMBER_OF_SPROCKETS]; // teeth-count for the sprockets.
                                                // Wake me up when > 12 sprockets on the rear is an option 
                                                // on any velomobile.
    BATTERY_TYPE batteryType;                   // Type of battery used.
    float voltageOffset;                        // Offset for the voltage sensor.
    char veloName[MAXIMUM_LENGTH_VELONAME];     // Model & serial number of the bike.
} VeloSettings;

// Local error handling
typedef enum { NO_ERROR=0, ERROR_SETTINGS_TOO_LARGE, ERROR_READING } EEPROM_ERROR;
