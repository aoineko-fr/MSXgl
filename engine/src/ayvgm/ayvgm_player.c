// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
#include "ayvgm_player.h"
#include "bios_mainrom.h"
#if (USE_AYVGM_SCC)
#include "scc.h"
#endif

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
			case 0xF0: // Special
			#if (USE_AYVGM_SCC)
				switch(*g_ayVGM_Pointer)
				{
				// F1 1n    0x81 = n           Frequency channel 1 - MSB
				// F1 3n    0x83 = n           Frequency channel 2 - MSB
				// F1 5n    0x85 = n           Frequency channel 3 - MSB
				// F1 7n    0x87 = n           Frequency channel 4 - MSB
				// F1 9n    0x89 = n           Frequency channel 5 - MSB
				// F1 An    0x8A = n           Volume channel 1
				// F1 Bn    0x8B = n           Volume channel 2
				// F1 Cn    0x8C = n           Volume channel 3
				// F1 Dn    0x8D = n           Volume channel 4
				// F1 En    0x8E = n           Volume channel 5
				// F1 0n    0x8F = n           On/off switch channel 1 to 5
				// F1 Fn    0x8F = n | 0x10    On/off switch channel 1 to 5
				case 0xF1:
				{
					u8 val = g_ayVGM_Pointer[1];
					if((val & 0xF0) == 0x00)
						SCC_SetRegister(0x8F, val & 0x0F);
					else if((val & 0xF0) == 0xF0)
						SCC_SetRegister(0x8F, (val & 0x0F) | 0x10);
					else
						SCC_SetRegister(0x80 + (val >> 4), val & 0x0F);
					g_ayVGM_Pointer++;
					break;
				}
				case 0xF0: // F0 nn    0x80 = nn    Frequency channel 1 - LSB
				case 0xF2: // F2 nn    0x82 = nn    Frequency channel 2 - LSB
				case 0xF4: // F4 nn    0x84 = nn    Frequency channel 3 - LSB
				case 0xF6: // F6 nn    0x86 = nn    Frequency channel 4 - LSB
				case 0xF8: // F8 nn    0x88 = nn    Frequency channel 5 - LSB
				{
					u8 val = g_ayVGM_Pointer[1];
					SCC_SetRegister(0x80 + (*g_ayVGM_Pointer & 0x0F), val);
					g_ayVGM_Pointer++;
					break;
				}
				// F9 nn[32]    nn[32] -> 0x00[32]    Waveform channel 1
				// FA nn[32]    nn[32] -> 0x20[32]    Waveform channel 2
				// FB nn[32]    nn[32] -> 0x40[32]    Waveform channel 3
				// FC nn[32]    nn[32] -> 0x60[32]    Waveform channel 4
				// FD nn[32]    nn[32] -> 0xA0[32]    Waveform channel 5 (SCC+ only)
				case 0xF5: // nn[bb] -> (0x00 + aa)[bb] (with bb > 1)
				{
					u8 reg = g_ayVGM_Pointer[1];
					u8 num = g_ayVGM_Pointer[2];
					for(u8 i = 0; i < num; i++)
						SCC_SetRegister(reg + i, g_ayVGM_Pointer[3 + i]);
					g_ayVGM_Pointer += 2 + num;
					break;
				}
				case 0xFE: // End of music with loop
					if(g_ayVGM_State & AYVGM_STATE_LOOP)
					{
						u16 loopOffset = *(u16*)++g_ayVGM_Pointer;
						g_ayVGM_Pointer = (const u8*)(g_ayVGM_Header) + sizeof(struct ayVGM_Header) + loopOffset;
						return;
					}
				case 0xFF: // End of music
					ayVGM_Stop();
					return;
				}
			#else
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
			#endif

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
