[![Arduino CI](https://github.com/XbergCode/DateTimeFunctions/workflows/Arduino%20CI/badge.svg)](https://github.com/marketplace/actions/arduino_ci)
[![Arduino-lint](https://github.com/XbergCode/DateTimeFunctions/actions/workflows/arduino-lint.yml/badge.svg)](https://github.com/XbergCode/DateTimeFunctions/actions/workflows/arduino-lint.yml)
[![JSON check](https://github.com/XbergCode/DateTimeFunctions/actions/workflows/jsoncheck.yml/badge.svg)](https://github.com/XbergCode/DateTimeFunctions/actions/workflows/jsoncheck.yml)
[![License: MIT](https://img.shields.io/github/license/XbergCode/DateTimeFunctions)](https://github.com/XbergCode/DateTimeFunctions/blob/master/LICENSE)
[![GitHub release (latest by date)](https://img.shields.io/github/v/release/XbergCode/DateTimeFunctions)](https://github.com/XbergCode/DateTimeFunctions/releases)
![code size:](https://img.shields.io/github/languages/code-size/XbergCode/DateTimeFunctions)


# DateTimeFunctions

Arduino library to work with date & time.  
Tested on: Uno, Mega, ESP32 & NodeMCU  


## Description

The DateTimeFunctions library contains collection of functions to work with date and time.  
You can easily make date and clock strings.  

Date formats are: "1 = DD/MM/YYYY & 2 = MM/DD/YYYY & 3 = YYYY/MM/DD & 4 = YYYY/DD/MM".  
Date strings can be made with or without month name, example: 01/Jan/2023 or 01/01/2023  
Date strings can be made with or without leading zeros, example: 01/01/2023 or 1/1/2023  

Clock strings can be made with or without leading hour zeros, example: 01:01 or 1:01  
Clock strings can be made with or without seconds, example: 01:01:01 or 01:01  
Clock strings can be made in 24 hour and 12 hour format.  


> [!NOTE]  
> 
> It is possible to use Flash or SRAM mermory. See line 17 in DateTimeFunctions.h ⬅️ Default is Flash.  
> You can save up to 27.bytes(SRAM) by using one buffer if you use conSec2Time/64. See Line 44-48 in DateTimeFunctions.cpp.  


## Interface

The following functions are implemented:

### Include.  
```cpp
// Include the library.
#include <DateTimeFunctions.h>

// Set the library class object name.  
DateTimeFunctions dTF;
```

### Set.  
```cpp
// Set the date format.  
void setDFormat(const uint8_t _SETDATEFORMAT);
// Date Format: 1 = DD/MM/YYYY & 2 = MM/DD/YYYY & 3 = YYYY/MM/DD & 4 = YYYY/DD/MM

// Set the clock format "24.h / 12.h."  
void setCFormat(const bool _SETBOOL);
// Clock Format: ture = 24.Hours & false = 12.Hours

// Set the date zero.  
void setDZero(const bool _SETBOOL);
// Date Zero: true = 01/01/YYYY & false = 1/1/YYYY

// Set the hour zero.  
void setHZero(const bool _SETBOOL);
// Hour Zero: true = 01:MM:SS & false = 1:MM:SS
```

### Check.  
```cpp
// Check for leap year.  
bool isLeap(const uint16_t _YEAR);
// Returns: true or false

// Check for AM / PM.  
bool isPM(const uint8_t _HOUR);
// Returns: true or false
```

### Get.  
```cpp
// Get day of year.  
uint16_t getDOY(const uint8_t _DAY, const uint8_t _MONTH, const uint16_t _YEAR);
// Returns: 0 ... 366

// Get day of week.  
uint8_t getDOW(const uint8_t _DAY, const uint8_t _MONTH, const uint16_t _YEAR);
// Returns: 0 ... 6 "Sun ... Sat"

// Get day of week name short.  
char* getDOWNS(const uint8_t _DOW);
// Returns: Sun ... Sat

// Get day of week name long.  
char* getDOWNL(const uint8_t _DOW);
// Returns: Sunday ... Saturday

// Get week of year.  
uint8_t getWOY(const uint8_t _DAY, const uint8_t _MONTH, const uint16_t _YEAR);
// Returns: 1 ... 53

// Get weeks in year.  
uint8_t getWIY(const uint16_t _YEAR);
// Returns: 52 or 53

// Get days in year.  
uint16_t getDIY(const uint16_t _YEAR);
// Returns: 365 or 366

// Get days left in year.  
uint16_t getDLIY(const uint8_t _DAY, const uint8_t _MONTH, const uint16_t _YEAR);
// Returns: 0 ... 365

// Get days in month.  
uint8_t getDIM(const uint8_t _MONTH, const uint16_t _YEAR);
// Returns: 28 ... 31

// Get month name short.  
char* getMNS(const uint8_t _MONTH);
// Returns: Jan ... Dec

// Get month name long.  
char* getMNL(const uint8_t _MONTH);
// Returns: January ... December

// Get days between date.  
uint16_t getDBD(const uint8_t _DAY1, const uint8_t _MONTH1, const uint16_t _YEAR1, const uint8_t _DAY2, const uint8_t _MONTH2, const uint16_t _YEAR2);
// Returns: 0 ... 65535

// Get seconds from midnight.  
uint32_t getSFM(const uint8_t _HOUR, const uint8_t _MIN, const uint8_t _SEC);
// Returns: 0 ... 86399

// Get minutes from midnight.  
uint16_t getMFM(const uint8_t _HOUR, const uint8_t _MIN);
// Returns: 0 ... 1439

// Get AM or PM.  
char* getAMPM(const uint8_t _HOUR);
// Returns: AM or PM
```

### Convert.  
```cpp
// Convert 24.h to 12.h.  
uint8_t con24To12(const uint8_t _HOUR);
// Returns: 1 ... 12

// Convert 12.h to 24.h.  
uint8_t con12To24(const uint8_t _HOUR, const char _AorP);
// Returns: 0 ... 23

// Convert day of year to date.  
char* conDOY2DATE(const uint16_t _DOY, const uint16_t _YEAR, const bool _USEMONTHNAME = false);
// Returns: Date string

// Convert day of year to day.  
uint8_t conDOY2D(const uint16_t _DOY, const uint16_t _YEAR);
// Returns: 1 ... 31

// Convert day of year to month.  
uint8_t conDOY2M(const uint16_t _DOY, const uint16_t _YEAR);
// Returns: 1 ... 12

// Convert date time to unix time.  
uint32_t conDT2UT(const uint8_t _DAY, const uint8_t _MONTH, const uint16_t _YEAR, const uint8_t _HOUR, const uint8_t _MIN, const uint8_t _SEC);
// Returns: 0 ... 4294967295

// Convert unix time to date time.  
char* conUT2DT(const uint64_t _UNIXTIME, const bool _USEMONTHNAME = false);
// Returns: Date time string

// Convert seconds to time.  
char* conSec2Time(const uint32_t _SEC);
// Returns: Time string "10.sec ... 10.year, 11.days, 12.hours, 13.min, 14.sec"

// Convert seconds to time "64.bits".  
char* conSec2Time64(const uint64_t _SEC);
// Returns: Time string "10.sec ... 10.year, 11.days, 12.hours, 13.min, 14.sec"

// Convert seconds to date time.  
char* conSec2DT(const uint64_t _SEC, const bool _USEMONTHNAME = false);
// Returns: Date string

// Convert seconds to date time, from year X.  
char* conSec2DT(const uint32_t _SEC, const uint16_t _YEAR, const bool _USEMONTHNAME = false);
// Returns: Date string

// Convert date time to seconds.  
uint32_t conDT2Sec(const uint8_t _DAY, const uint8_t _MONTH, const uint16_t _YEAR, const uint32_t _HOUR, const uint32_t _MIN, const uint8_t _SEC);
// Returns: 0 ... 51909555

// Convert time to seconds.  
uint32_t conTime2Sec(const uint32_t _DAYS, const uint16_t _YEARS, const uint32_t _HOURS, const uint32_t _MIN, const uint8_t _SEC);
// Returns: 0 ... 4294967295

// Convert number to roman numerals.  
char* conNum2Roman(const uint16_t _IN);
// Returns: N ... MMMMMMMMMM

// Convert Gregorian to Julian calendar.  
char* conGre2Jul(const uint8_t _DAY, const uint8_t _MONTH, const uint16_t _YEAR, const bool _USEMONTHNAME = false);
// Returns: Date string
```

### Date.  
```cpp
// Date to string.  
char* date2Str(const uint8_t _DAY, const uint8_t _MONTH, const uint16_t _YEAR, const bool _USEMONTHNAME = false);
// Returns: Date string
```

### Clock.  
```cpp
// Clock to string.  
char* clock2Str(const uint8_t _HOUR, const uint8_t _MIN, const uint8_t _SEC = 0, const bool _USESEC = false);
// Returns: Clock string "01:01 or 1:01" / "01:01:01 or 1:01:01"

// Clock to string with AM / PM.  
char* clock2StrAMPM(const uint8_t _HOUR, const uint8_t _MIN, const uint8_t _SEC = 0, const bool _USESEC = false);
// Returns: Clock string with AM or PM "01:01 AM or 1:01 AM" / "01:01:01 AM or 1:01:01 AM"

// Seconds to clock string.  
char* sec2Clock(const uint32_t _SECONDS, const bool _USESEC = false);
// Returns: Clock string "01:01 or 1:01" / "01:01:01 or 1:01:01"

// Seconds to clock string with AM / PM.  
char* sec2ClockAMPM(const uint32_t _SECONDS, const bool _USESEC = false);
// Returns: Clock string with AM or PM "01:01 AM or 1:01 AM" / "01:01:01 AM or 1:01:01 AM"

// Roman clock 24.h format.  
char* romanClock(const uint8_t _HOUR, const uint8_t _MIN);
// Returns: N:N ... XXIII:LIX

// Roman clock 12.h format.  
char* romanClock12(const uint8_t _HOUR, const uint8_t _MIN);
// Returns: I:N ... XI:LIX
```

## Outputs

### Check.  
2023 Is Not a Leap Year  
20.Hour is PM  

### Get.  
Day Of Year: 59  
Day Of Week: 2  
Day Of Week Name Short: Tue  
Day Of Week Name Long: Tuesday  
Week Of Year: 9  
Weeks In Year: 52  
Days In Year: 365  
Days Left In Year: 306  
Days In Month: 28  
Month Name Short: Feb  
Month Name Long: February  
Days Between Date: 1395  
Seconds From Midnight: 72000  
Minutes From Midnight: 1200  
AM or PM: PM  

### Convert.  
24.hour To 12.hour: 8  
12.hour To 24.hour: 20  
Day Of Year To Date: 28/02/2023  
Day Of Year To Day: 28  
Day Of Year To Month: 2  
Date Time To Unix Time: 1677614400  
Unix Time To Date Time: 28/Feb/2023 - 20:00:00  
Seconds To Date Time: 1/3/1 - 05:20:35  
Seconds To Date Time: 07/09/2024 - 4:45:50 PM  
Seconds To Time: 1.year, 4.days, 15.hours, 23.min, 20.sec  
Seconds To Time 64: 1.year, 4.days, 15.hours, 23.min, 20.sec  
Date Time To Seconds: 3079400  
Time To Seconds: 31937000  
Number To Roman: MMXXIII  
Gregorian To Julian: 15/02/2023  

### Date.  
Date To String: 28/02/2023  
Date To String: 28/Feb/2023  
Date To String: 2/28/2023  
Date To String: Feb/28/2023  
Date Time String: Tuesday 28/02/2023 - 20:00:00  
Date Time String: Tuesday Feb/28/2023 - 8:00 PM  

### Clock.  
Clock To String: 20:00:00  
Clock To String: 08:00  
Clock To String With AM PM: 8:00:00 PM  
Seconds To Clock: 13:20  
Seconds To Clock: 1:20:09  
Seconds To Clock With AM PM: 1:20 PM  
Roman Clock 24: XX:N  
Roman Clock 12: VIII:N  


## Operation

See examples.


## Performance

Most time is spend creating strings.
