// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄  ▄▄ ▄  ▄▄▄  ▄ ▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██  ██ █ ██   ██▀█
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄ ▀█▀  ▀█▄█ ██ █
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
#include "lvgm_player.h"
// #include "bios_mainrom.h"

//=============================================================================
// DEFINES
//=============================================================================

// Special operator
enum LVGM_OP
{
	LVGM_OP_PSG			= 0xF0, // Start of PSG chunk (default when not defined)
	LVGM_OP_OPLL		= 0xF1, // Start of MSX-MUSIC chunk
	LVGM_OP_OPL1		= 0xF2, // Start of MSX-AUDIO chunk
	LVGM_OP_SCC			= 0xF3, // Start of SCC chunk
	LVGM_OP_SCCI		= 0xF4, // Start of SCC+ chunk
	LVGM_OP_PSG2		= 0xF5, // Start of secondary PSG chunk
	LVGM_OP_OPL4		= 0xF7, // Start of Moonsound chunk
	LVGM_OP_NOTIFY		= 0xFD, // Optional markers
	LVGM_OP_LOOP		= 0xFE, // Loop position
	LVGM_OP_END			= 0xFF, // End of song
};

// Default callback prototype
bool LVGM_Defaultcallback(u8 id);

// Decode function signature
typedef void (*LVGM_DecodeCB)(void); // Callback default signature

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// PSG regsiter convertor       -  1  2  3  4  5+ 6  7+ 8  9+ A   B+  C
const u8  g_LVGM_RegTable[] = { 0, 1, 3, 5, 6, 6, 8, 8, 9, 9, 10, 10, 13 };

// File ident data
const u8  g_LVGM_Ident[4] = { 'l', 'V', 'G', 'M' };

//=============================================================================
// MEMORY DATA
//=============================================================================

const struct LVGM_Header* g_LVGM_Header;
const u8*                 g_LVGM_Pointer;
const u8*                 g_LVGM_LoopAddr;
u8                        g_LVGM_Wait;
u8                        g_LVGM_State;
u8                        g_LVGM_CurChip;
LVGM_DecodeCB             g_LVGM_Decode;
#if (USE_LVGM_NOTIFY)
LVGM_NotifyCB             g_LVGM_Callback = LVGM_Defaultcallback;
#endif

//=============================================================================
// FUNCTIONS
//=============================================================================

#if (USE_LVGM_PSG)
//-----------------------------------------------------------------------------
//
void LVGM_DecodePSG()
{
	u8 op = *g_LVGM_Pointer & 0xF0;
	switch(op)
	{
		case 0x00: // R#0n = nn
		{
			u8 reg = *g_LVGM_Pointer;
			PSG_SetRegister(reg, *++g_LVGM_Pointer);
			break;
		}

		case 0xD0: // Zeroed register (R#n = 0)
		{
			u8 reg = *g_LVGM_Pointer & 0x0F;
			PSG_SetRegister(reg, 0);
			break;
		}

		// case 0x10: // R#1 = n
		// case 0x20: // R#3 = n
		// case 0x30: // R#5 = n
		// case 0x40: // R#6 = n
		// case 0x50: // R#6 = n + 0x10
		// case 0x60: // R#8 = n
		// case 0x70: // R#8 = n + 0x10
		// case 0x80: // R#9 = n
		// case 0x90: // R#9 = n + 0x10
		// case 0xA0: // R#10 = n
		// case 0xB0: // R#10 = n + 0x10
		// case 0xC0: // R#13 = n
		default:
		{
			PSG_SetRegister(g_LVGM_RegTable[*g_LVGM_Pointer >> 4], *g_LVGM_Pointer);
			break;
		}
	}
}
#endif

#if (USE_LVGM_MSXMUSIC)
// OPLL lookup table           0     1     2     3     4     5     6
const u8 g_lVGM_OPLL_Cnt[] = { 8,    9,    9,    9,    3,    3,    3 };
const u8 g_lVGM_OPLL_Reg[] = { 0x00, 0x10, 0x20, 0x30, 0x16, 0x26, 0x36 };

//-----------------------------------------------------------------------------
//
void LVGM_DecodeOPLL()
{
	u8 op = *g_LVGM_Pointer;
	switch(op >> 4)
	{
		case 0x0: // Register #rr (00~38h) set to nn
		case 0x1:
		case 0x2:
		case 0x3:
			MSXMusic_SetRegister(op, *++g_LVGM_Pointer);
			break;

		case 0x4: // 4x nn[] | Copy Y bytes from R#x0
		{
			u8 id = op & 0x0F;
			u8 cnt = g_lVGM_OPLL_Cnt[id];
			u8 reg = g_lVGM_OPLL_Reg[id];
			loop(i, cnt)
				MSXMusic_SetRegister(reg++, *++g_LVGM_Pointer);
			break;
		}
		case 0x5: // 5x nn | fill Y bytes from R#x0
		{
			u8 id = op & 0x0F;
			u8 cnt = g_lVGM_OPLL_Cnt[id];
			u8 reg = g_lVGM_OPLL_Reg[id];
			u8 val = *++g_LVGM_Pointer;
			loop(i, cnt)
				MSXMusic_SetRegister(reg++, val);
			break;
		}
		case 0x6: // 6n rr vv | Set n+3 bytes (3~18) start from register #rr
		{
			u8 cnt = (*g_LVGM_Pointer & 0x0F) + 3;
			u8 reg = *++g_LVGM_Pointer;
			u8 val = *++g_LVGM_Pointer;
			loop(i, cnt)
				MSXMusic_SetRegister(reg++, val);
			break;
		}
		case 0x7: // 6n rr vv | Set n+3 bytes (3~18) start from register #rr
		{
			u8 cnt = (*g_LVGM_Pointer & 0x0F) + 3;
			u8 reg = *++g_LVGM_Pointer;
			loop(i, cnt)
				MSXMusic_SetRegister(reg++, *++g_LVGM_Pointer);
			break;
		}
		case 0x8: // Register #rr (80~B8h) set to 0
		case 0x9:
		case 0xA:
		case 0xB:
			MSXMusic_SetRegister(op & 0x7F, 0);
			break;
	}
}
#endif

#if (USE_LVGM_MSXAUDIO)
//-----------------------------------------------------------------------------
//
void LVGM_DecodeOPL1()
{
}
#endif

#if (USE_LVGM_SCC)
// SCC lookup table           0     1     2     3     4
const u8 g_lVGM_SCC_Reg[] = { 0x00, 0x20, 0x40, 0x60, 0x80 };

//-----------------------------------------------------------------------------
//
void  LVGM_DecodeSCC()
{
	u8 op = *g_LVGM_Pointer;
	if(op < 0xB0) // rr nn | Register #rr (00~AF) set to nn
	{
		SCC_SetRegister(op, *++g_LVGM_Pointer);
	}
	else if(op < 0xB8) // Bx | Waveform
	{
		u8 reg = g_lVGM_SCC_Reg[op & 0x07];
		loop(i, 32)
			SCC_SetRegister(reg++, *++g_LVGM_Pointer);
	}
	else if(op < 0xC0) // Bx | Waveform
	{
		u8 reg = g_lVGM_SCC_Reg[op & 0x07];
		u8 val = *++g_LVGM_Pointer;
		loop(i, 32)
			SCC_SetRegister(reg++, val);
	}
	else if(op < 0xD0) // Cn rr vv | Set n+3 bytes (3~18) start from register #rr
	{
		u8 cnt = (op & 0x0F) + 3;
		u8 reg = *++g_LVGM_Pointer;
		u8 val = *++g_LVGM_Pointer;
		loop(i, cnt)
			SCC_SetRegister(reg++, val);
	}
	else if(op < 0xE0) // Dn rr vv[] | Copy n+3 bytes (3~18) start from register #rr
	{
		u8 cnt = (op & 0x0F) + 3;
		u8 reg = *++g_LVGM_Pointer;
		loop(i, cnt)
			SCC_SetRegister(reg++, *++g_LVGM_Pointer);
	}
}
#endif

#if (USE_LVGM_SCCI)
//-----------------------------------------------------------------------------
//
void  LVGM_DecodeSCCI()
{
}
#endif

#if (USE_LVGM_PSG2)
//-----------------------------------------------------------------------------
//
void  LVGM_DecodePSG2()
{
}
#endif

#if (USE_LVGM_OPL4)
//-----------------------------------------------------------------------------
//
void  LVGM_DecodeOPL4()
{
}
#endif

//-----------------------------------------------------------------------------
// Play a LVGM data
bool LVGM_Play(const void* addr, bool loop)
{
	g_LVGM_Header = (const struct LVGM_Header*)(addr);
	for(u8 i = 0; i < 4; i++)
		if(g_LVGM_Header->Ident[i] != g_LVGM_Ident[i])
			return FALSE;

	g_LVGM_State = 0;
	if(g_LVGM_Header->Option & LVGM_OPTION_50HZ)
		g_LVGM_State |= LVGM_STATE_50HZ;
	if(loop)
		g_LVGM_State |= LVGM_STATE_LOOP;
	
	g_LVGM_Pointer = (const u8*)(g_LVGM_Header) + 2;
	if(g_LVGM_Header->Option & LVGM_OPTION_DEVICE)
		g_LVGM_Pointer++;
	
	g_LVGM_LoopAddr = g_LVGM_Pointer;
	g_LVGM_Wait = 0;
	g_LVGM_CurChip = LVGM_CHIP_PSG;
	g_LVGM_Decode = LVGM_DecodePSG;

	LVGM_Pause();
	LVGM_Resume();
	return TRUE;
}

//-----------------------------------------------------------------------------
// Play a LVGM data
void LVGM_Stop()
{
	LVGM_Pause();

	g_LVGM_Pointer = (const u8*)(g_LVGM_Header) + 2;
	if(g_LVGM_Header->Option & LVGM_OPTION_DEVICE)
		g_LVGM_Pointer++;
	g_LVGM_Wait = 0;
}

//-----------------------------------------------------------------------------
//
void LVGM_Pause()
{
	g_LVGM_State &= ~LVGM_STATE_PLAY;
	
	#if (USE_LVGM_PSG)
	PSG_Mute();
	#endif

	#if (USE_LVGM_MSXMUSIC)
	MSXMusic_Mute();
	#endif
	
	#if (USE_LVGM_MSXAUDIO)
	MSXAudio_Mute();
	#endif
	
	#if (USE_LVGM_SCC)
	SCC_Mute();
	#endif
}

//-----------------------------------------------------------------------------
//
void LVGM_Decode()
{
	// Check if the music is playing
	if(!(g_LVGM_State & LVGM_STATE_PLAY))
		return;
	
	// Check if there are still waiting cycles
	if(g_LVGM_Wait != 0)
	{
		g_LVGM_Wait--;
		return;
	}
	
	// Parse music data
	while(1)
	{
		u8 op = *g_LVGM_Pointer & 0xF0;

		if (op == 0xE0) // Wait n+1 cycles (1~16)
		{
			g_LVGM_Wait += (*g_LVGM_Pointer & 0x0F);
			g_LVGM_Pointer++;
			return;
		}
		else if (op == 0xF0) // Special markers
		{
			switch(*g_LVGM_Pointer)
			{
			#if (USE_LVGM_PSG)
			case LVGM_OP_PSG: // PSG / AY-3-8910
				g_LVGM_CurChip = LVGM_CHIP_PSG;
				g_LVGM_Decode = LVGM_DecodePSG;
				break;
			#endif

			#if (USE_LVGM_MSXMUSIC)
			case LVGM_OP_OPLL: // MSX-MUSIC / YM2413 / OPLL
				g_LVGM_CurChip = LVGM_CHIP_OPLL;
				g_LVGM_Decode = LVGM_DecodeOPLL;
				break;
			#endif

			#if (USE_LVGM_MSXAUDIO)
			case LVGM_OP_OPL1: // MSX-AUDIO / Y8950 / OPL1 + ADPCM
				g_LVGM_CurChip = LVGM_CHIP_OPL1;
				g_LVGM_Decode = LVGM_DecodeOPL1;
				break;
			#endif

			#if (USE_LVGM_SCC)
			case LVGM_OP_SCC: // SCC / K051649
				g_LVGM_CurChip = LVGM_CHIP_SCC;
				g_LVGM_Decode = LVGM_DecodeSCC;
				break;
			#endif

			#if (USE_LVGM_SCCI)
			case LVGM_OP_SCCI: // SCC+ / K052539
				g_LVGM_CurChip = LVGM_CHIP_SCCI;
				g_LVGM_Decode = LVGM_DecodeSCCI;
				break;
			#endif

			#if (USE_LVGM_PSG2)
			case LVGM_OP_PSG2: // 2nd PSG
				g_LVGM_CurChip = LVGM_CHIP_PSG2;
				g_LVGM_Decode = LVGM_DecodePSG2;
				break;
			#endif

			#if (USE_LVGM_OPL4)
			case LVGM_OP_OPL4: // MOONBLASTER / YMF278
				g_LVGM_CurChip = LVGM_CHIP_OPL4;
				g_LVGM_Decode = LVGM_DecodeOPL4;
				break;
			#endif

			case LVGM_OP_NOTIFY:
			#if (USE_LVGM_NOTIFY)
				if(g_LVGM_Callback(*++g_LVGM_Pointer))
					continue;
			#else
				g_LVGM_Pointer++; // Skip notification
			#endif
				break;

			case LVGM_OP_LOOP: // Loop position
				g_LVGM_LoopAddr = g_LVGM_Pointer + 1;
				break;

			case LVGM_OP_END: // End of song
				if(g_LVGM_State & LVGM_STATE_LOOP) // handle loop
				{
					g_LVGM_Pointer = g_LVGM_LoopAddr;
					break;
				}
				LVGM_Stop();
				return;
			}
		}
		else
			g_LVGM_Decode();

		g_LVGM_Pointer++;
	}
}

#if (USE_LVGM_NOTIFY)
// Default dummy callback handler
bool LVGM_Defaultcallback(u8 id) { return TRUE; }
#endif

#if (0)
	switch(*g_LVGM_Pointer)
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
		u8 val = g_LVGM_Pointer[1];
		if((val & 0xF0) == 0x00)
			SCC_SetRegister(0x8F, val & 0x0F);
		else if((val & 0xF0) == 0xF0)
			SCC_SetRegister(0x8F, (val & 0x0F) | 0x10);
		else
			SCC_SetRegister(0x80 + (val >> 4), val & 0x0F);
		g_LVGM_Pointer++;
		break;
	}
	case 0xF0: // F0 nn    0x80 = nn    Frequency channel 1 - LSB
	case 0xF2: // F2 nn    0x82 = nn    Frequency channel 2 - LSB
	case 0xF4: // F4 nn    0x84 = nn    Frequency channel 3 - LSB
	case 0xF6: // F6 nn    0x86 = nn    Frequency channel 4 - LSB
	case 0xF8: // F8 nn    0x88 = nn    Frequency channel 5 - LSB
	{
		u8 val = g_LVGM_Pointer[1];
		SCC_SetRegister(0x80 + (*g_LVGM_Pointer & 0x0F), val);
		g_LVGM_Pointer++;
		break;
	}
	// F9 nn[32]    nn[32] -> 0x00[32]    Waveform channel 1
	// FA nn[32]    nn[32] -> 0x20[32]    Waveform channel 2
	// FB nn[32]    nn[32] -> 0x40[32]    Waveform channel 3
	// FC nn[32]    nn[32] -> 0x60[32]    Waveform channel 4
	// FD nn[32]    nn[32] -> 0xA0[32]    Waveform channel 5 (SCC+ only)
	case 0xF5: // nn[bb] -> (0x00 + aa)[bb] (with bb > 1)
	{
		u8 reg = g_LVGM_Pointer[1];
		u8 num = g_LVGM_Pointer[2];
		for(u8 i = 0; i < num; i++)
			SCC_SetRegister(reg + i, g_LVGM_Pointer[3 + i]);
		g_LVGM_Pointer += 2 + num;
		break;
	}
	case 0xFE: // End of music with loop
		if(g_LVGM_State & LVGM_STATE_LOOP)
		{
			u16 loopOffset = *(u16*)++g_LVGM_Pointer;
			g_LVGM_Pointer = (const u8*)(g_LVGM_Header) + sizeof(struct LVGM_Header) + loopOffset;
			return;
		}
	case 0xFF: // End of music
		LVGM_Stop();
		return;
	}
#endif