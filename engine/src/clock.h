//_____________________________________________________________________________
//   ▄▄   ▄ ▄  ▄▄▄ ▄▄ ▄                                                        
//  ██ ▀ ██▀█ ▀█▄  ▀█▄▀                                                        
//  ▀█▄▀ ██ █ ▄▄█▀ ██ █                                                        
//_____________________________________________________________________________
// Real-time clock module (RP-5C01)
//
// Réferences:
//  - Ricoh RP/RF5C01A application manual
//  - https://www.msx.org/wiki/Ricoh_RP-5C01
//_____________________________________________________________________________
#pragma once
#include "core.h"
#include "system_port.h"

#define RTC_REG_MODE		0xD // Mode Register
#define RTC_REG_TEST		0xE // Test Register (write only)
#define RTC_REG_RESET		0xF // Reset Register (write only)

//-----------------------------------------------------------------------------
// RTC MODE REGISTER (0xD)
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	X	X	X	X	ES	AE	M1	M0	
//					│	│	└───┴── Select Block
//					│	└────────── Alarm Enable (0=Disable, 1=Enable) ; Not used in MSX
//					└────────────── Enable Seconds (0=Freeze, 1=Active)

#define RTC_MODE_BLOCK_0	0 // Current Time and Day
#define RTC_MODE_BLOCK_1	1 // Alarm, Hour Mode, Year Type
#define RTC_MODE_BLOCK_2	2 // Adjust, Screen, Beep, Logo Screen, Language
#define RTC_MODE_BLOCK_3	3 // Title, Password, Prompt
#define RTC_MODE_ALARM_ON	4 // Alarm Enable
#define RTC_MODE_ALARM_OFF	0 // Alarm Disable
#define RTC_MODE_SEC_ON		8 // Seconds Enable 
#define RTC_MODE_SEC_OFF	0 // Seconds Disable

#define RTC_MODE_TIME		RTC_MODE_BLOCK_0
#define RTC_MODE_ALARM		RTC_MODE_BLOCK_1
#define RTC_MODE_SCREEN		RTC_MODE_BLOCK_2
#define RTC_MODE_TEXT		RTC_MODE_BLOCK_3

//-----------------------------------------------------------------------------
// RTC TEST REGISTER  (0xE)
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	X	X	X	X	T3	T2	T1	T0	
//					│	│	│	└── Increment Seconds at a rate of 16384Hz
//					│	│	└────── Increment Minutes at a rate of 16384Hz
//					│	└────────── Increment Hours at a rate of 16384Hz
//					└────────────── Increment Days at a rate of 16384Hz

#define RTC_TEST_SEC		(1<<0)
#define RTC_TEST_MIN		(1<<1)
#define RTC_TEST_HOUR		(1<<2)
#define RTC_TEST_DAY		(1<<3)

//-----------------------------------------------------------------------------
// RTC RESET REGISTER  (0xF)
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	X	X	X	X	C1	C16	CR	AR	
//					│	│	│	└── Reset all Alarm Registers to zero (1=Reset)	
//					│	│	└────── Reset fractions smaller than 1 second (1=Reset)	
//					│	└────────── Enable 16Hz clock output (0=Enable?; Not used in MSX)
//					└────────────── Enable 1Hz clock output (0=Enable; Not used in MSX)

#define RTC_RESET_ALARM		(1<<0)
#define RTC_RESET_SEC		(1<<1)
#define RTC_RESET_16HZ		(1<<2)
#define RTC_RESET_1HZ		(1<<3)

//-----------------------------------------------------------------------------
// BLOCK 0 - TIME
//-----------------------------------------------------------------------------

#define RTC_TIME_SEC		0x0 // Units counter for seconds (4 bits)
#define RTC_TIME_10SEC		0x1 // Tens counter for seconds (3 bits)
#define RTC_TIME_MIN		0x2 // Units counter for minutes (4 bits)
#define RTC_TIME_10MIN		0x3 // Tens counter for minutes (3 bits)
#define RTC_TIME_HOUR		0x4 // Units counter for hours (4 bits). Hours are counted from 0-23 when the 12/24 hour bit is set. Alternately hours could count from 0-11 (AM), and then from 20-31 (PM) when 12/24 hour bit is cleared.
#define RTC_TIME_10HOUR		0x5 // Tens counter for hours (2 bits). If 12-hour mode is set: 0=for a.m., 1=for p.m. (?)
#define RTC_TIME_WEEKDAY	0x6 // Weekday counter (3 bits)
#define RTC_TIME_DAY		0x7 // Units counter for days of the month (4 bits)
#define RTC_TIME_10DAY		0x8 // Tens counter for days of the month (2 bits)
#define RTC_TIME_MONTH		0x9 // Units counter for months (4 bits)
#define RTC_TIME_10MONTH	0xA // Tens counter for months (1 bit)
#define RTC_TIME_YEAR		0xB // Units counter for years (4 bits)
#define RTC_TIME_10YEAR		0xC // Tens counter for years (4 bits)

#define RTC_DAY_SUNDAY		0 // Sunday
#define RTC_DAY_MONDAY		1 // Monday
#define RTC_DAY_TUESDAY		2 // Tuesday
#define RTC_DAY_WEDNESDAY	3 // Wednesday
#define RTC_DAY_THURSDAY	4 // Thursday
#define RTC_DAY_FRIDAY		5 // Friday
#define RTC_DAY_SATURDAY	6 // Saturday

//-----------------------------------------------------------------------------
// BLOCK 1 - ALARM
//-----------------------------------------------------------------------------

#define RTC_ALARM_MIN		0x2 // Units counter for minutes of alarm (4 bits)
#define RTC_ALARM_10MIN		0x3 // Tens counter for minutes of alarm (3 bits)
#define RTC_ALARM_HOUR		0x4 // Units counter for hours of alarm (4 bits)
#define RTC_ALARM_10HOUR	0x5 // Tens counter for hours of alarm (2 bits)
#define RTC_ALARM_WEEKDAY	0x6 // Weekday counter for alarm (3 bits)
#define RTC_ALARM_DAY		0x7 // Units counter for days of the month for alarm (4 bits)
#define RTC_ALARM_10DAY		0x8 // Tens counter for days of the month for alarm (2 bits)
#define RTC_ALARM_24H		0xA // Hour mode (1 bit). 0=Sets to 12-hour system. 1=sets to 24-hour system.
#define RTC_ALARM_YEAR		0xB // Year type (2 bits)




//-----------------------------------------------------------------------------
// FUNCTIONS
//-----------------------------------------------------------------------------

/// Initialize the clock module
void Clock_Initialize();

/// Set clock mode
void Clock_SetMode(u8 mode) __FASTCALL;

/// Get current clock second counter (0-59)
u8 Clock_GetSecond();

/// Get current clock minute counter (0-59)
u8 Clock_GetMinute();

/// Get current clock hour counter (0-23)
u8 Clock_GetHour();

/// Get current clock day-of-week counter (0-6)
u8 Clock_GetDayOfWeek();

/// Get current clock day counter (1-31)
u8 Clock_GetDay();

/// Get current clock month counter (1-12)
u8 Clock_GetMonth();

/// Get current clock year counter (0-99)
u8 Clock_GetYear();

/// Read a RTC register value
u8 Clock_Read(u8 reg) __FASTCALL;

/// Write a RTC register value
void Clock_Write(u8 reg, u8 value);

#if (USE_CLOCK_EXTRA)

/// Get current clock day of week string
const c8* Clock_GetDayOfWeekString();

/// Get current clock month counter string
const c8* Clock_GetMonthString();

/// Get current clock 4-digits year counter (1980-2079)
u16 Clock_GetYear4();

#endif // (USE_CLOCK_EXTRA)
