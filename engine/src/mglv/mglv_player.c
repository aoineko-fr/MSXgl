// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄  ▄▄ ▄  ▄▄▄  ▄ ▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██  ██ █ ██   ██▀█
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▄ ▀█▀  ▀█▄█ ██ █
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
#include "mglv_player.h"
#include "vdp.h"

//=============================================================================
// DEFINES
//=============================================================================

#define VRAM_START_ADDR (24 * 128)

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// File ident data
const u8 g_MGLV_Ident[3] = { 'M', 'G', 'V' };

// Function prototypes
void MGLV_DefaultEventCallback(u8 cmd);

//=============================================================================
// MEMORY DATA
//=============================================================================

const struct MGLV_Header* g_MGLV_Header;
const u8* g_MGLV_Start;
const u8* g_MGLV_Pointer;
u16 g_MGLV_VRAMAddr;
MGLV_EventCallback g_MGLV_EventCallback = MGLV_DefaultEventCallback;
bool g_MGLV_SetVRAM;
u8 g_MGLV_Counter;
u8 g_MGLV_Timer;
bool g_MGLV_Loop;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Function to be called in the interruption handler
void MGLV_VBlankHandler()
{
	g_MGLV_Counter++;
}

//-----------------------------------------------------------------------------
// Default event callback
void MGLV_DefaultEventCallback(u8 cmd)
{
	cmd;
}

//-----------------------------------------------------------------------------
// Start movie playback.
bool MGLV_Play(const void* addr)
{
	g_MGLV_Header = (struct MGLV_Header*)addr;
	g_MGLV_Start = addr;
	g_MGLV_Timer = 5;
	g_MGLV_Loop = FALSE;
	#if (MGLV_HAS_HEADER)
		if(g_MGLV_Header->Flag & MGLV_IMAGE_HEADER)
		{
			g_MGLV_Start += 8;
			g_MGLV_Timer = g_MGLV_Header->Replay & MGLV_FRAME_SKIP_MASK;
			if (g_MGLV_Header->Replay & MGLV_LOOP)
				g_MGLV_Loop = TRUE;
		}
		else
		{
			g_MGLV_Start += 4;
		}
	#endif

	g_MGLV_Pointer = g_MGLV_Start; 
	g_MGLV_VRAMAddr = VRAM_START_ADDR;
	g_MGLV_SetVRAM = TRUE;
	g_MGLV_Counter = g_MGLV_Timer;

	return TRUE;
}

//-----------------------------------------------------------------------------
// Set VRAM 14-bits address 
void MGLV_SetVRAM_14B(u16 dest) __PRESERVES(b, c, d, e, iyl, iyh)
{
	dest; // HL
	__asm
		ld		a, l
		out		(P_VDP_ADDR), a			// RegPort = (dest & 0x00FF);
		ld		a, h
		and		a, #0x3F
		or		a, #F_VDP_WRIT
		out		(P_VDP_ADDR), a			// RegPort = ((dest >> 8) & 0x3F) + F_VDP_WRIT;
	__endasm;
}

//-----------------------------------------------------------------------------
// Set VRAM 16-bits address 
void MGLV_SetVRAM_16B(u16 dest) __PRESERVES(b, c, d, e, iyl, iyh)
{
	dest; // HL
	__asm
		ld		a, h
		rlca
		rlca
		and		a, #0x03
		out		(P_VDP_ADDR), a			// RegPort = (page << 2) + (dest >> 14);
		ld		a, #VDP_REG(14)
		out		(P_VDP_ADDR), a			// RegPort = VDP_REG(14);
		
		ld		a, l
		out		(P_VDP_ADDR), a			// RegPort = (dest & 0x00FF);
		ld		a, h
		and		a, #0x3F
		or		a, #F_VDP_WRIT
		out		(P_VDP_ADDR), a			// RegPort = ((dest >> 8) & 0x3F) + F_VDP_WRIT;
	__endasm;
}

//-----------------------------------------------------------------------------
// Set VRAM 17-bits address 
void MGLV_SetVRAM_17B(u32 dest) __PRESERVES(b, c, iyl, iyh)
{
	dest; // HL:DE
	__asm
		// Setup address register 
		ld		a, l
		add		a, a
		add		a, a
		ld		h, a
		ld		a, d
		rlca
		rlca
		and		a, #0x03
		add		a, h
		out		(P_VDP_ADDR), a			// RegPort = (page << 2) + (dest >> 14);
		ld		a, #VDP_REG(14)
		out		(P_VDP_ADDR), a			// RegPort = VDP_REG(14);
		
		ld		a, e
		out		(P_VDP_ADDR), a			// RegPort = (dest & 0x00FF);
		ld		a, d
		and		a, #0x3F
		or		a, #F_VDP_WRIT
		out		(P_VDP_ADDR), a			// RegPort = ((dest >> 8) & 0x3F) + F_VDP_WRIT;
	__endasm;
}

//-----------------------------------------------------------------------------
// Fill VRAM area with a given value [MSX2/2+/TR]
//
// Parameters:
//   value		- Byte value to copy in VRAM
//   count		- Nomber of byte to copy in VRAM
void MGLV_FillVRAM(u8 value, u16 count) __PRESERVES(c, h, l, iyl, iyh)
{
	value;		// A
	count;		// DE

	__asm
		// fast 16-bits loop
		ld		b, e					// number of loops is in DE
		dec		de						// calculate DB value (destroys B, D and E)
		inc		d
	fll_loop_start:
		out		(P_VDP_DATA), a			// fill			12cc
		djnz	fll_loop_start			//				14cc
		dec		d
		jp		nz, fll_loop_start
	__endasm;
}

//-----------------------------------------------------------------------------
// Write data from RAM to VRAM [MSX2/2+/TR]
//
// Parameters:
//   src		- Source data address in RAM
//   count		- Nomber of byte to copy in VRAM
void MGLV_WriteVRAM(const u8* src, u16 count) __PRESERVES(iyl, iyh)
{
	src;      // HL
	count;    // DE

	__asm
		ld		c, #P_VDP_DATA			// data register
		// Handle count LSB
		ld		a, e					// count LSB
		and		a
		jp		z, wrt_loop_init		// skip LSB
		ld		b, a					// send (count & 0x00FF) bytes
		otir
		// Handle count MSB
	wrt_loop_init:
		ld		a, d					// count MSB
	wrt_loop_start:
		and		a
		jp		z, wrt_loop_end			// finished
		// ld		b, #0					// send 256 bytes packages
		// otir
	.rept 256
		outi							// 18 t-states
	.endm
		dec		a
		jp		wrt_loop_start
	wrt_loop_end:
	__endasm;
}

//-----------------------------------------------------------------------------
// Decode a frame of movie
void MGLV_Decode()
{
	if (g_MGLV_Counter < g_MGLV_Timer)
		return;

	g_MGLV_Counter = 0;

mglvDecodeLoop:

	while(1)
	{
		u8 cmd = *g_MGLV_Pointer & 0xF;
		switch(cmd)
		{
		//---------- EVENTS ----------
		case MGLV_CMD_END_VIDEO:	// 00					End of data (handle loop if needed)
		{
			if (g_MGLV_Loop)
			{
				g_MGLV_Pointer = g_MGLV_Start;
				g_MGLV_EventCallback(cmd);
				goto mglvDecodeLoop;
			}
			return;
		}
		case MGLV_CMD_END_SEGMENT:	// 01					End of segment (increment segment index and reset data pointer)
		{
			g_MGLV_EventCallback(cmd);
			goto mglvDecodeLoop;
		}
		case MGLV_CMD_END_LINE:		// 02					End of line
		{
			break;
		}
		case MGLV_CMD_END_TABLE:	// x3					End table and switch to next one
		{
			break;
		}

		//---------- SKIP ----------
		case MGLV_CMD_SKIP_4B:		// n4					Skip n bytes(1 - 15)
		{
			u8 n = *g_MGLV_Pointer >> 4;
			g_MGLV_VRAMAddr += n;
			g_MGLV_SetVRAM = TRUE;
			break;
		}
		case MGLV_CMD_SKIP_8B:		// 05	nn				Skip nn bytes(1 - 255)
		{
			u8 nn = *++g_MGLV_Pointer;
			g_MGLV_VRAMAddr += nn;
			g_MGLV_SetVRAM = TRUE;
			break;
		}
		case MGLV_CMD_SKIP_16B:		// 06	nnnn			Skip nnnn bytes(1 - 65535)
		{
			u16 nnnn = *(u16*)++g_MGLV_Pointer;
			g_MGLV_VRAMAddr += nnnn;
			g_MGLV_SetVRAM = TRUE;
			g_MGLV_Pointer++;
			break;
		}
		case MGLV_CMD_SKIP_FRAME:	// 07					Skip a frame / End of frame
		{
			g_MGLV_VRAMAddr = VRAM_START_ADDR;
			g_MGLV_SetVRAM = TRUE;
			g_MGLV_Pointer++;
			return;
		}

		//---------- FILL ----------
		case MGLV_CMD_FILL_4B:		// n8	vv				Fill n bytes(1 - 15) with vv value
		{
			u8 n = *g_MGLV_Pointer >> 4;
			u8 vv = *++g_MGLV_Pointer;
			if (g_MGLV_SetVRAM)
			{
				MGLV_SetVRAM_16B(g_MGLV_VRAMAddr);
				g_MGLV_SetVRAM = FALSE;
			}
			MGLV_FillVRAM(vv, n);
			g_MGLV_VRAMAddr += n;
			break;
		}
		case MGLV_CMD_FILL_8B:		// 09	vv,nn			Fill nn bytes(1 - 255) with vv value
		{
			u8 nn = *++g_MGLV_Pointer;
			u8 vv = *++g_MGLV_Pointer;
			if (g_MGLV_SetVRAM)
			{
				MGLV_SetVRAM_16B(g_MGLV_VRAMAddr);
				g_MGLV_SetVRAM = FALSE;
			}
			MGLV_FillVRAM(vv, nn);
			g_MGLV_VRAMAddr += nn;
			break;
		}
		case MGLV_CMD_FILL_16B:		// 0A	vv,nnnn			Fill nnnn bytes(1 - 65535) with vv value
		{
			u16 nnnn = *(u16*)++g_MGLV_Pointer;
			g_MGLV_Pointer += 2;
			u8 vv = *g_MGLV_Pointer;
			if (g_MGLV_SetVRAM)
			{
				MGLV_SetVRAM_16B(g_MGLV_VRAMAddr);
				g_MGLV_SetVRAM = FALSE;
			}
			MGLV_FillVRAM(vv, nnnn);
			g_MGLV_VRAMAddr += nnnn;
			break;
		}
		case MGLV_CMD_FILL_FRAME:	// 0B	vv				Fill a full frame with vv value
		{
			u8 vv = *++g_MGLV_Pointer;
			g_MGLV_VRAMAddr = VRAM_START_ADDR;
			if (g_MGLV_SetVRAM)
			{
				MGLV_SetVRAM_16B(g_MGLV_VRAMAddr);
				g_MGLV_SetVRAM = FALSE;
			}
			MGLV_FillVRAM(vv, (u16)256 * 144);
			break;
		}

		//---------- COPY ----------
		case MGLV_CMD_COPY_4B:		// nC	vv[n]			Copy n bytes(1 - 15) from vv[n] data table
		{
			u8 n = *g_MGLV_Pointer >> 4;
			if (g_MGLV_SetVRAM)
			{
				MGLV_SetVRAM_16B(g_MGLV_VRAMAddr);
				g_MGLV_SetVRAM = FALSE;
			}
			MGLV_WriteVRAM(++g_MGLV_Pointer, n);
			g_MGLV_Pointer += n;
			g_MGLV_VRAMAddr += n;
			goto mglvDecodeLoop;
		}
		case MGLV_CMD_COPY_8B:		// 0D	nn,vv[nn]		Copy nn bytes(1 - 255) from vv[nn] data table
		{
			u8 nn = *++g_MGLV_Pointer;
			if (g_MGLV_SetVRAM)
			{
				MGLV_SetVRAM_16B(g_MGLV_VRAMAddr);
				g_MGLV_SetVRAM = FALSE;
			}
			MGLV_WriteVRAM(++g_MGLV_Pointer, nn);
			g_MGLV_Pointer += nn;
			g_MGLV_VRAMAddr += nn;
			goto mglvDecodeLoop;
		}
		case MGLV_CMD_COPY_16B:		// 0E	nnnn,vv[nnnn]	Copy nnnn bytes(1 - 65535) from vv[nnnn] data table
		{
			u16 nnnn = *(u16*)++g_MGLV_Pointer;
			g_MGLV_Pointer += 2;
			if (g_MGLV_SetVRAM)
			{
				MGLV_SetVRAM_16B(g_MGLV_VRAMAddr);
				g_MGLV_SetVRAM = FALSE;
			}
			MGLV_WriteVRAM(g_MGLV_Pointer, nnnn);
			g_MGLV_Pointer += nnnn;
			g_MGLV_VRAMAddr += nnnn;
			goto mglvDecodeLoop;
		}
		case MGLV_CMD_COPY_FRAME:	// 0F	vv[]			Copy a full frame from data table(raw frame)
		{
			g_MGLV_VRAMAddr = VRAM_START_ADDR;
			if (g_MGLV_SetVRAM)
			{
				MGLV_SetVRAM_16B(g_MGLV_VRAMAddr);
				g_MGLV_SetVRAM = FALSE;
			}
			MGLV_WriteVRAM(++g_MGLV_Pointer, (u16)256 * 144);
			g_MGLV_Pointer += (u16)256 * 144 - 1;
			break;
		}
		}
		g_MGLV_Pointer++;
	}
}
