// __________________________
// ██▀█▀██▀▀▀█▀▀█▀█  ▄▄▄ ▄▄  │   ▄▄▄                ▄▄      
// █  ▄ █▄ ▀██▄ ▀▄█ ██   ██  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █▀▀ ▄█  █ █ ▀█▄█ ██▄▄│  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀──────────┘                 ▀▀
//  PT3 replayer sample
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"
#include "pt3/pt3_player.h"

//=============================================================================
// DEFINES
//=============================================================================

// Song data structure
struct SongData
{
	u8*			Raw;
	const c8*	Name;
	u16			Size;
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

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font/font_mgl_std0.h"
#include "font/font_mgl_symbol1.h"

// Note table
#include "pt3/pt3_notetable2.h"

// Music
#include "data/pt3/smario.h"
#include "data/pt3/Beg!nsum.h"
#include "data/pt3/Fret.h"

// Songs data table
const struct SongData g_SongData[] = {
	{ g_smario, 	"smario",		sizeof(g_smario) },
	{ g_Beg_nsum, 	"Beg!nsum",		sizeof(g_Beg_nsum) },
	{ g_Fret,     	"Fret",			sizeof(g_Fret) },
};

// Character animation
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

//=============================================================================
// MEMORY DATA
//=============================================================================

u8   g_CurrentSong = 0;
u8   g_VBlank = 0;
u8   g_Frame = 0;
bool g_Loop = false;
bool g_Mute[3] = { false, false, false };

//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// H_TIMI interrupt hook
void VBlankHook()
{
	g_VBlank = 1;
	g_Frame++;

	PT3_Decode(); 
	PT3_UpdatePSG();
}

//-----------------------------------------------------------------------------
// Wait for V-Blank period
void WaitVBlank()
{
	while(g_VBlank == 0) {}
	g_VBlank = 0;
}

// Set a new song
void SetSong(u8 songId) __FASTCALL
{
	g_CurrentSong = songId;
	
	PT3_InitSong(g_SongData[songId].Raw);

	Print_SetFont(g_Font_MGL_Std0);
	VDP_CommandHMMV(64, 56, 6*8, 8, 0x44);
	Print_SetPosition(64, 56);
	Print_DrawText(g_SongData[songId].Name);
}

//-----------------------------------------------------------------------------
// Draw the VU meter
void DrawVUMeter()
{
	#define VU_X	(64+10)
	#define VU_MAX	7
	#define VU_H	4
	
	g_VDP_Command.NY = VU_H;
	g_VDP_Command.ARG = 0; 
	g_VDP_Command.CMD = VDP_CMD_HMMV;

	// Channel A
	g_VDP_Command.DY = 64 + (8*1) + ((8-VU_H)/2); 
	u8 volA = PT3_GetVolume(PSG_CHANNEL_A) >> 1;
	if(volA > VU_MAX)
		volA = VU_MAX;
	if(volA > 0)
	{
		g_VDP_Command.DX = VU_X;
		g_VDP_Command.NX = volA * 8;
		g_VDP_Command.CLR = 0x22;
		VPD_CommandSetupR36();
	}
	if(volA < VU_MAX)
	{
		g_VDP_Command.DX = VU_X + volA * 8;
		g_VDP_Command.NX = (VU_MAX - volA) * 8;
		g_VDP_Command.CLR = 0x11;
		VPD_CommandSetupR36();
	}

	// Channel B
	g_VDP_Command.DY = 64 + (8*2) + ((8-VU_H)/2); 
	u8 volB = PT3_GetVolume(PSG_CHANNEL_B) >> 1;
	if(volB > VU_MAX)
		volB = VU_MAX;
	if(volB > 0)
	{
		g_VDP_Command.DX = VU_X;
		g_VDP_Command.NX = volB * 8;
		g_VDP_Command.CLR = 0x22;
		VPD_CommandSetupR36();
	}
	if(volB < VU_MAX)
	{
		g_VDP_Command.DX = VU_X + volB * 8;
		g_VDP_Command.NX = (VU_MAX - volB) * 8;
		g_VDP_Command.CLR = 0x11;
		VPD_CommandSetupR36();
	}

	// Channel C
	g_VDP_Command.DY = 64 + (8*3) + ((8-VU_H)/2); 
	u8 volC = PT3_GetVolume(PSG_CHANNEL_C)>> 1;
	if(volC > VU_MAX)
		volC = VU_MAX;
	if(volC > 0)
	{
		g_VDP_Command.DX = VU_X;
		g_VDP_Command.NX = volC * 8;
		g_VDP_Command.CLR = 0x22;
		VPD_CommandSetupR36();
	}
	if(volC < VU_MAX)
	{
		g_VDP_Command.DX = VU_X + volC * 8;
		g_VDP_Command.NX = (VU_MAX - volC) * 8;
		g_VDP_Command.CLR = 0x11;
		VPD_CommandSetupR36();
	}
}

//-----------------------------------------------------------------------------
// Play/resume the current music
void Play()
{
	Print_SetFont(g_Font_MGL_Symbol1);
	Print_SetMode(PRINT_MODE_BITMAP);
	
	Print_SetColor(0x9, 0x5);
	Print_SetPosition(1, 33);
	Print_DrawChar(CHR_PLAY);

	Print_SetColor(0xF, 0x5);
	Print_SetPosition(11, 33);
	Print_DrawChar(CHR_PAUSE);
	Print_SetPosition(21, 33);
	Print_DrawChar(CHR_STOP);
	
	PT3_Resume();
}

//-----------------------------------------------------------------------------
// Pause the current music
void Pause()
{
	Print_SetFont(g_Font_MGL_Symbol1);
	Print_SetMode(PRINT_MODE_BITMAP);

	Print_SetColor(0x9, 0x5);
	Print_SetPosition(11, 33);
	Print_DrawChar(CHR_PAUSE);

	Print_SetColor(0xF, 0x5);
	Print_SetPosition(1, 33);
	Print_DrawChar(CHR_PLAY);
	Print_SetPosition(21, 33);
	Print_DrawChar(CHR_STOP);
	
	PT3_Pause();
}

//-----------------------------------------------------------------------------
// Stop the current music
void Stop()
{
	Print_SetFont(g_Font_MGL_Symbol1);
	Print_SetMode(PRINT_MODE_BITMAP);

	Print_SetColor(0x9, 0x5);
	Print_SetPosition(11, 33);
	Print_DrawChar(CHR_PAUSE);

	Print_SetColor(0xF, 0x5);
	Print_SetPosition(1, 33);
	Print_DrawChar(CHR_PLAY);
	Print_SetPosition(21, 33);
	Print_DrawChar(CHR_STOP);
	
	PT3_Pause();
	PT3_InitSong(g_SongData[g_CurrentSong].Raw);
}

//-----------------------------------------------------------------------------
// Set the current music loop flag
void Loop(bool enable) __FASTCALL
{
	Print_SetFont(g_Font_MGL_Symbol1);
	Print_SetMode(PRINT_MODE_BITMAP);
	Print_SetColor(enable ? 0x9 : 0xF, 0x5);
	Print_SetPosition(51, 33);
	Print_DrawChar(CHR_LOOP);

	g_Loop = enable;
	PT3_SetLoop(enable);
}

//-----------------------------------------------------------------------------
// Mute one of the channels
void Mute(u8 chan, bool bMute)
{
	g_Mute[chan] = bMute;
	Print_SetFont(g_Font_MGL_Symbol1);
	Print_SetMode(PRINT_MODE_BITMAP);
	Print_SetColor(bMute ? 0x9 : 0xF, 0x4);
	Print_SetPosition(64, 64 + 8 + 8*chan);
	Print_DrawChar(bMute ? CHR_MUTE : CHR_SOUND);
	
	PT3_Mute(chan, bMute);
}

//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	// INIT SCREEN

	VDP_SetMode(VDP_MODE_SCREEN5);
	VDP_EnableSprite(false);
	VDP_SetColor(0x4);
	VDP_CommandHMMV(0, 0, 256, 212, 0x44);

	Print_SetBitmapFont(null);
	Print_SetFont(g_Font_MGL_Std0);
	Print_SetMode(PRINT_MODE_BITMAP_TRANS);
	Print_SetColor(0xF, 0);
	Print_SetPosition(4, 4);
	Print_DrawText("MGL - PT3 PLAYER SAMPLE");
	Draw_Box(0, 0, 255, 14, 0x0F, 0);

	Print_SetPosition(0, 56);
	Print_DrawText("Music\n");		
	Print_DrawText("Pattern\n");		
	Print_DrawText("Chan. A\n");		
	Print_DrawText("Chan. B\n");		
	Print_DrawText("Chan. C\n");		

	Print_SetColor(0x5, 0x4);
	Print_SetPosition(0, 204);
	Print_DrawText("<>:Song  Space:Pause  123:Mute  Del:Loop");		

	// PLAYER
	
	VDP_CommandHMMV(0, 32, 10*6+1, 8+3, 0x55);

	Print_SetColor(0xF, 0x5);
	Print_SetFont(g_Font_MGL_Symbol1);
	Print_SetMode(PRINT_MODE_BITMAP);
	// Previous
	Print_SetPosition(31, 33);
	Print_DrawChar(CHR_PREV);
	// Next
	Print_SetPosition(41, 33);
	Print_DrawChar(CHR_NEXT);
	
	// Output
	Print_SetColor(0xF, 0x4);
	Print_SetPosition(64, 64+8*1);
	Print_DrawChar(CHR_SOUND);
	Print_SetPosition(64, 64+8*2);
	Print_DrawChar(CHR_SOUND);
	Print_SetPosition(64, 64+8*3);
	Print_DrawChar(CHR_SOUND);
	
	// INIT PT3

	PT3_Init();
	PT3_SetNoteTable(PT3_NT2);
	PT3_SetLoop(true);
	//PT3_SetFinishCB(Stop);

	SetSong(1);
	Loop(true);
	Play();

	Bios_SetHookCallback(H_TIMI, VBlankHook);

	u8 prevRow8 = 0xFF;
	u8 prevRow0 = 0xFF;
	u8 prevPattern = 0xFF;
	while(1)
	{
		WaitVBlank();

		// Display Vumeter
		DrawVUMeter();

		// DEFAULT FONT ----------------------------------------------------------

		Print_SetFont(g_Font_MGL_Std0);
		Print_SetColor(0xF, 0x4);

		Print_SetPosition(255-8, 4);
		u8 chr = g_Frame & 0x03;
		Print_DrawChar(g_ChrAnim[chr]);
		
		// Display Current Pattern
		u8 pattern = PT3_GetPattern();
		if(pattern != prevPattern)
		{
			Print_SetPosition(64, 64);
			VDP_CommandHMMV(64+6, 64, 2*6, 8, 0x44);
			Print_DrawInt(pattern);
		}
		prevPattern = pattern;


		// Read keyboard matrix row #8
		u8 row8 = g_NEWKEY[KEY_ROW(KEY_SPACE)];
		if((IS_KEY_PRESSED(row8, KEY_RIGHT)) && (IS_KEY_RELEASED(prevRow8, KEY_RIGHT))) //Next song
		{
			if(g_CurrentSong < numberof(g_SongData) - 1)
				g_CurrentSong++;
			else
				g_CurrentSong = 0;
			SetSong(g_CurrentSong);
		}
		else if((IS_KEY_PRESSED(row8, KEY_LEFT)) && (IS_KEY_RELEASED(prevRow8, KEY_LEFT))) // Previous song
		{
			if(g_CurrentSong > 0)
				g_CurrentSong--;
			else
				g_CurrentSong = numberof(g_SongData) - 1;
			SetSong(g_CurrentSong);
		}

		// PLAYER FONT -----------------------------------------------------------
		
		if((IS_KEY_PRESSED(row8, KEY_SPACE)) && (IS_KEY_RELEASED(prevRow8, KEY_SPACE))) // Pause / Resume
		{
			if(PT3_IsPlaying())
				Pause();
			else
				Play();
		}
		else if((IS_KEY_PRESSED(row8, KEY_DEL)) && (IS_KEY_RELEASED(prevRow8, KEY_DEL))) // Loop
		{
			Loop(1 - g_Loop);
		}
		prevRow8 = row8;

		// Read keyboard matrix row #0		
		u8 row0 = g_NEWKEY[KEY_ROW(KEY_0)];
		if((IS_KEY_PRESSED(row0, KEY_1)) && (IS_KEY_RELEASED(prevRow0, KEY_1))) // Mute channel A
		{
			Mute(0, 1 - g_Mute[0]);
		}
		else if((IS_KEY_PRESSED(row0, KEY_2)) && (IS_KEY_RELEASED(prevRow0, KEY_2))) // Mute channel B
		{
			Mute(1, 1 - g_Mute[1]);
		}
		else if((IS_KEY_PRESSED(row0, KEY_3)) && (IS_KEY_RELEASED(prevRow0, KEY_3))) // Mute channel C
		{
			Mute(2, 1 - g_Mute[2]);
		}
		prevRow0 = row0;

		// PT3_Decode(); 
	}
}