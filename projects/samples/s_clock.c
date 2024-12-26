// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Real-time clock module sample
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"

//=============================================================================
// DEFINES
//=============================================================================

// Digital watch parameters
#define DIGIT_Y		24
#define DIGIT_DATE	2
#define DIGIT_TIME	206

// Analog watch parameters
#define ANALOG_X	128
#define ANALOG_Y	120-8
#define ANALOG_R	70

// Library's logo
#define MSX_GL "\x01\x02\x03\x04\x05\x06"

// Functions prototype
void DisplayClock();
void UpdateClock();
void DisplaySaveData();
void UpdateSaveData();

extern const c8 g_HexChar[16];

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Trigonometric tables
#include "mathtable/mt_trigo_360.inc"

// Fonts
#include "font/font_mgl_sample6.h"
#include "font/font_mgl_digit2.h"

// Animation characters
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

// Days of the week string table
const c8* g_DayOfWeek[] =
{
	"Sunday",
	"Monday",
	"Tuesday",
	"Wednesday",
	"Thursday",
	"Friday",
	"Saturday",	
};

// Month string table
const c8* g_Month[] =
{
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

// Valid bits table
u8 g_ValidBits[4][13] =
{
	//  0    1    2    3    4    5    6    7    8    9    10   11   12
	{ 0xF, 0x7, 0xF, 0x7, 0xF, 0x3, 0x7, 0xF, 0x3, 0xF, 0x1, 0xF, 0xF, },
	{ 0x0, 0x0, 0xF, 0x7, 0xF, 0x3, 0x7, 0xF, 0x3, 0x0, 0x3, 0x3, 0x0, },
	{ 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, },
	{ 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, },
};

//=============================================================================
// MEMORY DATA
//=============================================================================

// Current time
u8 g_Sec;
u8 g_Min;
u8 g_Hour;

// Previous time
u8 g_PrevSec;
u8 g_PrevMin;
u8 g_PrevHour;

// Save data
u8 g_SaveData[6];

callback g_Update = NULL;

//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Update hour display
void UpdateHour()
{
	// Update digital watch
	if(g_Hour != g_PrevHour)
	{
		Print_SetPosition(DIGIT_TIME+6*0, DIGIT_Y);
		Print_SetColor(0x8, 0x1);
		Print_DrawText("@@");
		
		Print_SetPosition(DIGIT_TIME+6*0, DIGIT_Y);
		Print_SetColor(0x9, 0x1);
		if(g_Hour < 10)
			Print_DrawChar('0');
		Print_DrawInt(g_Hour);
	}

	// Update analog watch
	u16 idx = ((g_PrevHour + 9) * 30) % 360;
	i16 dx = ((i16)g_Cosinus360[idx]) >> 8;
	i16 dy = ((i16)g_Sinus360[idx]) >> 8;
	Draw_Line(ANALOG_X, ANALOG_Y, ANALOG_X + dx, ANALOG_Y + dy, 0x1, 0);

	idx = ((g_Hour + 9) * 30) % 360;
	dx = ((i16)g_Cosinus360[idx]) >> 8;
	dy = ((i16)g_Sinus360[idx]) >> 8;
	Draw_Line(ANALOG_X, ANALOG_Y, ANALOG_X + dx, ANALOG_Y + dy, 0xF, 0);
}

//-----------------------------------------------------------------------------
// Update minute display
void UpdateMinute()
{
	// Update digital watch
	if(g_Min != g_PrevMin)
	{
		Print_SetPosition(DIGIT_TIME+6*3, DIGIT_Y);
		Print_SetColor(0x8, 0x1);
		Print_DrawText("@@");

		Print_SetPosition(DIGIT_TIME+6*3, DIGIT_Y);
		Print_SetColor(0x9, 0x1);
		if(g_Min < 10)
			Print_DrawChar('0');
		Print_DrawInt(g_Min);
	}

	// Update analog watch
	u16 idx = ((g_PrevMin + 45) * 6) % 360;	
	i16 dx = ((i16)g_Cosinus360[idx]) >> 7;
	i16 dy = ((i16)g_Sinus360[idx]) >> 7;
	Draw_Line(ANALOG_X, ANALOG_Y, ANALOG_X + dx, ANALOG_Y + dy, 0x1, 0);

	idx = ((g_Min + 45) * 6) % 360;	
	dx = ((i16)g_Cosinus360[idx]) >> 7;
	dy = ((i16)g_Sinus360[idx]) >> 7;
	Draw_Line(ANALOG_X, ANALOG_Y, ANALOG_X + dx, ANALOG_Y + dy, 0xF, 0);
}

//-----------------------------------------------------------------------------
// Update second display
void UpdateSecond()
{
	// Update digital watch
	if(g_Sec != g_PrevSec)
	{
		Print_SetPosition(DIGIT_TIME+6*6, DIGIT_Y);
		Print_SetColor(0x8, 0x1);
		Print_DrawText("@@");

		Print_SetPosition(DIGIT_TIME+6*6, DIGIT_Y);
		Print_SetColor(0x9, 0x1);
		if(g_Sec < 10)
			Print_DrawChar('0');
		Print_DrawInt(g_Sec);
	}

	// Update analog watch
	u16 idx = ((g_PrevSec + 45) * 6) % 360;	
	i16 dx = ((i16)g_Cosinus360[idx]) >> 7;
	i16 dy = ((i16)g_Sinus360[idx]) >> 7;
	Draw_Line(ANALOG_X, ANALOG_Y, ANALOG_X + dx, ANALOG_Y + dy, 0x1, 0);

	idx = ((g_Sec + 45) * 6) % 360;	
	dx = ((i16)g_Cosinus360[idx]) >> 7;
	dy = ((i16)g_Sinus360[idx]) >> 7;
	Draw_Line(ANALOG_X, ANALOG_Y, ANALOG_X + dx, ANALOG_Y + dy, 0x9, 0);
}

//-----------------------------------------------------------------------------
// Display the clock
void DisplayClock()
{
	// Initialize print system
	Print_SetBitmapFont(g_Font_MGL_Sample6);
	Print_SetColor(0xFF, 0x11);
	Print_Clear();
	Print_SetPosition(0, 0);
	Print_DrawText(MSX_GL " RTC SAMPLE - Clock");
	Draw_LineH(0, 255, 12, 0xF, 0);

	Print_SetPosition(0, 204);
	Print_DrawText("F1:Data Save");

	// Initialize the RTC module
	RTC_Initialize();
	RTC_Set24H(TRUE);

	// Draw the static part of the analog watch
	Draw_Circle(ANALOG_X, ANALOG_Y, ANALOG_R, 0xF, 0);

	loop(i, 12)
	{
		// Analog clock hour numbers
		u16 idx = (((i * 5) + 45) * 6) % 360;
		i16 dx = ((i16)g_Cosinus360[idx]) >> 6;
		i16 dy = ((i16)g_Sinus360[idx]) >> 6;
		i16 dx2 = ((i16)g_Cosinus360[idx]) >> 10;
		i16 dy2 = ((i16)g_Sinus360[idx]) >> 10;
		Print_SetPosition(ANALOG_X + dx - 3 - dx2, ANALOG_Y + dy - 4 - dy2);
		Print_DrawInt((i == 0) ? 12 : i);

		// Analog clock hour marks
		dx = ((i16)g_Cosinus360[idx]) >> 7;
		dy = ((i16)g_Sinus360[idx]) >> 7;
		dx2 = ((i16)g_Cosinus360[idx]) >> 9;
		dy2 = ((i16)g_Sinus360[idx]) >> 9;
		Draw_Line(ANALOG_X + dx + dx2, ANALOG_Y + dy + dy2 + 1, ANALOG_X + dx + 2 * dx2, ANALOG_Y + dy + 2 * dy2 + 1, 0xEE, 0);
	}

	// Draw the static part of the digital watch
	Print_SetFont(g_Font_MGL_Digit2);
	Print_SetMode(PRINT_MODE_BITMAP_TRANS);

	Print_SetColor(0x8, 0x1);
	Print_SetPosition(DIGIT_DATE, DIGIT_Y);
	Print_DrawCharX('@', 42);

	Print_SetColor(0x9, 0x1);
	Print_SetPosition(DIGIT_DATE, DIGIT_Y);
	Print_DrawText(g_DayOfWeek[RTC_GetDayOfWeek()]);
	Print_Space();
	Print_DrawText(g_Month[RTC_GetMonth() - 1]);
	Print_Space();
	Print_DrawInt(RTC_GetDay());
	Print_Space();
	Print_DrawInt(1980 + RTC_GetYear());

	Print_SetPosition(DIGIT_TIME+6*2, DIGIT_Y);
	Print_DrawChar(':');
	Print_SetPosition(DIGIT_TIME+6*5, DIGIT_Y);
	Print_DrawChar(':');

	// Initialize time variables
	g_Hour = 0xFF;
	g_Min = 0xFF;
	g_Sec = 0xFF;

	g_Update = UpdateClock;
}

//-----------------------------------------------------------------------------
// Update the clock
void UpdateClock()
{
	// Update time variables
	g_PrevHour = g_Hour;
	g_Hour = RTC_GetHour();
	g_PrevMin = g_Min;
	g_Min = RTC_GetMinute();
	g_PrevSec = g_Sec;
	g_Sec = RTC_GetSecond();

	// Wait for (linkly) the screen synchronization interruption
	Halt();

	// Update display
	UpdateHour();
	UpdateMinute();
	UpdateSecond();

	if(Keyboard_IsKeyPressed(KEY_F1))
		DisplaySaveData();
}

extern const u32 g_AppSignature;

//-----------------------------------------------------------------------------
// Draw an ID
void DrawID(const u8* data)
{
	for(u8 i = 0; i < 4; ++i)
	{
		if((*data >= 0x21) && (*data <= 0x7E))
			Print_DrawChar(*data++);
		else
			Print_DrawFormat("[%2x]", *data++);
	}
}

//-----------------------------------------------------------------------------
// Display the save data
void DisplaySaveData()
{
	// Initialize print system
	Print_SetBitmapFont(g_Font_MGL_Sample6);
	Print_SetColor(0xFF, 0x11);
	Print_Clear();
	Print_SetPosition(0, 0);
	Print_DrawText(MSX_GL " RTC SAMPLE - Save");
	Draw_LineH(0, 255, 12, 0xF, 0);

	RTC_SetMode(RTC_MODE_BLOCK_3);
	u8 type = RTC_Read(0);

	// 
	for(u8 b = 0; b < 4; ++b)
	{
		Print_SetColor(0xFF, 0x11);
		Print_SetPosition(22 + b * 56, 20);
		Print_DrawFormat("Block#%i", b);
		RTC_SetMode(b);
		for(u8 r = 0; r < 13; ++r)
		{
			Print_SetColor(0xFF, 0x11);
			if(b == 0)
			{
				Print_SetPosition(0, 32 + r * 8);
				Print_DrawInt(r);
			}

			u8 col = 0xFF;
			u8 v = RTC_Read(r);
			if((b == 3) && (r == 0))
			{
				if((v == RTC_DATA_SAVE) || (v == RTC_DATA_SIGNSAVE))
					col = 0x33;
				else
					col = 0x99;
			}
			else if((b == 3) && ((type == RTC_DATA_SAVE) || (type == RTC_DATA_SIGNSAVE)))
				col = 0x55;
			else if((b == 2) && (r == 0))
			{
				if(v == RTC_INIT_DONE)
					col = 0x33;
				else
					col = 0x99;
			}
			else if((b == 1) && (r >= 2) && (r <= 8) && (type == RTC_DATA_SIGNSAVE))
				col = 0xBB;

			Print_SetColor(col, 0x11);
			Print_SetPosition(24 + b * 56, 32 + r * 8);
			Print_DrawChar(g_HexChar[v]);
			Print_Space();
			u8 bit = 8;
			for(u8 i = 0; i < 4; ++i)
			{
				if(g_ValidBits[b][r] & bit)
					Print_SetColor(col, 0x11);
				else
					Print_SetColor(0xEE, 0x11);
				Print_DrawChar((v & bit) ? '1' : '0');
				bit >>= 1;
			}
		}
	}

	const c8* typeStr;
	switch(type)
	{
	case RTC_DATA_TITLE:    typeStr = "Title";    break;
	case RTC_DATA_PASSWORD: typeStr = "Password"; break;
	case RTC_DATA_PROMPT:   typeStr = "Prompt";   break;
	case RTC_DATA_SAVE:     typeStr = "Save";     break;
	case RTC_DATA_SIGNSAVE: typeStr = "SignSave"; break;
	case RTC_DATA_INVALID:  typeStr = "Invalid";  break;
	default:                typeStr = "Unknown";
	}

	u8 data[6];

	Print_SetColor(0xFF, 0x11);
	Print_SetPosition(0, 144);
	Print_DrawFormat("Type: %i %s", type, typeStr);
	if((type == RTC_DATA_SAVE) || (type == RTC_DATA_SIGNSAVE))
	{
		bool bOK = FALSE;
		if(type == RTC_DATA_SAVE)
			bOK = RTC_LoadData(data);
		else
			bOK = RTC_LoadDataSigned(data);
		Print_DrawFormat(" %2x-%2x-%2x-%2x-%2x-%2x %s", data[0], data[1], data[2], data[3], data[4], data[5], bOK ? "OK" : "NOK");
	}

	const u8* sign = (const u8*)&g_AppSignature;
	u8 comp[4];
	comp[0] = sign[0] & 0x7F;
	comp[1] = sign[1] & 0x7F;
	comp[2] = sign[2];
	comp[3] = 0;

	RTC_SetMode(RTC_MODE_BLOCK_1);
	u8 cmos[4];
	cmos[0] = RTC_Read(2) + (RTC_Read(3) << 4);
	cmos[1] = RTC_Read(4) + (RTC_Read(6) << 4);
	cmos[2] = RTC_Read(5) + (RTC_Read(7) << 2) + (RTC_Read(8) << 6);
	cmos[3] = 0;

	Print_SetPosition(0, 154);
	Print_DrawText("Sign: ");
	DrawID(sign);
	Print_DrawText(" (");
	DrawID(comp);
	Print_DrawText(")");
	
	Print_SetPosition(0, 164);
	Print_DrawText("CMOS: ");
	DrawID(cmos);

	Print_SetPosition(0, 204);
	Print_DrawText("F1:Clock  \x83:Save  \x84:Signed  \x85:Erase all");

	g_Update = UpdateSaveData;
}

//-----------------------------------------------------------------------------
// Update the save data
void UpdateSaveData()
{
	// Wait for (linkly) the screen synchronization interruption
	Halt();

	if(Keyboard_IsKeyPressed(KEY_SPACE))
	{
		for(u8 i = 0; i < 6; ++i)
			g_SaveData[i] = (i * 2) + 1 + ((i * 2) << 4);
		RTC_SaveData(g_SaveData);
		DisplaySaveData();
	}
	if(Keyboard_IsKeyPressed(KEY_RET))
	{
		for(u8 i = 0; i < 6; ++i)
			g_SaveData[i] = 0xBE;
		RTC_SaveDataSigned(g_SaveData);
		DisplaySaveData();
	}
	if(Keyboard_IsKeyPressed(KEY_BS))
	{
		for(u8 b = 0; b < 4; ++b)
		{
			RTC_SetMode(b);
			for(u8 r = 0; r < 13; ++r)
				RTC_Write(r, 0xFF);
		}
		DisplaySaveData();
	}

	if(Keyboard_IsKeyPressed(KEY_F1))
		DisplayClock();
}

//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	// Initialize display system
	VDP_SetMode(VDP_MODE_SCREEN5);
	VDP_EnableVBlank(TRUE);
	VDP_EnableSprite(FALSE);
	VDP_SetLineCount(VDP_LINE_212);
	VDP_SetColor(0x1);
	VDP_SetPaletteEntry(8, RGB16(2, 0, 0));
	VDP_SetPaletteEntry(9, RGB16(7, 0, 0));

	DisplayClock();

	// Main loop
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		g_Update();
	}

	Bios_Exit(0);
}