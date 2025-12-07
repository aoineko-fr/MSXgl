// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  ayFX replayer sample
//─────────────────────────────────────────────────────────────────────────────
// This sample program showcase the use of the ayFX module to play sound effect.
// The replaye support both sound banks and individual sound file.
// See ayFX editor for more detail: https://shiru.untergrund.net/software.shtml
// Note: This program showcase the use of ayFX replayer with PT3 msic replayer
//       but ayFX can be use as stand alone. 
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"
#include "pt3/pt3_player.h"
#include "ayfx/ayfx_player.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x01\x02\x03\x04\x05\x06"

// Song data structure
struct SFXData
{
	const u8*	Raw;  // Pointer to sound data
	const c8*	Name; // Name of the sound file
};

// Player font character
#define CHR_PLAY		112
#define CHR_PAUSE		113
#define CHR_STOP		114
#define CHR_REC			115
#define CHR_NEXT		116
#define CHR_PREV		117
#define CHR_FORW		118
#define CHR_BACK		119
#define CHR_EJECT		120
#define CHR_LOOP		121
#define CHR_SHUFF		122
#define CHR_PLUS		123
#define CHR_MINUS		124
#define CHR_SOUND		126
#define CHR_MUTE		127

// Player display parameters
#define PT3_Y			(24)
#define PT3_PLY_X		(0)
#define PT3_PLY_Y		(PT3_Y + 36)
#define AYFX_Y			(88)
#define PSG_Y			(150)
#define VU_X			(64+10)
#define VU_MAX			7
#define VU_H			4

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts data
#include "font/font_mgl_sample6.h"
#include "font/font_mgl_symbol1.h"

// Note table for PT3 replayer
#include "pt3/pt3_notetable2.h"

// Music
#include "content/pt3/Beg!nsum.h"

// SFX bank (sound pack)
#include "content/ayfx/ayfx_bank.h"

// SFX file (individual sound)
#include "content/ayfx/ayfx_fx017.h"
#include "content/ayfx/ayfx_fx019.h"

// ayFX files list
const struct SFXData g_SFXFiles[] =
{
	{ g_ayfx_fx017, "ayfx_fx017.afx" },
	{ g_ayfx_fx019, "ayfx_fx019.afx" },
};

// Character animation (sign-of-life)
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

//=============================================================================
// MEMORY DATA
//=============================================================================

// Vertical synchronization flag
bool g_VBlank = FALSE;

// Frame counter
u8   g_Frame = 0;

// Is music looping?
bool g_Loop = FALSE;

// PSG mute flag for the 3 chanels 
bool g_Mute[3] = { FALSE, FALSE, FALSE };

// Index of the current sound in he bank
u8   g_BankFXNum = 0;

// Index of the individual sound data
u8   g_FileFXNum = 0;

//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// H_TIMI interrupt hook
void VBlankHook()
{
	g_VBlank = TRUE;
	g_Frame++;

	PT3_Decode(); // Update data to write to the PSG regiters from the music
	ayFX_Update(); // Overwrite PSG registers data for the SFX
	PT3_UpdatePSG(); // Update PSG registers
}

//-----------------------------------------------------------------------------
// Wait for V-Blank period
void WaitVBlank()
{
	while (!g_VBlank) {}
	g_VBlank = FALSE;
}

//-----------------------------------------------------------------------------
// Draw the VU meter
void DrawVUMeter()
{
	g_VDP_Command.NY = VU_H;
	g_VDP_Command.ARG = 0; 
	g_VDP_Command.CMD = VDP_CMD_HMMV;

	// Channel A
	g_VDP_Command.DY = PSG_Y + (8*2) + ((8-VU_H)/2); 
	u8 volA = PT3_GetVolume(PSG_CHANNEL_A) >> 1;
	if (volA > VU_MAX)
		volA = VU_MAX;
	if (volA > 0)
	{
		g_VDP_Command.DX = VU_X;
		g_VDP_Command.NX = volA * 8;
		g_VDP_Command.CLR = 0x22;
		VDP_CommandSetupR36();
	}
	if (volA < VU_MAX)
	{
		g_VDP_Command.DX = VU_X + volA * 8;
		g_VDP_Command.NX = (VU_MAX - volA) * 8;
		g_VDP_Command.CLR = 0x11;
		VDP_CommandSetupR36();
	}

	// Channel B
	g_VDP_Command.DY = PSG_Y + (8*3) + ((8-VU_H)/2); 
	u8 volB = PT3_GetVolume(PSG_CHANNEL_B) >> 1;
	if (volB > VU_MAX)
		volB = VU_MAX;
	if (volB > 0)
	{
		g_VDP_Command.DX = VU_X;
		g_VDP_Command.NX = volB * 8;
		g_VDP_Command.CLR = 0x22;
		VDP_CommandSetupR36();
	}
	if (volB < VU_MAX)
	{
		g_VDP_Command.DX = VU_X + volB * 8;
		g_VDP_Command.NX = (VU_MAX - volB) * 8;
		g_VDP_Command.CLR = 0x11;
		VDP_CommandSetupR36();
	}

	// Channel C
	g_VDP_Command.DY = PSG_Y + (8*4) + ((8-VU_H)/2); 
	u8 volC = PT3_GetVolume(PSG_CHANNEL_C)>> 1;
	if (volC > VU_MAX)
		volC = VU_MAX;
	if (volC > 0)
	{
		g_VDP_Command.DX = VU_X;
		g_VDP_Command.NX = volC * 8;
		g_VDP_Command.CLR = 0x22;
		VDP_CommandSetupR36();
	}
	if (volC < VU_MAX)
	{
		g_VDP_Command.DX = VU_X + volC * 8;
		g_VDP_Command.NX = (VU_MAX - volC) * 8;
		g_VDP_Command.CLR = 0x11;
		VDP_CommandSetupR36();
	}
}

//-----------------------------------------------------------------------------
// Play/resume the current music
void PlayMusic()
{
	// Resume music playback
	PT3_Resume();

	// Update screen information
	Print_SetFont(g_Font_MGL_Symbol1);
	Print_SetColor(0x9, 0);
	Print_SetPosition(PT3_PLY_X+1, PT3_PLY_Y+1);
	Print_DrawChar(CHR_PLAY);
	Print_SetColor(0xF, 0);
	Print_SetPosition(PT3_PLY_X+11, PT3_PLY_Y+1);
	Print_DrawChar(CHR_PAUSE);
	Print_SetPosition(PT3_PLY_X+21, PT3_PLY_Y+1);
	Print_DrawChar(CHR_STOP);
}

//-----------------------------------------------------------------------------
// Pause the current music
void PauseMusic()
{
	// Pause current music playback
	PT3_Pause();

	// Update screen information
	Print_SetFont(g_Font_MGL_Symbol1);
	Print_SetColor(0x9, 0);
	Print_SetPosition(PT3_PLY_X+11, PT3_PLY_Y+1);
	Print_DrawChar(CHR_PAUSE);
	Print_SetColor(0xF, 0);
	Print_SetPosition(PT3_PLY_X+1, PT3_PLY_Y+1);
	Print_DrawChar(CHR_PLAY);
	Print_SetPosition(PT3_PLY_X+21, PT3_PLY_Y+1);
	Print_DrawChar(CHR_STOP);
}

//-----------------------------------------------------------------------------
// Stop the current music
void StopMusic()
{
	// Pause he current music playback and rewind the music
	PT3_Pause();
	PT3_InitSong(g_Beg_nsum);

	// Update screen information
	Print_SetFont(g_Font_MGL_Symbol1);
	Print_SetColor(0x9, 0);
	Print_SetPosition(PT3_PLY_X+11, PT3_PLY_Y+1);
	Print_DrawChar(CHR_PAUSE);
	Print_SetColor(0xF, 0);
	Print_SetPosition(PT3_PLY_X+1, PT3_PLY_Y+1);
	Print_DrawChar(CHR_PLAY);
	Print_SetPosition(PT3_PLY_X+21, PT3_PLY_Y+1);
	Print_DrawChar(CHR_STOP);
}

//-----------------------------------------------------------------------------
// Set the current music loop flag
void LoopMusic(bool enable)
{
	// Tell if curent music must loop or not
	PT3_SetLoop(enable);
	g_Loop = enable;

	// Update screen information
	Print_SetFont(g_Font_MGL_Symbol1);
	Print_SetColor(enable ? 0x9 : 0xF, 0);
	Print_SetPosition(PT3_PLY_X+51, PT3_PLY_Y+1);
	Print_DrawChar(CHR_LOOP);
	Print_SetColor(0xF, 0);
}

//-----------------------------------------------------------------------------
// Mute one of the channels
void MuteMusic(u8 chan, bool bMute)
{
	// Mute the given channel
	PT3_Mute(chan, bMute);
	g_Mute[chan] = bMute;

	// Update screen information
	Print_SetFont(g_Font_MGL_Symbol1);
	Print_SetColor(bMute ? 0x9 : 0xF, 0);
	Print_SetPosition(64, PSG_Y + 8*2 + 8*chan);
	Print_DrawChar(bMute ? CHR_MUTE : CHR_SOUND);
	Print_SetColor(0xF, 0);
}

//-----------------------------------------------------------------------------
// Play an individual sound data
void PlayFileSFX(u8 id)
{
	// Play a SFX file
	ayFX_Play(g_SFXFiles[id].Raw);

	// Update screen information
	Print_SetFont(g_Font_MGL_Sample6);
	VDP_CommandHMMV(64, AYFX_Y+8*4, 16*6, 8, 0x44);
	Print_SetPosition(64, AYFX_Y+8*4);
	Print_DrawText(g_SFXFiles[id].Name);
}

//-----------------------------------------------------------------------------
// Play a sound froma bank
void PlayBankSFX(u8 id, u8 prio)
{
	// Play the given SFX from the current bank
	ayFX_PlayBank(id, prio);

	// Update screen information
	Print_SetFont(g_Font_MGL_Sample6);
	VDP_CommandHMMV(64, AYFX_Y+8*3, 7*6, 8, 0x44);
	Print_SetPosition(64, AYFX_Y+8*3);
	Print_DrawInt(id + 1);
	Print_DrawChar('/');
	Print_DrawInt(ayFX_GetBankNumber());
}

//-----------------------------------------------------------------------------
// Select the channel where SFX are replayed
void ChannelSFX(u8 chan)
{
	// Mute current sound effect and set the new cannel to replay SFX
	ayFX_Mute();
	ayFX_SetChannel(chan);

	// Update screen information
	Print_SetFont(g_Font_MGL_Sample6);
	VDP_CommandHMMV(64, AYFX_Y+8*2, 6, 8, 0x44);
	Print_SetPosition(64, AYFX_Y+8*2);
	Print_DrawChar('A' + chan);
}

//-----------------------------------------------------------------------------
// Print command reminder
void PrintHelp(u8 y, const c8* str)
{
	u8 len = String_Length(str);
	Print_SetFont(g_Font_MGL_Sample6);
	Print_SetColor(0x5, 0);
	Print_SetPosition(255 - 6 * len, y);
	Print_DrawText(str);
	Print_SetColor(0xF, 0);
}

//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	// Initialize screen mode 5 (MSX2)
	VDP_SetMode(VDP_MODE_SCREEN5);
	VDP_EnableSprite(FALSE);
	VDP_SetColor(0x4);
	VDP_CommandHMMV(0, 0, 256, 212, 0x44);

	// Initialize fonts
	Print_SetBitmapFont(NULL);
	Print_SetFont(g_Font_MGL_Sample6);
	Print_SetMode(PRINT_MODE_BITMAP_TRANS);
	Print_SetColor(0xF, 0);
	Print_SetPosition(4, 4);
	Print_DrawText(MSX_GL "  ayFX SAMPLE");
	Draw_Box(0, 0, 255, 14, 0x0F, 0);

	// Iitialize PT3 music
	PT3_Init();
	PT3_SetNoteTable(PT3_NT2);
	PT3_SetLoop(TRUE);
	PT3_InitSong(g_Beg_nsum);

	// INIT ayFX

	ayFX_InitBank(g_ayfx_bank);
	ayFX_SetChannel(PSG_CHANNEL_A);
	ayFX_SetMode(AYFX_MODE_FIXED);
	
	//-----------------------------------------------------------------------------
	// PT3

	Print_SetPosition(0, PT3_Y);
	Print_DrawText(" PT3\n\n");
	Draw_LineH(0, 255, PT3_Y+10, 0x5, 0);
	Print_DrawText("Music\n");
	Print_DrawText("Pattern\n");		

	Print_SetPosition(64, PT3_Y+8*2);
	Print_DrawText("Beg!nsum");

	// PT3 Player
	VDP_CommandHMMV(PT3_PLY_X, PT3_PLY_Y, 10*6+1, 8+3, 0x55);
	Print_SetFont(g_Font_MGL_Symbol1);
	Print_SetPosition(PT3_PLY_X+31, PT3_PLY_Y+1);
	Print_DrawChar(CHR_PREV);	// Previous
	Print_SetPosition(PT3_PLY_X+41, PT3_PLY_Y+1);
	Print_DrawChar(CHR_NEXT);	// Next
	
	LoopMusic(TRUE);
	PauseMusic();

	// Help
	PrintHelp(PT3_Y+8*2, "Space:Play/Pause");
	PrintHelp(PT3_Y+8*3, "Del:Loop");

	//-----------------------------------------------------------------------------
	// ayFX

	Print_SetFont(g_Font_MGL_Sample6);

	Print_SetPosition(0, AYFX_Y);
	Print_DrawText(" ayFX\n\n");
	Draw_LineH(0, 255, AYFX_Y+10, 0x5, 0);
	Print_DrawText("Channel\n");
	Print_DrawText("FX Bank\n");
	Print_DrawText("FX File\n");

	// Help
	PrintHelp(AYFX_Y+8*2, "^v:Channel");
	PrintHelp(AYFX_Y+8*3, "<>:Bank SFX");
	PrintHelp(AYFX_Y+8*4, "Home:File SFX");

	ChannelSFX(PSG_CHANNEL_A);
	PlayBankSFX(0, 0);


	//-----------------------------------------------------------------------------
	// PSG

	Print_SetPosition(0, PSG_Y);
	Print_DrawText(" PSG\n\n");		
	Draw_LineH(0, 255, (u8)(PSG_Y + 10), 0x5, 0);
	Print_DrawText("Chan. A\n");		
	Print_DrawText("Chan. B\n");
	Print_DrawText("Chan. C\n");

	// Output
	MuteMusic(PSG_CHANNEL_A, FALSE);
	MuteMusic(PSG_CHANNEL_B, FALSE);
	MuteMusic(PSG_CHANNEL_C, FALSE);

	// Help
	PrintHelp((u8)(PSG_Y+8*2), "123:Mute");

	//-----------------------------------------------------------------------------

	Bios_SetHookCallback(H_TIMI, VBlankHook);

	u8 fxChan = ayFX_GetChannel();
	u8 prevRow8 = 0xFF;
	u8 prevRow0 = 0xFF;
	u8 prevPattern = 0xFF;
	while (1)
	{
		WaitVBlank();

		// Display Vumeter
		DrawVUMeter();

		// DEFAULT FONT ----------------------------------------------------------

		Print_SetFont(g_Font_MGL_Sample6);

		VDP_CommandHMMV(255-8, 4, 6, 8, 0x44);
		VDP_CommandWait();
		Print_SetPosition(255-8, 4);
		u8 chr = g_Frame & 0x03;
		Print_DrawChar(g_ChrAnim[chr]);
		
		// Display Current Pattern
		u8 pattern = PT3_GetPattern();
		if (pattern != prevPattern)
		{
			VDP_CommandHMMV(64, PT3_Y+8*3, 3*6, 8, 0x44);
			Print_SetPosition(64, PT3_Y+8*3);
			Print_DrawInt(pattern);
		}
		prevPattern = pattern;


		// Read keyboard matrix row #8
		u8 row8 = g_NEWKEY[KEY_ROW(KEY_SPACE)];
		if ((IS_KEY_PRESSED(row8, KEY_RIGHT)) && (IS_KEY_RELEASED(prevRow8, KEY_RIGHT))) // Next SFX
		{
			g_BankFXNum++;
			if (g_BankFXNum >= ayFX_GetBankNumber())
				g_BankFXNum = 0;
			PlayBankSFX(g_BankFXNum, 0);
		}
		else if ((IS_KEY_PRESSED(row8, KEY_LEFT)) && (IS_KEY_RELEASED(prevRow8, KEY_LEFT))) // Previous SFX
		{
			if (g_BankFXNum > 0)
				g_BankFXNum--;
			else
				g_BankFXNum = ayFX_GetBankNumber() - 1;
			PlayBankSFX(g_BankFXNum, 15);
		}

		if ((IS_KEY_PRESSED(row8, KEY_UP)) && (IS_KEY_RELEASED(prevRow8, KEY_UP))) // Next Channel
		{
			fxChan++;
			if (fxChan > 2)
				fxChan = 0;
			ChannelSFX(fxChan);
		}
		else if ((IS_KEY_PRESSED(row8, KEY_DOWN)) && (IS_KEY_RELEASED(prevRow8, KEY_DOWN))) // Previous Channel
		{
			if (fxChan > 0)
				fxChan--;
			else
				fxChan = 2;
			ChannelSFX(fxChan);
		}

		// PLAYER FONT -----------------------------------------------------------
		
		if ((IS_KEY_PRESSED(row8, KEY_SPACE)) && (IS_KEY_RELEASED(prevRow8, KEY_SPACE))) // Pause / Resume
		{
			if (PT3_IsPlaying())
				PauseMusic();
			else
				PlayMusic();
		}
		else if ((IS_KEY_PRESSED(row8, KEY_DEL)) && (IS_KEY_RELEASED(prevRow8, KEY_DEL))) // Loop
		{
			LoopMusic(1 - g_Loop);
		}
		else if ((IS_KEY_PRESSED(row8, KEY_HOME)) && (IS_KEY_RELEASED(prevRow8, KEY_HOME))) // 
		{
			g_FileFXNum++;
			g_FileFXNum %= numberof(g_SFXFiles);
			PlayFileSFX(g_FileFXNum);
		}
		// else if ((IS_KEY_PRESSED(row8, KEY_INS)) && (IS_KEY_RELEASED(prevRow8, KEY_INS))) // 
		// {
		// }
		
		prevRow8 = row8;

		// Read keyboard matrix row #0
		u8 row0 = g_NEWKEY[KEY_ROW(KEY_0)];
		if ((IS_KEY_PRESSED(row0, KEY_1)) && (IS_KEY_RELEASED(prevRow0, KEY_1))) // Mute channel A
		{
			MuteMusic(0, 1 - g_Mute[0]);
		}
		else if ((IS_KEY_PRESSED(row0, KEY_2)) && (IS_KEY_RELEASED(prevRow0, KEY_2))) // Mute channel B
		{
			MuteMusic(1, 1 - g_Mute[1]);
		}
		else if ((IS_KEY_PRESSED(row0, KEY_3)) && (IS_KEY_RELEASED(prevRow0, KEY_3))) // Mute channel C
		{
			MuteMusic(2, 1 - g_Mute[2]);
		}
		prevRow0 = row0;


		// PT3_Decode(); 
	}
}