//
//    FILE: DateTimeFunctions.cpp
//  AUTHOR: XbergCode
// VERSION: 1.0.0
// PURPOSE: Collection of date & time functions. Check, Convert, Get, Clock & Date. Works with RTC and NTP.
//     URL: https://github.com/XbergCode/DateTimeFunctions
//
// HISTORY
//   1.0.0  11/09/2023  Initial version
//

#include "DateTimeFunctions.h"                    // Include The DateTimeFunctions Header

#if DATETIMEFUNCTIONS_MEMORY_USE == 0             // Use Flash
// Days In Month
const uint8_t DateTimeFunctions::DIM_N[12] PROGMEM = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};  // Days In Month - Not Leap Year
const uint8_t DateTimeFunctions::DIM_L[12] PROGMEM = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};  // Days In Month - Leap Year
// Month Names
const char DateTimeFunctions::MONTHNAME_S[12][4] PROGMEM = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
const char DateTimeFunctions::MONTHNAME_L[12][10] PROGMEM = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
// Day Of Week Names
const char DateTimeFunctions::DAYNAME_S[7][4] PROGMEM = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const char DateTimeFunctions::DAYNAME_L[7][10] PROGMEM = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
#else                                             // Use SRAM
// Days In Month
const uint8_t DateTimeFunctions::DIM_N[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // Days In Month - Not Leap Year
const uint8_t DateTimeFunctions::DIM_L[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // Days In Month - Leap Year
// Month Names
const char DateTimeFunctions::MONTHNAME_S[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
const char DateTimeFunctions::MONTHNAME_L[12][10] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
// Day Of Week Names
const char DateTimeFunctions::DAYNAME_S[7][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const char DateTimeFunctions::DAYNAME_L[7][10] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
#endif

// Static String Buffer
char DateTimeFunctions::__dateTimeStaticBuffer[27];    // Static String Buffer < If neither conSec2Time nor conSec2Time64 is used.
//char DateTimeFunctions::__dateTimeStaticBuffer[46];    // Static String Buffer < If conSec2Time is used
//char DateTimeFunctions::__dateTimeStaticBuffer[54];    // Static String Buffer < If conSec2Time64 is used

// Static String Buffer - For conSec2Time & conSec2Time64
// This is 27.bytes(SRAM) bigger than using one string 54.b size if conSec2Time/64 is used.
// Else saves 27.bytes(SRAM) if conSec2Time/64 is not used.
// If you are going to use conSec2Time/64 and want to safe some space.
// Block lines: 37 & 49 >
// then unblock lines: 38 or 39 >
// Next go to line: 891-892 and line: 967-968 >
// and unblock "char* _buffer = __dateTimeStaticBuffer;" and block "char* _buffer = __conSec2TimeBuffer;"
char DateTimeFunctions::__conSec2TimeBuffer[54];       // Static String Buffer < For conSec2Time & conSec2Time64

// Date Format, Clock Format & Date Time Zero's
uint8_t DateTimeFunctions::_DATEFORMAT_ = DATETIMEFUNCTIONS_DEFAULT_DATE_FORMAT; // Date Format
bool DateTimeFunctions::_CLOCKFORMAT_   = true;        // Clock Format: ture = 24.Hours & false = 12.Hours
bool DateTimeFunctions::_USEDATEZERO_   = true;        // Date Zero: true = 01/01/YYYY & false = 1/1/YYYY
bool DateTimeFunctions::_USEHOURZERO_   = true;        // Hour Zero: true = 01:MM:SS & false = 1:MM:SS


//---------------------- Set ----------------------//

// Set The Class Object Name
DateTimeFunctions::DateTimeFunctions() {}

// Set The Date Format - setDFormat(1-4) 1-4
void DateTimeFunctions::setDFormat(const uint8_t _SETDATEFORMAT) {
  // Set The Date Format
  // 1 = DD/MM/YYYY & 2 = MM/DD/YYYY & 3 = YYYY/MM/DD & 4 = YYYY/DD/MM
  if (_SETDATEFORMAT >= 1 && _SETDATEFORMAT <= 4)
    _DATEFORMAT_ = _SETDATEFORMAT;
  else
    _DATEFORMAT_ = DATETIMEFUNCTIONS_DEFAULT_DATE_FORMAT;
}

// Set The Clock Format - setCFormat(true/false) - true = 24.Hours & false = 12.Hours
void DateTimeFunctions::setCFormat(const bool _SETBOOL) {
  // Set The Clock Format
  _CLOCKFORMAT_ = _SETBOOL; // ture = 24.Hours & false = 12.Hours
}

// Set The Date Zero - setDZero(true/false) - true = 01/01/YYYY & false = 1/1/YYYY
void DateTimeFunctions::setDZero(const bool _SETBOOL) {
  // Set The Date Zero
  _USEDATEZERO_ = _SETBOOL;              // true = Use Zero's & false = Dont Use Zero's
}

// Set The Hour Zero - setHZero(true/false) - true = 01:MM:SS & false = 1:MM:SS
void DateTimeFunctions::setHZero(const bool _SETBOOL) {
  // Set The Hour Zero
  _USEHOURZERO_ = _SETBOOL;              // true = Use Zero's & false = Dont Use Zero's
}


//--------------------- Check ---------------------//

// Check For Leap Year - isLeap(year) = Returns: true If Leap Year "0-1"
bool DateTimeFunctions::isLeap(const uint16_t _YEAR) {
  // Check For Leap Year
  if (_YEAR % 400U == 0U || (_YEAR % 4U == 0U && _YEAR % 100U != 0U)) return true; // Leap Year     = 1
  else return false;                                                               // Not Leap Year = 0
}

// Check For AM / PM - isPM(hour) = Returns: true If PM "0-1"
bool DateTimeFunctions::isPM(const uint8_t _HOUR) {
  // Check For AM / PM
  // _HOUR needs to be in 24.hour format.
  if (_HOUR >= 12) return true;
  else return false;
}

//---------------------- Get ----------------------//

// Get Day Of Year - getDOY(day, month, year) = Returns: 1-366
uint16_t DateTimeFunctions::getDOY(const uint8_t _DAY, const uint8_t _MONTH, const uint16_t _YEAR) {
  // Day Of Year
  uint16_t _doy = 0U;                                                   // Final DOY
  uint8_t _i = 0;                                                       // for() Counter
  if (_YEAR % 400U == 0U || (_YEAR % 4U == 0U && _YEAR % 100U != 0U)) { // Leap Year
    #if DATETIMEFUNCTIONS_MEMORY_USE == 0                               // Use Flash
      for (_i = 0; _i < _MONTH - 1; _i++) _doy += PM(DIM_L + _i);       // Add The Months - Use Flash
    #else                                                               // Use SRAM
      for (_i = 0; _i < _MONTH - 1; _i++) _doy += DIM_L[_i];            // Add The Months - Use SRAM
    #endif
    _doy += _DAY;                                                       // Add The Days
  }
  else {                                                                // Not Leap Year
    #if DATETIMEFUNCTIONS_MEMORY_USE == 0                               // Use Flash
      for (_i = 0; _i < _MONTH - 1; _i++) _doy += PM(DIM_N + _i);       // Add The Months - Use Flash
    #else                                                               // Use SRAM
      for (_i = 0; _i < _MONTH - 1; _i++) _doy += DIM_N[_i];            // Add The Months - Use SRAM
    #endif
    _doy += _DAY;                                                       // Add The Days
  }
  // Return The Day Of Year
  return _doy;                                                          // Return The Day Of Year
}

// Get Day Of Week - getDOW(day, month, year) = Returns: 0-6 "Sun-Sat"
uint8_t DateTimeFunctions::getDOW(const uint8_t _DAY, const uint8_t _MONTH, const uint16_t _YEAR) {
  // Day Of Week
  // https://en.wikipedia.org/wiki/Determination_of_the_day_of_the_week
  // Sakamoto's method.
  // Sunday = 0 | Monday = 1 | Tuesday = 2 | Wednesday = 3 | Thursday = 4 | Friday = 5 | Saturday = 6
  #if DATETIMEFUNCTIONS_MEMORY_USE == 0                               // Use Flash
    const static uint8_t _DOWMONTHCAL[12] PROGMEM = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
  #else                                                               // Use SRAM
    const static uint8_t _DOWMONTHCAL[12] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
  #endif
  
  uint16_t _year = _YEAR;
  if (_MONTH < 3) _year -= 1U;
  #if DATETIMEFUNCTIONS_MEMORY_USE == 0                               // Use Flash
    return (_year + _year / 4 - _year / 100 + _year / 400 + PM(_DOWMONTHCAL + (_MONTH - 1)) + _DAY) % 7;
  #else                                                               // Use SRAM
    return (_year + _year / 4 - _year / 100 + _year / 400 + _DOWMONTHCAL[_MONTH - 1] + _DAY) % 7;
  #endif
}

// Get Day Of Week Name Short - getDOWNS(0-6) = Returns: Sun-Sat
char* DateTimeFunctions::getDOWNS(const uint8_t _DOW) {
  // Day Of Week Name Short
  //const static char DAYNAME_S[7][4] PROGMEM = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
  char* _buffer = __dateTimeStaticBuffer;                                   // String Buffer
  #if DATETIMEFUNCTIONS_MEMORY_USE == 0                                     // Use Flash
    if (_DOW >= 7) return strcpy_P(_buffer, PSTR("Err"));                   // Error
    return strcpy_P(_buffer, DAYNAME_S[_DOW]);                              // Make The String
  #else                                                                     // Use SRAM
    if (_DOW >= 7) return strcpy(_buffer, "Err");                           // Error
    return strcpy(_buffer, DAYNAME_S[_DOW]);                                // Make The String
  #endif
}

// Get Day Of Week Name Long - getDOWNL(0-6) = Returns: Sunday-Saturday
char* DateTimeFunctions::getDOWNL(const uint8_t _DOW) {
  // Day Of Week Name Long - 10924 / 285
  //const static char DAYNAME_L[7][10] PROGMEM = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
  char* _buffer = __dateTimeStaticBuffer;                                   // String Buffer
  #if DATETIMEFUNCTIONS_MEMORY_USE == 0                                     // Use Flash
    if (_DOW >= 7) return strcpy_P(_buffer, PSTR("Error"));                 // Error
    return strcpy_P(_buffer, DAYNAME_L[_DOW]);                              // Make The String
  #else                                                                     // Use SRAM
    if (_DOW >= 7) return strcpy(_buffer, "Error");                         // Error
    return strcpy(_buffer, DAYNAME_L[_DOW]);                                // Make The String
  #endif
}

// Get Week Of Year - getWOY(day, month, year) = Returns: 1-53 (ISO 8601)
uint8_t DateTimeFunctions::getWOY(const uint8_t _DAY, const uint8_t _MONTH, const uint16_t _YEAR) {
  // Week Of Year
  // https://en.wikipedia.org/wiki/ISO_week_date
  uint8_t _woy;                                                 // Week Of Year - (WOY)
  const uint16_t _DOY = getDOY(_DAY, _MONTH, _YEAR);            // Get Day Of Year - (DOY)
  uint8_t _DOW = getDOW(_DAY, _MONTH, _YEAR);                   // Get DOW - Sunday = 0 & Saturday = 6
  if (_DOW == 0) _DOW = 7;                                      // Change DOW - Monday = 1 & Sunday = 7
  _woy = (10 + _DOY - _DOW) / 7;                                // Week Of Year - Raw
  if (_woy == 0 || _woy == 53) {                                // In January Or December
    // December
    if (_woy == 53) {                                           // December
      const uint8_t _dec31DOW = getDOW(31, 12, _YEAR);          // Get DOW - Sunday = 0 & Saturday = 6
      if (_dec31DOW >= 1 && _dec31DOW <= 3) _woy = 1;           // Dec 31 Is On Monday, Tuesday, or Wednesday
      else if (_dec31DOW == 4) _woy = 53;                       // Dec 31 Is On Thursday
      else if (_dec31DOW == 5) {                                // Dec 31 Is On Friday
        _woy = 52;                                              // Dec 31 Is On Friday
        if (_YEAR % 400U == 0U || (_YEAR % 4U == 0U && _YEAR % 100U != 0U)) // Leap Year
          _woy = 53;                                            // Dec 31 Is On Friday For Leap Year
      }
      else if (_dec31DOW == 6 || _dec31DOW == 0) _woy = 52;     // Dec 31 Is On Saturday or Sunday
    }
    // January
    else {                                                      // January
      const uint8_t _jan1DOW = getDOW(1, 1, _YEAR);             // Get DOW - Sunday = 0 & Saturday = 6
      if (_jan1DOW == 5) _woy = 53;                             // Jan 1 Is On Friday
      else if (_jan1DOW == 6) {                                 // Jan 1 Is On Saturday
        _woy = 52;                                              // Jan 1 Is On Saturday
        if (_YEAR % 400U == 0U || (_YEAR % 4U == 0U && _YEAR % 100U != 0U)) // Leap Year
          _woy = 53;                                            // Jan 1 Is On Saturday For Leap Year
      }
      else if (_jan1DOW == 0) _woy = 52;                        // Jan 1 Is On Sunday
      else _woy = 1;                                            // Jan 1 Is On Monday, Tuesday, Wednesday or Thursday
    }
  }
  return _woy;                                                  // Return The Week Of Year
}

// Get Weeks In Year - getWIY(year) = Returns: 52-53
uint8_t DateTimeFunctions::getWIY(const uint16_t _YEAR) {
  // Weeks In Year
  // https://en.wikipedia.org/wiki/ISO_week_date
  // 53 week years occur on all years that have Thursday as the 1st of January,
  // and on leap years that start on Wednesday as the 1st of January.
  const uint8_t _DOW = getDOW(1, 1, _YEAR);  // Get The Day Of The Week 1 January
  if (_DOW == 3 && isLeap(_YEAR)) return 53; // Jan 1 Is On Wednesday & Its Leap Year. 53 Weeks
  else if (_DOW == 4) return 53;             // Jan 1 Is On Thursday. 53 Weeks
  else return 52;                            // 52 Weeks
}

// Get Days In Year - getDIY(year) = Returns: 365-366
uint16_t DateTimeFunctions::getDIY(const uint16_t _YEAR) {
  // Days In Year
  // https://en.wikipedia.org/wiki/Common_year
  // https://en.wikipedia.org/wiki/Leap_year
  if (_YEAR % 400U == 0U || (_YEAR % 4U == 0U && _YEAR % 100U != 0U)) return 366U; // Leap Year     - 366 Days
  else return 365U;                                                                // Not Leap Year - 365 Days
}

// Get Days Left In Year - getDLIY(day, month, year) = Returns: 0-365
uint16_t DateTimeFunctions::getDLIY(const uint8_t _DAY, const uint8_t _MONTH, const uint16_t _YEAR) {
  // Days Left In Year
  uint16_t _diy = 0U;                                                   // Days In Year
  uint16_t _doy = 0U;                                                   // Day Of Year
  uint8_t _i = 0;                                                       // for() Counter
  if (_YEAR % 400U == 0U || (_YEAR % 4U == 0U && _YEAR % 100U != 0U)) { // Leap Year
    _diy = 366U;                                                        // 366 Days
    #if DATETIMEFUNCTIONS_MEMORY_USE == 0                               // Use Flash
      for (_i = 0; _i < _MONTH - 1; _i++) _doy += PM(DIM_L + _i);       // Add The Months - Use Flash
    #else                                                               // Use SRAM
      for (_i = 0; _i < _MONTH - 1; _i++) _doy += DIM_L[_i];            // Add The Months - Use SRAM
    #endif
    _doy += _DAY;                                                       // Add The Days
  }
  else {                                                                // Not Leap Year
    _diy = 365U;                                                        // 365 Days
    #if DATETIMEFUNCTIONS_MEMORY_USE == 0                               // Use Flash
      for (_i = 0; _i < _MONTH - 1; _i++) _doy += PM(DIM_N + _i);       // Add The Months - Use Flash
    #else                                                               // Use SRAM
      for (_i = 0; _i < _MONTH - 1; _i++) _doy += DIM_N[_i];            // Add The Months - Use SRAM
    #endif
    _doy += _DAY;                                                       // Add The Days
  }
  return _diy - _doy;                                                   // Return The Days Left In Year
}

// Get Days In Month - getDIM(month, year) = Returns: 28-31
uint8_t DateTimeFunctions::getDIM(const uint8_t _MONTH, const uint16_t _YEAR) {
  // Days In Month
  #if DATETIMEFUNCTIONS_MEMORY_USE == 0                                 // Use Flash
    if (_YEAR % 400U == 0U || (_YEAR % 4U == 0U && _YEAR % 100U != 0U)) // Leap Year
      return PM(DIM_L + _MONTH - 1U);                                   // Return
    else                                                                // Not A Leap Year
      return PM(DIM_N + _MONTH - 1U);                                   // Return
  #else                                                                 // Use SRAM
    if (_YEAR % 400U == 0U || (_YEAR % 4U == 0U && _YEAR % 100U != 0U)) // Leap Year
      return DIM_L[_MONTH - 1U];                                        // Return
    else                                                                // Not A Leap Year
      return DIM_N[_MONTH - 1U];                                        // Return
  #endif
}

// Get Month Name Short - getMNS(month) = Returns: Jan-Dec
char* DateTimeFunctions::getMNS(const uint8_t _MONTH) {
  // Month Names Short
  //const static char MONTHNAME_S[12][4] PROGMEM = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  char* _buffer = __dateTimeStaticBuffer;                                     // String Buffer
  #if DATETIMEFUNCTIONS_MEMORY_USE == 0                                       // Use Flash
    if (_MONTH <= 0 || _MONTH >= 13) return strcpy_P(_buffer, PSTR("Err"));   // Error
    return strcpy_P(_buffer, MONTHNAME_S[_MONTH - 1]);                        // Make The String
  #else                                                                       // Use SRAM
    if (_MONTH <= 0 || _MONTH >= 13) return strcpy(_buffer, "Err");           // Error
    return strcpy(_buffer, MONTHNAME_S[_MONTH - 1]);                          // Make The String
  #endif
}

// Get Month Name Long - getMNL(month) = Returns: January-December
char* DateTimeFunctions::getMNL(const uint8_t _MONTH) {
  // Month Names Long
  //const static char MONTHNAME_L[12][10] PROGMEM = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
  char* _buffer = __dateTimeStaticBuffer;                                     // String Buffer
  #if DATETIMEFUNCTIONS_MEMORY_USE == 0                                       // Use Flash
    if (_MONTH <= 0 || _MONTH >= 13) return strcpy_P(_buffer, PSTR("Error")); // Error
    return strcpy_P(_buffer, MONTHNAME_L[_MONTH - 1]);                        // Make The String
  #else                                                                       // Use SRAM
    if (_MONTH <= 0 || _MONTH >= 13) return strcpy(_buffer, "Error");         // Error
    return strcpy(_buffer, MONTHNAME_L[_MONTH - 1]);                          // Make The String
  #endif
}

// Get Days Between Date - getDBD(day1, month1, year1, day2, month2, year2) = Returns: 0-65535
uint16_t DateTimeFunctions::getDBD(const uint8_t _DAY1, const uint8_t _MONTH1, const uint16_t _YEAR1, const uint8_t _DAY2, const uint8_t _MONTH2, const uint16_t _YEAR2) {
  // Days Between Date
  
  // Wrong Format - Return
  if (_YEAR1 > _YEAR2) return 0U;
  if (_YEAR1 >= _YEAR2 && _MONTH1 >= _MONTH2 && _DAY1 > _DAY2) return 0U;
  if (_YEAR1 == _YEAR2 && _MONTH1 >= _MONTH2 && _DAY1 >= _DAY2) return 0U;

  // Variables
  uint16_t _years = 0U;                                   // Leap Days + Days Between Years
  uint8_t _i = 0;                                         // for() Counter

  // Count Total Days For Date 1
  uint16_t _days1 = 365U + _DAY1;                         // One Year + Days
  for (_i = 0; _i < _MONTH1 - 1; _i++)                    // Roll Thro The Months
    #if DATETIMEFUNCTIONS_MEMORY_USE == 0                 // Use Flash
      _days1 += PM(DIM_N + _i);                           // Add The Months Days - Use Flash Memory
    #else                                                 // Use SRAM
      _days1 += DIM_N[_i];                                // Add The Months Days - Use SRAM Memory
    #endif
  _years = _YEAR1;                                        // Set Year For Leap Days Calculating
  if (_MONTH1 <= 2) _years--;                             // Subtrack One Year From Years
  _days1 += _years / 4U - _years / 100U + _years / 400U;  // Calculate The Total Leap Days

  // Count Total Days For Date 2
  uint16_t _days2 = 365U + _DAY2;                         // One Year + Days
  for (_i = 0; _i < _MONTH2 - 1; _i++)                    // Roll Thro The Months
    #if DATETIMEFUNCTIONS_MEMORY_USE == 0                 // Use Flash
      _days2 += PM(DIM_N + _i);                           // Add The Months Days - Use Flash Memory
    #else                                                 // Use SRAM
      _days2 += DIM_N[_i];                                // Add The Months Days - Use SRAM Memory
    #endif
  _years = _YEAR2;                                        // Set Year For Leap Days Calculating
  if (_MONTH2 <= 2) _years--;                             // Subtrack One Year From Years
  _days2 += _years / 4U - _years / 100U + _years / 400U;  // Calculate The Total Leap Days

  // Days Between Years
  _years = (_YEAR2 - _YEAR1) * 365U;                      // Calculate The Days Between
  _days2 += _years;                                       // Add The Days Between

  // Return The Days Between Dates
  return _days2 - _days1;                                 // Return The Days Between Dates
}

// Get Seconds From Midnight - getSFM(hour, minute, seconds) = Returns: 0-86399
uint32_t DateTimeFunctions::getSFM(const uint8_t _HOUR, const uint8_t _MIN, const uint8_t _SEC) {
  // Seconds From Midnight
  return (_HOUR * 3600UL) + (_MIN * 60UL) + _SEC;
}

// Get Minutes From Midnight - getMFM(hour, minute) = Returns: 0-1439
uint16_t DateTimeFunctions::getMFM(const uint8_t _HOUR, const uint8_t _MIN) {
  // Minutes From Midnight
  return (_HOUR * 60U) + _MIN;
}

// Get AM / PM - getAMPM(hour) = Returns: AM or PM
char* DateTimeFunctions::getAMPM(const uint8_t _HOUR) {
  // Get AM / PM
  // _HOUR needs to be in 24.hour format.
  // Max String = XX = 2.characters.
  char* _buffer = __dateTimeStaticBuffer;            // String Buffer
  if (_HOUR >= 12) _buffer[0] = 'P';                 // PM (Post Meridiem) "after noon" - Add The P
  else _buffer[0] = 'A';                             // AM (Ante Meridiem) "before noon" - Add The A
  _buffer[1] = 'M';                                  // Add The M
  _buffer[2] = '\0';                                 // Null Terminate The String
  return _buffer;                                    // Return The Time String
}


//-------------------- Convert --------------------//

// Convert 24.Hour To 12.Hour - con24To12(hour) = Returns: 1-12
uint8_t DateTimeFunctions::con24To12(const uint8_t _HOUR) {
  // Convert 24.Hour To 12.Hour
  uint8_t _hour = _HOUR;                                  // Get The Hour
  if (_HOUR > 12) _hour -= 12;                            // Convert 24.h To 12.h (PM)
  else if (_HOUR == 0) _hour = 12;                        // Convert 24.h To 12.h (AM)
  return _hour;                                           // Return The Hour
}

// Convert 12.Hour To 24.Hour - con12To24(hour, AorP) = Returns: 0-23
uint8_t DateTimeFunctions::con12To24(const uint8_t _HOUR, const char _AorP) {
  // Convert 12.Hour To 24.Hour
  uint8_t _hour = _HOUR;                                  // Get The Hour
  if ((_AorP == 'P' || _AorP == 'p') && _HOUR != 12)      // Convert 12.h To 24.h (PM)
    _hour += 12;                                          // Add To The Hour
  else if ((_AorP == 'A' || _AorP == 'a') && _HOUR == 12) // Convert 12.h To 24.h (AM) 
    _hour = 0;                                            // Set The Hour To 0
  return _hour;                                           // Return The Hour
}

// Make The Date Time String - Private
char* DateTimeFunctions::MTDTS(const uint8_t _DAY, const uint8_t _MONTH, const uint16_t _YEAR, const uint8_t _HOUR, const uint8_t _MIN, const uint8_t _SEC, const uint8_t _TYPE) {
  // Make The Date Time String
  // _TYPE: 0 = 01/01/0000 - HH:MM:SS  &  1 = 01/Jan/0000 - HH:MM:SS  &  2 = 01/01/0000  &  3 = 01/Jan/0000
  // Max String = 31/12/65535 - 23:59:59 = 22.characters.
  // Max String = 31/Dec/65535 - 23:59:59 = 23.characters.
  // Max String = 31/Dec/65535 - 12:59:59 AM = 26.characters.
  char* _buffer = __dateTimeStaticBuffer;                     // String Buffer
  uint8_t _index = 0;                                         // String Index

  // Make The String
  if (_DATEFORMAT_ == 1) {                                    // DD/MM/YYYY
    // Day
    if (_USEDATEZERO_ || _DAY >= 10)                          // Add The First Digit
      _buffer[_index++] = (_DAY / 10) + '0';                  // Add The First Digit In Date
    _buffer[_index++] = (_DAY % 10) + '0';                    // Add The Second Digit In Date
    _buffer[_index++] = '/';                                  // Add The /

    // Month
    if (_TYPE == 0 || _TYPE == 2) {                           // Add Month Number
      if (_USEDATEZERO_ || _MONTH >= 10)                      // Add The First Digit
        _buffer[_index++] = (_MONTH / 10) + '0';              // Add The First Digit In Month
      _buffer[_index++] = (_MONTH % 10) + '0';                // Add The Second Digit In Month
      _buffer[_index++] = '/';                                // Add The /
    }
    else if (_TYPE == 1 || _TYPE == 3) {                      // Add The Month Names (Short)
      #if DATETIMEFUNCTIONS_MEMORY_USE == 0                                       // Use Flash
        _buffer[_index++] = (char)pgm_read_word(&(MONTHNAME_S[_MONTH - 1][0]));   // First Characters
        _buffer[_index++] = (char)pgm_read_word(&(MONTHNAME_S[_MONTH - 1][1]));   // Second Characters
        _buffer[_index++] = (char)pgm_read_word(&(MONTHNAME_S[_MONTH - 1][2]));   // Third Characters
      #else                                                                       // Use SRAM
        _buffer[_index++] = MONTHNAME_S[_MONTH - 1][0];                           // First Characters
        _buffer[_index++] = MONTHNAME_S[_MONTH - 1][1];                           // Second Characters
        _buffer[_index++] = MONTHNAME_S[_MONTH - 1][2];                           // Third Characters
      #endif
      _buffer[_index++] = '/';                                                    // Add The /
    }

    // Year
    if ((_YEAR / 1000U) > 0)
      _buffer[_index++] = (_YEAR / 1000U) + '0';              // Add The First Digit In Year
    if (((_YEAR / 10U) / 10U) % 10U > 0 || (_YEAR / 1000U) > 0)
      _buffer[_index++] = ((_YEAR / 10U) / 10U) % 10U + '0';  // Add The Second Digit In Year
    if ((_YEAR / 10U) % 10U > 0 || ((_YEAR / 10U) / 10U) % 10U > 0)
      _buffer[_index++] = (_YEAR / 10U) % 10U + '0';          // Add The Third Digit In Year
    _buffer[_index++] = _YEAR % 10U + '0';                    // Add The Fourth Digit In Year
  }
  else if (_DATEFORMAT_ == 2) {                               // MM/DD/YYYY
    // Month
    if (_TYPE == 0 || _TYPE == 2) {                           // Add Month Number
      if (_USEDATEZERO_ || _MONTH >= 10)                      // Add The First Digit
        _buffer[_index++] = (_MONTH / 10) + '0';              // Add The First Digit In Month
      _buffer[_index++] = (_MONTH % 10) + '0';                // Add The Second Digit In Month
      _buffer[_index++] = '/';                                // Add The /
    }
    else if (_TYPE == 1 || _TYPE == 3) {                      // Add The Month Names (Short)
      #if DATETIMEFUNCTIONS_MEMORY_USE == 0                                       // Use Flash
        _buffer[_index++] = (char)pgm_read_word(&(MONTHNAME_S[_MONTH - 1][0]));   // First Characters
        _buffer[_index++] = (char)pgm_read_word(&(MONTHNAME_S[_MONTH - 1][1]));   // Second Characters
        _buffer[_index++] = (char)pgm_read_word(&(MONTHNAME_S[_MONTH - 1][2]));   // Third Characters
      #else                                                                       // Use SRAM
        _buffer[_index++] = MONTHNAME_S[_MONTH - 1][0];                           // First Characters
        _buffer[_index++] = MONTHNAME_S[_MONTH - 1][1];                           // Second Characters
        _buffer[_index++] = MONTHNAME_S[_MONTH - 1][2];                           // Third Characters
      #endif
      _buffer[_index++] = '/';                                                    // Add The /
    }

    // Day
    if (_USEDATEZERO_ || _DAY >= 10)                          // Add The First Digit
      _buffer[_index++] = (_DAY / 10) + '0';                  // Add The First Digit In Date
    _buffer[_index++] = (_DAY % 10) + '0';                    // Add The Second Digit In Date
    _buffer[_index++] = '/';                                  // Add The /

    // Year
    if ((_YEAR / 1000U) > 0)
      _buffer[_index++] = (_YEAR / 1000U) + '0';              // Add The First Digit In Year
    if (((_YEAR / 10U) / 10U) % 10U > 0 || (_YEAR / 1000U) > 0)
      _buffer[_index++] = ((_YEAR / 10U) / 10U) % 10U + '0';  // Add The Second Digit In Year
    if ((_YEAR / 10U) % 10U > 0 || ((_YEAR / 10U) / 10U) % 10U > 0)
      _buffer[_index++] = (_YEAR / 10U) % 10U + '0';          // Add The Third Digit In Year
    _buffer[_index++] = _YEAR % 10U + '0';                    // Add The Fourth Digit In Year
  }
  else if (_DATEFORMAT_ == 3) {                               // YYYY/MM/DD
    // Year
    if ((_YEAR / 1000U) > 0)
      _buffer[_index++] = (_YEAR / 1000U) + '0';              // Add The First Digit In Year
    if (((_YEAR / 10U) / 10U) % 10U > 0 || (_YEAR / 1000U) > 0)
      _buffer[_index++] = ((_YEAR / 10U) / 10U) % 10U + '0';  // Add The Second Digit In Year
    if ((_YEAR / 10U) % 10U > 0 || ((_YEAR / 10U) / 10U) % 10U > 0)
      _buffer[_index++] = (_YEAR / 10U) % 10U + '0';          // Add The Third Digit In Year
    _buffer[_index++] = _YEAR % 10U + '0';                    // Add The Fourth Digit In Year
    _buffer[_index++] = '/';                                  // Add The /

    // Month
    if (_TYPE == 0 || _TYPE == 2) {                           // Add Month Number
      if (_USEDATEZERO_ || _MONTH >= 10)                      // Add The First Digit
        _buffer[_index++] = (_MONTH / 10) + '0';              // Add The First Digit In Month
      _buffer[_index++] = (_MONTH % 10) + '0';                // Add The Second Digit In Month
      _buffer[_index++] = '/';                                // Add The /
    }
    else if (_TYPE == 1 || _TYPE == 3) {                      // Add The Month Names (Short)
      #if DATETIMEFUNCTIONS_MEMORY_USE == 0                                       // Use Flash
        _buffer[_index++] = (char)pgm_read_word(&(MONTHNAME_S[_MONTH - 1][0]));   // First Characters
        _buffer[_index++] = (char)pgm_read_word(&(MONTHNAME_S[_MONTH - 1][1]));   // Second Characters
        _buffer[_index++] = (char)pgm_read_word(&(MONTHNAME_S[_MONTH - 1][2]));   // Third Characters
      #else                                                                       // Use SRAM
        _buffer[_index++] = MONTHNAME_S[_MONTH - 1][0];                           // First Characters
        _buffer[_index++] = MONTHNAME_S[_MONTH - 1][1];                           // Second Characters
        _buffer[_index++] = MONTHNAME_S[_MONTH - 1][2];                           // Third Characters
      #endif
      _buffer[_index++] = '/';                                                    // Add The /
    }

    // Day
    if (_USEDATEZERO_ || _DAY >= 10)                          // Add The First Digit
      _buffer[_index++] = (_DAY / 10) + '0';                  // Add The First Digit In Date
    _buffer[_index++] = (_DAY % 10) + '0';                    // Add The Second Digit In Date
  }
  else if (_DATEFORMAT_ == 4) {                               // YYYY/DD/MM
    // Year
    if ((_YEAR / 1000U) > 0)
      _buffer[_index++] = (_YEAR / 1000U) + '0';              // Add The First Digit In Year
    if (((_YEAR / 10U) / 10U) % 10U > 0 || (_YEAR / 1000U) > 0)
      _buffer[_index++] = ((_YEAR / 10U) / 10U) % 10U + '0';  // Add The Second Digit In Year
    if ((_YEAR / 10U) % 10U > 0 || ((_YEAR / 10U) / 10U) % 10U > 0)
      _buffer[_index++] = (_YEAR / 10U) % 10U + '0';          // Add The Third Digit In Year
    _buffer[_index++] = _YEAR % 10U + '0';                    // Add The Fourth Digit In Year
    _buffer[_index++] = '/';                                  // Add The /

    // Day
    if (_USEDATEZERO_ || _DAY >= 10)                          // Add The First Digit
      _buffer[_index++] = (_DAY / 10) + '0';                  // Add The First Digit In Date
    _buffer[_index++] = (_DAY % 10) + '0';                    // Add The Second Digit In Date
    _buffer[_index++] = '/';                                  // Add The /

    // Month
    if (_TYPE == 0 || _TYPE == 2) {                           // Add Month Number
      if (_USEDATEZERO_ || _MONTH >= 10)                      // Add The First Digit
        _buffer[_index++] = (_MONTH / 10) + '0';              // Add The First Digit In Month
      _buffer[_index++] = (_MONTH % 10) + '0';                // Add The Second Digit In Month
    }
    else if (_TYPE == 1 || _TYPE == 3) {                      // Add The Month Names (Short)
      #if DATETIMEFUNCTIONS_MEMORY_USE == 0                                       // Use Flash
        _buffer[_index++] = (char)pgm_read_word(&(MONTHNAME_S[_MONTH - 1][0]));   // First Characters
        _buffer[_index++] = (char)pgm_read_word(&(MONTHNAME_S[_MONTH - 1][1]));   // Second Characters
        _buffer[_index++] = (char)pgm_read_word(&(MONTHNAME_S[_MONTH - 1][2]));   // Third Characters
      #else                                                                       // Use SRAM
        _buffer[_index++] = MONTHNAME_S[_MONTH - 1][0];                           // First Characters
        _buffer[_index++] = MONTHNAME_S[_MONTH - 1][1];                           // Second Characters
        _buffer[_index++] = MONTHNAME_S[_MONTH - 1][2];                           // Third Characters
      #endif
    }
  }
  else {                                                      // DD/MM/YYYY
    // Day
    if (_USEDATEZERO_ || _DAY >= 10)                          // Add The First Digit
      _buffer[_index++] = (_DAY / 10) + '0';                  // Add The First Digit In Date
    _buffer[_index++] = (_DAY % 10) + '0';                    // Add The Second Digit In Date
    _buffer[_index++] = '/';                                  // Add The /

    // Month
    if (_TYPE == 0 || _TYPE == 2) {                           // Add Month Number
      if (_USEDATEZERO_ || _MONTH >= 10)                      // Add The First Digit
        _buffer[_index++] = (_MONTH / 10) + '0';              // Add The First Digit In Month
      _buffer[_index++] = (_MONTH % 10) + '0';                // Add The Second Digit In Month
      _buffer[_index++] = '/';                                // Add The /
    }
    else if (_TYPE == 1 || _TYPE == 3) {                      // Add The Month Names (Short)
      #if DATETIMEFUNCTIONS_MEMORY_USE == 0                                       // Use Flash
        _buffer[_index++] = (char)pgm_read_word(&(MONTHNAME_S[_MONTH - 1][0]));   // First Characters
        _buffer[_index++] = (char)pgm_read_word(&(MONTHNAME_S[_MONTH - 1][1]));   // Second Characters
        _buffer[_index++] = (char)pgm_read_word(&(MONTHNAME_S[_MONTH - 1][2]));   // Third Characters
      #else                                                                       // Use SRAM
        _buffer[_index++] = MONTHNAME_S[_MONTH - 1][0];                           // First Characters
        _buffer[_index++] = MONTHNAME_S[_MONTH - 1][1];                           // Second Characters
        _buffer[_index++] = MONTHNAME_S[_MONTH - 1][2];                           // Third Characters
      #endif
      _buffer[_index++] = '/';                                                    // Add The /
    }

    // Year
    if ((_YEAR / 1000U) > 0)
      _buffer[_index++] = (_YEAR / 1000U) + '0';              // Add The First Digit In Year
    if (((_YEAR / 10U) / 10U) % 10U > 0 || (_YEAR / 1000U) > 0)
      _buffer[_index++] = ((_YEAR / 10U) / 10U) % 10U + '0';  // Add The Second Digit In Year
    if ((_YEAR / 10U) % 10U > 0 || ((_YEAR / 10U) / 10U) % 10U > 0)
      _buffer[_index++] = (_YEAR / 10U) % 10U + '0';          // Add The Third Digit In Year
    _buffer[_index++] = _YEAR % 10U + '0';                    // Add The Fourth Digit In Year
  }

  // Clock
  if (_TYPE == 0 || _TYPE == 1) {                             // Add The Clock
    _buffer[_index++] = ' ';                                  // Add The Space
    _buffer[_index++] = '-';                                  // Add The -
    _buffer[_index++] = ' ';                                  // Add The Space
    // Hours
    if (_CLOCKFORMAT_) {                                      // 24.Hours
      if (_USEHOURZERO_ || _HOUR >= 10)                       // Add The First Digit
        _buffer[_index++] = (_HOUR / 10) + '0';               // Add The First Digit In Hours
      _buffer[_index++] = (_HOUR % 10) + '0';                 // Add The Second Digit In Hours
    }
    else {                                                    // 12.Hours
      uint8_t _hour = con24To12(_HOUR);                       // Convert 24.h To 12.h
      if (_USEHOURZERO_ || _hour >= 10)                       // Add The First Digit
        _buffer[_index++] = (_hour / 10) + '0';               // Add The First Digit In Hours
      _buffer[_index++] = (_hour % 10) + '0';                 // Add The Second Digit In Hours
    }
    _buffer[_index++] = ':';                                  // Add The :
    // Minutes
    _buffer[_index++] = (_MIN / 10) + '0';                    // Add The First Digit In Minutes
    _buffer[_index++] = (_MIN % 10) + '0';                    // Add The Second Digit In Minutes
    _buffer[_index++] = ':';                                  // Add The :
    // Seconds
    _buffer[_index++] = (_SEC / 10) + '0';                    // Add The First Digit In Seconds
    _buffer[_index++] = (_SEC % 10) + '0';                    // Add The Second Digit In Seconds
    // AM / PM
    if (!_CLOCKFORMAT_) {                                     // Add The AM / PM
      _buffer[_index++] = ' ';                                // Add The Space
      if (_HOUR >= 12)                                        // PM (Post Meridiem) "after noon"
        _buffer[_index++] = 'P';                              // Add The P
      else                                                    // AM (Ante Meridiem) "before noon"
        _buffer[_index++] = 'A';                              // Add The A
      _buffer[_index++] = 'M';                                // Add The M
    }
  }

  // Null Terminate The String
  _buffer[_index] = '\0';                                     // Null Terminate The String
  // Return The Time String
  return _buffer;                                             // Return The Time String
}

// Convert Day Of Year To Date - conDOY2DATE(DOY, year, false/true) = Returns: DD/MM/YYYY Or DD/Jan/YYYY
char* DateTimeFunctions::conDOY2DATE(const uint16_t _DOY, const uint16_t _YEAR, const bool _USEMONTHNAME) {
  // Convert Day Of Year To Date
  // Max String = DD/MMM/YYYY = 11.characters.
  // Variables
  uint8_t _i = 0;                                                   // for() Counter
  uint8_t _month = 0;                                               // Month Of The Year
  uint16_t _day = 0U;                                               // Day Of The Month

  // Check If The Year Is a Leap Year Or Not
  if (_YEAR%400U == 0U || (_YEAR%4U == 0U && _YEAR%100U != 0U)) {   // Leap Year
    // Find Out The Month Of The Year
    while (_day < _DOY) {                                           // Find The Month Of The Year
      #if DATETIMEFUNCTIONS_MEMORY_USE == 0                         // Use Flash
        _day += PM(DIM_L + _month);                                 // Calculate The Month - Use Flash Memory
      #else                                                         // Use SRAM
        _day += DIM_L[_month];                                      // Calculate The Month - Use SRAM Memory
      #endif
      _month++;                                                     // Go To Next Month
    }

    // Find Out The Day Of The Month
    _day = 0U;                                                      // Reset The Day Calculator
    #if DATETIMEFUNCTIONS_MEMORY_USE == 0                           // Use Flash
      for (_i = 0; _i < _month - 1; _i++) _day += PM(DIM_L + _i);   // Find The Day Of The Month - Calculate The Day - Use Flash Memory
    #else                                                           // Use SRAM
      for (_i = 0; _i < _month - 1; _i++) _day += DIM_L[_i];        // Find The Day Of The Month - Calculate The Day - Use SRAM Memory
    #endif
    _day = _DOY - _day;                                             // Set The Day Of the Month
  }
  else {                                                            // Not Leap Year
    // Find Out The Month Of The Year
    while (_day < _DOY) {                                           // Find The Month Of The Year
      #if DATETIMEFUNCTIONS_MEMORY_USE == 0                         // Use Flash
        _day += PM(DIM_N + _month);                                 // Calculate The Month - Use Flash Memory
      #else                                                         // Use SRAM
        _day += DIM_N[_month];                                      // Calculate The Month - Use SRAM Memory
      #endif
      _month++;                                                     // Go To Next Month
    }

    // Find Out The Day Of The Month
    _day = 0U;                                                      // Reset The Day Calculator
    #if DATETIMEFUNCTIONS_MEMORY_USE == 0                           // Use Flash
      for (_i = 0; _i < _month - 1; _i++) _day += PM(DIM_N + _i);   // Find The Day Of The Month - Calculate The Day - Use Flash Memory
    #else                                                           // Use SRAM
      for (_i = 0; _i < _month - 1; _i++) _day += DIM_N[_i];        // Find The Day Of The Month - Calculate The Day - Use SRAM Memory
    #endif
    _day = _DOY - _day;                                             // Set The Day Of the Month
  }

  // Make The String
  if (_USEMONTHNAME) return MTDTS(_day, _month, _YEAR, 0, 0, 0, 3); // Use MTDTS() - Type 3
  else return MTDTS(_day, _month, _YEAR, 0, 0, 0, 2);               // Use MTDTS() - Type 2
}

// Convert Day Of Year To Day - conDOY2D(DOY, year) = Returns: 1-31
uint8_t DateTimeFunctions::conDOY2D(const uint16_t _DOY, const uint16_t _YEAR) {
  // Convert Day Of Year To Day
  uint16_t _day   = 0U;                                           // Day Calculator
  uint8_t  _month = 0;                                            // Month Counter
  uint8_t _dayIs  = 1;                                            // Return Day
  uint8_t _i = 0;                                                 // for() Counter

  // Check If The Year Is a Leap Year Or Not
  if (_YEAR%400U == 0U || (_YEAR%4U == 0U && _YEAR%100U != 0U)) { // Leap Year
    // Find Out The Month Of The Year
    while (_day < _DOY) {                                         // Find The Month Of The Year
      #if DATETIMEFUNCTIONS_MEMORY_USE == 0                       // Use Flash
        _day += PM(DIM_L + _month);                               // Calculate The Month - Use Flash Memory
      #else                                                       // Use SRAM
        _day += DIM_L[_month];                                    // Calculate The Month - Use SRAM Memory
      #endif
      _month++;                                                   // Go To Next Month
    }

    // Find Out The Day Of The Month
    _day = 0U;                                                    // Reset The Day Calculator
    #if DATETIMEFUNCTIONS_MEMORY_USE == 0                         // Use Flash
      for (_i = 0; _i < _month - 1; _i++) _day += PM(DIM_L + _i); // Find The Day Of The Month - Calculate The Day - Use Flash Memory
    #else                                                         // Use SRAM
      for (_i = 0; _i < _month - 1; _i++) _day += DIM_L[_i];      // Find The Day Of The Month - Calculate The Day - Use SRAM Memory
    #endif
    _dayIs = _DOY - _day;                                         // Set The Day Of the Month
  }
  else {                                                          // Not Leap Year
    // Find Out The Month Of The Year
    while (_day < _DOY) {                                         // Find The Month Of The Year
      #if DATETIMEFUNCTIONS_MEMORY_USE == 0                       // Use Flash
        _day += PM(DIM_N + _month);                               // Calculate The Month - Use Flash Memory
      #else                                                       // Use SRAM
        _day += DIM_N[_month];                                    // Calculate The Month - Use SRAM Memory
      #endif
      _month++;                                                   // Go To Next Month
    }

    // Find Out The Day Of The Month
    _day = 0U;                                                    // Reset The Day Calculator
    #if DATETIMEFUNCTIONS_MEMORY_USE == 0                         // Use Flash
      for (_i = 0; _i < _month - 1; _i++) _day += PM(DIM_N + _i); // Find The Day Of The Month - Calculate The Day - Use Flash Memory
    #else                                                         // Use SRAM
      for (_i = 0; _i < _month - 1; _i++) _day += DIM_N[_i];      // Find The Day Of The Month - Calculate The Day - Use SRAM Memory
    #endif
    _dayIs = _DOY - _day;                                         // Set The Day Of the Month
  }

  // Return The Day
  return _dayIs;                                                  // Return The Day
}

// Convert Day Of Year To Month - conDOY2M(DOY, year) = Returns: 1-12
uint8_t DateTimeFunctions::conDOY2M(const uint16_t _DOY, const uint16_t _YEAR) {
  // Convert Day Of Year To Month
  uint16_t _day = 0U;                                             // Day Calculator
  uint8_t  _month = 0;                                            // Month Counter

  // Check If The Year Is a Leap Year Or Not
  if (_YEAR%400U == 0U || (_YEAR%4U == 0U && _YEAR%100U != 0U)) { // Leap Year
    while (_day < _DOY) {                                         // Find The Month Of The Year
      #if DATETIMEFUNCTIONS_MEMORY_USE == 0                       // Use Flash
        _day += PM(DIM_L + _month);                               // Calculate The Month - Use Flash Memory
      #else                                                       // Use SRAM
        _day += DIM_L[_month];                                    // Calculate The Month - Use SRAM Memory
      #endif
      _month++;                                                   // Go To Next Month
    }
  }
  else {                                                          // Not Leap Year
    while (_day < _DOY) {                                         // Find The Month Of The Year
      #if DATETIMEFUNCTIONS_MEMORY_USE == 0                       // Use Flash
      _day += PM(DIM_N + _month);                                 // Calculate The Month - Use Flash Memory
      #else                                                       // Use SRAM
        _day += DIM_N[_month];                                    // Calculate The Month - Use SRAM Memory
      #endif
      _month++;                                                   // Go To Next Month
    }
  }

  // Return The Month
  return _month;                                                  // Return The Month
}

// Convert Date Time To Unix Time - conDT2UT(day, month, year, hour, minute, second) = Returns: Seconds Elapsed Since 01/01/1970 - 00:00:00 UTC
uint32_t DateTimeFunctions::conDT2UT(const uint8_t _DAY, const uint8_t _MONTH, const uint16_t _YEAR, const uint8_t _HOUR, const uint8_t _MIN, const uint8_t _SEC) {  // Calculate Unix Time
  // Convert Date Time To Unix Time
  const uint16_t _DOY = getDOY(_DAY, _MONTH, _YEAR);       // Get Day Of The Year
  uint8_t _leapYearDay = 0;                                // Dont Subtract One Day Of The Year
  if (_YEAR % 400U == 0U || (_YEAR % 4U == 0U && _YEAR % 100U != 0U)) // Leap Year
    _leapYearDay = 1;                                      // Subtract One Day Of The Leap Year
  const uint32_t _SECONDS_FROM_1970_TO_2000 = 946684800UL; // Seconds From 1970 to 2000
  const uint32_t _SECINONEYEAR = 31536000UL;               // Seconds In One Year                                                 (Leap days since 2000)    -    (Subtract One Day Of The Leap Year)
  return (_SECONDS_FROM_1970_TO_2000 + ((((((_YEAR - 2000UL) * _SECINONEYEAR) + _DOY * 86400UL) + _HOUR * 3600UL) + _MIN * 60UL) + _SEC) + (((_YEAR - 2000UL) / 4UL) * 86400UL)) - (_leapYearDay * 86400UL);
}

// Convert Unix Time To Date Time - conUT2DT(unixTime, false/true) = Returns: DD/MM/YYYY - HH:MM:SS Or DD/Jan/YYYY - HH:MM:SS
char* DateTimeFunctions::conUT2DT(const uint64_t _UNIXTIME, const bool _USEMONTHNAME) {
  // Convert Unix Time To Human Readable Format
  // Max String = DD/MMM/YYYY - HH:MM:SS = 22.characters.
  // Variables
  int32_t _extraDays = 0L;                                    // DOY
  uint32_t _daysTillNow = _UNIXTIME / 86400UL;                // Days / Year Calculate
  const uint32_t _TOTALSEC = _UNIXTIME % 86400UL;             // Total Seconds In Current Day
  uint16_t _year = 1970U;                                     // Year (Unix Time Starts At 01/01/1970)
  uint8_t _day = 0, _month = 0;                               // Day & Month
  uint8_t _leapYearFlag = 0, _index = 0;                      // Leap Year Flag, Month Index

  // Calculate Current Year
  while (true) {
    if (_year % 400U == 0U || (_year % 4U == 0U && _year % 100U != 0U)) {
      if (_daysTillNow < 366UL) break;
      _daysTillNow -= 366UL;
    }
    else {
      if (_daysTillNow < 365UL) break;
      _daysTillNow -= 365UL;
    }
    _year++;
  }
  // Updating extradays because it will give days till previous day and we have to include the current day.
  _extraDays = _daysTillNow + 1L;

  // Check For Leap Year
  if (_year % 400U == 0U || (_year % 4U == 0U && _year % 100U != 0U)) _leapYearFlag = 1;

  // Calculate Month & DOY
  if (_leapYearFlag == 1) {
    while (true) {
      if (_index == 1) {
        if (_extraDays - 29L < 0L) break;
        _month++;                                             // Add To Month
        _extraDays -= 29L;                                    // Subtract Of extraDays
      }
      else {
        #if DATETIMEFUNCTIONS_MEMORY_USE == 0                 // Use Flash
          if (_extraDays - PM(DIM_N + _index) < 0L) break;    // Use Flash Memory
          _month++;                                           // Add To Month
          _extraDays -= PM(DIM_N + _index);                   // Use Flash Memory
        #else                                                 // Use SRAM
          if (_extraDays - DIM_N[_index] < 0L) break;         // Use SRAM Memory
          _month++;                                           // Add To Month
          _extraDays -= DIM_N[_index];                        // Use SRAM Memory
        #endif
      }
      _index++;                                               // Add To Index
    }
  }
  else {
    while (true) {
      #if DATETIMEFUNCTIONS_MEMORY_USE == 0                   // Use Flash
        if (_extraDays - PM(DIM_N + _index) < 0L) break;      // Use Flash Memory
        _month++;                                             // Add To Month
        _extraDays -= PM(DIM_N + _index);                     // Use Flash Memory
      #else                                                   // Use SRAM
        if (_extraDays - DIM_N[_index] < 0L) break;           // Use SRAM Memory
        _month++;                                             // Add To Month
        _extraDays -= DIM_N[_index];                          // Use SRAM Memory
      #endif
      _index++;                                               // Add To Index
    }
  }

  // Current Month & Day
  if (_extraDays > 0L) {
    _month++;                                                 // Add To Month
    _day = _extraDays;                                        // Set The Day
  }
  else {
    if (_month == 2 && _leapYearFlag == 1) _day = 29;         // Set The Day
    #if DATETIMEFUNCTIONS_MEMORY_USE == 0                     // Use Flash
      else _day = PM(DIM_N + _month - 1);                     // Use Flash Memory
    #else                                                     // Use SRAM
      else _day = DIM_N[_month - 1];                          // Use SRAM Memory
    #endif
  }

  // Calculate Hours, Minutes & Seconds
  const uint8_t _HOURS   = _TOTALSEC / 3600UL;                // Hours
  const uint8_t _MINUTES = (_TOTALSEC % 3600UL) / 60UL;       // Minutes
  const uint8_t _SECONDS = _TOTALSEC % 60UL;                  // Seconds

  // Make The String - DD/MM/YYYY - HH:MM:SS
  if (_USEMONTHNAME) return MTDTS(_day, _month, _year, _HOURS, _MINUTES, _SECONDS, 1); // Use MTDTS() - Type 1
  else return MTDTS(_day, _month, _year, _HOURS, _MINUTES, _SECONDS, 0);               // Use MTDTS() - Type 0
}

// Convert Seconds To Time - conSec2Time(uint32_t seconds) = Returns: X.year, X.days, X.hours, X.min, X.sec
char* DateTimeFunctions::conSec2Time(const uint32_t _SEC) {
  // Convert Seconds To Time
  // Max Value  = 136.years, 70.days, 6.hours, 28.min, 15.sec
  // Max String = 135.years, 364.days, 23.hours, 59.min, 59.sec = 45.characters.
  //char* _buffer = __dateTimeStaticBuffer;                   // String Buffer
  char* _buffer = __conSec2TimeBuffer;                      // String Buffer

  // Change Total Seconds To: Seconds, Minutes, Hours, Days And Years.
  const uint8_t _seconds = _SEC % 60UL;                     // Calculate The Seconds
  const uint8_t _minutes = (_SEC / 60UL) % 60UL;            // Calculate The Minutes
  const uint8_t _hours   = (_SEC / 3600UL) % 24UL;          // Calculate The Hours
  const uint16_t _days   = (_SEC / 86400UL) % 365UL;        // Calculate The Days
  const uint16_t _years  = (_SEC / 86400UL) / 365UL;        // Calculate The Years

  // Make The Time String
  #if DATETIMEFUNCTIONS_MEMORY_USE == 0                     // Use Flash
  if (_years == 0U) {                                       // Less Than One Year
    if (_days == 0U && _hours == 0U && _minutes == 0U && _seconds == 1U)
      sprintf_P(_buffer, PSTR("%u.second"), _seconds);
    else if (_days == 0U && _hours == 0U && _minutes == 0U && _seconds > 1U)
      sprintf_P(_buffer, PSTR("%u.seconds"), _seconds);
    else if (_days == 0U && _hours == 0U && _minutes == 1U)
      sprintf_P(_buffer, PSTR("%u.minute, %u.sec"), _minutes, _seconds);
    else if (_days == 0U && _hours == 0U && _minutes > 1U)
      sprintf_P(_buffer, PSTR("%u.minutes, %u.sec"), _minutes, _seconds);
    else if (_days == 0U && _hours == 1U)
      sprintf_P(_buffer, PSTR("%u.hour, %u.min, %u.sec"), _hours, _minutes, _seconds);
    else if (_days == 0U && _hours > 1U)
      sprintf_P(_buffer, PSTR("%u.hours, %u.min, %u.sec"), _hours, _minutes, _seconds);
    else if (_days == 1U && _hours >= 0U && _hours <= 1)
      sprintf_P(_buffer, PSTR("%u.day, %u.hour, %u.min, %u.sec"), _days, _hours, _minutes, _seconds);
    else if (_days == 1U && _hours > 1U)
      sprintf_P(_buffer, PSTR("%u.day, %u.hours, %u.min, %u.sec"), _days, _hours, _minutes, _seconds);
    else if (_days > 1U && _hours >= 0U && _hours <= 1)
      sprintf_P(_buffer, PSTR("%u.days, %u.hour, %u.min, %u.sec"), _days, _hours, _minutes, _seconds);
    else if (_days > 1U && _hours > 1U)
      sprintf_P(_buffer, PSTR("%u.days, %u.hours, %u.min, %u.sec"), _days, _hours, _minutes, _seconds);
  }
  else if (_years == 1U)                                    // At One Year
    sprintf_P(_buffer, PSTR("%u.year, %u.days, %u.hours, %u.min, %u.sec"), _years, _days, _hours, _minutes, _seconds);
  else                                                      // More Than One Year
    sprintf_P(_buffer, PSTR("%u.years, %u.days, %u.hours, %u.min, %u.sec"), _years, _days, _hours, _minutes, _seconds);
  
  #else                                                     // Use SRAM
  if (_years == 0U) {                                       // Less Than One Year
    if (_days == 0U && _hours == 0U && _minutes == 0U && _seconds == 1U)
      sprintf(_buffer, "%u.second", _seconds);
    else if (_days == 0U && _hours == 0U && _minutes == 0U && _seconds > 1U)
      sprintf(_buffer, "%u.seconds", _seconds);
    else if (_days == 0U && _hours == 0U && _minutes == 1U)
      sprintf(_buffer, "%u.minute, %u.sec", _minutes, _seconds);
    else if (_days == 0U && _hours == 0U && _minutes > 1U)
      sprintf(_buffer, "%u.minutes, %u.sec", _minutes, _seconds);
    else if (_days == 0U && _hours == 1U)
      sprintf(_buffer, "%u.hour, %u.min, %u.sec", _hours, _minutes, _seconds);
    else if (_days == 0U && _hours > 1U)
      sprintf(_buffer, "%u.hours, %u.min, %u.sec", _hours, _minutes, _seconds);
    else if (_days == 1U && _hours >= 0U && _hours <= 1)
      sprintf(_buffer, "%u.day, %u.hour, %u.min, %u.sec", _days, _hours, _minutes, _seconds);
    else if (_days == 1U && _hours > 1U)
      sprintf(_buffer, "%u.day, %u.hours, %u.min, %u.sec", _days, _hours, _minutes, _seconds);
    else if (_days > 1U && _hours >= 0U && _hours <= 1)
      sprintf(_buffer, "%u.days, %u.hour, %u.min, %u.sec", _days, _hours, _minutes, _seconds);
    else if (_days > 1U && _hours > 1U)
      sprintf(_buffer, "%u.days, %u.hours, %u.min, %u.sec", _days, _hours, _minutes, _seconds);
  }
  else if (_years == 1U)                                    // At One Year
    sprintf(_buffer, "%u.year, %u.days, %u.hours, %u.min, %u.sec", _years, _days, _hours, _minutes, _seconds);
  else                                                      // More Than One Year
    sprintf(_buffer, "%u.years, %u.days, %u.hours, %u.min, %u.sec", _years, _days, _hours, _minutes, _seconds);
  #endif
  
  return _buffer;                                           // Return The String
}

// Convert Seconds To Time - conSec2Time(uint64_t seconds) = Returns: X.year, X.days, X.hours, X.min, X.sec
char* DateTimeFunctions::conSec2Time64(const uint64_t _SEC) {
  // Convert Seconds To Time
  // Uint64 Max: 18446744073709551615.sec =
  // Max String: 584942417355.years, 26.days, 7.hours, 0.min, 15.sec = Max: 53.characters.
  //char* _buffer = __dateTimeStaticBuffer;                          // String Buffer
  char* _buffer = __conSec2TimeBuffer;                             // String Buffer

  // Change Total Seconds To: Seconds, Minutes, Hours, Days And Years.
  const uint8_t _seconds = _SEC % 60ULL;                           // Calculate The Seconds
  const uint8_t _minutes = (_SEC / 60ULL) % 60ULL;                 // Calculate The Minutes
  const uint8_t _hours   = (_SEC / 3600ULL) % 24ULL;               // Calculate The Hours
  const uint16_t _days   = (_SEC / 86400ULL) % 365ULL;             // Calculate The Days
  const uint64_t _years  = _SEC / 31536000ULL;                     // Calculate The Years

  // Make The Year String (uint64_t)
  static char _year_Str[13];                                       // uint64_t _year String
  uint8_t _index = 0U;                                             // String Place Counter
  uint64_t Integer = _years;                                       // Use uint64_t
  if (Integer == 0) {                                              // Years Is 0
    _year_Str[0] = '0';
    _year_Str[1] = '\0';
  }
  // Make The String
  while (Integer > 0) {                                            // Add The Integer To The String
    _year_Str[_index++] = (Integer % 10) + '0';
    Integer /= 10;
  }
  _year_Str[_index] = '\0';                                        // Null Terminate The String
  // Reverse The String
  for (uint8_t i = 0, j = _index - 1; i < _index / 2; i++, j--) {  // Reverse The String
    char c = _year_Str[i];
    _year_Str[i] = _year_Str[j];
    _year_Str[j] = c;
  }

  // Make The Time String
  #if DATETIMEFUNCTIONS_MEMORY_USE == 0                     // Use Flash
  if (_years == 0U) {                                       // Less Than One Year
    if (_days == 0U && _hours == 0U && _minutes == 0U && _seconds == 1U)
      sprintf_P(_buffer, PSTR("%u.second"), _seconds);
    else if (_days == 0U && _hours == 0U && _minutes == 0U && _seconds > 1U)
      sprintf_P(_buffer, PSTR("%u.seconds"), _seconds);
    else if (_days == 0U && _hours == 0U && _minutes == 1U)
      sprintf_P(_buffer, PSTR("%u.minute, %u.sec"), _minutes, _seconds);
    else if (_days == 0U && _hours == 0U && _minutes > 1U)
      sprintf_P(_buffer, PSTR("%u.minutes, %u.sec"), _minutes, _seconds);
    else if (_days == 0U && _hours == 1U)
      sprintf_P(_buffer, PSTR("%u.hour, %u.min, %u.sec"), _hours, _minutes, _seconds);
    else if (_days == 0U && _hours > 1U)
      sprintf_P(_buffer, PSTR("%u.hours, %u.min, %u.sec"), _hours, _minutes, _seconds);
    else if (_days == 1U && _hours >= 0U && _hours <= 1)
      sprintf_P(_buffer, PSTR("%u.day, %u.hour, %u.min, %u.sec"), _days, _hours, _minutes, _seconds);
    else if (_days == 1U && _hours > 1U)
      sprintf_P(_buffer, PSTR("%u.day, %u.hours, %u.min, %u.sec"), _days, _hours, _minutes, _seconds);
    else if (_days > 1U && _hours >= 0U && _hours <= 1)
      sprintf_P(_buffer, PSTR("%u.days, %u.hour, %u.min, %u.sec"), _days, _hours, _minutes, _seconds);
    else if (_days > 1U && _hours > 1U)
      sprintf_P(_buffer, PSTR("%u.days, %u.hours, %u.min, %u.sec"), _days, _hours, _minutes, _seconds);
  }
  else if (_years == 1U)                                    // At One Year
    sprintf_P(_buffer, PSTR("%s.year, %u.days, %u.hours, %u.min, %u.sec"), _year_Str, _days, _hours, _minutes, _seconds);
  else                                                      // More Than One Year
    sprintf_P(_buffer, PSTR("%s.years, %u.days, %u.hours, %u.min, %u.sec"), _year_Str, _days, _hours, _minutes, _seconds);
  
  #else                                                     // Use SRAM
  if (_years == 0U) {                                       // Less Than One Year
    if (_days == 0U && _hours == 0U && _minutes == 0U && _seconds == 1U)
      sprintf(_buffer, "%u.second", _seconds);
    else if (_days == 0U && _hours == 0U && _minutes == 0U && _seconds > 1U)
      sprintf(_buffer, "%u.seconds", _seconds);
    else if (_days == 0U && _hours == 0U && _minutes == 1U)
      sprintf(_buffer, "%u.minute, %u.sec", _minutes, _seconds);
    else if (_days == 0U && _hours == 0U && _minutes > 1U)
      sprintf(_buffer, "%u.minutes, %u.sec", _minutes, _seconds);
    else if (_days == 0U && _hours == 1U)
      sprintf(_buffer, "%u.hour, %u.min, %u.sec", _hours, _minutes, _seconds);
    else if (_days == 0U && _hours > 1U)
      sprintf(_buffer, "%u.hours, %u.min, %u.sec", _hours, _minutes, _seconds);
    else if (_days == 1U && _hours >= 0U && _hours <= 1)
      sprintf(_buffer, "%u.day, %u.hour, %u.min, %u.sec", _days, _hours, _minutes, _seconds);
    else if (_days == 1U && _hours > 1U)
      sprintf(_buffer, "%u.day, %u.hours, %u.min, %u.sec", _days, _hours, _minutes, _seconds);
    else if (_days > 1U && _hours >= 0U && _hours <= 1)
      sprintf(_buffer, "%u.days, %u.hour, %u.min, %u.sec", _days, _hours, _minutes, _seconds);
    else if (_days > 1U && _hours > 1U)
      sprintf(_buffer, "%u.days, %u.hours, %u.min, %u.sec", _days, _hours, _minutes, _seconds);
  }
  else if (_years == 1U)                                    // At One Year
    sprintf(_buffer, "%s.year, %u.days, %u.hours, %u.min, %u.sec", _year_Str, _days, _hours, _minutes, _seconds);
  else                                                      // More Than One Year
    sprintf(_buffer, "%s.years, %u.days, %u.hours, %u.min, %u.sec", _year_Str, _days, _hours, _minutes, _seconds);
  #endif
  
  return _buffer;                                           // Return The String
}

// Convert Seconds To Date Time - conSec2DT(seconds, false/true) = Returns: DD/MM/YYYY - HH:MM:SS Or DD/Jan/YYYY - HH:MM:SS
char* DateTimeFunctions::conSec2DT(const uint64_t _SEC, const bool _USEMONTHNAME) {
  // Convert Seconds To Date Time - DD/MM/YYYY - HH:MM:SS
  // 32.bit Max: 136.years, 70.days, 6.hours, 28.min, 15.sec
  // 64.bit Max: 584942417355.years, 26.days, 7.hours, 0.min, 15.sec
  // Max String = 31/Dec/65535 - 23:59:59 = 23.characters.

  // Variables
  const uint8_t _SECONDS = _SEC % 60ULL;                    // Calculate The Seconds
  const uint8_t _MINUTES = (_SEC / 60ULL) % 60ULL;          // Calculate The Minutes
  const uint8_t _HOURS   = (_SEC / 3600ULL) % 24ULL;        // Calculate The Hours
  const uint32_t _tDays  = _SEC / 86400ULL;                 // Calculate The Total Days
  uint32_t _day   = 0UL;                                    // Day Nr
  uint8_t  _month = 0;                                      // Month Nr
  uint16_t _year  = 0U;                                     // Year Nr
  uint8_t  _index = 0;                                      // Index For Month

  // Find Out Day And Month
  if (_tDays > 0UL) {                                       // If We Have Some Days
    while (_day < _tDays) {                                 // While Day Is Less Than Total Days
      // Reset The Index & Add To Year
      if (_index == 12) {                                   // At Month 12
        _index = 0;                                         // Reset The Index
        _year++;                                            // Add To Year
      }
      // Add The Leap Year Day
      if (_year > 0U) {                                     // Dont Add Leap Year Day @ Year 0
        if (_year % 400U == 0U || (_year % 4U == 0U && _year % 100U != 0U)) {
          if (_index >= 2 && _index < 3) _day++;            // Add The Leap Year Day
        }
      }
      // Add To Day
      #if DATETIMEFUNCTIONS_MEMORY_USE == 0                 // Use Flash
        _day += PM(DIM_N + _index);                         // Add Month Days To The Day
      #else                                                 // Use SRAM
        _day += DIM_N[_index];                              // Add Month Days To The Day
      #endif
      // Add To Index
      if (_day < _tDays) _index++;                          // Dont Add If We Got The Value
    }
    // Find Out The Day In The Month
    #if DATETIMEFUNCTIONS_MEMORY_USE == 0                   // Use Flash
      if (_index > 0) _day -= PM(DIM_N + _index);           // Go Back One Month - Use Flash Memory
      else _day -= PM(DIM_N + 11);                          // Go Back One Month = Month 12
    #else                                                   // Use SRAM
      if (_index > 0) _day -= DIM_N[_index];                // Go Back One Month - Use Flash Memory
      else _day -= DIM_N[11];                               // Go Back One Month = Month 12
    #endif
    // Set The Day
    _day = _tDays - _day;                                   // Set The Day
    if (_day == 0UL && _index == 2) {                       // On The Leap Year Day
      _day = 29UL;                                          // Set The Leap Year Day
      _index = 1;                                           // Set The Leap Year Month
    }
    // Set The Month
    _month = _index + 1;                                    // Set The Month
  }

  // Make The String - DD/MM/YYYY - HH:MM:SS
  if (_USEMONTHNAME) return MTDTS(_day, _month, _year, _HOURS, _MINUTES, _SECONDS, 1); // Use MTDTS() - Type 1
  else return MTDTS(_day, _month, _year, _HOURS, _MINUTES, _SECONDS, 0);               // Use MTDTS() - Type 0
}

// Convert Seconds To Date Time - conSec2DT(seconds, year, false/true) = Returns: DD/MM/YYYY - HH:MM:SS Or DD/Jan/YYYY - HH:MM:SS
char* DateTimeFunctions::conSec2DT(const uint32_t _SEC, const uint16_t _YEAR, const bool _USEMONTHNAME) {
  // Convert Seconds To Date Time
  // Max String = DD/MMM/YYYY - HH:MM:SS = 22.characters.
  const uint32_t _LEAPDAYS = 86400UL * (((_YEAR - 4UL) - 1970UL) / 4UL);
  const uint32_t _UNIXTIME = ((_YEAR - 1970UL) * 31536000UL + _LEAPDAYS) + _SEC;
  return conUT2DT(_UNIXTIME, _USEMONTHNAME);          // Use conUT2DT()
}

// Convert Date Time To Seconds - conDT2Sec(day, month, year, hour, min, sec) = Returns: uint32_t
uint32_t DateTimeFunctions::conDT2Sec(const uint8_t _DAY, const uint8_t _MONTH, const uint16_t _YEAR, const uint32_t _HOUR, const uint32_t _MIN, const uint8_t _SEC) {
  // Convert Date Time To Seconds
  // Max Value  = 51909555.Seconds
  uint16_t _m2Days = 0U;                                                  // Days In Month + Days
  for (uint8_t i = 0; i < _MONTH - 1; i++) {                              // Count The Month Days
    #if DATETIMEFUNCTIONS_MEMORY_USE == 0                                 // Use Flash
      if (_YEAR % 400U == 0U || (_YEAR % 4U == 0U && _YEAR % 100U != 0U)) // Leap Year
        _m2Days += PM(DIM_L + i);                                         // Leap Year - Use Flash Memory
      else                                                                // Not Leap Year
        _m2Days += PM(DIM_N + i);                                         // Not Leap Year - Use Flash Memory
    #else                                                                 // Use SRAM
      if (_YEAR % 400U == 0U || (_YEAR % 4U == 0U && _YEAR % 100U != 0U)) // Leap Year
        _m2Days += DIM_L[i];                                              // Leap Year - Use SRAM Memory
      else                                                                // Not Leap Year
        _m2Days += DIM_N[i];                                              // Not Leap Year - Use SRAM Memory
    #endif
  }
  _m2Days += _DAY;                                                        // Add The Days
  // Return Total Seconds
  return (_m2Days * 86400UL) + (_HOUR * 3600UL) + (_MIN * 60UL) + _SEC;   // Return
}

// Convert Days, Years, Hours, Minutes And Seconds To Seconds - conTime2Sec(days, years, hours, min, sec) = Returns: uint32_t
uint32_t DateTimeFunctions::conTime2Sec(const uint32_t _DAYS, const uint16_t _YEARS, const uint32_t _HOURS, const uint32_t _MIN, const uint8_t _SEC) {
  // Convert Days, Years, Hours, Minutes And Seconds To Seconds.
  // Max Value  = 136.years, 70.days, 6.hours, 28.min, 15.sec
  return (_YEARS * 31536000UL) + (_DAYS * 86400UL) + (_HOURS * 3600UL) + (_MIN * 60UL) + _SEC;
}

// Convert Number To Roman Numerals - conNum2Roman(0-10000) = Returns: N-MMMMMMMMMM
char* DateTimeFunctions::conNum2Roman(const uint16_t _IN) {
  // Convert Number To Roman Numerals. 0 to 10000
  // https://en.wikipedia.org/wiki/Roman_numerals
  // The highest number that can be expressed in Roman numerals is actually 3999.
  // 3999 is written as MMMCMXCIX. This is because the number 4000 would have to be written as MMMM,
  // which goes against the principle of not having four consecutive letters of the same type together.
  #if DATETIMEFUNCTIONS_MEMORY_USE == 0                                 // Use Flash
    const static char _ROMANS[13][3] PROGMEM = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
  #else                                                                 // Use SRAM
    const static char _ROMANS[13][3] = {"M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"};
  #endif
  const uint16_t _ROMANVALUES[13] = {1000U, 900U, 500U, 400U, 100U, 90U, 50U, 40U, 10U, 9U, 5U, 4U, 1U};

  // Max String = (8888) = MMMMMMMMDCCCLXXXVIII = 20.characters.
  char* _buffer = __dateTimeStaticBuffer;                               // String Buffer
  int32_t _in = _IN;                                                    // Set The Value To int32_t

  // Lets Just Stop At 10000. Else The buffer Needs To Be Bigger Than 21
  if (_IN > 10000U) {                                                   // In Value Is To Big
    #if DATETIMEFUNCTIONS_MEMORY_USE == 0                               // Use Flash
      strcpy_P(_buffer, PSTR("Error, To Big."));                        // Make The String
    #else                                                               // Use SRAM
      strcpy(_buffer, "Error, To Big.");                                // Make The String
    #endif
    return _buffer;                                                     // Return The String
  }

  // 0 Is Written As "N" or "NULLA"
  if (_IN == 0U) {                                                      // 0 is written N or NULLA
    _buffer[0] = 'N';                                                   // Add The N
    _buffer[1] = '\0';                                                  // Null Terminate The String
    return _buffer;                                                     // Return The String
  }

  // Make The String
  uint8_t _index = 0;                                                   // Buffer Index
  uint8_t _i = 0;                                                       // Array Index
  for (uint8_t i = 0; i < 13; ++i) {                                    // Loop Thro The Romans Letters
    while (_in - _ROMANVALUES[i] >= 0) {                                // While The Cal Value Is 0 Or Bigger
      _in -= _ROMANVALUES[i];                                           // Calculate The Values
      while (true) {                                                    // Add To The String
        #if DATETIMEFUNCTIONS_MEMORY_USE == 0                           // Use Flash
          _buffer[_index++] = (char)pgm_read_word(&(_ROMANS[i][_i++])); // Add This
          if (_i >= strlen_P(_ROMANS[i]) - 1) {                         // Reset & Break
            _i = 0;                                                     // Reset Array Index
            break;                                                      // Stop The String While Function
          }
        #else                                                           // Use SRAM
          _buffer[_index++] = _ROMANS[i][_i++];                         // Add This
          if (_i >= strlen(_ROMANS[i]) - 1) {                           // Reset & Break
            _i = 0;                                                     // Reset Array Index
            break;                                                      // Stop The String While Function
          }
        #endif
      }
    }
  }
  _buffer[_index] = '\0';                                               // Null Terminate The String
  return _buffer;                                                       // Return The String
}

// Convert Gregorian To Julian Calendar - conGre2Jul(day, month, year, false/true) = Returns: DD/MM/YYYY Or DD/Jan/YYYY
char* DateTimeFunctions::conGre2Jul(const uint8_t _DAY, const uint8_t _MONTH, const uint16_t _YEAR, const bool _USEMONTHNAME) {
  // Convert Gregorian To Julian Calendar - The Roman Calendar Version
  // https://en.wikipedia.org/wiki/Julian_calendar#
  // https://en.wikipedia.org/wiki/Conversion_between_Julian_and_Gregorian_calendars
  // Formula To Calculate Day Difference: ((year / 100) - ((year / 100) / 4)) - 2
  // Example: ((2023 / 100) - ((2023 / 100) / 4)) - 2 = 13 & ((2100 / 100) - ((2100 / 100) / 4)) - 2 = 14
  // Take this formula with a grain of salt, as there is no public formula to make the convert.
  // But like you can see in the "Formula Test" below, it is pretty accurate.
  // List From Wikipedia.
  //       Year: 200, 300, 500, 600, 700, 900, 1000, 1100, 1300, 1400, 1500, 1700, 1800, 1900, 2100
  // Difference:   0,   1,   2,   3,   4,   5,    6,    7,    8,    9,    10,   11,  12,   13,   14
  // Formula Test:
  // 200-299=0 _ 300-499=1 _ 500-599=2 _ 600-699=3 _ 700-899=4 _ 900-999=5 _ 1000-1099=6
  // 1100-1299=7 _ 1300-1399=8 _ 1400-1499=9 _ 1500-1699=10 _ 1700-1799=11 _ 1800-1899=12
  // 1900-2099=13 _ 2100-2199=14 _ 2200-2299=15 _ 2300-2499=16 _ 2500-2599=17 _ 2600-2699=18

  // Max String = DD/MMM/YYYY = 11.characters.

  // Variables
  uint8_t  _day   = 0;                                              // Day Nr
  uint8_t  _month = 0;                                              // Month Nr
  uint16_t _year  = 0U;                                             // Year Nr
  const uint8_t _DIFF = ((_YEAR / 100) - ((_YEAR / 100) / 4)) - 2;  // Calculate The Days Difference

  // Year
  if (_DAY <= _DIFF && _MONTH == 1) _year = _YEAR - 1U;             // Set The Year
  else _year = _YEAR;                                               // Set The Year
  // Day And Month
  if (_DAY > _DIFF && _MONTH >= 1) {                                // We Are In Same Year & More Than _DIFF Days
    _day = _DAY - _DIFF;                                            // Calculate The Day
    _month = _MONTH;                                                // Set The Month
  }
  else if (_DAY <= _DIFF && _MONTH > 1) {                           // We Are In Same Year & Less Than _DIFF Days
    #if DATETIMEFUNCTIONS_MEMORY_USE == 0                           // Use Flash
      _day = (PM(DIM_N + (_MONTH - 2)) - _DIFF) + _DAY;             // Calculate The Day
    #else                                                           // Use SRAM
      _day = (DIM_N[_MONTH - 2] - _DIFF) + _DAY;                    // Calculate The Day
    #endif
    _month = _MONTH - 1;                                            // Set The Month
  }
  else {                                                            // We Are Still In December
    #if DATETIMEFUNCTIONS_MEMORY_USE == 0                           // Use Flash
      _day = (PM(DIM_N + 11) - _DIFF) + _DAY;                       // Calculate The Day
    #else                                                           // Use SRAM
      _day = (DIM_N[11] - _DIFF) + _DAY;                            // Calculate The Day
    #endif
    _month = 12;                                                    // Set The Month
  }

  // Make The String
  if (_USEMONTHNAME) return MTDTS(_day, _month, _year, 0, 0, 0, 3); // Use MTDTS() - Type 3
  else return MTDTS(_day, _month, _year, 0, 0, 0, 2);               // Use MTDTS() - Type 2
}


//--------------------- Date ----------------------//

// Date To String - conDate2Str(day, month, year, false/true) = Returns: DD/MM/YYYY Or DD/Jan/YYYY
char* DateTimeFunctions::date2Str(const uint8_t _DAY, const uint8_t _MONTH, const uint16_t _YEAR, const bool _USEMONTHNAME) {
  // Date To String
  // Max String = DD/MMM/YYYY = 11.characters.
  if (_USEMONTHNAME) return MTDTS(_DAY, _MONTH, _YEAR, 0, 0, 0, 3); // Use MTDTS() - Type 3
  else return MTDTS(_DAY, _MONTH, _YEAR, 0, 0, 0, 2);               // Use MTDTS() - Type 2
}


//--------------------- Clock ---------------------//

// Make The Clock String - Private
char* DateTimeFunctions::MTCS(const uint8_t _HOUR, const uint8_t _MIN, const uint8_t _SEC, const bool _USESEC, const bool _AMPM) {
  // Make The Clock String
  // _AMPM: false = 24.h & 12.h / true = 12.h With AM/PM
  // Max String = HH:MM:SS XX = 11.characters.
  char* _buffer = __dateTimeStaticBuffer;                     // String Buffer
  uint8_t _index = 0;                                         // String Index
  uint8_t _hour = _HOUR;                                      // Get The Hour
  if ((_HOUR > 12 || _HOUR == 0) && (_AMPM || !_CLOCKFORMAT_))// We Need To Convert 24.h To 12.h
    _hour = con24To12(_HOUR);                                 // Convert 24.h To 12.h
  // Make The String
  if (_USEHOURZERO_ || _hour >= 10)                           // Add 0 Or First Digit In Hours
    _buffer[_index++] = (_hour / 10) + '0';                   // Add The First Digit In Hours
  _buffer[_index++] = (_hour % 10) + '0';                     // Add The Second Digit In Hours
  _buffer[_index++] = ':';                                    // Add The :
  // Minutes
  _buffer[_index++] = (_MIN / 10) + '0';                      // Add The First Digit In Minutes
  _buffer[_index++] = (_MIN % 10) + '0';                      // Add The Second Digit In Minutes
  // Seconds
  if (_USESEC) {                                              // Add Seconds
    _buffer[_index++] = ':';                                  // Add The :
    _buffer[_index++] = (_SEC / 10) + '0';                    // Add The First Digit In Seconds
    _buffer[_index++] = (_SEC % 10) + '0';                    // Add The Second Digit In Seconds
  }
  // AM / PM
  if (_AMPM) {                                                // Add The AM / PM
    _buffer[_index++] = ' ';                                  // Add The Space
    if (_HOUR >= 12)                                          // PM (Post Meridiem) "after noon"
      _buffer[_index++] = 'P';                                // Add The P
    else                                                      // AM (Ante Meridiem) "before noon"
      _buffer[_index++] = 'A';                                // Add The A
    _buffer[_index++] = 'M';                                  // Add The M
  }
  _buffer[_index] = '\0';                                     // Null Terminate The String

  // Return The Time String
  return _buffer;                                             // Return The Time String
}

// Clock To String - clock2Str(hour, minute, seconds, false/true) = Returns: 10:10 / 10:10:10
// bool: false = "10:10" & true = "10:10:10"
char* DateTimeFunctions::clock2Str(const uint8_t _HOUR, const uint8_t _MIN, const uint8_t _SEC, const bool _USESEC) {
  // Clock To String
  // Max String = 10:10:10 = 8.characters.
  return MTCS(_HOUR, _MIN, _SEC, _USESEC, false);             // Use MTCS()
}

// Clock To String With AM / PM - clock2StrAMPM(hour, minute, seconds, false/true) = Returns: 10:10 / 10:10:10
// bool: false = "10:10 AM" & true = "10:10:10 AM"
char* DateTimeFunctions::clock2StrAMPM(const uint8_t _HOUR, const uint8_t _MIN, const uint8_t _SEC, const bool _USESEC) {
  // Clock To String 12.hour With AM / PM
  // Max String = 10:10:10 XX = 11.characters.
  return MTCS(_HOUR, _MIN, _SEC, _USESEC, true);              // Use MTCS()
}

// Seconds To Clock - sec2Clock(seconds, false/true) = Returns: 10:10 / 10:10:10
// bool: false = "10:10" & true = "10:10:10"
char* DateTimeFunctions::sec2Clock(const uint32_t _SECONDS, const bool _USESEC) {
  // Seconds To Clock
  // Max String = 10:10:10 = 8.characters.
  const uint8_t _SEC  = _SECONDS % 60UL;                      // Calculate The Seconds
  const uint8_t _MIN  = (_SECONDS / 60UL) % 60UL;             // Calculate The Minutes
  const uint8_t _HOUR = (_SECONDS / 3600UL) % 24UL;           // Calculate The Hours
  return MTCS(_HOUR, _MIN, _SEC, _USESEC, false);             // Use MTCS()
}

// Seconds To Clock With AM / PM - sec2ClockAMPM(seconds, false/true) = Returns: 10:10 AM / 10:10:10 AM
// bool: false = "10:10 AM" & true = "10:10:10 AM"
char* DateTimeFunctions::sec2ClockAMPM(const uint32_t _SECONDS, const bool _USESEC) {
  // Seconds To Clock 12.hour With AM / PM
  // Max String = 10:10:10 XX = 11.characters.
  const uint8_t _SEC  = _SECONDS % 60UL;                      // Calculate The Seconds
  const uint8_t _MIN  = (_SECONDS / 60UL) % 60UL;             // Calculate The Minutes
  const uint8_t _HOUR = (_SECONDS / 3600UL) % 24UL;           // Calculate The Hours
  return MTCS(_HOUR, _MIN, _SEC, _USESEC, true);              // Use MTCS()
}

// Roman Clock 24.hours - romanClock(hour, minute) = Returns: N:N ... XXIII:LIX
char* DateTimeFunctions::romanClock(const uint8_t _HOUR, const uint8_t _MIN) {
  // Roman Clock 24.hours
  char* _buffer = __dateTimeStaticBuffer;                         // String Buffer
  char romanClockH[6];                                            // Hour String
  char romanClockM[6];                                            // Minute String
  strcpy(romanClockH, conNum2Roman(_HOUR));                       // Convert The Hours
  strcpy(romanClockM, conNum2Roman(_MIN));                        // Convert The Minutes
  #if DATETIMEFUNCTIONS_MEMORY_USE == 0                           // Use Flash
    sprintf_P(_buffer, PSTR("%s:%s"), romanClockH, romanClockM);  // Make The String
  #else                                                           // Use SRAM
    sprintf(_buffer, "%s:%s", romanClockH, romanClockM);          // Make The String
  #endif
  return _buffer;                                                 // Return The String
}

// Roman Clock 12.hours - romanClock12(hour, minute) = Returns: I:N ... XI:LIX
char* DateTimeFunctions::romanClock12(const uint8_t _HOUR, const uint8_t _MIN) {
  // Roman Clock 12.hours
  const uint8_t _hour = con24To12(_HOUR);                       // Convert 24.h To 12.h
  return romanClock(_hour, _MIN);                               // Use The romanClock()
}


// End Of File.
