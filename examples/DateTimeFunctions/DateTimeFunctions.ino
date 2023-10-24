//
//    FILE: DateTimeFunctions.ino
//  AUTHOR: XbergCode
// VERSION: 1.0.0
// PURPOSE: Collection of date & time functions. Check, Convert, Get, Clock & Date. Works with RTC and NTP.
//     Set: DateFormat(setDFormat), ClockFormat(setCFormat), DateZero(setDZero), HourZero(setHZero).
//   Check: LeapYear(isLeap), AMorPM(isPM).
//     Get: DayOfYear(getDOY), DayOfWeek(getDOW), DayOfWeekNameShort(getDOWNS), DayOfWeekNameLong(getDOWNL), WeekOfYear(getWOY),
//    -||-: WeeksInYear(getWIY), DaysInYear(getDIY), DaysLeftInYear(getDLIY), DaysInMonth(getDIM), MonthNameShort(getMNS),
//    -||-: MonthNameLong(getMNL), DaysBetweenDate(getDBD), SecondsFromMidnight(getSFM), MinutesFromMidnight(getMFM) AMorPM(getAMPM).
// Convert: 24To12hour(con24To12), 12To24Hour(con12To24), DoyToDate(conDOY2DATE), DoyToDay(conDOY2D), DoyToMonth(conDOY2M),
//    -||-: DateTimeToUnixTime(conDT2UT), UnixTimeToDateTime(conUT2DT), SecondsToDateTime(conSec2DT), SecondsToTime(conSec2Time),
//    -||-: DateTimeToSeconds(conDT2Sec), TimeToSeconds(conTime2Sec), NumbersToRomanNumerals(conNum2Roman), GregorianToJulianCalendar(conGre2Jul).
//    Date: DateToString(date2Str).
//   Clock: ClockToString(clock2Str), ClockToStringAM/PM(clock2StrAMPM),
//    -||-: SecondsToClock(sec2Clock), SecondsToClockAM/PM(sec2ClockAMPM),
//    -||-: RomanToClock24H(roman2Clock), RomanClock12H(romanClock12).
//    Note: It is possible to use Flash or SRAM mermory. See line 17 in DateTimeFunctions.h Default is Flash.
//    -||-: You can save up to 27.bytes(SRAM) by using one buffer if you use conSec2Time/64. See Line 44-48 in DateTimeFunctions.cpp.
//     URL: https://github.com/XbergCode/DateTimeFunctions
//

// This example shows all the functions that are in this library.
// Tested on: Uno, Mega, ESP32 & NodeMCU

// DateTimeFunctions Library.
#include <DateTimeFunctions.h>                   // Include The DateTimeFunctions Library
DateTimeFunctions dTF;                           // Set The Class Object Name

// Date & Time. From RTC or NTP.
const uint8_t  day    = 28;                      // Day
const uint8_t  month  = 2;                       // Month
const uint16_t year   = 2023U;                   // Year
const uint8_t  hour   = 20;                      // Hour
const uint8_t  minute = 0;                       // Minute
const uint8_t  second = 0;                       // Second


void setup() {
  Serial.begin(500000UL);                        // Start The Serial @ Baudrate
  delay(500UL);                                  // Give Serial Time To Wake Up
  Serial.print(F("Date Time Functions. - "));    // Serial Print Sketch Purpose
  Serial.print(F("Version: ")); Serial.println(DATETIMEFUNCTIONS_VERSION);

  // Default Date Time Format
  Serial.println(F("Default Date Time Format: DD/MM/YYYY - HH:MM:SS"));


  //---------------------- Set ----------------------//

  // Set The Date Format
  //dTF.setDFormat(1);                             // Set The Date Format = DD/MM/YYYY < Default
  //dTF.setDFormat(2);                             // Set The Date Format = MM/DD/YYYY
  //dTF.setDFormat(3);                             // Set The Date Format = YYYY/MM/DD
  //dTF.setDFormat(4);                             // Set The Date Format = YYYY/DD/MM

  // Set The Clock Format
  //dTF.setCFormat(true);                          // Set The Clock Format = 24.Hours < Default
  //dTF.setCFormat(false);                         // Set The Clock Format = 12.Hours

  // Set The Date Zero
  //dTF.setDZero(true);                            // Set The Date Zero = 01/01/YYYY < Default
  //dTF.setDZero(false);                           // Set The Date Zero = 1/1/YYYY

  // Set The Hour Zero
  //dTF.setHZero(true);                            // Set The Hour Zero = 01:MM:SS < Default
  //dTF.setHZero(false);                           // Set The Hour Zero = 1:MM:SS


  //--------------------- Check ---------------------//
  Serial.println(F("\nCheck."));

  // Check For Leap Year - isLeap(year) = Returns: true If Leap Year "0-1" (Bool)
  Serial.print(year);
  if (dTF.isLeap(year)) Serial.println(F(" Is a Leap Year"));
  else Serial.println(F(" Is Not a Leap Year"));

  // Check For AM / PM - isPM(hour) = Returns: true If PM "0-1" (Bool)
  Serial.print(hour);
  if (dTF.isPM(hour)) Serial.println(F(".Hour is PM")); // Send hour in 24.h format
  else Serial.println(F(".Hour is AM"));


  //---------------------- Get ----------------------//
  Serial.println(F("\nGet."));

  // Get Day Of Year - getDOY(day, month, year) = Returns: 1-366 (uint16_t)
  const uint16_t DOY = dTF.getDOY(day, month, year); // Get Day Of Year
  Serial.print(F("Day Of Year: ")); Serial.println(DOY);

  // Get Day Of Week - getDOW(day, month, year) = Returns: 0-6 "Sun-Sat" (uint8_t)
  const uint8_t DOW = dTF.getDOW(day, month, year);  // Get Day Of Week
  Serial.print(F("Day Of Week: ")); Serial.println(DOW);

  // Get Day Of Week Name Short - getDOWNS(DOW) = Returns: Sun-Sat (char)
  Serial.print(F("Day Of Week Name Short: ")); Serial.println(dTF.getDOWNS(DOW));

  // Get Day Of Week Name Long - getDOWNL(DOW) = Returns: Sunday-Saturday (char)
  Serial.print(F("Day Of Week Name Long: ")); Serial.println(dTF.getDOWNL(DOW));

  // Get Week Of Year - getWOY(day, month, year) = Returns: 1-53 (ISO 8601) (uint8_t)
  Serial.print(F("Week Of Year: ")); Serial.println(dTF.getWOY(day, month, year));

  // Get Weeks In Year - getWIY(year) = Returns: 52-53 (uint8_t)
  Serial.print(F("Weeks In Year: ")); Serial.println(dTF.getWIY(year));

  // Get Days In Year - getDIY(year) = Returns: 365-366 (uint16_t)
  Serial.print(F("Days In Year: ")); Serial.println(dTF.getDIY(year));

  // Get Days Left In Year - getDLIY(day, month, year) = Returns: 0-365 (uint16_t)
  Serial.print(F("Days Left In Year: ")); Serial.println(dTF.getDLIY(day, month, year));

  // Get Days In Month - getDIM(month, year) = Returns: 28-31 (uint8_t)
  Serial.print(F("Days In Month: ")); Serial.println(dTF.getDIM(month, year));

  // Get Month Name Short - getMNS(month) = Returns: Jan-Dec (char)
  Serial.print(F("Month Name Short: ")); Serial.println(dTF.getMNS(month));

  // Get Month Name Long - getMNL(month) = Returns: January-December (char)
  Serial.print(F("Month Name Long: ")); Serial.println(dTF.getMNL(month));

  // Get Days Between Date - getDBD(day1, month1, year1, day2, month2, year2) = Returns: 0-65535 (uint16_t)
  //const uint16_t DBD = dTF.getDBD(1, 1, 2024, 1, 1, 2023); // Wrong Format = 0
  const uint16_t DBD = dTF.getDBD(15, 5, 2020, 10, 3, 2024); // Get Days Between Date
  Serial.print(F("Days Between Date: ")); Serial.println(DBD);

  // Get Seconds From Midnight - getSFM(hour, minute, seconds) = Returns: 0-86399 (uint32_t)
  Serial.print(F("Seconds From Midnight: ")); Serial.println(dTF.getSFM(hour, minute, second));

  // Get Minutes From Midnight - getMFM(hour, minute) = Returns: 0-1439 (uint16_t)
  Serial.print(F("Minutes From Midnight: ")); Serial.println(dTF.getMFM(hour, minute));

  // Get AM / PM - getAMPM(hour) = Returns: AM or PM (char)
  Serial.print(F("AM or PM: ")); Serial.println(dTF.getAMPM(hour)); // Send hour in 24.h format


  //-------------------- Convert --------------------//
  Serial.println(F("\nConvert."));

  // Convert 24.Hour To 12.Hour - con24To12(hour) = Returns: 1-12 (uint8_t)
  const uint8_t hour12 = dTF.con24To12(hour);  // Convert 24.h To 12.h
  Serial.print(F("24.hour To 12.hour: ")); Serial.println(hour12);

  // Convert 12.Hour To 24.Hour - con12To24(hour, AorP) = Returns: 0-23 (uint8_t)
  const char AorP = dTF.getAMPM(hour)[0];      // Get The A or P With getAMPM
  Serial.print(F("12.hour To 24.hour: ")); Serial.println(dTF.con12To24(hour12, AorP));
  //Serial.print(F("12.hour To 24.hour: ")); Serial.println(dTF.con12To24(12, 'P')); // Example.

  // Convert Day Of Year To Date - conDOY2DATE(DOY, year, false/true) = Returns: DD/MM/YYYY Or DD/Jan/YYYY (char)
  Serial.print(F("Day Of Year To Date: ")); Serial.println(dTF.conDOY2DATE(DOY, year));       // DD/MM/YYYY
  //Serial.print(F("Day Of Year To Date: ")); Serial.println(dTF.conDOY2DATE(DOY, year, true)); // DD/Jan/YYYY

  // Convert Day Of Year To Day - conDOY2D(DOY, year) = Returns: 1-31 (uint8_t)
  const uint8_t _day = dTF.conDOY2D(DOY, year);   // Convert DOY To Day
  Serial.print(F("Day Of Year To Day: ")); Serial.println(_day);

  // Convert Day Of Year To Month - conDOY2M(DOY, year) = Returns: 1-12 (uint8_t)
  const uint8_t _month = dTF.conDOY2M(DOY, year); // Convert DOY To Month
  Serial.print(F("Day Of Year To Month: ")); Serial.println(_month);

  // Convert Date Time To Unix Time - conDT2UT(day, month, year, hour, minute, second) = Returns: Seconds Elapsed Since 01/01/1970 - 00:00:00 UTC (uint32_t)
  const uint32_t unixTime = dTF.conDT2UT(day, month, year, hour, minute, second);  // Convert Date Time To Unix Time
  Serial.print(F("Date Time To Unix Time: ")); Serial.println(unixTime);

  // Convert Unix Time To Date Time - conUT2DT(unixTime, false/true) = Returns: DD/MM/YYYY - HH:MM:SS Or DD/Jan/YYYY - HH:MM:SS (char)
  //Serial.print(F("Unix Time To Date Time: ")); Serial.println(dTF.conUT2DT(unixTime));       // DD/MM/YYYY - HH:MM:SS
  Serial.print(F("Unix Time To Date Time: ")); Serial.println(dTF.conUT2DT(unixTime, true)); // DD/Jan/YYYY - HH:MM:SS

  // Set The Zero's
  dTF.setDZero(false);                           // Set The Date Zero = 1/1/YYYY
  dTF.setHZero(true);                            // Set The Hour Zero = 01:MM:SS

  // Convert Seconds To Date Time - conSec2DT(seconds, false/true) = Returns: DD/MM/YYYY - HH:MM:SS Or DD/Jan/YYYY - HH:MM:SS (char)
  const uint64_t _SECONDS = 31536000ULL * 1ULL + 86400ULL * 60ULL + 3600ULL * 5ULL + 60ULL * 20ULL + 35ULL; // 01/03/1 - 5:20:35
  Serial.print(F("Seconds To Date Time: ")); Serial.println(dTF.conSec2DT(_SECONDS));        // DD/MM/YYYY - HH:MM:SS
  //Serial.print(F("Seconds To Date Time: ")); Serial.println(dTF.conSec2DT(_SECONDS, true));  // DD/Jan/YYYY - HH:MM:SS

  // Change The Clock Format To 12 Hours And Date & Hour Zero
  dTF.setCFormat(false);                         // Set The Clock Format = 12.Hours
  dTF.setDZero(true);                            // Set The Date Zero = 01/01/YYYY
  dTF.setHZero(false);                           // Set The Hour Zero = 1:MM:SS

  // Convert Seconds To Date Time - conSec2DT(seconds, year, false/true) = Returns: DD/MM/YYYY - HH:MM:SS Or DD/Jan/YYYY - HH:MM:SS (char)
  const uint32_t SECONDS = 251UL * 86400UL + 16UL * 3600UL + 45UL * 60UL + 50UL; // = 07/09/2024 - 16:45:50 or 07/09/2024 - 4:45:50
  Serial.print(F("Seconds To Date Time: ")); Serial.println(dTF.conSec2DT(SECONDS, 2024U));       // DD/MM/YYYY - HH:MM:SS
  //Serial.print(F("Seconds To Date Time: ")); Serial.println(dTF.conSec2DT(SECONDS, 2024U, true)); // DD/Jan/YYYY - HH:MM:SS

  // Change The Clock Format To 24 Hours
  dTF.setCFormat(true);                          // Set The Clock Format = 24.Hours

  // Convert Seconds To Time - conSec2Time(uint32_t seconds) = Returns: X.year, X.days, X.hours, X.min, X.sec (char)
  // uint32_t Version. - Based on 365.day year = 31536000.seconds.
  const uint32_t TotalSeconds = 31937000UL;      // = 1.year, 4.days, 15.hours, 23.min, 20.sec
  //const uint32_t TotalSeconds = 4294967295UL;    // MAX = 136.years, 70.days, 6.hours, 28.min, 15.sec (49.710.days)
  Serial.print(F("Seconds To Time: ")); Serial.println(dTF.conSec2Time(TotalSeconds));

  // Convert Seconds To Time - conSec2Time64(uint64_t seconds) = Returns: X.year, X.days, X.hours, X.min, X.sec (char)
  // uint64_t Version. - Based on 365.day year = 31536000.seconds.
  const uint64_t TotalSeconds64 = 31937000ULL;   // = 1.year, 4.days, 15.hours, 23.min, 20.sec
  //const uint64_t TotalSeconds64 = 18446744073709551615ULL; // MAX = 584942417355.years, 26.days, 7.hours, 0.min, 15.sec (213.503.982.334.601.days)
  Serial.print(F("Seconds To Time 64: ")); Serial.println(dTF.conSec2Time64(TotalSeconds64));
  
  // Convert Date Time To Seconds - conDT2Sec(day, month, year, hour, min, sec) = Returns: (uint32_t)
  const uint8_t d = 4, mon = 2, y = 2023, h = 15, min = 23, s = 20;          // = 3079400.Seconds
  //const uint16_t d = 255, mon = 12, y = 2024, h = 255, min = 255, s = 255;   // MAX = 51909555.Seconds
  Serial.print(F("Date Time To Seconds: ")); Serial.println(dTF.conDT2Sec(d, mon, y, h, min, s));

  // Convert Days, Years, Hours, Minutes And Seconds To Seconds - conTime2Sec(days, years, hours, min, sec) = Returns: (uint32_t)
  const uint8_t _d = 4, _y = 1, _h = 15, _min = 23, _s = 20;                 // = 31937000.Seconds
  //const uint8_t _d = 11, _y = 136, _h = 6, _min = 28, _s = 15;               // MAX = 4294967295.Seconds
  Serial.print(F("Time To Seconds: ")); Serial.println(dTF.conTime2Sec(_d, _y, _h, _min, _s));

  // Convert Number To Roman Numerals - conNum2Roman(0-10000) = Returns: N-MMMMMMMMMM (char)
  const uint16_t _year = 2023U;                  // 2023 Is Written Like: MMXXIII
  Serial.print(F("Number To Roman: ")); Serial.println(dTF.conNum2Roman(_year));

  // Convert Gregorian To Julian Calendar - conGre2Jul(day, month, year, false/true) = Returns: DD/MM/YYYY Or DD/Jan/YYYY (char)
  Serial.print(F("Gregorian To Julian: ")); Serial.println(dTF.conGre2Jul(day, month, year));       // DD/MM/YYYY
  //Serial.print(F("Gregorian To Julian: ")); Serial.println(dTF.conGre2Jul(day, month, year, true)); // DD/Jan/YYYY

  //--------------------- Date ----------------------//
  Serial.println(F("\nDate."));
  
  // Date To String - date2Str(day, month, year, false/true) = Returns: DD/MM/YYYY Or DD/Jan/YYYY
  Serial.print(F("Date To String: ")); Serial.println(dTF.date2Str(day, month, year));       // DD/MM/YYYY
  Serial.print(F("Date To String: ")); Serial.println(dTF.date2Str(day, month, year, true)); // DD/Jan/YYYY
  dTF.setDFormat(2);                                                                         // Set The Date Format = MM/DD/YYYY
  dTF.setDZero(false);                                                                       // Set The Date Zero = 1/1/YYYY
  Serial.print(F("Date To String: ")); Serial.println(dTF.date2Str(day, month, year));       // DD/MM/YYYY
  Serial.print(F("Date To String: ")); Serial.println(dTF.date2Str(day, month, year, true)); // DD/Jan/YYYY

  // Change The Date Format And Date Zero
  dTF.setDFormat(1);                                                                         // Set The Date Format = DD/MM/YYYY
  dTF.setDZero(true);                                                                        // Set The Date Zero = 01/01/YYYY
  
  // Make Date Time String = Tuesday 28/02/2023 - 20:00:00
  char dateTime_Str[36];                                                                     // Date Time Buffer
  char DOW_Str[10];                                                                          // Day Of Week Buffer
  char Date_Str[12];                                                                         // Date Buffer
  char Clock_Str[9];                                                                         // Clock Buffer
  strcpy(DOW_Str, dTF.getDOWNL(DOW));                                                        // Make The DOW String
  strcpy(Date_Str, dTF.date2Str(day, month, year));                                          // Make The Date String
  strcpy(Clock_Str, dTF.clock2Str(hour, minute, second, true));                              // Make The Clock String
  sprintf_P(dateTime_Str, PSTR("%s %s - %s"), DOW_Str, Date_Str, Clock_Str);                 // Make The Date Time String
  Serial.print(F("Date Time String: ")); Serial.println(dateTime_Str);

  // Change The Date Format
  dTF.setDFormat(2);                                                                         // Set The Date Format = MM/DD/YYYY

  // Make Date Time String - With AM PM = Tuesday Feb/28/2023 - 8:00 PM
  strcpy(Date_Str, dTF.date2Str(day, month, year, true));                                    // Make The Date String
  strcpy(Clock_Str, dTF.clock2StrAMPM(hour, minute));                                        // Make The Clock String
  sprintf_P(dateTime_Str, PSTR("%s %s - %s"), DOW_Str, Date_Str, Clock_Str);                 // Make The Date Time String
  Serial.print(F("Date Time String: ")); Serial.println(dateTime_Str);


  //--------------------- Clock ---------------------//
  Serial.println(F("\nClock."));

  // Clock To String - clock2Str(hour, minute, seconds, bool) = Returns: 10:10 / 10:10:10 (char)
  Serial.print(F("Clock To String: "));
  Serial.println(dTF.clock2Str(hour, minute, second, true));      // false = "10:10" & true = "10:10:10" < Default false

  // Change The Clock Format To 12 Hours & Hour Zero
  dTF.setCFormat(false);                                          // Set The Clock Format = 12.Hours
  dTF.setHZero(true);                                             // Set The Hour Zero = 01:MM:SS

  // Clock To String - clock2Str(hour, minute, seconds, bool) = Returns: 10:10 / 10:10:10 (char)
  Serial.print(F("Clock To String: "));
  Serial.println(dTF.clock2Str(hour, minute));                    // false = "10:10" & true = "10:10:10" < Default false

  // Change The Clock Format To 24 Hours & Hour Zero
  // Just to show there is no need to change the clock format when AMPM is used.
  dTF.setCFormat(true);                                          // Set The Clock Format = 24.Hours
  dTF.setHZero(false);                                           // Set The Hour Zero = 1:MM:SS

  // Clock To String 12.hour With AM / PM - clock2StrAMPM(hour, minute, seconds, bool) = Returns: 10:10 AM / 10:10:10 AM (char)
  Serial.print(F("Clock To String With AM PM: "));
  Serial.println(dTF.clock2StrAMPM(hour, minute, second, true)); // false = "10:10" & true = "10:10:10" - Default false

  // Seconds To Clock
  const uint32_t SECONDSTOCLOCK = 13UL*3600UL + 20UL*60UL + 9UL; // Make Some Time In Seconds

  // Seconds To Clock - sec2Clock(seconds, bool) = Returns: 10:10 / 10:10:10 (char)
  Serial.print(F("Seconds To Clock: "));
  Serial.println(dTF.sec2Clock(SECONDSTOCLOCK));                 // false = "10:10" & true = "10:10:10" - Default false

  // Change The Clock Format To 12 Hours
  dTF.setCFormat(false);                                         // Set The Clock Format = 12.Hours

  // Seconds To Clock - sec2Clock(seconds, bool) = Returns: 10:10 / 10:10:10 (char)
  Serial.print(F("Seconds To Clock: "));
  Serial.println(dTF.sec2Clock(SECONDSTOCLOCK, true));           // false = "10:10" & true = "10:10:10" - Default false

  // Change The Clock Format To 24 Hours
  // Just to show there is no need to change the clock format when AMPM is used.
  dTF.setCFormat(true);                                          // Set The Clock Format = 24.Hours

  // Seconds To Clock 12.hour With AM / PM - sec2ClockAMPM(seconds, bool) = Returns: 10:10 AM / 10:10:10 AM (char)
  Serial.print(F("Seconds To Clock With AM PM: "));
  Serial.println(dTF.sec2ClockAMPM(SECONDSTOCLOCK));             // false = "10:10" & true = "10:10:10" - Default false

  // Roman Clock - romanClock(hour, minute) = Returns: N:N ... XXIII:LIX (char)
  Serial.print(F("Roman Clock 24: ")); Serial.println(dTF.romanClock(hour, minute));

  // Roman Clock - romanClock12(hour, minute) = Returns: I:N ... XII:LIX (char)
  Serial.print(F("Roman Clock 12: ")); Serial.println(dTF.romanClock12(hour, minute));

  Serial.println(F("\nDone."));

  /* Serial Outputs:
     Date Time Functions. - Version: 1.0.0

     Default Date Time Format: DD/MM/YYYY - HH:MM:SS

     Check.
     2023 Is Not a Leap Year
     20.Hour is PM

     Get.
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

     Convert.
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

     Date.
     Date To String: 28/02/2023
     Date To String: 28/Feb/2023
     Date To String: 2/28/2023
     Date To String: Feb/28/2023
     Date Time String: Tuesday 28/02/2023 - 20:00:00
     Date Time String: Tuesday Feb/28/2023 - 8:00 PM

     Clock.
     Clock To String: 20:00:00
     Clock To String: 08:00
     Clock To String With AM PM: 8:00:00 PM
     Seconds To Clock: 13:20
     Seconds To Clock: 1:20:09
     Seconds To Clock With AM PM: 1:20 PM
     Roman Clock 24: XX:N
     Roman Clock 12: VIII:N

     Done.
  */
}

void loop() {

}


// End Of File.
