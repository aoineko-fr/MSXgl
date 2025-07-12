// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Hello world sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "device/lightgun.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x02\x03\x04\x05"

#define COLUMN_PORT1		15
#define COLUMN_PORT2		28

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Font
#include "font/font_mgl_sample8.h"

// Animation characters
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

// Sinus table (64 samples)
const unsigned char g_Sinus64[] = {
	0x00, 0x02, 0x03, 0x05, 0x06, 0x08, 0x09, 0x0A, 
	0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x0F, 0x10, 0x10, 
	0x10, 0x10, 0x10, 0x0F, 0x0F, 0x0E, 0x0D, 0x0C, 
	0x0B, 0x0A, 0x09, 0x08, 0x06, 0x05, 0x03, 0x02, 
	0x00, 0xFE, 0xFD, 0xFB, 0xFA, 0xF8, 0xF7, 0xF6, 
	0xF5, 0xF4, 0xF3, 0xF2, 0xF1, 0xF1, 0xF0, 0xF0, 
	0xF0, 0xF0, 0xF0, 0xF1, 0xF1, 0xF2, 0xF3, 0xF4, 
	0xF5, 0xF6, 0xF7, 0xF8, 0xFA, 0xFB, 0xFD, 0xFE, 
};

//=============================================================================
// MEMORY DATA
//=============================================================================

u8 g_SpritData[8 * 4];

u8 g_SpritePosX;
u8 g_SpritePosY;
u8 g_SpriteColor = COLOR_WHITE;

u16 g_FrameCount;
u16 g_MoveCount;
bool g_Move = FALSE;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Update sprite position
void UpdatePosition()
{
	g_SpritePosX = g_MoveCount++ / 2;
	g_SpritePosY = 100 + 2 * g_Sinus64[(g_MoveCount / 2) & 0x3F];
}

//-----------------------------------------------------------------------------
// Update sprite attributes
void UpdateSprite()
{
	VDP_SetSpriteSM1(0, g_SpritePosX,       g_SpritePosY,      0, g_SpriteColor);
	VDP_SetSpriteSM1(2, g_SpritePosX,       g_SpritePosY + 16, 0, g_SpriteColor);
	VDP_SetSpriteSM1(1, g_SpritePosX + 16,  g_SpritePosY,      0, g_SpriteColor);
	VDP_SetSpriteSM1(3, g_SpritePosX + 16,  g_SpritePosY + 16, 0, g_SpriteColor);
}

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	// Initialize screen
	VDP_SetMode(VDP_MODE_SCREEN1); // Initialize screen mode 1
	VDP_ClearVRAM();
	VDP_SetColor(COLOR_BLACK);

	// Initialize font
	Print_SetTextFont(g_Font_MGL_Sample8, 1);
	Print_SetColor(COLOR_WHITE, COLOR_BLACK);
	
	// Draw static text
	Print_DrawText(MSX_GL " LIGHT GUN SAMPLE");
	Print_DrawLineH(0, 1, 32);

	Print_DrawTextAt(COLUMN_PORT1-3, 2, "Port 1");
	Print_DrawTextAt(COLUMN_PORT1-4, 4, "--BA\x8A\x8B\x89\x88");
	Print_DrawTextAt(COLUMN_PORT2-3, 2, "Port 2");
	Print_DrawTextAt(COLUMN_PORT2-4, 4, "--BA\x8A\x8B\x89\x88");

	Print_DrawTextAt(0,  7, "ASCII Terminator");
	Print_DrawTextAt(1,  9, "Trigger");
	Print_DrawTextAt(1, 10, "Light");

	Print_DrawTextAt(0, 12, "Gun-Stick");
	Print_DrawTextAt(1, 14, "Trigger");
	Print_DrawTextAt(1, 15, "Light");

	Print_DrawTextAt(0, 17, "Phenix Light Gun");
	Print_DrawTextAt(1, 19, "Triggers");
	Print_DrawTextAt(1, 20, "Light");

	Print_DrawLineH(0, 22, 32);
	Print_DrawTextAt(0, 23, "M:Move");

	g_FrameCount = 0;
	g_MoveCount = 160 * 2;

	// Initialize sprite
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16);
	loop(i, 4 * 8)	
		g_SpritData[i] = 0xFF;
	VDP_LoadSpritePattern(g_SpritData, 0, 32);
	UpdatePosition();
	UpdateSprite();

	u8 keyWait = 0;

	// Main loop
	while (!Keyboard_IsKeyPressed(KEY_ESC))
	{
		Print_SetPosition(31, 0);
		Print_DrawChar(g_ChrAnim[g_FrameCount++ & 0x03]);

		if (keyWait == 0 && Keyboard_IsKeyPressed(KEY_M))
		{
			g_Move = !g_Move;
			keyWait = 20;
		}
		else if (keyWait > 0)
		{
			keyWait--;
		}


		if (g_Move)
		{
			UpdatePosition();
			UpdateSprite();
		}

		loop (i, 2)
		{
			u8 x = (i == 0) ? COLUMN_PORT1 : COLUMN_PORT2;
			
			u8 joy = LightGun_Read((i == 0) ? JOY_PORT_1 : JOY_PORT_2);
			Print_DrawBin8At(x-4, 5, joy);
			Print_DrawHex8At(x-7, 5, joy);
			
			Print_DrawCharAt(x,  9, LightGun_ASCII_GetTrigger(joy) ? 0x0C : 0x0B);
			Print_DrawCharAt(x, 10, LightGun_ASCII_GetLight(joy) ? 0x0C : 0x0B);

			Print_DrawCharAt(x, 14, LightGun_GunStick_GetTrigger(joy) ? 0x0C : 0x0B);
			Print_DrawCharAt(x, 15, LightGun_GunStick_GetLight(joy) ? 0x0C : 0x0B);

			Print_DrawCharAt(x + 0, 19, LightGun_Phenix_GetTriggerA(joy) ? 0x0C : 0x0B);
			Print_DrawCharAt(x + 1, 19, LightGun_Phenix_GetTriggerB(joy) ? 0x0C : 0x0B);
			Print_DrawHex8At(x, 20, LightGun_Phenix_GetLight(joy));
		}
	}
}
