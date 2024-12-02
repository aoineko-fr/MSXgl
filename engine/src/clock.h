// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄  ▄▄            ▄▄   
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ ▀ ██  ▄█▀▄ ▄█▀▀ ██▄▀ 
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄▀ ▀█▄ ▀█▄▀ ▀█▄▄ ██ █ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Real-time clock module (RP-5C01)
//
// Réferences:
//  - Ricoh RP/RF5C01A application manual
//  - https://www.msx.org/wiki/Real_Time_Clock_Programming
//─────────────────────────────────────────────────────────────────────────────
#pragma once

#include "core.h"
#include "system_port.h"

//-----------------------------------------------------------------------------
// CHECK OPTIONS
//-----------------------------------------------------------------------------

// RTC_USE_CLOCK
#ifndef RTC_USE_CLOCK
	#warning RTC_USE_CLOCK is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define RTC_USE_CLOCK			TRUE
#endif

// RTC_USE_CLOCK_EXTRA
#ifndef RTC_USE_CLOCK_EXTRA
	#warning RTC_USE_CLOCK_EXTRA is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define RTC_USE_CLOCK_EXTRA		TRUE
#endif

// RTC_USE_SAVEDATA
#ifndef RTC_USE_SAVEDATA
	#warning RTC_USE_SAVEDATA is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define RTC_USE_SAVEDATA		TRUE
#endif

// RTC_USE_SAVESIGNED
#ifndef RTC_USE_SAVESIGNED
	#warning RTC_USE_SAVESIGNED is not defined in "msxgl_config.h"! Default value will be used: TRUE
	#define RTC_USE_SAVESIGNED		TRUE
#endif

//-----------------------------------------------------------------------------
// RTC MODE REGISTER (0xD)
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	X	X	X	X	TE	AE	M1	M0	
//					│	│	└───┴── Select Block
//					│	└────────── Alarm Enable (0=Disable, 1=Enable) ; Not used in MSX
//					└────────────── Timer Enable (0=Disable, 1=Enable)
#define RTC_REG_MODE				0xD // Mode Register

#define RTC_MODE_BLOCK_0			0 // Current Time and Day
#define RTC_MODE_BLOCK_1			1 // Alarm, Hour Mode, Year Type
#define RTC_MODE_BLOCK_2			2 // Adjust, Screen, Beep, Logo Screen, Language
#define RTC_MODE_BLOCK_3			3 // Title, Password, Prompt + MSXgl custom save data
#define RTC_MODE_ALARM_ON			4 // Alarm Enable
#define RTC_MODE_ALARM_OFF			0 // Alarm Disable
#define RTC_MODE_TIMER_ON			8 // Timer Enable 
#define RTC_MODE_TIMER_OFF			0 // Timer Disable

//-----------------------------------------------------------------------------
// RTC TEST REGISTER  (0xE)
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	X	X	X	X	T3	T2	T1	T0	
//					│	│	│	└── Increment Seconds at a rate of 16384Hz
//					│	│	└────── Increment Minutes at a rate of 16384Hz
//					│	└────────── Increment Hours at a rate of 16384Hz
//					└────────────── Increment Days at a rate of 16384Hz
#define RTC_REG_TEST				0xE // Test Register (write only)

#define RTC_TEST_SEC				(1<<0)
#define RTC_TEST_MIN				(1<<1)
#define RTC_TEST_HOUR				(1<<2)
#define RTC_TEST_DAY				(1<<3)

//-----------------------------------------------------------------------------
// RTC RESET REGISTER  (0xF)
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//	X	X	X	X	C1	C16	CR	AR	
//					│	│	│	└── Reset all Alarm Registers to zero (1=Reset)	
//					│	│	└────── Reset fractions smaller than 1 second (1=Reset)	
//					│	└────────── Enable 16Hz clock output (0=Enable?; Not used in MSX)
//					└────────────── Enable 1Hz clock output (0=Enable; Not used in MSX)
#define RTC_REG_RESET				0xF // Reset Register (write only)

#define RTC_RESET_ALARM				(1<<0)
#define RTC_RESET_SEC				(1<<1)
#define RTC_RESET_16HZ				(1<<2)
#define RTC_RESET_1HZ				(1<<3)

//-----------------------------------------------------------------------------
// BLOCK 0 - TIME
//-----------------------------------------------------------------------------
// This block stores the current time, day of the week and date.
#define RTC_MODE_TIME				RTC_MODE_BLOCK_0

#define RTC_REG_TIME_SEC			0x0 // Units counter for seconds (4 bits)
#define RTC_REG_TIME_10SEC			0x1 // Tens counter for seconds (3 bits)
#define RTC_REG_TIME_MIN			0x2 // Units counter for minutes (4 bits)
#define RTC_REG_TIME_10MIN			0x3 // Tens counter for minutes (3 bits)
#define RTC_REG_TIME_HOUR			0x4 // Units counter for hours (4 bits). Hours are counted from 0-23 when the 12/24 hour bit is set. Alternately hours could count from 0-11 (AM), and then from 20-31 (PM) when 12/24 hour bit is cleared.
#define RTC_REG_TIME_10HOUR			0x5 // Tens counter for hours (2 bits). If 12-hour mode is set: 0=for a.m., 1=for p.m. (?)
#define RTC_REG_TIME_WEEKDAY		0x6 // Weekday counter (3 bits)
#define RTC_REG_TIME_DAY			0x7 // Units counter for days of the month (4 bits)
#define RTC_REG_TIME_10DAY			0x8 // Tens counter for days of the month (2 bits)
#define RTC_REG_TIME_MONTH			0x9 // Units counter for months (4 bits)
#define RTC_REG_TIME_10MONTH		0xA // Tens counter for months (1 bit)
#define RTC_REG_TIME_YEAR			0xB // Units counter for years (4 bits)
#define RTC_REG_TIME_10YEAR			0xC // Tens counter for years (4 bits)

#define RTC_DAY_SUNDAY				0 // Sunday
#define RTC_DAY_MONDAY				1 // Monday
#define RTC_DAY_TUESDAY				2 // Tuesday
#define RTC_DAY_WEDNESDAY			3 // Wednesday
#define RTC_DAY_THURSDAY			4 // Thursday
#define RTC_DAY_FRIDAY				5 // Friday
#define RTC_DAY_SATURDAY			6 // Saturday

//-----------------------------------------------------------------------------
// BLOCK 1 - ALARM
//-----------------------------------------------------------------------------
// This block is used to set the alarm or the timer. Some bits are also used to select the 12/24 hour time mode and indicate the leap year. The alarm function is an optional function not used by MSX computers.
#define RTC_MODE_ALARM				RTC_MODE_BLOCK_1

#define RTC_REG_ALARM_MIN			0x2 // Units counter for minutes of alarm (4 bits)
#define RTC_REG_ALARM_10MIN			0x3 // Tens counter for minutes of alarm (3 bits)
#define RTC_REG_ALARM_HOUR			0x4 // Units counter for hours of alarm (4 bits)
#define RTC_REG_ALARM_10HOUR		0x5 // Tens counter for hours of alarm (2 bits)
#define RTC_REG_ALARM_WEEKDAY		0x6 // Weekday counter for alarm (3 bits)
#define RTC_REG_ALARM_DAY			0x7 // Units counter for days of the month for alarm (4 bits)
#define RTC_REG_ALARM_10DAY			0x8 // Tens counter for days of the month for alarm (2 bits)
#define RTC_REG_ALARM_24H			0xA // Hour mode (1 bit). 0=Sets to 12-hour system. 1=sets to 24-hour system.
#define RTC_REG_ALARM_YEAR			0xB // Year type (2 bits)

#define RTC_HOURMODE_12H			0 // 12 hour mode using am/pm
#define RTC_HOURMODE_24H			1 // 24 hour mode

//-----------------------------------------------------------------------------
// BLOCK 2 - SYSTEM SETTING
//-----------------------------------------------------------------------------
// The system uses this block to store several parameters it uses for initialization.
#define RTC_MODE_SYSTEM				RTC_MODE_BLOCK_2
#define RTC_MODE_SCREEN				RTC_MODE_BLOCK_2

#define RTC_REG_SYS_INIT			0x0 // 10 if registers are OK
#define RTC_REG_SYS_X_ADJUST		0x1 // X-Adjust (-7 to 8)
#define RTC_REG_SYS_Y_ADJUST		0x2 // Y-Adjust (-7 to 8)
#define RTC_REG_SYS_SCREEN			0x3 // Alternating even/uneven | Interlacing | SCREEN 0 / 1
#define RTC_REG_SYS_WIDTH_LSB		0x4 // Least significant 4 bits of WIDTH value
#define RTC_REG_SYS_WIDTH_MSB		0x5 // Most significant 3 bits of WIDTH value
#define RTC_REG_SYS_TXT				0x6 // Initial text color
#define RTC_REG_SYS_BG				0x7 // Initial background color
#define RTC_REG_SYS_BORDER			0x8 // Initial border color
#define RTC_REG_SYS_KEY				0x9 // Transfer speed | Printer | Keyclick | Key off/on
#define RTC_REG_SYS_BEEP			0xA // BEEP timbre | BEEP volume
#define RTC_REG_SYS_LOGO			0xB // Startup logo screen colors
#define RTC_REG_SYS_AREA_CODE		0xC // Area code

#define RTC_INIT_DONE				0b1010

#define RTC_AREA_JAPAN				0 // Japan
#define RTC_AREA_US					1 // US
#define RTC_AREA_INT				2 // International
#define RTC_AREA_GB					3 // Great Britain
#define RTC_AREA_FRANCE				4 // France
#define RTC_AREA_GERMANY			5 // Germany
#define RTC_AREA_ITALY				6 // Italy
#define RTC_AREA_SPAIN				7 // Spain
#define RTC_AREA_AE					8 // Arab Emirates
#define RTC_AREA_KOREA				9 // Korea
#define RTC_AREA_USSR				10 // USSR
// 11 ~ 15 = Undefined (as of 02/05/1986)

//-----------------------------------------------------------------------------
// BLOCK 3 - TEXT
//-----------------------------------------------------------------------------
// The system uses this block to store the characters used for the password, the prompt or the title.
#define RTC_MODE_TEXT				RTC_MODE_BLOCK_3
#define RTC_MODE_DATA				RTC_MODE_BLOCK_3

#define RTC_REG_DATATYPE			0x0 // Data type (0~15):

#define RTC_DATA_TITLE				0 // Title
#define RTC_DATA_PASSWORD			1 // Password
#define RTC_DATA_PROMPT				2 // Prompt
// 3 to 15 = Undefined as of 02/05/1986
#define RTC_DATA_SAVE				6 // Game save data
#define RTC_DATA_SIGNSAVE			7 // Signed game save data (include application signature)
#define RTC_DATA_INVALID			15

//-----------------------------------------------------------------------------
// FUNCTIONS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Group: Core
//-----------------------------------------------------------------------------

// Function: RTC_SetMode
// Set clock mode
//
// Parameters:
//   mode - The clock mode flags
inline void RTC_SetMode(u8 mode) { g_RTC_AddrPort = RTC_REG_MODE; g_RTC_DataPort = mode | (RTC_MODE_ALARM_OFF + RTC_MODE_TIMER_ON); }

// Function: RTC_Read
// Read a RTC register value
//
// Parameters:
//   reg - RTC register number
//
// Return:
//   Register value (4 bits)
inline u8 RTC_Read(u8 reg) { g_RTC_AddrPort = reg; return 0xF & g_RTC_DataPort; }

// Function: RTC_Write
// Write a RTC register value
//
// Parameters:
//   reg - RTC register number
//   value - The value to write in the register
inline void RTC_Write(u8 reg, u8 value) { g_RTC_AddrPort = reg; g_RTC_DataPort = value; }

// Function: RTC_WriteRaw
// Write data in CMOS in the current block
// Use RTC_SetMode to change current block
//
// Parameters:
//   reg - Register index to start copy
//   data - Data to copy
//   num - Number of bytes to copy (from 1 to 6)
void RTC_WriteRaw(u8 reg, u8* data, u8 num);

// Function: RTC_ReadRaw
// Read data from CMOS in the current block
// Use RTC_SetMode to change current block
//
// Parameters:
//   reg - Register index to start reading
//   data - Data to read
//   num - Number of bytes to read (from 1 to 6)
void RTC_ReadRaw(u8 reg, u8* data, u8 num);

//-----------------------------------------------------------------------------
// Group: Date/Time
//-----------------------------------------------------------------------------

// Function: RTC_Initialize
// Initialize the clock module
inline void RTC_Initialize() { RTC_SetMode(RTC_MODE_TIME); }

#if (RTC_USE_CLOCK)

// Function: RTC_Set24H
// Set 12/24 hours mode
// Need RTC_USE_CLOCK define to be set in MSXgl configuration
//
// Parameters:
//   enable - TRUE for 24h mode and FALSE for 12h mode
inline void RTC_Set24H(bool enable) { RTC_SetMode(RTC_MODE_ALARM); RTC_Write(RTC_REG_ALARM_24H, enable ? 1 : 0); }

// Function: RTC_GetSecond
// Get current clock second counter (0-59)
// Need RTC_USE_CLOCK define to be set in MSXgl configuration
//
// Return:
//   Current clock second
u8 RTC_GetSecond();

// Function: RTC_GetMinute
// Get current clock minute counter (0-59)
// Need RTC_USE_CLOCK define to be set in MSXgl configuration
//
// Return:
//   Current clock minute
u8 RTC_GetMinute();

// Function: RTC_GetHour
// Get current clock hour counter (0-23)
// Need RTC_USE_CLOCK define to be set in MSXgl configuration
//
// Return:
//   Current clock hour
u8 RTC_GetHour();

// Function: RTC_IsPM
// Check if current hour is PM or AM
// Need RTC_USE_CLOCK define to be set in MSXgl configuration
//
// Return:
//   FALSE if current hour is AM
inline bool RTC_IsPM() { return RTC_GetHour() >= 12; }

// Function: RTC_GetDayOfWeek
// Get current clock day-of-week counter (0-6)
// Need RTC_USE_CLOCK define to be set in MSXgl configuration
//
// Return:
//   Current clock day-of-week
u8 RTC_GetDayOfWeek();

// Function: RTC_GetDay
// Get current clock day counter (1-31)
// Need RTC_USE_CLOCK define to be set in MSXgl configuration
//
// Return:
//   Current clock day
u8 RTC_GetDay();

// Function: RTC_GetMonth
// Get current clock month counter (1-12)
// Need RTC_USE_CLOCK define to be set in MSXgl configuration
//
// Return:
//   Current clock month
u8 RTC_GetMonth();

// Function: RTC_GetYear
// Get current clock year counter (0-99)
// Need RTC_USE_CLOCK define to be set in MSXgl configuration
//
// Return:
//   Current clock year
u8 RTC_GetYear();

//-----------------------------------------------------------------------------
// Group: Extra
//-----------------------------------------------------------------------------
#if (RTC_USE_CLOCK_EXTRA)

// Function: RTC_GetDayOfWeekString
// Get current clock day of week string
// Need RTC_USE_CLOCK and RTC_USE_CLOCK_EXTRA define to be set in MSXgl configuration
//
// Return:
//   Zero-terminated string with day of week 
const c8* RTC_GetDayOfWeekString();

// Function: RTC_GetMonthString
// Get current clock month counter string
// Need RTC_USE_CLOCK and RTC_USE_CLOCK_EXTRA define to be set in MSXgl configuration
//
// Return:
//   Zero-terminated string with month
const c8* RTC_GetMonthString();

// Function: RTC_GetYear4
// Get current clock 4-digits year counter (1980-2079)
// Need RTC_USE_CLOCK and RTC_USE_CLOCK_EXTRA define to be set in MSXgl configuration
//
// Return:
//   4-digits year counter 
u16 RTC_GetYear4();

#endif // (RTC_USE_CLOCK_EXTRA)
#endif // (RTC_USE_CLOCK)

//-----------------------------------------------------------------------------
// Group: System Setting
//-----------------------------------------------------------------------------

// Function: RTC_IsSettingOK
// Set area code
inline bool RTC_IsSettingOK() { RTC_SetMode(RTC_MODE_SCREEN); return RTC_Read(RTC_REG_SYS_INIT) == RTC_INIT_DONE; }

// Function: RTC_SetAreaCode
// Set area code
inline void RTC_SetAreaCode(u8 code) { RTC_SetMode(RTC_MODE_SCREEN); RTC_Write(RTC_REG_SYS_AREA_CODE, code); }

// Function: RTC_GetAreaCode
// Get area code
inline u8 RTC_GetAreaCode() { RTC_SetMode(RTC_MODE_SCREEN); return RTC_Read(RTC_REG_SYS_AREA_CODE); }

//-----------------------------------------------------------------------------
// Group: Custom Data
//-----------------------------------------------------------------------------

// Function: RTC_GetDataType
// Get area code
inline u8 RTC_GetDataType() { RTC_SetMode(RTC_MODE_DATA); return RTC_Read(RTC_REG_DATATYPE); }

#if (RTC_USE_SAVEDATA)

// Function: RTC_SaveData
// Save 6 bytes data to CMOS's block 3
// Need RTC_USE_SAVEDATA define to be set in MSXgl configuration
//
// Parameters:
//   data - 6 bytes buffer to be saved
void RTC_SaveData(u8* data);

// Function: RTC_LoadData
// Load 6 bytes data from CMOS's block 3
// Need RTC_USE_SAVEDATA define to be set in MSXgl configuration
//
// Parameters:
//   data - 6 bytes buffer to load data
bool RTC_LoadData(u8* data);

#endif // (RTC_USE_SAVEDATA)

#if (defined(APPSIGN) && RTC_USE_SAVESIGNED)

// Function: RTC_SaveDataSigned
// Save 6 bytes data to CMOS's block 3
// Need RTC_USE_SAVESIGNED define to be set in MSXgl configuration and AppSignature BuildTool option to TRUE
//
// Parameters:
//   data - 6 bytes buffer to be saved
void RTC_SaveDataSigned(u8* data);

// Function: RTC_LoadDataSigned
// Load 6 bytes data from CMOS's block 3
// Need RTC_USE_SAVESIGNED define to be set in MSXgl configuration and AppSignature BuildTool option to TRUE
//
// Parameters:
//   data - 6 bytes buffer to load data
bool RTC_LoadDataSigned(u8* data);

#endif // (defined(APPSIGN) && RTC_USE_SAVESIGNED)
