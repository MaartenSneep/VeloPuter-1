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
 */

// What action to perform
#define WRITE_DEFUALT_SETTINGS       1
#define READ_SETTINGS                0
#define VELOMOBILE_MAKE_MODEL_SERIAL "IntercityBike DF 144"
#define SPROCKETS                    {11, 12, 13, 15, 17, 19, 22, 25, 28, 32, 0}
#define RING                         53
#define FRONT_WHEEL                  1450
#define REAR_WHEEL                   1990
 
#include <Arduino.h>
#include <EEPROM.h>

// Useful methods in EEPROM:
// * https://www.arduino.cc/en/Reference/EEPROMGet
// * https://www.arduino.cc/en/Reference/EEPROMPut

// Definition of the structure with the settings
#include "VeloPuterSettingsDefinitions.h"
#include "VeloPuterSettingsIO.cpp"


#if WRITE_DEFUALT_SETTINGS
void writeDefaultsAndCheck(){
    // settings with defaults
    VeloSettings settings, read_settings;
    
    byte sprockets[MAXIMUM_NUMBER_OF_SPROCKETS] = SPROCKETS;
    char name[MAXIMUM_LENGTH_VELONAME] = VELOMOBILE_MAKE_MODEL_SERIAL;
    
    settings.wheelCircumference_mm = FRONT_WHEEL;
    settings.rearWheelCircumference_mm = REAR_WHEEL;
    settings.numTimesToBlink = 6;
    settings.mediumHeadlightIntensity = 96;
    settings.initialHeadlightIntensity = INTENSITY_low;
    settings.initialRearlightIntensity = INTENSITY_off;
    settings.teethOnChainring = RING;
    settings.hasSchlumpf = false;
    for (int i=0; i < MAXIMUM_NUMBER_OF_SPROCKETS; ++i) {
        settings.TeethOnCassette[i] = sprockets[i];
        if(sprockets[i]) {
            settings.numCogs = i+1;
        } else {
            break;
        }
    }
    settings.batteryType = BATTERY_LiFePO4;
    settings.voltageOffset = 0.00f;
    
    for (int i=0; i < MAXIMUM_LENGTH_VELONAME; ++i){
        settings.veloName[i] = name[i];
        if (!name[i]) break;
    }
    
    Serial.print("Writing settings to EEPROM for bike '");
    Serial.print(settings.veloName);
    Serial.println("'");
    
    if (writeSettings(settings) != NO_ERROR) {
        Serial.println("Settings too large error");
        return;
    }
    
    // verify settings
    read_settings = readSettings();
    
    if (read_settings.wheelCircumference_mm != settings.wheelCircumference_mm) {
        Serial.print("Error in wheelCircumference_mm, read "); 
        Serial.print(read_settings.wheelCircumference_mm, DEC); 
        Serial.print(" expected ");
        Serial.println(settings.wheelCircumference_mm, DEC); 
        blink(ERROR_READING); 
        return;
    }
    if (read_settings.rearWheelCircumference_mm != settings.rearWheelCircumference_mm) {
        Serial.print("Error in rearWheelCircumference_mm, read "); 
        Serial.print(read_settings.rearWheelCircumference_mm, DEC); 
        Serial.print(" expected ");
        Serial.println(settings.rearWheelCircumference_mm, DEC);
        blink(ERROR_READING); 
        return;
    }
    if (read_settings.numTimesToBlink != settings.numTimesToBlink) {
        Serial.print("Error in numTimesToBlink, read "); 
        Serial.print(read_settings.numTimesToBlink, DEC); 
        Serial.print(" expected ");
        Serial.println(settings.numTimesToBlink, DEC); 
        blink(ERROR_READING); 
        return;
    }
    if (read_settings.mediumHeadlightIntensity != settings.mediumHeadlightIntensity) {
        Serial.print("Error in mediumHeadlightIntensity, read "); 
        Serial.print(read_settings.mediumHeadlightIntensity, DEC); 
        Serial.print(" expected ");
        Serial.println(settings.mediumHeadlightIntensity, DEC); 
        blink(ERROR_READING); 
        return;
    }
    if (read_settings.initialHeadlightIntensity != settings.initialHeadlightIntensity) {
        Serial.print("Error in initialHeadlightIntensity, read "); 
        Serial.print(read_settings.initialHeadlightIntensity, DEC); 
        Serial.print(" expected ");
        Serial.println(settings.initialHeadlightIntensity, DEC); 
        blink(ERROR_READING); 
        return;
    }
    if (read_settings.initialRearlightIntensity != settings.initialRearlightIntensity) {
        Serial.print("Error in initialRearlightIntensity, read "); 
        Serial.print(read_settings.initialRearlightIntensity, DEC); 
        Serial.print(" expected ");
        Serial.println(settings.initialRearlightIntensity, DEC); 
        blink(ERROR_READING); 
        return;
    }
    if (read_settings.teethOnChainring != settings.teethOnChainring) {
        Serial.print("Error in teethOnChainring, read "); 
        Serial.print(read_settings.teethOnChainring, DEC); 
        Serial.print(" expected ");
        Serial.println(settings.teethOnChainring, DEC); 
        blink(ERROR_READING); 
        return;
    }
    if (read_settings.hasSchlumpf != settings.hasSchlumpf) {
        Serial.print("Error in hasSchlumpf, read "); 
        Serial.print(read_settings.hasSchlumpf, DEC); 
        Serial.print(" expected ");
        Serial.println(settings.hasSchlumpf, DEC); 
        blink(ERROR_READING); 
        return;
    }
    if (read_settings.numCogs != settings.numCogs) {
        Serial.print("Error in numCogs, read "); 
        Serial.print(read_settings.numCogs, DEC); 
        Serial.print(" expected ");
        Serial.println(settings.numCogs, DEC); 
        blink(ERROR_READING); 
        return;
    }
    for (int i = 0; i < MAXIMUM_NUMBER_OF_SPROCKETS; ++i) {
        if (read_settings.TeethOnCassette[i] != settings.TeethOnCassette[i]) {
            Serial.print("Error in TeethOnCassette, read "); 
            Serial.print(read_settings.TeethOnCassette[i], DEC); 
            Serial.print(" expected ");
            Serial.print(settings.TeethOnCassette[i], DEC); 
            Serial.print(" in index ");
            Serial.println(i, DEC);
            blink(ERROR_READING); 
            return;
        }
    }
    if (read_settings.batteryType != settings.batteryType) {
        Serial.print("Error in batteryType, read "); 
        Serial.print(read_settings.batteryType, DEC); 
        Serial.print(" expected ");
        Serial.println(settings.batteryType, DEC); 
        blink(ERROR_READING); 
        return;
    }
    if (read_settings.voltageOffset != settings.voltageOffset) {
        Serial.print("Error in voltageOffset, read "); 
        Serial.print(read_settings.voltageOffset, 4); 
        Serial.print(" expected ");
        Serial.println(settings.voltageOffset, 4); 
        blink(ERROR_READING); 
        return;
    }
    for (int i=0; i < MAXIMUM_LENGTH_VELONAME; ++i){
        if (read_settings.veloName[i] != settings.veloName[i]) {
            Serial.print("Error in veloName, read '"); 
            Serial.print(read_settings.veloName); 
            Serial.print("' expected '");
            Serial.print(settings.veloName); 
            Serial.println("'");
            blink(ERROR_READING); 
            return;
        }
    }
    
    // no differences found
    Serial.print("All OK"); 
    Serial.println();
    blink(NO_ERROR);
}
#elif READ_SETTINGS
void readSettingsWriteToSerial(){
    VeloSettings settings;
    
    // verify settings
    settings = readSettings();
    
    Serial.print("wheelCircumference_mm\t"); 
    Serial.println(settings.wheelCircumference_mm, DEC); 

    Serial.print("rearWheelCircumference_mm\t"); 
    Serial.println(settings.rearWheelCircumference_mm, DEC); 

    Serial.print("numTimesToBlink\t"); 
    Serial.println(settings.numTimesToBlink, DEC); 

    Serial.print("mediumHeadlightIntensity\t"); 
    Serial.println(settings.mediumHeadlightIntensity, DEC); 

    Serial.print("initialHeadlightIntensity\t"); 
    Serial.println(settings.initialHeadlightIntensity, DEC); 

    Serial.print("initialRearlightIntensity\t"); 
    Serial.println(settings.initialRearlightIntensity, DEC); 

    Serial.print("teethOnChainring\t"); 
    Serial.println(settings.teethOnChainring, DEC); 

    Serial.print("hasSchlumpf\t"); 
    Serial.println(settings.hasSchlumpf, DEC); 

    Serial.print("numCogs\t"); 
    Serial.println(settings.numCogs, DEC); 

    for (int i = 0; i < MAXIMUM_NUMBER_OF_SPROCKETS; ++i) {
        Serial.print("TeethOnCassette["); 
        Serial.print(i, DEC);
        Serial.print("]\t"); 
        Serial.println(settings.TeethOnCassette[i], DEC); 
    }

    Serial.print("batteryType\t"); 
    Serial.println(settings.batteryType, DEC); 

    Serial.print("voltageOffset\t"); 
    Serial.println(settings.voltageOffset, 4); 

    Serial.print("veloName\t'"); 
    Serial.print(settings.veloName); 
    Serial.println("'");
}
#endif

void setup(){
    Serial.begin(9600);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }
    
#if WRITE_DEFUALT_SETTINGS
    writeDefaultsAndCheck();
#elif READ_SETTINGS
    readSettingsWriteToSerial();
#endif

    Serial.end();
}

void loop(){
    /* empty loop */
}
