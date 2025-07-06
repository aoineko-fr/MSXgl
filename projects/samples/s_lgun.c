// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Hello world sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "input.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x02\x03\x04\x05"

#define LIGHTGUN_ASCII_TRIGGER		JOY_INPUT_TRIGGER_B
#define LIGHTGUN_ASCII_LIGHT		JOY_INPUT_TRIGGER_A
#define LIGHTGUN_GUNSTICK_TRIGGER	JOY_INPUT_TRIGGER_A		
#define LIGHTGUN_GUNSTICK_LIGHT		JOY_INPUT_DIR_DOWN

#define COLUMN_PORT1		15
#define COLUMN_PORT2		28

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Font
#include "font/font_mgl_sample8.h"

// Animation characters
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

//=============================================================================
// MEMORY DATA
//=============================================================================

u8 g_SpritData[8 * 4];

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Detects the light gun trigger
inline u8 LightGun_Read(u8 port) { return Joystick_Read(port); }

inline bool LightGun_ASCII_GetTrigger(u8 state) { return (state & LIGHTGUN_ASCII_TRIGGER) == 0; }
inline u8   LightGun_ASCII_GetLight  (u8 state) { return (state & LIGHTGUN_ASCII_LIGHT) != 0; }

inline bool LightGun_GunStick_GetTrigger(u8 state) { return (state & LIGHTGUN_GUNSTICK_TRIGGER) == 0; }
inline u8   LightGun_GunStick_GetLight  (u8 state) { return (state & LIGHTGUN_GUNSTICK_LIGHT) == 0; }

inline bool LightGun_Phenix_GetTriggerA(u8 state) { return (state & JOY_INPUT_TRIGGER_A) == 0; }
inline bool LightGun_Phenix_GetTriggerB(u8 state) { return (state & JOY_INPUT_TRIGGER_B) == 0; }
inline u8   LightGun_Phenix_GetLight   (u8 state) { return ~state & 0x07; }

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
	Print_DrawTextAt(1, 19, "Trigger A");
	Print_DrawTextAt(1, 20, "Trigger B");
	Print_DrawTextAt(1, 21, "Light");

	// Initialize sprite
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16);
	loop(i, 4 * 8)	
		g_SpritData[i] = 0xFF;
	VDP_LoadSpritePattern(g_SpritData, 0, 32);
	VDP_SetSpriteSM1(0,  100,  100, 0, COLOR_WHITE);
	// VDP_SetSpriteSM1(1,  60,  60, 0, COLOR_MEDIUM_RED);
	// VDP_SetSpriteSM1(2,  80,  80, 0, COLOR_MEDIUM_GREEN);
	// VDP_SetSpriteSM1(3, 100, 100, 0, COLOR_DARK_BLUE);
	// VDP_SetSpriteSM1(4, 120, 120, 0, COLOR_LIGHT_YELLOW);
	// VDP_SetSpriteSM1(5, 140, 140, 0, COLOR_CYAN);
	// VDP_SetSpriteSM1(6, 160, 160, 0, COLOR_MAGENTA);

	u8 count = 0;

	// Main loop
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		Print_SetPosition(31, 0);
		Print_DrawChar(g_ChrAnim[count++ & 0x03]);

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

			Print_DrawCharAt(x, 19, LightGun_Phenix_GetTriggerA(joy) ? 0x0C : 0x0B);
			Print_DrawCharAt(x, 20, LightGun_Phenix_GetTriggerB(joy) ? 0x0C : 0x0B);
			Print_DrawHex8At(x, 21, LightGun_Phenix_GetLight(joy));
		}
	}
}
