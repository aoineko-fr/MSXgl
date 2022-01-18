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

// ay regsiter convertor         1  2  3  4  5' 6  7' 8  9' A   B'  C
const u8  g_ayVGM_RegTable[] = { 1, 3, 5, 6, 6, 8, 8, 9, 9, 10, 10, 13 };

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
	if(!(g_ayVGM_State & AYVGM_STATE_PLAY))
		return;
	while(g_ayVGM_Wait == 0)
	{
		u8 op = *g_ayVGM_Pointer >> 4;
		u8 val = *g_ayVGM_Pointer & 0x0F;
		switch(op)
		{
		case 0x0:
			g_ayVGM_Pointer++;
			PSG_SetRegister(val, *g_ayVGM_Pointer);
			break;
		case 0x5:
		case 0x7:
		case 0x9:
		case 0xB:
			val |= 0x10;
		case 0x1:
		case 0x2:
		case 0x3:
		case 0x4:
		case 0x6:
		case 0x8:
		case 0xA:
		case 0xC:
			PSG_SetRegister(g_ayVGM_RegTable[--op], val);
			break;
		case 0xD: // 50 Hz wait
			if(g_ayVGM_State & AYVGM_STATE_50HZ)
				g_ayVGM_Wait += 1 + val;
			break;
		case 0xE: // 60 Hz wait
			if(!(g_ayVGM_State & AYVGM_STATE_50HZ))
				g_ayVGM_Wait += 1 + val;
			break;
		case 0xF:
			// handle loop
			if((val == 0xE) && (g_ayVGM_State & AYVGM_STATE_LOOP))
			{
				g_ayVGM_Pointer++;
				u16 loopOffset = *(u16*)g_ayVGM_Pointer;
				g_ayVGM_Pointer = (const u8*)(g_ayVGM_Header) + sizeof(struct ayVGM_Header) + loopOffset;
				return;
			}
			// if(val == 0xF)
			ayVGM_Stop();
			return;
		}
		g_ayVGM_Pointer++;
	}

	g_ayVGM_Wait--;
}
