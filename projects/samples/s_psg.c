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
#include "psg.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x02\x03\x04\x05"

// 
typedef void (*UICall)(u8 chan, u8 action);

//
struct UIEntry
{
	u8     PosX;
	u8     PosY;
	u8     Chan;
	UICall Func;	
};

// Move directions
#define MOVE_UP				-3
#define MOVE_RIGHT			1
#define MOVE_DOWN			3
#define MOVE_LEFT			-1

// Action
#define ACTION_NONE			0
#define ACTION_PRESS_UP		1
#define ACTION_HOLD_UP		2
#define ACTION_PRESS_DOWN	3
#define ACTION_HOLD_DOWN	4

//
#define CURSOR_ID			25

#define TONE_FREQ_OFFSET	0x20
#define TONE_FREQ_MAX		0x0FFF
#define TONE_VOL_OFFSET		0x01
#define TONE_VOL_MAX		0x0F
#define ENV_FREQ_OFFSET		0x0200
#define ENV_FREQ_MAX		0xFFFF
#define ENV_SHAPE_OFFSET	0x01
#define ENV_SHAPE_MAX		0x0F
#define NOISE_FREQ_OFFSET	0x02
#define NOISE_FREQ_MAX		0x1F

//
void SetToneEnable(u8 chan, u8 action);
void SetTonePeriod(u8 chan, u8 action);
void SetToneVolume(u8 chan, u8 action);
void SetEnvEnable(u8 chan, u8 action);
void SetEnvPeriod(u8 chan, u8 action);
void SetEnvShape(u8 chan, u8 action);
void SetNoiseEnable(u8 chan, u8 action);
void SetNoisePeriod(u8 chan, u8 action);

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font\font_mgl_sample8.h"

//
const u8 g_ShapesId[16] = { 0, 0, 0, 0,	1, 1, 1, 1,	2, 0, 3, 4,	5, 6, 7, 1 };

//
const c8* g_ShapesName[8] =
{                                       //         _                     _
	"\x1F\xAC\xAC\xAC\xAC\xAC\xAC\xAC",	// 0-3, 9  _ \__________________ _   
										//         _                     _
	"\xAA\xAC\xAC\xAC\xAC\xAC\xAC\xAC",	// 4-7, 15 _ /|_________________ _
										//         _                     _
	"\xAB\xAB\xAB\xAB\xAB\xAB\xAB\xAB", // 8       _ \|\|\|\|\|\|\|\|\|\ _
										//         _                     _
	"\x1F\x1E\x1F\x1E\x1F\x1E\x1F\x1E", // 10      _ \/\/\/\/\/\/\/\/\/\ _
										//         _   _________________ _
	"\xAB\xAD\xAD\xAD\xAD\xAD\xAD\xAD", // 11      _ \|                  _
										//         _                     _
	"\xAA\xAA\xAA\xAA\xAA\xAA\xAA\xAA", // 12      _ /|/|/|/|/|/|/|/|/|/ _
										//         _  __________________ _
	"\x1E\xAD\xAD\xAD\xAD\xAD\xAD\xAD",	// 13      _ /                   _
										//         _                     _
	"\x1E\x1F\x1E\x1F\x1E\x1F\x1E\x1F", // 14      _ /\/\/\/\/\/\/\/\/\/ _
};

// Color shade
const u8 g_ColorBlink[4] = { COLOR_LIGHT_RED, COLOR_MEDIUM_RED, COLOR_DARK_RED, COLOR_MEDIUM_RED };

//
const struct UIEntry g_UIEntries[24] =
{
	{ 64,  56, 0, SetToneEnable },  { 128,  56, 1, SetToneEnable },  { 192,  56, 2, SetToneEnable },
	{ 64,  64, 0, SetTonePeriod },  { 128,  64, 1, SetTonePeriod },  { 192,  64, 2, SetTonePeriod },
	{ 64,  72, 0, SetToneVolume },  { 128,  72, 1, SetToneVolume },  { 192,  72, 2, SetToneVolume },
	{ 64,  88, 0, SetEnvEnable },   { 128,  88, 1, SetEnvEnable },   { 192,  88, 2, SetEnvEnable },
	{ 64,  96, 0, SetEnvPeriod },   { 128,  96, 1, NULL },           { 192,  96, 2, NULL },
	{ 64, 104, 0, SetEnvShape  },   { 128, 104, 1, NULL },           { 192, 104, 2, NULL },
	{ 64, 120, 0, SetNoiseEnable }, { 128, 120, 1, SetNoiseEnable }, { 192, 120, 2, SetNoiseEnable },
	{ 64, 128, 0, SetNoisePeriod }, { 128, 128, 1, NULL },           { 192, 128, 2, NULL },
};

//=============================================================================
// MEMORY DATA
//=============================================================================

u8 g_CursorPos = 0;

//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
//
void SetToneEnable(u8 chan, u8 action)
{
	u8 enable = (g_PSG_Regs.Mixer & (1 << chan)) ? 0 : 1;
	if(action == ACTION_HOLD_UP)
		enable = 1;
	else if(action == ACTION_HOLD_DOWN)
		enable = 0;
	PSG_EnableTone(chan, enable);

	VDP_SetSpritePattern(chan, enable ? 0x0C : 0x0B);
}

//-----------------------------------------------------------------------------
//
void SetTonePeriod(u8 chan, u8 action)
{
	u16 val = g_PSG_Regs.Tone[chan];
	if(action == ACTION_HOLD_UP)
	{
		if(val < TONE_FREQ_MAX - TONE_FREQ_OFFSET)
			val += TONE_FREQ_OFFSET;
		else
			val = TONE_FREQ_MAX;
	}
	else if(action == ACTION_HOLD_DOWN)
	{
		if(val > TONE_FREQ_OFFSET)
			val -= TONE_FREQ_OFFSET;
		else
			val = 0;
	}
	PSG_SetTone(chan, val);

	Print_SetPosition(10 + chan * 8, 8);
	Print_DrawFormat("%3x", val);
	VDP_SetSpritePattern(3 + chan, (u8)(0xB0 + (val >> 9)));
}

//-----------------------------------------------------------------------------
//
void SetToneVolume(u8 chan, u8 action)
{
	u8 val = g_PSG_Regs.Volume[chan];
	u8 env = val & PSG_F_ENV;
	val &= 0x0F;
	if(action == ACTION_HOLD_UP)
	{
		if(val < TONE_VOL_MAX - TONE_VOL_OFFSET)
			val += TONE_VOL_OFFSET;
		else
			val = TONE_VOL_MAX;
	}
	else if(action == ACTION_HOLD_DOWN)
	{
		if(val > TONE_VOL_OFFSET)
			val -= TONE_VOL_OFFSET;
		else
			val = 0;
	}
	PSG_SetVolume(chan, val | env);

	Print_SetPosition(10 + chan * 8, 9);
	Print_DrawFormat("%1x", val);
	VDP_SetSpritePattern(6 + chan, (u8)(0xB0 + (val >> 1)));
}

//-----------------------------------------------------------------------------
//
void SetEnvEnable(u8 chan, u8 action)
{
	u8 enable = (g_PSG_Regs.Volume[chan] & PSG_F_ENV) ? 1 : 0;
	if(action == ACTION_HOLD_UP)
		enable = 1;
	else if(action == ACTION_HOLD_DOWN)
		enable = 0;
	PSG_EnableEnvelope(chan, enable);

	VDP_SetSpritePattern(9 + chan, enable ? 0x0C : 0x0B);
}

//-----------------------------------------------------------------------------
//
void SetEnvPeriod(u8 chan, u8 action)
{
	u16 val = g_PSG_Regs.Envelope;
	if(action == ACTION_HOLD_UP)
	{
		if(val < ENV_FREQ_MAX - ENV_FREQ_OFFSET)
			val += ENV_FREQ_OFFSET;
		else
			val = ENV_FREQ_MAX;
	}
	else if(action == ACTION_HOLD_DOWN)
	{
		if(val > ENV_FREQ_OFFSET)
			val -= ENV_FREQ_OFFSET;
		else
			val = 0;
	}
	PSG_SetEnvelope(val);

	Print_SetPosition(10 + chan * 8, 12);
	Print_DrawFormat("%4x", val);
	VDP_SetSpritePattern(12 + chan, (u8)(0xB0 + (val >> 13)));
}

//-----------------------------------------------------------------------------
//
void SetEnvShape(u8 chan, u8 action)
{
	u8 val = g_PSG_Regs.Shape;
	val &= 0x0F;
	if(action == ACTION_PRESS_UP)
	{
		if(val < ENV_SHAPE_MAX - ENV_SHAPE_OFFSET)
			val += ENV_SHAPE_OFFSET;
		else
			val = ENV_SHAPE_MAX;
	}
	else if(action == ACTION_PRESS_DOWN)
	{
		if(val > ENV_SHAPE_OFFSET)
			val -= ENV_SHAPE_OFFSET;
		else
			val = 0;
	}
	PSG_SetShape(val);

	Print_SetPosition(10 + chan * 8, 13);
	Print_DrawFormat("%1x %s", g_PSG_Regs.Shape & 0x0F, g_ShapesName[g_ShapesId[g_PSG_Regs.Shape & 0x0F]]);

	VDP_SetSpritePattern(15 + chan, 0x82);
}

//-----------------------------------------------------------------------------
//
void SetNoiseEnable(u8 chan, u8 action)
{
	u8 enable = (g_PSG_Regs.Mixer & (8 << chan)) ? 0 : 1;
	if(action == ACTION_HOLD_UP)
		enable = 1;
	else if(action == ACTION_HOLD_DOWN)
		enable = 0;
	PSG_EnableNoise(chan, enable);

	VDP_SetSpritePattern(18 + chan, enable ? 0x0C : 0x0B);
}

//-----------------------------------------------------------------------------
//
void SetNoisePeriod(u8 chan, u8 action)
{
	u8 val = g_PSG_Regs.Noise;
	if(action == ACTION_HOLD_UP)
	{
		if(val < NOISE_FREQ_MAX - NOISE_FREQ_OFFSET)
			val += NOISE_FREQ_OFFSET;
		else
			val = NOISE_FREQ_MAX;
	}
	else if(action == ACTION_HOLD_DOWN)
	{
		if(val > NOISE_FREQ_OFFSET)
			val -= NOISE_FREQ_OFFSET;
		else
			val = 0;
	}
	PSG_SetNoise(val);

	Print_SetPosition(10 + chan * 8, 16);
	Print_DrawFormat("%2x", val);
	VDP_SetSpritePattern(21 + chan, (u8)(0xB0 + (val >> 2)));
}

//-----------------------------------------------------------------------------
// Set cursor to new position
void MoveCursor(i8 offset)
{
	u8 newPos = g_CursorPos;
	while(1)
	{
		newPos = (24 + newPos + offset) % 24;
		if(g_UIEntries[newPos].Func != 0)
			break;
	}
	VDP_SetSpriteColorSM1(g_CursorPos, COLOR_WHITE);
	VDP_SetSpritePosition(CURSOR_ID, g_UIEntries[newPos].PosX - 8, g_UIEntries[newPos].PosY - 1);
	VDP_SetSpriteColorSM1(newPos, COLOR_LIGHT_RED);
	g_CursorPos = newPos;
}


//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	VDP_SetMode(VDP_MODE_SCREEN1); // Initialize screen mode 1 (G1)
	VDP_ClearVRAM();
	VDP_EnableVBlank(TRUE);

	Print_SetTextFont(g_Font_MGL_Sample8, 0); // Initialize font
	VDP_FillVRAM_16K(0, g_ScreenPatternLow, 8);

	Print_DrawText(MSX_GL " PSG SAMPLE");
	Print_DrawLineH(0, 1, 32);
	
	#if (PSG_CHIP == PSG_INTERNAL)
		Print_DrawFormat("Chip:  Intern (%2x)\n", PSG_PORT_REG);
	#elif (PSG_CHIP == PSG_EXTERNAL)
		Print_DrawFormat("Chip:  Extern (%2x)\n", PSG_PORT_REG);
	#elif (PSG_CHIP == PSG_BOTH)
		Print_DrawFormat("Chip:  Both (%2x+%2x)\n", PSG_PORT_REG, PSG2_PORT_REG);
	#endif
	#if (PSG_ACCESS == PSG_DIRECT)
		Print_DrawText("Access:Direct\n");
	#else
		Print_DrawText("Access:Indirect\n");
	#endif
	
	//-------------------------------------------------------------------------
	// Sprite
	
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_8 + VDP_SPRITE_SCALE_1);
	VDP_LoadSpritePattern(g_Font_MGL_Sample8 + 4, 0, g_Font_MGL_Sample8[3] - g_Font_MGL_Sample8[2]);

	VDP_SetSpriteSM1(CURSOR_ID, 100, 100, 0x8A, COLOR_LIGHT_RED);
	MoveCursor(0);


	//-------------------------------------------------------------------------
	// Setup PSG
	PSG_SetTone(0, 0x400);
	PSG_SetTone(1, 0x800);
	PSG_SetTone(2, 0xA00);
	PSG_SetNoise(0x10);
	PSG_SetMixer(0);
	PSG_SetVolume(0, 0xF);
	PSG_SetVolume(1, 0xF);
	PSG_SetVolume(2, 0xF);
	PSG_SetEnvelope(0x8000);
	PSG_SetShape(8);

	//-------------------------------------------------------------------------
	// Draw page
	Print_SetPosition(8, 5);
	Print_DrawText("Chan A  Chan B  Chan C\n");
	Print_DrawCharX(0x1C, 32);
	Print_DrawText("Tone\n");
	Print_DrawText("Freq\n");
	Print_DrawText("Vol\n");
	Print_DrawCharX(0x1C, 32);
	Print_DrawText("Env\n");
	Print_DrawText("Freq\n");
	Print_DrawText("Shape\n");
	Print_DrawCharX(0x1C, 32);
	Print_DrawText("Noise\n");
	Print_DrawText("Freq\n");
	Print_DrawCharX(0x1C, 32);
	
	Print_SetPosition(4, 19);
	for(u8 i = 0; i < 14; ++i)
		Print_DrawFormat("%1x ", i);
	Print_DrawText("\nRAW:");

	// Footer
	Print_DrawLineH(0, 22, 32);
	Print_SetPosition(0, 23);
	Print_DrawText("\x81\x82\x80:Move H:Up N:Down");
	
	// Sprite
	for(u8 i = 0; i < 24; ++i)
	{
		const struct UIEntry* entry = &g_UIEntries[i];
		if(entry->Func)
			VDP_SetSpriteSM1(i, entry->PosX, entry->PosY - 1, 0, COLOR_WHITE);
		else
			VDP_SetSpriteSM1(i, 0, 213, 0, 0);
	}
	
	SetToneEnable(0, ACTION_NONE);
	SetToneEnable(1, ACTION_NONE);
	SetToneEnable(2, ACTION_NONE);
	SetTonePeriod(0, ACTION_NONE);
	SetTonePeriod(1, ACTION_NONE);
	SetTonePeriod(2, ACTION_NONE);
	SetToneVolume(0, ACTION_NONE);
	SetToneVolume(1, ACTION_NONE);
	SetToneVolume(2, ACTION_NONE);
	SetEnvEnable(0, ACTION_NONE);
	SetEnvEnable(1, ACTION_NONE);
	SetEnvEnable(2, ACTION_NONE);
	SetEnvPeriod(0, ACTION_NONE);
	SetEnvShape(0, ACTION_NONE);
	SetNoiseEnable(0, ACTION_NONE);
	SetNoiseEnable(1, ACTION_NONE);
	SetNoiseEnable(2, ACTION_NONE);
	SetNoisePeriod(0, ACTION_NONE);

	u8 count = 0;
	u8 prevRow8 = 0xFF;
	u8 prevUp = 0;
	u8 prevDown = 0;
	while(1)
	{
		VDP_SetSpriteColorSM1(CURSOR_ID, g_ColorBlink[(count >> 2) & 0x03]);
		VDP_SetSpriteColorSM1(g_CursorPos, g_ColorBlink[(count >> 2) & 0x03]);
		
		Print_SetPosition(31, 0);
		Print_DrawChar(0xB0 + ((count) & 0x07));

		// Move cursor
		u8 row8 = Keyboard_Read(8);
		if(IS_KEY_PRESSED(row8, KEY_UP) && !IS_KEY_PRESSED(prevRow8, KEY_UP))
			MoveCursor(MOVE_UP);
		else if(IS_KEY_PRESSED(row8, KEY_RIGHT) && !IS_KEY_PRESSED(prevRow8, KEY_RIGHT))
			MoveCursor(MOVE_RIGHT);
		else if(IS_KEY_PRESSED(row8, KEY_DOWN) && !IS_KEY_PRESSED(prevRow8, KEY_DOWN))
			MoveCursor(MOVE_DOWN);
		else if(IS_KEY_PRESSED(row8, KEY_LEFT) && !IS_KEY_PRESSED(prevRow8, KEY_LEFT))
			MoveCursor(MOVE_LEFT);
		prevRow8 = row8;

		// Increase value
		u8 newUp = Keyboard_IsKeyPressed(KEY_H);
		if(newUp)
		{
			const struct UIEntry* entry = &g_UIEntries[g_CursorPos];
			if(!prevUp)
				entry->Func(entry->Chan, ACTION_PRESS_UP);
			else
				entry->Func(entry->Chan, ACTION_HOLD_UP);
		}
		prevUp = newUp;
		
		u8 newDown = Keyboard_IsKeyPressed(KEY_N);
		if(newDown)
		{
			const struct UIEntry* entry = &g_UIEntries[g_CursorPos];
			if(!prevDown)
				entry->Func(entry->Chan, ACTION_PRESS_DOWN);
			else
				entry->Func(entry->Chan, ACTION_HOLD_DOWN);
		}
		prevDown = newDown;

		Halt();
		count++;
		
		Print_SetPosition(4, 20);
		i8* ptr = (u8*)g_PSG_Regs;
		for(u8 i = 0; i < 14; ++i)
			Print_DrawHex8(*ptr++);

		PSG_Apply();
	}
}