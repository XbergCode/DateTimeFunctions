#pragma once                                         // Run Once
//
//    FILE: DateTimeFunctions.h
//  AUTHOR: XbergCode
// VERSION: 1.0.0
// PURPOSE: Collection of date & time functions. Check, Convert, Get, Clock & Date. Works with RTC and NTP.
//     URL: https://github.com/XbergCode/DateTimeFunctions
//

// Library Version
#define DATETIMEFUNCTIONS_VERSION  (F("1.0.0\n"))    // Version 

// Date Format: 1 = DD/MM/YYYY & 2 = MM/DD/YYYY & 3 = YYYY/MM/DD & 4 = YYYY/DD/MM
#define DATETIMEFUNCTIONS_DEFAULT_DATE_FORMAT 1      // Default Date Format

// Use FLASH or SRAM Memory
#define DATETIMEFUNCTIONS_MEMORY_USE 0               // Use this memory. Flash = 0 & SRAM = 1>

// Include
#include <Arduino.h>                                 // Include The Arduino Library


class DateTimeFunctions {
  public:                                            // Public
    // Set
    DateTimeFunctions();                                                                                                // Set The Class Object Name
    void setDFormat(const uint8_t _SETDATEFORMAT);                                                                      // Set The Date Format
    void setCFormat(const bool _SETBOOL);                                                                               // Set The Clock Format
    void setDZero(const bool _SETBOOL);                                                                                 // Set The Date Zero
    void setHZero(const bool _SETBOOL);                                                                                 // Set The Hour Zero
    // Check
    bool isLeap(const uint16_t _YEAR);                                                                                  // Check For Leap Year
    bool isPM(const uint8_t _HOUR);                                                                                     // Check For AM / PM
    // Get
    uint16_t getDOY(const uint8_t _DAY, const uint8_t _MONTH, const uint16_t _YEAR);                                    // Get Day Of Year
    uint8_t getDOW(const uint8_t _DAY, const uint8_t _MONTH, const uint16_t _YEAR);                                     // Get Day Of Week
    char* getDOWNS(const uint8_t _DOW);                                                                                 // Get Day Of Week Name Short
    char* getDOWNL(const uint8_t _DOW);                                                                                 // Get Day Of Week Name Long
    uint8_t getWOY(const uint8_t _DAY, const uint8_t _MONTH, const uint16_t _YEAR);                                     // Get Week Of Year
    uint8_t getWIY(const uint16_t _YEAR);                                                                               // Get Weeks In Year
    uint16_t getDIY(const uint16_t _YEAR);                                                                              // Get Days In Year
    uint16_t getDLIY(const uint8_t _DAY, const uint8_t _MONTH, const uint16_t _YEAR);                                   // Get Days Left In Year
    uint8_t getDIM(const uint8_t _MONTH, const uint16_t _YEAR);                                                         // Get Days In Month
    char* getMNS(const uint8_t _MONTH);                                                                                 // Get Month Name Short
    char* getMNL(const uint8_t _MONTH);                                                                                 // Get Month Name Long
    uint16_t getDBD(const uint8_t _DAY1, const uint8_t _MONTH1, const uint16_t _YEAR1, const uint8_t _DAY2, const uint8_t _MONTH2, const uint16_t _YEAR2); // Get Days Between Date
    uint32_t getSFM(const uint8_t _HOUR, const uint8_t _MIN, const uint8_t _SEC);                                       // Get Seconds From Midnight
    uint16_t getMFM(const uint8_t _HOUR, const uint8_t _MIN);                                                           // Get Minutes From Midnight
    char* getAMPM(const uint8_t _HOUR);                                                                                 // Get AM / PM
    // Convert
    uint8_t con24To12(const uint8_t _HOUR);                                                                             // Convert 24.Hour To 12.Hour
    uint8_t con12To24(const uint8_t _HOUR, const char _AorP);                                                           // Convert 12.Hour To 24.Hour
    char* conDOY2DATE(const uint16_t _DOY, const uint16_t _YEAR, const bool _USEMONTHNAME = false);                     // Convert Day Of Year To Date
    uint8_t conDOY2D(const uint16_t _DOY, const uint16_t _YEAR);                                                        // Convert Day Of Year To Day
    uint8_t conDOY2M(const uint16_t _DOY, const uint16_t _YEAR);                                                        // Convert Day Of Year To Month
    uint32_t conDT2UT(const uint8_t _DAY, const uint8_t _MONTH, const uint16_t _YEAR, const uint8_t _HOUR, const uint8_t _MIN, const uint8_t _SEC); // Convert Date Time To Unix Time
    char* conUT2DT(const uint64_t _UNIXTIME, const bool _USEMONTHNAME = false);                                         // Convert Unix Time To Date Time
    char* conSec2Time(const uint32_t _SEC);                                                                             // Convert Seconds To Time
    char* conSec2Time64(const uint64_t _SEC);                                                                           // Convert Seconds To Time
    char* conSec2DT(const uint64_t _SEC, const bool _USEMONTHNAME = false);                                             // Convert Seconds To Date Time
    char* conSec2DT(const uint32_t _SEC, const uint16_t _YEAR, const bool _USEMONTHNAME = false);                       // Convert Seconds To Date Time
    uint32_t conDT2Sec(const uint8_t _DAY, const uint8_t _MONTH, const uint16_t _YEAR, const uint32_t _HOUR, const uint32_t _MIN, const uint8_t _SEC); // Convert Date Time To Seconds
    uint32_t conTime2Sec(const uint32_t _DAYS, const uint16_t _YEARS, const uint32_t _HOURS, const uint32_t _MIN, const uint8_t _SEC); // Convert Days, Years, Hours, Minutes And Seconds To Seconds
    char* conNum2Roman(const uint16_t _IN);                                                                             // Convert Number To Roman Numerals
    char* conGre2Jul(const uint8_t _DAY, const uint8_t _MONTH, const uint16_t _YEAR, const bool _USEMONTHNAME = false); // Convert Gregorian To Julian Calendar
    // Date
    char* date2Str(const uint8_t _DAY, const uint8_t _MONTH, const uint16_t _YEAR, const bool _USEMONTHNAME = false);   // Date To String
    // Clock
    char* clock2Str(const uint8_t _HOUR, const uint8_t _MIN, const uint8_t _SEC = 0, const bool _USESEC = false);       // Clock To String
    char* clock2StrAMPM(const uint8_t _HOUR, const uint8_t _MIN, const uint8_t _SEC = 0, const bool _USESEC = false);   // Clock To String With AM / PM
    char* sec2Clock(const uint32_t _SECONDS, const bool _USESEC = false);                                               // Seconds To Clock 24.hour
    char* sec2ClockAMPM(const uint32_t _SECONDS, const bool _USESEC = false);                                           // Seconds To Clock 12.hour With AM / PM
    char* romanClock(const uint8_t _HOUR, const uint8_t _MIN);                                                          // Roman Clock 24.hours
    char* romanClock12(const uint8_t _HOUR, const uint8_t _MIN);                                                        // Roman Clock 12.hours
    
  private:                                           // Private
    #define PM pgm_read_byte_near                    // Read Progmem Integer's
    // Days In Month
    static const uint8_t DIM_N[];                    // Days In Month Normal Year
    static const uint8_t DIM_L[];                    // Days In Month Leap Year
    // Month Names
    static const char MONTHNAME_S[][4];              // Month Names Short
    static const char MONTHNAME_L[][10];             // Month Names Full
    // Day Of Week Names
    static const char DAYNAME_S[][4];                // Day Names Short
    static const char DAYNAME_L[][10];               // Day Names Full
    // Static String Buffer
    static char __dateTimeStaticBuffer[];            // Static Buffer
    static char __conSec2TimeBuffer[];               // Static Buffer < For conSec2Time & conSec2Time64
    // Convert - Make The Date Time String
    char* MTDTS(const uint8_t _DAY, const uint8_t _MONTH, const uint16_t _YEAR, const uint8_t _HOUR, const uint8_t _MIN, const uint8_t _SEC, const uint8_t _TYPE);
    // Clock - Make The Clock String
    char* MTCS(const uint8_t _HOUR, const uint8_t _MIN, const uint8_t _SEC, const bool _USESEC, const bool _AMPM);
    // Date Format, Clock Format & Date Time Zero's
    static uint8_t _DATEFORMAT_;  // Date Format: 1 = DD/MM/YYYY & 2 = MM/DD/YYYY & 3 = YYYY/MM/DD & 4 = YYYY/DD/MM
    static bool _CLOCKFORMAT_;    // Clock Format: ture = 24.Hours & false = 12.Hours
    static bool _USEDATEZERO_;    // Date Zero: true = 01/01/YYYY & false = 1/1/YYYY
    static bool _USEHOURZERO_;    // Hour Zero: true = 01:MM:SS & false = 1:MM:SS
};

// End Of File.
