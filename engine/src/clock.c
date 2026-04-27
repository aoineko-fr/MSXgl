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
//  - https://www.msx.org/wiki/Ricoh_RP-5C01
//─────────────────────────────────────────────────────────────────────────────
#include "clock.h"

#if (MSX_VERSION >= MSX_2)

//-----------------------------------------------------------------------------
// Write data in CMOS
void RTC_WriteRaw(u8 reg, u8* data, u8 num)
{
	for (u8 i = 0; i < num; ++i)
	{
		RTC_Write(reg++, *data & 0xF);
		RTC_Write(reg++, *data >> 4);
		data++;
	}
}

//-----------------------------------------------------------------------------
// Read data from CMOS
void RTC_ReadRaw(u8 reg, u8* data, u8 num)
{
	for (u8 i = 0; i < num; ++i)
	{
		*data = RTC_Read(reg++);
		*data |= RTC_Read(reg++) << 4;
		data++;
	}
}

#if (RTC_USE_CLOCK)

//-----------------------------------------------------------------------------
// Get current clock second counter (0-59)
u8 RTC_GetSecond()
{
	RTC_SetMode(RTC_MODE_TIME);
	u8 ret = RTC_Read(RTC_REG_TIME_SEC);
	ret += 10 * RTC_Read(RTC_REG_TIME_10SEC);
	return ret;
}

//-----------------------------------------------------------------------------
// Get current clock minute counter (0-59)
u8 RTC_GetMinute()
{
	RTC_SetMode(RTC_MODE_TIME);
	u8 ret = RTC_Read(RTC_REG_TIME_MIN);
	ret += 10 * RTC_Read(RTC_REG_TIME_10MIN);
	return ret;
}

//-----------------------------------------------------------------------------
// Get current clock hour counter (0-23)
u8 RTC_GetHour()
{
	RTC_SetMode(RTC_MODE_TIME);
	u8 ret = RTC_Read(RTC_REG_TIME_HOUR);
	ret += 10 * RTC_Read(RTC_REG_TIME_10HOUR);
	return ret;
}

//-----------------------------------------------------------------------------
// Get current clock day-of-week counter (0-6)
u8 RTC_GetDayOfWeek()
{
	RTC_SetMode(RTC_MODE_TIME);
	return RTC_Read(RTC_REG_TIME_WEEKDAY);
}

//-----------------------------------------------------------------------------
// Get current clock day counter (1-31)
u8 RTC_GetDay()
{
	RTC_SetMode(RTC_MODE_TIME);
	u8 ret = RTC_Read(RTC_REG_TIME_DAY);
	ret += 10 * RTC_Read(RTC_REG_TIME_10DAY);
	return ret;
}

//-----------------------------------------------------------------------------
// Get current clock month counter (1-12)
u8 RTC_GetMonth()
{
	RTC_SetMode(RTC_MODE_TIME);
	u8 ret = RTC_Read(RTC_REG_TIME_MONTH);
	ret += 10 * RTC_Read(RTC_REG_TIME_10MONTH);
	return ret;
}

//-----------------------------------------------------------------------------
// Get current clock year counter (0-99)
u8 RTC_GetYear()
{
	RTC_SetMode(RTC_MODE_TIME);
	u8 ret = RTC_Read(RTC_REG_TIME_YEAR);
	ret += 10 * RTC_Read(RTC_REG_TIME_10YEAR);
	return ret;
}

#if (RTC_USE_CLOCK_EXTRA)

const c8* g_RTC_DayOfWeek[] = {
	"Sunday",
	"Monday",
	"Tuesday",
	"Wednesday",
	"Thursday",
	"Friday",
	"Saturday",	
};

const c8* g_RTC_Month[] = {
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
// Get current clock day of week string
const c8* RTC_GetDayOfWeekString()
{
	return g_RTC_DayOfWeek[RTC_GetDayOfWeek()];
}

//-----------------------------------------------------------------------------
// Get current clock month counter string
const c8* RTC_GetMonthString()
{
	return g_RTC_Month[RTC_GetMonth() - 1];
}

//-----------------------------------------------------------------------------
// Get current clock 4-number year counter (1980-2079)
u16 RTC_GetYear4()
{
	return 1980 + RTC_GetYear();
}

#endif // (RTC_USE_CLOCK_EXTRA)
#endif // (RTC_USE_CLOCK)

#if (RTC_USE_SAVEDATA)

//-----------------------------------------------------------------------------
// Save 6 bytes data to CMOS's block 3
void RTC_SaveData(u8* data)
{
	RTC_SetMode(RTC_MODE_BLOCK_3);
	RTC_Write(0, RTC_DATA_SAVE);
	RTC_WriteRaw(1, data, 6);
}

//-----------------------------------------------------------------------------
// Load 6 bytes data from CMOS's block 3
bool RTC_LoadData(u8* data)
{
	RTC_SetMode(RTC_MODE_BLOCK_3);
	if (RTC_Read(0) != RTC_DATA_SAVE) // Check data type
		return FALSE;
	RTC_ReadRaw(1, data, 6);
	return TRUE;
}

#endif // (RTC_USE_SAVEDATA)


#if (defined(APPSIGN) && RTC_USE_SAVESIGNED)

extern const u32 g_AppSignature;

//-----------------------------------------------------------------------------
// Save 6 bytes data to CMOS's block 3
void RTC_SaveDataSigned(u8* data)
{
	// Write application signature
	RTC_SetMode(RTC_MODE_BLOCK_1);
	const u8* sign = (const u8*)&g_AppSignature;
	// Signature 1st byte (company LSB)
 	RTC_Write(2, *sign      & 0xF ); // 4 bits
 	RTC_Write(3, *sign >> 4 & 0x7 ); // 3 bits
	sign++;
	// Signature 2nd byte (company MSB)
 	RTC_Write(4, *sign      & 0xF ); // 4 bits
 	RTC_Write(6, *sign >> 4 & 0x7 ); // 3 bits
	sign++;
	// Signature 4th byte (product LSB)
 	RTC_Write(5, *sign      & 0x3 ); // 2 bits
 	RTC_Write(7, *sign >> 2 & 0xF ); // 4 bits
 	RTC_Write(8, *sign >> 6 & 0x3 ); // 2 bits

	// Write data
	RTC_SetMode(RTC_MODE_BLOCK_3);
	RTC_Write(0, RTC_DATA_SIGNSAVE);
	RTC_WriteRaw(1, data, 6);
}

//-----------------------------------------------------------------------------
// Load 6 bytes data from CMOS's block 3
bool RTC_LoadDataSigned(u8* data)
{
	// Check application signature
	RTC_SetMode(RTC_MODE_BLOCK_1);
	const u8* sign = (const u8*)&g_AppSignature;
	// Signature 1st byte (company LSB)
	u8 a = *sign & 0x7F;
	u8 b = RTC_Read(2) + (RTC_Read(3) << 4);
	if (a != b)
		return RTC_DATA_INVALID;
	// Signature 2nd byte (company MSB)
	sign++;
	a = *sign & 0x7F;
	b = RTC_Read(4) + (RTC_Read(6) << 4);
	if (a != b)
		return RTC_DATA_INVALID;
	// Signature 4th byte (product LSB)
	sign++;
	a = *sign;
	b = RTC_Read(5) + (RTC_Read(7) << 2) + (RTC_Read(8) << 6);
	if (a != b)
		return RTC_DATA_INVALID;

	// Load data
	RTC_SetMode(RTC_MODE_BLOCK_3);
	if (RTC_Read(0) != RTC_DATA_SIGNSAVE)
		return FALSE;
	RTC_ReadRaw(1, data, 6);
	return TRUE;
}

#endif // (defined(APPSIGN) && RTC_USE_SAVESIGNED)

#endif // (MSX_VERSION >= MSX_2)