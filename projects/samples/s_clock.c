// __________________________
// ██▀█▀██▀▀▀█▀▀█▀█  ▄▄▄ ▄▄  │   ▄▄▄                ▄▄      
// █  ▄ █▄ ▀██▄ ▀▄█ ██   ██  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █▀▀ ▄█  █ █ ▀█▄█ ██▄▄│  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀──────────┘                 ▀▀
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
#define ANALOG_Y	120
#define ANALOG_R	70

// Library's logo
#define MSX_GL "\x01\x02\x03\x04\x05\x06"

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Trigonometric tables
#include "mathtable\mt_trigo_360.inc"

// Fonts
#include "font\font_mgl_sample6.h"
#include "font\font_mgl_digit2.h"

// Animation characters
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

// Days of the week string table
const c8* g_DayOfWeek[] = {
	"Sunday",
	"Monday",
	"Tuesday",
	"Wednesday",
	"Thursday",
	"Friday",
	"Saturday",	
};

// Month string table
const c8* g_Month[] = {
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
	i16 dx = g_Cosinus360[idx] >> 8;
	i16 dy = g_Sinus360[idx] >> 8;
	Draw_Line(ANALOG_X, ANALOG_Y, ANALOG_X + dx, ANALOG_Y + dy, 0x0, 0);

	idx = ((g_Hour + 9) * 30) % 360;	
	dx = g_Cosinus360[idx] >> 8;
	dy = g_Sinus360[idx] >> 8;
	Draw_Line(ANALOG_X, ANALOG_Y, ANALOG_X + dx, ANALOG_Y + dy, 0xFF, 0);
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
	i16 dx = g_Cosinus360[idx] >> 7;
	i16 dy = g_Sinus360[idx] >> 7;
	Draw_Line(ANALOG_X, ANALOG_Y, ANALOG_X + dx, ANALOG_Y + dy, 0x0, 0);

	idx = ((g_Min + 45) * 6) % 360;	
	dx = g_Cosinus360[idx] >> 7;
	dy = g_Sinus360[idx] >> 7;
	Draw_Line(ANALOG_X, ANALOG_Y, ANALOG_X + dx, ANALOG_Y + dy, 0xFF, 0);
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
	i16 dx = g_Cosinus360[idx] >> 7;
	i16 dy = g_Sinus360[idx] >> 7;
	Draw_Line(ANALOG_X, ANALOG_Y, ANALOG_X + dx, ANALOG_Y + dy, 0x0, 0);

	idx = ((g_Sec + 45) * 6) % 360;	
	dx = g_Cosinus360[idx] >> 7;
	dy = g_Sinus360[idx] >> 7;
	Draw_Line(ANALOG_X, ANALOG_Y, ANALOG_X + dx, ANALOG_Y + dy, 0x9, 0);
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
	VDP_EnableVBlank(true);
	VDP_EnableSprite(false);
	VDP_SetColor(0x1);
	VDP_SetPaletteEntry(8, RGB16(2, 0, 0));
	VDP_SetPaletteEntry(9, RGB16(7, 0, 0));

	// Initialize print system
	Print_SetBitmapFont(g_Font_MGL_Sample6);
	Print_SetColor(0xFF, 0x11);
	Print_Clear();
	Print_SetPosition(4, 4);
	Print_DrawText(MSX_GL "  RTC SAMPLE");
	Draw_Box(0, 0, 255, 14, 0xF, 0);
	
	// Initialize the RTC module
	Clock_Initialize();

	// Draw the static part of the analog watch
	Draw_Circle(ANALOG_X, ANALOG_Y, ANALOG_R, 0xF, 0);

	loop(i, 12)
	{
		// Analog clock hour numbers
		u16 idx = (((i * 5) + 45) * 6) % 360;
		i16 dx = g_Cosinus360[idx] >> 6;
		i16 dy = g_Sinus360[idx] >> 6;
		i16 dx2 = g_Cosinus360[idx] >> 10;
		i16 dy2 = g_Sinus360[idx] >> 10;
		Print_SetPosition(ANALOG_X + dx - 3 - dx2, ANALOG_Y + dy - 4 - dy2);
		Print_DrawInt((i == 0) ? 12 : i);

		// Analog clock hour marks
		dx = g_Cosinus360[idx] >> 7;
		dy = g_Sinus360[idx] >> 7;
		dx2 = g_Cosinus360[idx] >> 9;
		dy2 = g_Sinus360[idx] >> 9;
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
	Print_DrawText(g_DayOfWeek[Clock_GetDayOfWeek()]);
	Print_Space();
	Print_DrawText(g_Month[Clock_GetMonth() - 1]);
	Print_Space();
	Print_DrawInt(Clock_GetDay());
	Print_Space();
	Print_DrawInt(1980 + Clock_GetYear());

	Print_SetPosition(DIGIT_TIME+6*2, DIGIT_Y);
	Print_DrawChar(':');		
	Print_SetPosition(DIGIT_TIME+6*5, DIGIT_Y);
	Print_DrawChar(':');		

	// Initialize time variables
	g_Hour = 0xFF;
	g_Min = 0xFF;
	g_Sec = 0xFF;

	// Main loop
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		// Update time variables
		g_PrevHour = g_Hour;
		g_Hour = Clock_GetHour();
		g_PrevMin = g_Min;
		g_Min = Clock_GetMinute();
		g_PrevSec = g_Sec;
		g_Sec = Clock_GetSecond();
		
		// Wait for (linkly) the screen synchronization interruption
		Halt();
		
		// Update display
		UpdateHour();
		UpdateMinute();
		UpdateSecond();
	}

	Bios_Exit(0);
}