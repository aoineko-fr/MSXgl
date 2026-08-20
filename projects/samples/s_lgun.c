// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Hello world sample
//─────────────────────────────────────────────────────────────────────────────

#include "msxgl.h"
#include "device/lightgun.h"
#include "fsm.h"
#include "game/menu.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL						"\x02\x03\x04\x05"

// VRAM layout table
#define LAYOUT_SHOW_ADDR			0x1800
#define LAYOUT_HIDE_ADDR			0x1C00

#define COLOR_HIDE					COLOR_BLACK
#define LOG_COUNT_MAX				10
#define CLEAR_FRAME					2
#define DETECT_FRAME				2
#define INPUT_WAIT					10

// Lightgun driver ID
enum LGUN_DRIVER_ID
{
	LGUN_DRIVER_ASCII = 0,
	LGUN_DRIVER_GUNSTICK,
	LGUN_DRIVER_PHENIX,
//.....................................
	LGUN_DRIVER_MAX,
};

// Program state
enum STATE_ID
{
	STATE_INFO = 0,
	STATE_BLACK,
	STATE_DETECT,
//.....................................
	STATE_MAX,
};

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Font
#include "font/font_mgl_sample8.h"

// Sprite by GrafxKid (https://opengameart.org/content/super-random-sprites)
#include "content/data_sprt_16.h"

// Animation characters
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

// Sinus table (64 samples)
const u8 g_Sinus64[] = {
	0x00, 0x02, 0x03, 0x05, 0x06, 0x08, 0x09, 0x0A, 
	0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x0F, 0x10, 0x10, 
	0x10, 0x10, 0x10, 0x0F, 0x0F, 0x0E, 0x0D, 0x0C, 
	0x0B, 0x0A, 0x09, 0x08, 0x06, 0x05, 0x03, 0x02, 
	0x00, 0xFE, 0xFD, 0xFB, 0xFA, 0xF8, 0xF7, 0xF6, 
	0xF5, 0xF4, 0xF3, 0xF2, 0xF1, 0xF1, 0xF0, 0xF0, 
	0xF0, 0xF0, 0xF0, 0xF1, 0xF1, 0xF2, 0xF3, 0xF4, 
	0xF5, 0xF6, 0xF7, 0xF8, 0xFA, 0xFB, 0xFD, 0xFE, 
};

const u8 g_SpriteColor[2][2] = {
	{ COLOR_LIGHT_GREEN, COLOR_DARK_GREEN },
	{ COLOR_LIGHT_RED, COLOR_DARK_RED }
};

const u8 g_BGColorTab[] = { COLOR_DARK_BLUE, COLOR_LIGHT_BLUE, COLOR_CYAN, COLOR_WHITE, COLOR_BLACK };

void InitInfo();
void UpdateInfo();
void InitClear();
void UpdateClear();
void InitDetect();
void UpdateDetect();

const FSM_State g_StateInfo   = { 0, InitInfo,   UpdateInfo,   NULL };
const FSM_State g_StateClear  = { 0, InitClear,  UpdateClear,  NULL };
const FSM_State g_StateDetect = { 0, InitDetect, UpdateDetect, NULL };

//=============================================================================
// MEMORY DATA
//=============================================================================

bool g_VBlank = FALSE;
u16  g_Frame = 0;

u8   g_SpritData[8 * 4];

u8   g_SpritePosX;
u8   g_SpritePosY;

u8   g_Driver = LGUN_DRIVER_PHENIX;
u8   g_Port = 0;

bool g_Hide = FALSE;

u16  g_FrameCount = 0;
u16  g_MoveCount = 160 * 2;
bool g_Move = FALSE;

u8   g_LogCount = 0;

u8   g_Joy = 0;
bool g_Trig = FALSE;
bool g_TrigPrev = FALSE;
u8   g_Light = 0;

u8   g_KeyWait = 0;

u8   g_ClearFrame;
u8   g_DetectFrame;

bool g_ColorTest = FALSE;
u8   g_HitCounter = 0;
u8   g_ColorNum = 0;


//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Update sprite position
void UpdatePosition()
{
	g_SpritePosX = g_MoveCount++ / 2;
	g_SpritePosY = 128 + 2 * g_Sinus64[(g_MoveCount / 2) & 0x3F];
}

//-----------------------------------------------------------------------------
// Update sprite attributes
void UpdateSprite()
{
	u8 frame = (g_MoveCount / 8) % 6;
	const u8* color = g_SpriteColor[g_HitCounter ? 1 : 0];
	VDP_SetSpriteSM1(0, g_SpritePosX, g_SpritePosY, (frame * 4) + 0, color[0]);
	VDP_SetSpriteSM1(1, g_SpritePosX, g_SpritePosY, (frame * 4) + 24, color[1]);
}

//-----------------------------------------------------------------------------
//
void UpdateLightgun()
{
	// Update text
	if (g_Driver == LGUN_DRIVER_PHENIX)
		g_Joy = Lightgun_Phenix_GetRaw();
	else
		g_Joy = LightGun_Read((g_Port == 0) ? JOY_PORT_1 : JOY_PORT_2);

	Print_DrawBin8At(1, 5, g_Joy);
	Print_DrawHex8At(10, 5, g_Joy);

	g_TrigPrev = g_Trig;
	g_Trig = FALSE;
	g_Light = 0;
	switch (g_Driver)
	{
	case LGUN_DRIVER_ASCII:
		g_Trig = LightGun_ASCII_GetTrigger(g_Joy);
		g_Light = LightGun_ASCII_GetLight(g_Joy);
		break;
	case LGUN_DRIVER_GUNSTICK:
		g_Trig = LightGun_GunStick_GetTrigger(g_Joy);
		g_Light = LightGun_GunStick_GetLight(g_Joy);
		break;
	case LGUN_DRIVER_PHENIX:
		g_Trig = LightGun_Phenix_GetTriggerB();
		g_Light = LightGun_Phenix_GetLight();
		// g_Trig = LightGun_Phenix_GetTriggerA(g_Joy);
		// g_Light = LightGun_Phenix_GetLight(g_Joy);
		// Print_DrawCharAt(12, 8, LightGun_Phenix_GetTriggerB(g_Joy) ? 0x0C : 0x0B);
		break;
	}
	Print_DrawCharAt(10, 8, g_Trig ? 0x0C : 0x0B);
	Print_DrawCharAt(10, 9, g_Light ? 0x0C : 0x0B);
	Print_DrawHex8At(12, 9, g_Light);

	if (g_LogCount)
	{
		u8 y = 12 + LOG_COUNT_MAX - g_LogCount;
		Print_DrawBin8At(1, y, g_Joy);
		Print_DrawHex8At(10, y, g_Joy);
		if (g_Driver == LGUN_DRIVER_PHENIX)
		{
			c8 chr = 'x';
			switch (Lightgun_Phenix_GetState())
			{
			case LGUN_STATE_DETECT:	chr = 'D'; break;
			case LGUN_STATE_BLACK:	chr = 'B'; break;
			case LGUN_STATE_WHITE:	chr = 'W'; break;
			case LGUN_STATE_HIT:	chr = 'H'; break;
			}
			Print_DrawCharAt(15, y, chr);
		}
		g_LogCount--;
	}

	if (g_HitCounter)
		g_HitCounter--;
}


//-----------------------------------------------------------------------------
// Diplay information page
void DiplayPage()
{
	// Clear screen
	VDP_FillVRAM_16K(0, VDP_GetLayoutTable(), 32 * 24);

	// Header
	Print_DrawTextAt(0, 0, MSX_GL " LIGHT GUN SAMPLE");
	Print_DrawLineH(0, 1, 32);

	// Information
	Print_DrawTextAt(0, 3, (g_Port == 0) ? "PORT: 1" : "PORT: 2");
	Print_DrawTextAt(1, 4, "--BA\x8A\x8B\x89\x88");

	switch (g_Driver)
	{
	case LGUN_DRIVER_ASCII:
		Print_DrawTextAt(0, 7, "DRIVER: ASCII");
		break;
	case LGUN_DRIVER_GUNSTICK:
		Print_DrawTextAt(0, 7, "DRIVER: Gun-Stick");
		break;
	case LGUN_DRIVER_PHENIX:
		Print_DrawTextAt(0, 7, "DRIVER: Phenix");
		break;
	}
	Print_DrawTextAt(1, 8, "Triggers");
	Print_DrawTextAt(1, 9, "Light");

	Print_DrawTextAt(0, 11, "LOG:");

	// Footer
	Print_DrawLineH(0, 22, 32);
	Print_DrawTextAt(0, 23, "D:Driver P:Port M:Move H:Hide");
}

//-----------------------------------------------------------------------------
// Interrupt handler
void VDP_InterruptHandler()
{
	g_VBlank = TRUE;
	g_Frame++;
}

//-----------------------------------------------------------------------------
// Wait for v-blank signal
void WaitVBlank()
{
	while (!g_VBlank) {}
	g_VBlank = FALSE;
}

//-----------------------------------------------------------------------------
//
void InitInfo()
{
	VDP_EnableDisplay(TRUE);
	// VDP_SetColor(COLOR_HIDE);
	// VDP_RegWrite(2,(u8)(LAYOUT_HIDE_ADDR >> 10));
	// VDP_SetSpriteSM1(0, g_SpritePosX, g_SpritePosY, 48, COLOR_WHITE);
	// VDP_HideSpriteFrom(1);
	// // g_DetectFrame = DETECT_FRAME;
	// u8 y = 12 + LOG_COUNT_MAX - g_LogCount;
	// Print_DrawCharAt(13, y, 'W');



	VDP_SetColor(g_BGColorTab[g_ColorNum]);
	VDP_RegWrite(2, g_Hide ? (u8)(LAYOUT_HIDE_ADDR >> 10) : (u8)(LAYOUT_SHOW_ADDR >> 10));
	UpdateSprite();
}

//-----------------------------------------------------------------------------
//
void UpdateInfo()
{
	// Sign of life
	Print_DrawCharAt(31, 0, g_ChrAnim[g_FrameCount++ & 0x03]);

	// Handle input
	if (g_KeyWait == 0)
	{
		if (Keyboard_IsKeyPressed(KEY_D))
		{
			g_Driver++;
			if (g_Driver == LGUN_DRIVER_MAX)
				g_Driver = LGUN_DRIVER_ASCII;
			DiplayPage();
			g_KeyWait = INPUT_WAIT;
		}
		if (Keyboard_IsKeyPressed(KEY_P))
		{
			g_Port = 1 - g_Port; 
			DiplayPage();
			g_KeyWait = INPUT_WAIT;
		}
		if (Keyboard_IsKeyPressed(KEY_M))
		{
			g_Move = !g_Move;
			g_KeyWait = INPUT_WAIT;
		}
		if (Keyboard_IsKeyPressed(KEY_H))
		{
			TOGGLE(g_Hide);
			VDP_RegWrite(2, g_Hide ? (u8)(LAYOUT_HIDE_ADDR >> 10) : (u8)(LAYOUT_SHOW_ADDR >> 10));
			g_KeyWait = INPUT_WAIT;
		}
		if (Keyboard_IsKeyPressed(KEY_C))
		{
			if (g_ColorNum >= numberof(g_BGColorTab))
				g_ColorNum = 0;
			VDP_SetColor(g_BGColorTab[g_ColorNum++]);
			g_KeyWait = INPUT_WAIT;
		}
		if (Keyboard_IsKeyPressed(KEY_T))
		{
			TOGGLE(g_ColorTest);
			if (g_ColorTest)
			{
				VDP_SetSpriteSM1(2, 125, 16,      48, COLOR_WHITE);
				VDP_SetSpriteSM1(3, 157, 16,      48, COLOR_WHITE);
				VDP_SetSpriteSM1(4, 125, 16 + 32, 48, COLOR_WHITE);
				VDP_SetSpriteSM1(5, 157, 16 + 32, 48, COLOR_WHITE);
				VDP_SetSpriteSM1(6, 189, 16,      48, COLOR_BLACK);
				VDP_SetSpriteSM1(7, 221, 16,      48, COLOR_BLACK);
				VDP_SetSpriteSM1(8, 189, 16 + 32, 48, COLOR_BLACK);
				VDP_SetSpriteSM1(9, 221, 16 + 32, 48, COLOR_BLACK);
			}
			else
			{
				VDP_HideSpriteFrom(2);
			}
			g_KeyWait = INPUT_WAIT;
		}
	}	
	else if (g_KeyWait > 0)
	{
		g_KeyWait--;
	}

	// Update sprite
	if (g_Move && !g_HitCounter)
		UpdatePosition();
	UpdateSprite();

	// if (g_Trig && !g_TrigPrev)
	// {
	// 	g_LogCount = LOG_COUNT_MAX;
	// 	FSM_SetState(&g_StateClear);
	// }
}

//-----------------------------------------------------------------------------
//
void InitClear()
{
	VDP_SetColor(COLOR_HIDE);
	VDP_EnableDisplay(FALSE);
	// g_ClearFrame = CLEAR_FRAME;
	u8 y = 12 + LOG_COUNT_MAX - g_LogCount;
	Print_DrawCharAt(13, y, 'B');
}

//-----------------------------------------------------------------------------
//
void UpdateClear()
{
	// g_ClearFrame--;
	// if (!g_ClearFrame)
	// 	FSM_SetState(&g_StateDetect);
}

//-----------------------------------------------------------------------------
//
void InitDetect()
{
	VDP_EnableDisplay(TRUE);
	VDP_SetColor(COLOR_HIDE);
	VDP_RegWrite(2,(u8)(LAYOUT_HIDE_ADDR >> 10));
	VDP_SetSpriteSM1(0, g_SpritePosX, g_SpritePosY, 48, COLOR_WHITE);
	VDP_HideSpriteFrom(1);
	// g_DetectFrame = DETECT_FRAME;
	u8 y = 12 + LOG_COUNT_MAX - g_LogCount;
	Print_DrawCharAt(13, y, 'W');
}

//-----------------------------------------------------------------------------
//
void UpdateDetect()
{
	// g_DetectFrame--;
	// if (!g_DetectFrame)
	// 	FSM_SetState(&g_StateInfo);
}

//-----------------------------------------------------------------------------
//
void LGunEvent(u8 event)
{
	switch (event)
	{
	// Program must display a black screen
	case LGUN_EVENT_REF1:
		g_LogCount = LOG_COUNT_MAX;
	case LGUN_EVENT_REF2:
		FSM_SetState(&g_StateClear);
		break;
	
	// Program must display a color square where target is located
	case LGUN_EVENT_DETECT:
		FSM_SetState(&g_StateDetect);
		break;
	
	// Program can return to default display
	case LGUN_EVENT_GAME:
		FSM_SetState(&g_StateInfo);
		break;
	
	// Hit occurs
	case LGUN_EVENT_HIT:
		Print_DrawCharAt(31, 23, 'H');
		g_HitCounter = 15;
		break;
	}
}

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	// Initialize screen
	VDP_SetMode(VDP_MODE_SCREEN1); // Initialize screen mode 1
	VDP_SetColor(g_BGColorTab[g_ColorNum]);
	VDP_SetLayoutTable(LAYOUT_SHOW_ADDR);
	VDP_ClearVRAM();

	// Initialize font
	Print_SetTextFont(g_Font_MGL_Sample8, 1);
	Print_SetColor(COLOR_WHITE, 0);
	
	DiplayPage();

	// Initialize sprite
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16 + VDP_SPRITE_SCALE_2);
	u8 chrSprt = 0;
	for (u8 i = 0; i < 6; i++)
	{
		VDP_LoadSpritePattern(g_DataSprt16 + (i * 2 +  0) * 8, chrSprt++, 1);
		VDP_LoadSpritePattern(g_DataSprt16 + (i * 2 + 12) * 8, chrSprt++, 1);
		VDP_LoadSpritePattern(g_DataSprt16 + (i * 2 +  1) * 8, chrSprt++, 1);
		VDP_LoadSpritePattern(g_DataSprt16 + (i * 2 + 13) * 8, chrSprt++, 1);	
	}
	for (u8 i = 0; i < 6; i++)
	{
		VDP_LoadSpritePattern(g_DataSprt16 + (i * 2 + 24) * 8, chrSprt++, 1);
		VDP_LoadSpritePattern(g_DataSprt16 + (i * 2 + 36) * 8, chrSprt++, 1);
		VDP_LoadSpritePattern(g_DataSprt16 + (i * 2 + 25) * 8, chrSprt++, 1);
		VDP_LoadSpritePattern(g_DataSprt16 + (i * 2 + 37) * 8, chrSprt++, 1);
	}
	loop (i, 8 * 4)	
		g_SpritData[i] = 0xFF;
	VDP_LoadSpritePattern(g_SpritData, 48, 4);
	UpdatePosition();
	UpdateSprite();
	FSM_SetState(&g_StateInfo);
	LightGun_Phenix_Init(LGunEvent, JOY_PORT_1);

	// Main loop
	while (1)
	{
		// V-synch
		WaitVBlank();

		LightGun_Phenix_Update();
		UpdateLightgun();

		FSM_Update();
	}
}
