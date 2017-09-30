#include <Arduino.h>
#include <U8glib.h>

#define QUATRO   // Configuration management: STRADA QUILTJE and QUATRO 
#define Ug82

/* Arduino Micro pin configuration/ capabilities

    See also: https://www.arduino.cc/en/Main/ArduinoBoardMicro

  20 Digital input/outputs
  Intrupts: on 5 lines.
  PWM: on 7 lines
  Analog in: 12 lines

  0          INT     (Serial RX)
  1          INT     (Serial TX)
  2          INT     (TWI SDA)
  3      PWM INT     (TWI SCL)
  4/A6           ANA
  5      PWM
  6/A7   PWM     ANA
  7          INT
  8/A8           ANA
  9/A9   PWM     ANA     // Note that Timer 1 does break the pwm in this pin
  10/A10  PWM     ANA     // Note that Timer 1 does break the pwm in this pin
  11      PWM
  12/A11          ANA
  13      PWM
  A0              ANA
  A1              ANA
  A2              ANA
  A3              ANA
  A4              ANA
  A5              ANA



*/

const byte switchCadPin =       0;      // NOTE: 0 and 1 are the RX and TX pins.
const byte switchSpdPin =       1;      // NOTE: 0 and 1 are the RX and TX pins.
const byte OLED_SDA =           2;      // used for the oled display through u8glib
const byte OLED_SCL =           3;      // used for the oled display through u8glib
const byte switchConfigPin =    4;
const byte ledHeadPin =         5;
const byte ledAuxPin =          6;
const byte switchBrakePin =     7;
const byte switchAlarmPin =     8;
const byte ledRightPin =        9;
const byte ledLeftPin =         10;
const byte ledRearPin =         11;
const byte UNUSED3 =            12;
const byte speakerPin =         13;
const byte voltagePin =         A0;

#if defined(STRADA)
const byte switchHeadUpPin =    A2;
const byte switchHeadDownPin =  A1;
const byte switchRightPin =     A3;
const byte switchLeftPin =      A4;
#elif defined(QUILTJE)
const byte switchRightPin =     A3; //A1;
const byte switchLeftPin =      A4; //A2;
const byte switchHeadDownPin =  A1; //A3;
const byte switchHeadUpPin =    A2; //A4;
#elif defined(QUATRO)
const byte switchRightPin =     A1;
const byte switchLeftPin =      A2;
const byte switchHeadDownPin =  A3;
const byte switchHeadUpPin =    A4;
#endif
const byte UNUSED4 =            A5;


/*
   Other constants which are used throughout the programm:
*/
const long tSleep_ms = 300000;              // Timeout until sleep when cadence and speed sensor are active
const long tSleepNoCadSpd_ms = 1800000;     // Timeout until sleep when cadence and speed sensor are NOT active
const int  tPeriodBlink_ms = 333;           // 1.5 Hz Note that it actually is have a period.
const byte numTimesToBlink = 5;             // 7 times high, 6 times low, = 13 = 4.3 s
//
#if defined(STRADA)
const int  wheelCircumvention_mm = 1590;    // 406-50 wheel
#elif defined(QUILTJE)
const int  wheelCircumvention_mm = 1450;    // 406-28 wheel
#elif defined(QUATRO)
const int  wheelCircumvention_mm = 1540;   // 406-40 wheel
#endif

const byte speakerVolume = 50;
const byte setBrakeMaxTimeOn_s =  15; // max time for a brakelight to be on.
byte setOledIntensity = 0;

const byte setTeethOnCainring = 53; //Gert: 70, STefan: 75 MaartenS: 53
const float setTeethOnCassette[20] = {11.0, 13.0, 15.0, 17.0, 19.0, 22.0, 25.0, 28.0, 32.0, 36.0,
                                      2750.0,  3250.0,  37.5, 42.5, 47.5, 55.0, 62.5, 70.0, 80.0, 90.0
                                     }; // Quatro Note that I put two of the gears to 100 time the number to avoid confusion with the normal low gearing (36 etc). S11 and S13 will not be displayed.
const String setTeethOnCassette_string[20] = {"11", "13", "15", "17", "19", "22", "25", "28", "32", "36",
                                              "s11", "s13", "s15", "s17", "s19", "s22", "s25", "s28", "s32", "s36"
                                             }; // Quatro

//const byte setTeethOnCassette[10] = {11,12,14,16,18,20,22,25,28,32}; // Stefan


/*
  Defaullt intensiteiten
*/
const int leftLedOffIntensity = 0; // Note that currently pwm is disabled for the blinker pins. See of this has anything to do with the failing display/.
const int leftLedLowIntensity = 255;
const int leftLedMediumIntensity = 255;
const int leftLedHighIntensity = 255;
const int leftLedMaxIntensity = 255;

const int rightLedOffIntensity = 0;
const int rightLedLowIntensity = 255;
const int rightLedMediumIntensity = 255;
const int rightLedHighIntensity = 255;
const int rightLedMaxIntensity = 255;

const int rearLedOffIntensity = 8;
const int rearLedLowIntensity = 32;
const int rearLedMediumIntensity = 64;
const int rearLedHighIntensity = 64; // Note that this stops the up/down!
const int rearLedMaxIntensity = 255;

#if  defined(STRADA)
const int headLedOffIntensity = 0;
const int headLedLowIntensity = 16;
const int headLedMediumIntensity = 96;
const int headLedHighIntensity = 255;
const int headLedMaxIntensity = 255;

const int auxLedOffIntensity = 0; //aux is just an other headlinght
const int auxLedLowIntensity = 16;
const int auxLedMediumIntensity = 96;
const int auxLedHighIntensity = 255;
const int auxLedMaxIntensity = 255;

#elif defined(QUILTJE)
const int headLedOffIntensity = 0;
const int headLedLowIntensity = 32;
const int headLedMediumIntensity = 128;
const int headLedHighIntensity = 255;
const int headLedMaxIntensity = 255;

const int auxLedOffIntensity = 0; // aux is the floodlight
const int auxLedLowIntensity = 16;
const int auxLedMediumIntensity = 32;
const int auxLedHighIntensity = 255;
const int auxLedMaxIntensity = 255;

#elif defined(QUATRO)
const int headLedOffIntensity = 0;
const int headLedLowIntensity = 32;
const int headLedMediumIntensity = 96;
const int headLedHighIntensity = 255;
const int headLedMaxIntensity = 255;

const int auxLedOffIntensity = 0; // aux is the brakelight
const int auxLedLowIntensity = 255;
const int auxLedMediumIntensity = 255;
const int auxLedHighIntensity = 255;
const int auxLedMaxIntensity = 255;
#endif

