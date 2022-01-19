// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
#include "ayvgm_player.h"
#include "bios_mainrom.h"

//=============================================================================
// DEFINES
//=============================================================================


//=============================================================================
// READ-ONLY DATA
//=============================================================================

// ay regsiter convertor         -  1  2  3  4  5' 6  7' 8  9' A   B'  C
const u8  g_ayVGM_RegTable[] = { 0, 1, 3, 5, 6, 6, 8, 8, 9, 9, 10, 10, 13 };

// File ident data
const u8  g_ayVGM_Ident[4] = { 'a', 'y', 'M', ' ' };

//=============================================================================
// MEMORY DATA
//=============================================================================

const struct ayVGM_Header* g_ayVGM_Header;
const u8* g_ayVGM_Pointer;
u8        g_ayVGM_Wait;
u8        g_ayVGM_State;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Play a ayVGM data
bool ayVGM_Play(const void* addr, bool loop)
{
	g_ayVGM_Header = (const struct ayVGM_Header*)(addr);
	for(u8 i = 0; i < 4; i++)
		if(g_ayVGM_Header->Ident[i] != g_ayVGM_Ident[i])
			return false;

	g_ayVGM_State = 0;
	if(!(g_ayVGM_Header->Flag & AYVGM_FLAG_60HZ) || ((g_ayVGM_Header->Flag & AYVGM_FLAG_50HZ) && (g_ROMVersion.VSF)))
		g_ayVGM_State |= AYVGM_STATE_50HZ;
	if(loop)
		g_ayVGM_State |= AYVGM_STATE_LOOP;
	
	g_ayVGM_Pointer = (const u8*)(g_ayVGM_Header) + sizeof(struct ayVGM_Header);
	g_ayVGM_Wait = 0;
	ayVGM_Resume();
	return true;
}

//-----------------------------------------------------------------------------
// Play a ayVGM data
void ayVGM_Stop()
{
	ayVGM_Pause();
	g_ayVGM_Pointer = (const u8*)(g_ayVGM_Header) + sizeof(struct ayVGM_Header);
	g_ayVGM_Wait = 0;
}

//-----------------------------------------------------------------------------
//
void ayVGM_Decode()
{
	// Check if the music is playing
	if(!(g_ayVGM_State & AYVGM_STATE_PLAY))
		return;
	
	// Check if there are still waiting cycles
	if(g_ayVGM_Wait != 0)
	{
		g_ayVGM_Wait--;
		return;
	}
	
	// Parse music data
	while(1)
	{
		u8 op = *g_ayVGM_Pointer & 0xF0;
		switch(op)
		{
			case 0x00:
			{
				u8 reg = *g_ayVGM_Pointer & 0x0F;
				PSG_SetRegister(reg, *++g_ayVGM_Pointer);
				break;
			}
			case 0xD0: // 50 Hz wait
				if(g_ayVGM_State & AYVGM_STATE_50HZ)
				{
					g_ayVGM_Wait += *g_ayVGM_Pointer & 0x0F;
					g_ayVGM_Pointer++;
					return;
				}
				break;
			case 0xE0: // 60 Hz wait
				if(!(g_ayVGM_State & AYVGM_STATE_50HZ))
				{
					g_ayVGM_Wait += *g_ayVGM_Pointer & 0x0F;
					g_ayVGM_Pointer++;
					return;
				}
				break;
			case 0xF0:
				// handle loop
				if((*g_ayVGM_Pointer == 0xFE) && (g_ayVGM_State & AYVGM_STATE_LOOP))
				{
					u16 loopOffset = *(u16*)++g_ayVGM_Pointer;
					g_ayVGM_Pointer = (const u8*)(g_ayVGM_Header) + sizeof(struct ayVGM_Header) + loopOffset;
					return;
				}
				// if(val == 0xF)
				ayVGM_Stop();
				return;
			// case 0x10:
			// case 0x20:
			// case 0x30:
			// case 0x40:
			// case 0x50:
			// case 0x60:
			// case 0x70:
			// case 0x80:
			// case 0x90:
			// case 0xA0:
			// case 0xB0:
			// case 0xC0:
			default:
				PSG_SetRegister(g_ayVGM_RegTable[*g_ayVGM_Pointer >> 4], *g_ayVGM_Pointer);
				break;
		}
		g_ayVGM_Pointer++;
	}
}
