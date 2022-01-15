// __________________________
// ██▀█▀██▀▀▀█▀▀█▀█  ▄▄▄ ▄▄  │   ▄▄▄                ▄▄      
// █  ▄ █▄ ▀██▄ ▀▄█ ██   ██  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █▀▀ ▄█  █ █ ▀█▄█ ██▄▄│  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀──────────┘                 ▀▀
//  VGM format sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "psg.h"
#include "vgm\vgm_player.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x01\x02\x03\x04\x05\x06"

// VGM music entry
struct MusicEntry
{
	const c8* Name;
	const u8* Data;
	u8        Segment;
};

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font\font_mgl_sample6.h"


// Animation characters
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

// Music list
const struct MusicEntry g_MusicEntry[] =
{
	{ "Dragon Slayer 4  ", 0x8000, 2 },
	{ "Gambare Goemon   ", 0x8000, 3 },
	{ "Penguin Adventure", 0x8000, 4 },
	{ "Metal Gear       ", 0x8000, 5 },
	{ "Hi no Tori       ", 0x8000, 6 },
};

//=============================================================================
// MEMORY DATA
//=============================================================================

u8 g_CurrentMusic = 0;


//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
//
const c8* GetChipName(u8 type)
{
	switch(type)
	{
	case 0x00: return "AY8910";
	case 0x01: return "AY8912";
	case 0x02: return "AY8913";
	case 0x03: return "AY8930";
	case 0x10: return "YM2149";
	case 0x11: return "YM3439";
	case 0x12: return "YMZ284";
	case 0x13: return "YMZ294";
	}
	return "Unknow";
}

//-----------------------------------------------------------------------------
//
void DrawVGM(const u8* ptr)
{
	Print_SetPosition(34, 2);
	Print_DrawFormat("%4x", ptr);
	Print_SetPosition(34, 3);
	Print_DrawText("------");
	for(u8 i = 0; i < 1; ++i)
	{
		Print_SetPosition(34, 4 + i);
		if(*ptr == 0xA0) // AY8910, write value dd to register aa
		{
			Print_DrawFormat("R#%1x=%2x", ptr[1], ptr[2]);
			ptr += 2;
		}
		else if(*ptr == 0x61) // Wait n samples, n can range from 0 to 65535 (approx 1.49 seconds). Longer pauses than this are represented by multiple wait commands.
		{
			Print_DrawFormat("W=%4x", *(u16*)(ptr+1));
			ptr += 2;
		}
		else if(*ptr == 0x62) // Wait 735 samples (60th of a second), a shortcut for 0x61 0xdf 0x02
		{
			Print_DrawText("W=60Hz");
		}
		else if(*ptr == 0x63) // Wait 882 samples (50th of a second), a shortcut for 0x61 0x72 0x03
		{
			Print_DrawText("W=50Hz");
		}
		else if(*ptr == 0x66) // End of sound data
		{
			Print_DrawText("END!!!");
		}
		else if ((*ptr & 0xF0) == 0x70) // wait n+1 samples, n can range from 0 to 15.
		{
			Print_DrawFormat("W=%1x   ", 1 + *ptr & 0x0F);
		}
		else
		{
			Print_DrawFormat("~~%2x~~", *ptr);
			// Print_DrawHex8(*ptr);
		}
		Print_Return();
		ptr++;
	}
}

//-----------------------------------------------------------------------------
//
void SetMusic(u8 idx)
{
	g_CurrentMusic = idx;

	SET_BANK_SEGMENT(1, g_MusicEntry[idx].Segment);
	
	bool ok = VGM_Play(g_MusicEntry[idx].Data, true);

	Print_SetPosition(0, 2);
	Print_DrawFormat("\x8D Music #%i: %s", idx, g_MusicEntry[idx].Name);

	Print_SetPosition(0, 4);
	Print_DrawFormat("Ident:       %s   (%x)\n", ok ? "OK" : "Invalide", (u16)&g_VGM_Header->Ident);
	Print_DrawFormat("EOF offset:  %4X (%x)\n", g_VGM_Header->EoF_offset, (u16)&g_VGM_Header->EoF_offset + (u16)g_VGM_Header->EoF_offset);
	Print_DrawFormat("Version:     %4X\n", g_VGM_Header->Version);
	Print_DrawFormat("GD3 offset:  %4X (%x)\n", g_VGM_Header->GD3_offset, (u16)&g_VGM_Header->GD3_offset + (u16)g_VGM_Header->GD3_offset);
	Print_DrawFormat("Loop offset: %4X (%x)\n", g_VGM_Header->Loop_offset, (u16)&g_VGM_Header->Loop_offset + (u16)g_VGM_Header->Loop_offset);
	Print_DrawFormat("Rate:        %4X\n", g_VGM_Header->Rate);
	Print_DrawFormat("Data offset: %4X (%x)\n", g_VGM_Header->Data_offset, (u16)&g_VGM_Header->Data_offset + (u16)g_VGM_Header->Data_offset);
	Print_DrawFormat("AY8910 clk:  %X\n", g_VGM_Header->AY8910_clock);
	Print_DrawFormat("AY8910 type: %s\n", GetChipName(g_VGM_Header->AYT));
	Print_DrawFormat("AY8910 flag: %2x,%2x,%2x\n", g_VGM_Header->AY_Flags[0], g_VGM_Header->AY_Flags[1], g_VGM_Header->AY_Flags[2]);
	Print_Return();
	
	// u8* ptr = (u8*)((u16)&g_VGM_Header->Data_offset + (u16)g_VGM_Header->Data_offset);
	// DrawVGM(ptr);
}

//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	// Initialize screen
	VDP_SetMode(VDP_MODE_SCREEN0);
	VDP_ClearVRAM();
	VDP_EnableVBlank(true);

	// Initialize font
	Print_SetTextFont(g_Font_MGL_Sample6, 1); // Initialize font
	Print_DrawText(MSX_GL "  VGM Sample");
	Print_DrawLineH(0, 1, 40);

	// Decode VGM header
	SetMusic(0);

	// Footer
	Print_DrawLineH(0, 22, 40);
	Print_SetPosition(0, 23);
	Print_DrawText("");

	u8 count = 0;
	while(1)
	{
		Halt();
		// VDP_SetColor(0xFE);
		VGM_Decode();
		PSG_Apply();
		// VDP_SetColor(0xF0);

		DrawVGM(g_VGM_Pointer);

		Print_SetPosition(39, 0);
		u8 chr = count++ & 0x03;
		Print_DrawChar(g_ChrAnim[chr]);
		
		// Handle input
		u8 row8 = Keyboard_Read(8);

		// Change music
		if(IS_KEY_PRESSED(row8, KEY_RIGHT))
		{
			if(g_CurrentMusic < numberof(g_MusicEntry) - 1)
				SetMusic(g_CurrentMusic + 1);
		}
		else if(IS_KEY_PRESSED(row8, KEY_LEFT))
		{
			if(g_CurrentMusic > 0)
				SetMusic(g_CurrentMusic - 1);
		}

		// Explore VGM data
		/*if(IS_KEY_PRESSED(row8, KEY_SPACE))
		{
			if(IS_KEY_PRESSED(row8, KEY_UP))
			{
				g_VGM_Pointer -= 0x0100;
				DrawVGM(g_VGM_Pointer);
			}
			else if(IS_KEY_PRESSED(row8, KEY_DOWN))
			{
				g_VGM_Pointer += 0x0100;
				DrawVGM(g_VGM_Pointer);
			}
		}
		else
		{
			if(IS_KEY_PRESSED(row8, KEY_UP))
			{
				g_VGM_Pointer--;
				DrawVGM(g_VGM_Pointer);
			}
			else if(IS_KEY_PRESSED(row8, KEY_DOWN))
			{
				if(*g_VGM_Pointer == 0xA0) // reg copy
					g_VGM_Pointer += 3;
				else if(*g_VGM_Pointer == 0x61) // wait 16b
					g_VGM_Pointer += 3;
				else
					g_VGM_Pointer++;
				DrawVGM(g_VGM_Pointer);
			}
		}*/
	}
}