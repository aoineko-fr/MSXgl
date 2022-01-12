// ____________________________
// ██▀███▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄  ▄▄            ▄▄   
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ ▀ ██  ▄█▀▄ ▄█▀▀ ██▄▀ 
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄▀ ▀█▄ ▀█▄▀ ▀█▄▄ ██ █ 
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
// Real-time clock module (RP-5C01)
//
// Réferences:
//  - Ricoh RP/RF5C01A application manual
//  - https://www.msx.org/wiki/Ricoh_RP-5C01
//─────────────────────────────────────────────────────────────────────────────
#include "clock.h"

#if (MSX_VERSION >= MSX_2)

//-----------------------------------------------------------------------------
/// Initialize the clock module
void Clock_Initialize()
{
	Clock_SetMode(RTC_MODE_TIME + RTC_MODE_ALARM_OFF + RTC_MODE_SEC_ON);
}

//-----------------------------------------------------------------------------
/// Set clock mode
void Clock_SetMode(u8 mode) __FASTCALL
{
	g_RTC_AddrPort = RTC_REG_MODE;
	g_RTC_DataPort = mode;
}

//-----------------------------------------------------------------------------
/// Get current clock second counter (0-59)
u8 Clock_GetSecond()
{
	u8 ret;
	// g_RTC_AddrPort = RTC_REG_MODE;
	// g_RTC_DataPort = RTC_MODE_BLOCK_0;
	g_RTC_AddrPort = RTC_TIME_SEC;
	ret = (0xF & g_RTC_DataPort);
	g_RTC_AddrPort = RTC_TIME_10SEC;
	ret += 10 * (0xF & g_RTC_DataPort);
	return ret;
}

//-----------------------------------------------------------------------------
/// Get current clock minute counter (0-59)
u8 Clock_GetMinute()
{
	u8 ret;
	// g_RTC_AddrPort = RTC_REG_MODE;
	// g_RTC_DataPort = RTC_MODE_BLOCK_0;
	g_RTC_AddrPort = RTC_TIME_MIN;
	ret = (0xF & g_RTC_DataPort);
	g_RTC_AddrPort = RTC_TIME_10MIN;
	ret += 10 * (0xF & g_RTC_DataPort);
	return ret;
}

//-----------------------------------------------------------------------------
/// Get current clock hour counter (0-23)
u8 Clock_GetHour()
{
	u8 ret;
	// g_RTC_AddrPort = RTC_REG_MODE;
	// g_RTC_DataPort = RTC_MODE_BLOCK_0;
	g_RTC_AddrPort = RTC_TIME_HOUR;
	ret = (0xF & g_RTC_DataPort);
	g_RTC_AddrPort = RTC_TIME_10HOUR;
	ret += 10 * (0xF & g_RTC_DataPort);
	return ret;
}

//-----------------------------------------------------------------------------
/// Get current clock day-of-week counter (0-6)
u8 Clock_GetDayOfWeek()
{
	// g_RTC_AddrPort = RTC_REG_MODE;
	// g_RTC_DataPort = RTC_MODE_BLOCK_0;
	g_RTC_AddrPort = RTC_TIME_WEEKDAY;
	return (0xF & g_RTC_DataPort);
}

//-----------------------------------------------------------------------------
/// Get current clock day counter (1-31)
u8 Clock_GetDay()
{
	u8 ret;
	// g_RTC_AddrPort = RTC_REG_MODE;
	// g_RTC_DataPort = RTC_MODE_BLOCK_0;
	g_RTC_AddrPort = RTC_TIME_DAY;
	ret = (0xF & g_RTC_DataPort);
	g_RTC_AddrPort = RTC_TIME_10DAY;
	ret += 10 * (0xF & g_RTC_DataPort);
	return ret;
}

//-----------------------------------------------------------------------------
/// Get current clock month counter (1-12)
u8 Clock_GetMonth()
{
	u8 ret;
	// g_RTC_AddrPort = RTC_REG_MODE;
	// g_RTC_DataPort = RTC_MODE_BLOCK_0;
	g_RTC_AddrPort = RTC_TIME_MONTH;
	ret = (0xF & g_RTC_DataPort);
	g_RTC_AddrPort = RTC_TIME_10MONTH;
	ret += 10 * (0xF & g_RTC_DataPort);
	return ret;
}

//-----------------------------------------------------------------------------
/// Get current clock year counter (0-99)
u8 Clock_GetYear()
{
	u8 ret;
	// g_RTC_AddrPort = RTC_REG_MODE;
	// g_RTC_DataPort = RTC_MODE_BLOCK_0;
	g_RTC_AddrPort = RTC_TIME_YEAR;
	ret = (0xF & g_RTC_DataPort);
	g_RTC_AddrPort = RTC_TIME_10YEAR;
	ret += 10 * (0xF & g_RTC_DataPort);
	return ret;
}

//-----------------------------------------------------------------------------
/// Read a RTC register value
u8 Clock_Read(u8 reg) __FASTCALL
{
	g_RTC_AddrPort = reg;
	return g_RTC_DataPort;
}

//-----------------------------------------------------------------------------
/// Write a RTC register value
void Clock_Write(u8 reg, u8 value)
{
	g_RTC_AddrPort = reg;
	g_RTC_DataPort = value;
}

#if (USE_CLOCK_EXTRA)

const c8* g_Clock_DayOfWeek[] = {
	"Sunday",
	"Monday",
	"Tuesday",
	"Wednesday",
	"Thursday",
	"Friday",
	"Saturday",	
};

const c8* g_Clock_Month[] = {
	"January",
	"February",
	"March",            
	"April",
	"May",
	"June",
	"July",
	"August",       
	"September",
	"October",
	"November",
	"December",
};

//-----------------------------------------------------------------------------
/// Get current clock day of week string
const c8* Clock_GetDayOfWeekString()
{
	return g_Clock_DayOfWeek[Clock_GetDayOfWeek()];
}

//-----------------------------------------------------------------------------
/// Get current clock month counter string
const c8* Clock_GetMonthString()
{
	return g_Clock_Month[Clock_GetMonth() - 1];
}

//-----------------------------------------------------------------------------
/// Get current clock 4-number year counter (1980-2079)
u16 Clock_GetYear4()
{
	return 1980 + Clock_GetYear();
}

#endif // (USE_CLOCK_EXTRA)

#endif // (MSX_VERSION >= MSX_2)