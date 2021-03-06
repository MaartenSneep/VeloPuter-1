#include <Arduino.h>
#include <U8glib.h>

#define ICB_DF // Configuration management: STRADA QUILTJE QUATRO and ICB_DF

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

const byte switchCadPin =    0; // NOTE: 0 and 1 are the RX and TX pins.
const byte switchSpdPin =    1; // NOTE: 0 and 1 are the RX and TX pins.
const byte OLED_SDA =        2; // used for the oled display through u8glib
const byte OLED_SCL =        3; // used for the oled display through u8glib
const byte switchConfigPin = 4;
const byte ledHeadPin =      5;
const byte ledAuxPin =       6;
const byte switchBrakePin =  7;
const byte switchAlarmPin =  8;
const byte ledRightPin =     9;
const byte ledLeftPin =     10;
const byte ledRearPin =     11;
const byte UNUSED3 =        12;
const byte speakerPin =     13;
const byte voltagePin =     A0;

#if defined(STRADA)
const byte switchHeadUpPin   = A2;
const byte switchHeadDownPin = A1;
const byte switchRightPin    = A3;
const byte switchLeftPin     = A4;
#elif defined(QUILTJE)
const byte switchRightPin    = A3; //A1;
const byte switchLeftPin     = A4; //A2;
const byte switchHeadDownPin = A1; //A3;
const byte switchHeadUpPin   = A2; //A4;
#elif defined(QUATRO) || defined(ICB_DF)
const byte switchRightPin    = A1;
const byte switchLeftPin     = A2;
const byte switchHeadDownPin = A3;
const byte switchHeadUpPin   = A4;
#endif
const byte UNUSED4           = A5;

/*
   Other constants which are used throughout the programm:
*/
const long tSleep_ms = 300000; // Timeout until sleep when cadence and speed sensor are active
const long tSleepNoCadSpd_ms = 1800000; // Timeout until sleep when cadence and speed sensor are NOT active
const int tPeriodBlink_ms = 333; // 1.5 Hz Note that it actually is half a period.
const byte numTimesToBlink = 6; // 7 times high, 6 times low, = 13 = 4.3 s
//
#if defined(STRADA)
const word wheelCircumference_mm     = 1590; // 406-50 wheel
const word rearWheelCircumference_mm = 2075; // 559-50 wheel
#elif defined(QUILTJE)
const word wheelCircumference_mm     = 1450; // 406-28 wheel
const word rearWheelCircumference_mm = 1990; // 559-35 wheel
#elif defined(QUATRO)
const word wheelCircumference_mm     = 1540; // 406-40 wheel
const word rearWheelCircumference_mm = wheelCircumference_mm; // identical
#elif defined(ICB_DF)
const word wheelCircumference_mm     = 1450; // 406-28 wheel
const word rearWheelCircumference_mm = 1990; // 559-35 wheel
#endif
const float gearOnCassette_scaling  = ((float)rearWheelCircumference_mm / (float)wheelCircumference_mm);

const byte speakerVolume       = 50;
const byte setBrakeMaxTimeOn_s = 15; // max time for a brakelight to be on.
byte setOledIntensity          = 0;

#if defined(ICB_DF)
const byte setTeethOnChainring = 53; // MaartenS: 53
#elif defined(QUATRO)
const byte setTeethOnChainring = 70; // Gert: 70, STefan: 75 MaartenS: 53
#else
const byte setTeethOnChainring = 53; // MaartenS: 53
#endif

#if defined(QUATRO)
const byte setTeethOnCassette[20] = { 11, 13, 15, 17, 19, 22, 25, 28, 32, 36,
    2750, 3250, 37, 42, 47, 55, 62, 70, 80, 90 }; // Quatro Note that I put two of the gears to 100 time the number to avoid confusion with the normal low gearing (36 etc). S11 and S13 will not be displayed.
/*
const String setTeethOnCassette_string[20] = {"11", "13", "15", "17", "19", "22", "25", "28", "32", "36",
                                              "s11", "s13", "s15", "s17", "s19", "s22", "s25", "s28", "s32", "s36"
                                             };*/
#elif defined(ICB_DF)
const byte setTeethOnCassette[10] = { 11, 12, 13, 15, 17, 19, 22, 25, 28, 32 };
/*
const String setTeethOnCassette_string[10] = {"11", "12", "13", "15", "17", "19", "22", "25", "28", "32"};
*/
#else
const byte setTeethOnCassette[20] = { 11, 13, 15, 17, 19, 22, 25, 28, 32, 36,
    2750, 3250, 37, 42, 47, 55, 62, 70, 80, 90 }; // Quatro Note that I put two of the gears to 100 time the number to avoid confusion with the normal low gearing (36 etc). S11 and S13 will not be displayed.
/*
const String setTeethOnCassette_string[20] = {"11", "13", "15", "17", "19", "22", "25", "28", "32", "36",
                                              "s11", "s13", "s15", "s17", "s19", "s22", "s25", "s28", "s32", "s36"
                                             }; // Quatro
*/
#endif

/*
  Default intensiteiten
*/
const byte leftLedOffIntensity = 0; // Note that currently pwm is disabled for the blinker pins. See of this has anything to do with the failing display/.
const byte leftLedLowIntensity = 255;
const byte leftLedMediumIntensity = 255;
const byte leftLedHighIntensity = 255;
const byte leftLedMaxIntensity = 255;

const byte rightLedOffIntensity = 0;
const byte rightLedLowIntensity = 255;
const byte rightLedMediumIntensity = 255;
const byte rightLedHighIntensity = 255;
const byte rightLedMaxIntensity = 255;

const byte rearLedOffIntensity = 8;
const byte rearLedLowIntensity = 32;
const byte rearLedMediumIntensity = 64;
const byte rearLedHighIntensity = 64; // Note that this stops the up/down!
const byte rearLedMaxIntensity = 255;

#if defined(STRADA)
const byte headLedOffIntensity = 0;
const byte headLedLowIntensity = 16;
const byte headLedMediumIntensity = 96;
const byte headLedHighIntensity = 255;
const byte headLedMaxIntensity = 255;

const byte auxLedOffIntensity = 0; //aux is just an other headlinght
const byte auxLedLowIntensity = 16;
const byte auxLedMediumIntensity = 96;
const byte auxLedHighIntensity = 255;
const byte auxLedMaxIntensity = 255;

#elif defined(QUILTJE)
const byte headLedOffIntensity = 0;
const byte headLedLowIntensity = 32;
const byte headLedMediumIntensity = 128;
const byte headLedHighIntensity = 255;
const byte headLedMaxIntensity = 255;

const byte auxLedOffIntensity = 0; // aux is the floodlight
const byte auxLedLowIntensity = 16;
const byte auxLedMediumIntensity = 32;
const byte auxLedHighIntensity = 255;
const byte auxLedMaxIntensity = 255;

#elif defined(QUATRO)
const byte headLedOffIntensity = 0;
const byte headLedLowIntensity = 32;
const byte headLedMediumIntensity = 96;
const byte headLedHighIntensity = 255;
const byte headLedMaxIntensity = 255;

const byte auxLedOffIntensity = 0; // aux is the brakelight
const byte auxLedLowIntensity = 255;
const byte auxLedMediumIntensity = 255;
const byte auxLedHighIntensity = 255;
const byte auxLedMaxIntensity = 255;

#elif defined(ICB_DF)
const byte headLedOffIntensity = 0;
const byte headLedLowIntensity = 32;
const byte headLedMediumIntensity = 96;
const byte headLedHighIntensity = 255;
const byte headLedMaxIntensity = 255;

const byte auxLedOffIntensity = 0; // aux is not connected in the ICB_DF
const byte auxLedLowIntensity = 0;
const byte auxLedMediumIntensity = 0;
const byte auxLedHighIntensity = 0;
const byte auxLedMaxIntensity = 0;

#endif
