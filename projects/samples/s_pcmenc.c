// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  PCM-Encoder sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "pcm/pcmenc.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x01\x02\x03\x04\x05\x06"

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font/font_mgl_sample6.h"

// PCM-Encoder data
// #include "content/pcm/pcm_8k_hello.h"
// #include "content/pcm/pcm_11k_hello.h"
// #include "content/pcm/pcm_22k_hello.h"
#include "content/pcm/pcm_44k_hello.h"

// Animation characters
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };


//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	VDP_SetMode(VDP_MODE_SCREEN0); // Initialize screen mode 0 (text)
	VDP_ClearVRAM();

	// Header
	Print_SetTextFont(g_Font_MGL_Sample6, 1); // Initialize font
	Print_DrawText(MSX_GL "  PCM-Encoder Sample");
	Print_DrawLineH(0, 1, 40);

	// Footer
	Print_DrawLineH(0, 22, 40);
	Print_SetPosition(0, 23);
	Print_DrawText("\x81\x80:Sound | Play 1:8K 2:11K 3:22K 4:44K");

	u8 count = 0;
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		Print_SetPosition(39, 0);
		u8 chr = count++ & 0x03;
		Print_DrawChar(g_ChrAnim[chr]);

		if(Keyboard_IsKeyPressed(KEY_1))
			PCM_Play_8K((u16)g_pcm_44K_hello_bin_44K);
		else if(Keyboard_IsKeyPressed(KEY_2))
			PCM_Play_11K((u16)g_pcm_44K_hello_bin_44K);
		else if(Keyboard_IsKeyPressed(KEY_3))
			PCM_Play_22K((u16)g_pcm_44K_hello_bin_44K);
		else if(Keyboard_IsKeyPressed(KEY_4))
			PCM_Play_44K((u16)g_pcm_44K_hello_bin_44K);
	}
}