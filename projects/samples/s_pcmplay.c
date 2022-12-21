// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Hello world sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "pcm/pcmplay.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x01\x02\x03\x04\x05\x06"

//=============================================================================
// READ-ONLY DATA
//=============================================================================

#if (PCMPLAY_FREQ == PCMPLAY_8K)
	#include	"content/pcm/pp_hello_8k.h"
	#define		PCM_ADDR g_hello_8k
	#define		PCM_SIZE sizeof(g_hello_8k)
	#define		PCM_FREQ "8K"
#else // (PCMPLAY_FREQ == PCMPLAY_11K)
	#include	"content/pcm/pp_hello_11k.h"
	#define		PCM_ADDR g_hello_11k
	#define		PCM_SIZE sizeof(g_hello_11k)
	#define		PCM_FREQ "11K"
#endif

// Fonts
#include "font/font_mgl_sample6.h"

// Character animation data
const c8 g_ChrAnim[] = { '|', '\\', '-', '/' };

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	// Initialize screen
	VDP_SetMode(VDP_MODE_SCREEN0);
	VDP_SetColor(0);
	VDP_ClearVRAM();

	// Initialize font
	Print_SetTextFont(g_Font_MGL_Sample6, 1);
	Print_SetColor(COLOR_WHITE, COLOR_BLACK);

	Print_SetPosition(0, 0);
	Print_DrawText(MSX_GL " PCM-Play " PCM_FREQ " Sample");
	Print_DrawLineH(0, 1, 40);

	Print_DrawLineH(0, 22, 40);
	Print_SetPosition(0, 23);
	Print_DrawText("Space:Play");

	Bios_SetKeyClick(FALSE);

	u8 count = 0;
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		Halt();

		Print_SetPosition(39, 0);
		Print_DrawChar(g_ChrAnim[count++ & 0x03]);

		if(Keyboard_IsKeyPressed(KEY_SPACE))
			PCM_Play(PCM_ADDR, PCM_SIZE);
	}
}

