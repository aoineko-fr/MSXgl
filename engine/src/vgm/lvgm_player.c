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
u8                        g_LVGM_Devices;
u8                        g_LVGM_PSG_Default;
u8                        g_LVGM_Wait;
u8                        g_LVGM_State;
u8                        g_LVGM_CurChip;
LVGM_DecodeCB             g_LVGM_Decode;
#if (LVGM_USE_NOTIFY)
LVGM_NotifyCB             g_LVGM_Callback = LVGM_Defaultcallback;
#endif

//=============================================================================
// FUNCTIONS
//=============================================================================

#if (LVGM_USE_PSG)
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

		case 0xD0: // More common byte (R#n = X)
		{
			u8 reg = *g_LVGM_Pointer & 0x0F;
			PSG_SetRegister(reg, g_LVGM_PSG_Default);
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

#if (LVGM_USE_MSXMUSIC)
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

#if (LVGM_USE_MSXAUDIO)
//-----------------------------------------------------------------------------
//
void LVGM_DecodeOPL1()
{
	u8 reg = *g_LVGM_Pointer;
	u8 val = *++g_LVGM_Pointer;

	MSXAudio_SetRegister(reg, val);

	// u8 op = *g_LVGM_Pointer;
	// if((op & 0xF0) == 0xD0) // Da bb cc[a] | Copy a+3 bytes (3~18) start from R#bb
	// {
	// 	u8 cnt = (op & 0x0F) + 3;
	// 	u8 reg = *++g_LVGM_Pointer;
	// 	loop(i, cnt)
	// 		MSXAudio_SetRegister(reg++, *++g_LVGM_Pointer);
	// }
	// else // R#aa = bb
	// {
	// 	MSXAudio_SetRegister(op, *++g_LVGM_Pointer);
	// }
}
#endif

#if (LVGM_USE_SCC)
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

#if (LVGM_USE_SCCI)
//-----------------------------------------------------------------------------
//
void  LVGM_DecodeSCCI()
{
}
#endif

#if (LVGM_USE_PSG2)
//-----------------------------------------------------------------------------
//
void  LVGM_DecodePSG2()
{
}
#endif

#if (LVGM_USE_OPL4)
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
	// Check file indentification bytes
	g_LVGM_Header = (const struct LVGM_Header*)(addr);
	for(u8 i = 0; i < 4; i++)
		if(g_LVGM_Header->Ident[i] != g_LVGM_Ident[i])
			return FALSE;

	// Setup state
	g_LVGM_State = 0;
	if(g_LVGM_Header->Option & LVGM_OPTION_50HZ)
		g_LVGM_State |= LVGM_STATE_50HZ;
	if(loop)
		g_LVGM_State |= LVGM_STATE_LOOP;
	
	// Setup playback pointer
	g_LVGM_Pointer = (const u8*)(addr) + 5;
	if(g_LVGM_Header->Option & LVGM_OPTION_DEVICE)
		g_LVGM_Devices = *g_LVGM_Pointer++;
	else
		g_LVGM_Devices = LVGM_CHIP_PSG;

	// Setup default value
	g_LVGM_PSG_Default = 0;
	if(g_LVGM_Devices & LVGM_CHIP_PSG)
		g_LVGM_PSG_Default = *g_LVGM_Pointer++;

	// Init variables
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
	
	#if (LVGM_USE_PSG)
	PSG_Mute();
	#endif

	#if (LVGM_USE_MSXMUSIC)
	MSXMusic_Mute();
	#endif
	
	#if (LVGM_USE_MSXAUDIO)
	MSXAudio_Mute();
	#endif
	
	#if (LVGM_USE_SCC)
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
			#if (LVGM_USE_PSG)
			case LVGM_OP_PSG: // PSG / AY-3-8910
				g_LVGM_CurChip = LVGM_CHIP_PSG;
				g_LVGM_Decode = LVGM_DecodePSG;
				break;
			#endif

			#if (LVGM_USE_MSXMUSIC)
			case LVGM_OP_OPLL: // MSX-MUSIC / YM2413 / OPLL
				g_LVGM_CurChip = LVGM_CHIP_OPLL;
				g_LVGM_Decode = LVGM_DecodeOPLL;
				break;
			#endif

			#if (LVGM_USE_MSXAUDIO)
			case LVGM_OP_OPL1: // MSX-AUDIO / Y8950 / OPL1 + ADPCM
				g_LVGM_CurChip = LVGM_CHIP_OPL1;
				g_LVGM_Decode = LVGM_DecodeOPL1;
				break;
			#endif

			#if (LVGM_USE_SCC)
			case LVGM_OP_SCC: // SCC / K051649
				g_LVGM_CurChip = LVGM_CHIP_SCC;
				g_LVGM_Decode = LVGM_DecodeSCC;
				break;
			#endif

			#if (LVGM_USE_SCCI)
			case LVGM_OP_SCCI: // SCC+ / K052539
				g_LVGM_CurChip = LVGM_CHIP_SCCI;
				g_LVGM_Decode = LVGM_DecodeSCCI;
				break;
			#endif

			#if (LVGM_USE_PSG2)
			case LVGM_OP_PSG2: // 2nd PSG
				g_LVGM_CurChip = LVGM_CHIP_PSG2;
				g_LVGM_Decode = LVGM_DecodePSG2;
				break;
			#endif

			#if (LVGM_USE_OPL4)
			case LVGM_OP_OPL4: // MOONBLASTER / YMF278
				g_LVGM_CurChip = LVGM_CHIP_OPL4;
				g_LVGM_Decode = LVGM_DecodeOPL4;
				break;
			#endif

			case LVGM_OP_NOTIFY:
			#if (LVGM_USE_NOTIFY)
				if(g_LVGM_Callback(*++g_LVGM_Pointer))
					continue;
			#else
				g_LVGM_Pointer++; // Skip notification
			#endif
				break;

			case LVGM_OP_LOOP: // Loop position
			#if (LVGM_USE_NOTIFY)
				g_LVGM_Callback(0xFE);
			#endif
				g_LVGM_LoopAddr = g_LVGM_Pointer + 1;
				break;

			case LVGM_OP_END: // End of song
				if(g_LVGM_State & LVGM_STATE_LOOP) // handle loop
				{
				#if (LVGM_USE_NOTIFY)
					g_LVGM_Callback(0xFF);
				#endif
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

#if (LVGM_USE_NOTIFY)
// Default dummy callback handler
bool LVGM_Defaultcallback(u8 id) { id; return TRUE; }
#endif