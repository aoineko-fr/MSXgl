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

const MGLV_Header* g_MGLV_Header;
const u8* g_MGLV_Start;
const u8* g_MGLV_Pointer;
u16 g_MGLV_VRAMAddr;
MGLV_EventCallback g_MGLV_EventCallback = MGLV_DefaultEventCallback;
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
	g_MGLV_Header = (MGLV_Header*)addr;
	g_MGLV_Start = addr;
	g_MGLV_Timer = 5;
	g_MGLV_Loop = FALSE;
	#if (MGLV_HAS_HEADER)
		if (g_MGLV_Header->Flag & MGLV_IMAGE_HEADER)
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
	g_MGLV_Counter = g_MGLV_Timer;

	return TRUE;
}

// //-----------------------------------------------------------------------------
// // Set VRAM 14-bits address 
// void MGLV_SetVRAM_14B(u16 dest) __PRESERVES(b, c, d, e, iyl, iyh)
// {
// 	dest; // HL
// 	__asm
// 		ld		a, l
// 		out		(P_VDP_ADDR), a			// RegPort = (dest & 0x00FF);
// 		ld		a, h
// 		and		a, #0x3F
// 		or		a, #F_VDP_WRIT
// 		out		(P_VDP_ADDR), a			// RegPort = ((dest >> 8) & 0x3F) + F_VDP_WRIT;
// 	__endasm;
// }

//-----------------------------------------------------------------------------
// Set VRAM 16-bits address 
void MGLV_SetVRAM_16B(u16 dest) __NAKED __PRESERVES(b, c, d, e, iyl, iyh)
{
	dest; // HL

__asm
	ld		a, h
	rlca
	rlca
	and		a, #0x03
	di // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	out		(P_VDP_ADDR), a			// RegPort = (page << 2) + (dest >> 14);
	ld		a, #VDP_REG(14)
	out		(P_VDP_ADDR), a			// RegPort = VDP_REG(14);
	
	ld		a, l
	out		(P_VDP_ADDR), a			// RegPort = (dest & 0x00FF);
	ld		a, h
	and		a, #0x3F
	or		a, #F_VDP_WRIT
	ei // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	out		(P_VDP_ADDR), a			// RegPort = ((dest >> 8) & 0x3F) + F_VDP_WRIT;
	ret
__endasm;
}

// //-----------------------------------------------------------------------------
// // Set VRAM 17-bits address 
// void MGLV_SetVRAM_17B(u32 dest) __NAKED __PRESERVES(b, c, iyl, iyh)
// {
// 	dest; // HL:DE
// 	__asm
// 		// Setup address register 
// 		ld		a, l
// 		add		a, a
// 		add		a, a
// 		ld		h, a
// 		ld		a, d
// 		rlca
// 		rlca
// 		and		a, #0x03
// 		add		a, h
// 		out		(P_VDP_ADDR), a			// RegPort = (page << 2) + (dest >> 14);
// 		ld		a, #VDP_REG(14)
// 		out		(P_VDP_ADDR), a			// RegPort = VDP_REG(14);
		
// 		ld		a, e
// 		out		(P_VDP_ADDR), a			// RegPort = (dest & 0x00FF);
// 		ld		a, d
// 		and		a, #0x3F
// 		or		a, #F_VDP_WRIT
// 		out		(P_VDP_ADDR), a			// RegPort = ((dest >> 8) & 0x3F) + F_VDP_WRIT;
// 		ret
// 	__endasm;
// }

//-----------------------------------------------------------------------------
// Fill VRAM area with a given value [MSX2/2+/TR]
//
// Parameters:
//   value		- Byte value to copy in VRAM
//   count		- Number of byte to copy in VRAM
void MGLV_FillVRAM(u8 value, u16 count) __NAKED __PRESERVES(c, h, l, iyl, iyh)
{
	value; // A
	count; // DE

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


void MGLV_FillVRAM_256(u8 value) __PRESERVES(a, b, c, d, e, h, l, iyl, iyh);
void MGLV_FillVRAM_128(u8 value) __PRESERVES(a, b, c, d, e, h, l, iyl, iyh);
void MGLV_FillVRAM_64(u8 value) __PRESERVES(a, b, c, d, e, h, l, iyl, iyh);
void MGLV_FillVRAM_32(u8 value) __PRESERVES(a, b, c, d, e, h, l, iyl, iyh);
void MGLV_FillVRAM_16(u8 value) __PRESERVES(a, b, c, d, e, h, l, iyl, iyh);
void MGLV_FillVRAM_8(u8 value) __PRESERVES(a, b, c, d, e, h, l, iyl, iyh);

void MGLV_FillVRAM_X(u8 value, u8 count) __NAKED __PRESERVES(a, b, c, iyl, iyh)
{
	value; // A
	count; // L

__asm

	ld		h, #0					//  8|2
	ld		d, h					//  5|1
	ld		e, l					//  5|1
	add		hl, hl					// 12|1 cound*2
	add		hl, de 					// 12|1 cound*3
	ld		de, #_MGLV_FillVRAM_256	// 11|3
	add		hl, de					// 12|1 table+cound*3
	jp		(hl)					//  5|1

_MGLV_FillVRAM_256:
.rept 128
	out		(P_VDP_DATA), a			// 12|2
	nop								//  5|1
.endm
_MGLV_FillVRAM_128:
.rept 64
	out		(P_VDP_DATA), a			// 12|2
	nop								//  5|1
.endm
_MGLV_FillVRAM_64:
.rept 32
	out		(P_VDP_DATA), a			// 12|2
	nop								//  5|1
.endm
_MGLV_FillVRAM_32:
.rept 16
	out		(P_VDP_DATA), a			// 12|2
	nop								//  5|1
.endm
_MGLV_FillVRAM_16:
.rept 8
	out		(P_VDP_DATA), a			// 12|2
	nop								//  5|1
.endm
_MGLV_FillVRAM_8:
.rept 8
	out		(P_VDP_DATA), a			// 12|2
	nop								//  5|1
.endm
	ret
__endasm;
}

//-----------------------------------------------------------------------------
// Write data from RAM to VRAM [MSX2/2+/TR]
//
// Parameters:
//   src		- Source data address in RAM
//   count		- Number of byte to copy in VRAM
void MGLV_WriteVRAM(const u8* src, u16 count) __NAKED __PRESERVES(iyl, iyh)
{
	src;   // HL
	count; // DE

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
	ret
__endasm;
}

void MGLV_CopyVRAM_X(u16 count, const u8* src) __NAKED
{
	count; // A
	src;   // DE

__asm

	// ld		h, #0					//  8|2
	// ld		l, a					//  5|1
	add		hl, hl					// 12|1 cound*2
	ld		bc, #_MGLV_CopyVRAM_256	// 11|3
	add		hl, bc					// 12|1 table+cound*2
	ex		de, hl					//  5|1
	ld		c, #P_VDP_DATA			//  8|2 set IO port
	ld______iyh_d					// 10|2
	ld______iyl_e					// 10|2
	jp		(iy)					// 10|2

_MGLV_CopyVRAM_256:
.rept 256
	outi							// 18|2
.endm

__endasm;
}

//-----------------------------------------------------------------------------
// Decode a frame of movie
void MGLV_Decode()
{
	if (g_MGLV_Counter < g_MGLV_Timer)
		return;

	if (g_MGLV_Counter == g_MGLV_Timer)
		VDP_SetColor(COLOR_BLACK);
	else
		VDP_SetColor(COLOR_DARK_RED);

	g_MGLV_Counter = 0;
	MGLV_SetVRAM_16B(g_MGLV_VRAMAddr);

mglvDecodeLoop:

	while (1)
	{
		u8 cmd = *g_MGLV_Pointer & 0xF;
		switch (cmd)
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
			MGLV_SetVRAM_16B(g_MGLV_VRAMAddr);
			break;
		}
		case MGLV_CMD_SKIP_8B:		// 05	nn				Skip nn bytes(1 - 255)
		{
			u8 nn = *++g_MGLV_Pointer;
			g_MGLV_VRAMAddr += nn;
			MGLV_SetVRAM_16B(g_MGLV_VRAMAddr);
			break;
		}
		case MGLV_CMD_SKIP_16B:		// 06	nnnn			Skip nnnn bytes(1 - 65535)
		{
			u16 nnnn = *(u16*)++g_MGLV_Pointer;
			g_MGLV_VRAMAddr += nnnn;
			MGLV_SetVRAM_16B(g_MGLV_VRAMAddr);
			g_MGLV_Pointer++;
			break;
		}
		case MGLV_CMD_SKIP_FRAME:	// 07					Skip a frame / End of frame
		{
			g_MGLV_VRAMAddr = VRAM_START_ADDR;
			MGLV_SetVRAM_16B(g_MGLV_VRAMAddr);
			g_MGLV_Pointer++;
			return;
		}

		//---------- FILL ----------
		case MGLV_CMD_FILL_4B:		// n8	vv				Fill n bytes(1 - 15) with vv value
		{
			u8 n = *g_MGLV_Pointer >> 4;
			u8 vv = *++g_MGLV_Pointer;
			MGLV_FillVRAM_X(vv, 256-n);
			g_MGLV_VRAMAddr += n;
			break;
		}
		case MGLV_CMD_FILL_8B:		// 09	vv,nn			Fill nn bytes(1 - 255) with vv value
		{
			u8 nn = *++g_MGLV_Pointer;
			u8 vv = *++g_MGLV_Pointer;
			MGLV_FillVRAM_X(vv, 256-nn);
			g_MGLV_VRAMAddr += nn;
			break;
		}
		case MGLV_CMD_FILL_16B:		// 0A	vv,nnnn			Fill nnnn bytes(1 - 65535) with vv value
		{
			u16 nnnn = *(u16*)++g_MGLV_Pointer;
			g_MGLV_Pointer += 2;
			u8 vv = *g_MGLV_Pointer;
			MGLV_FillVRAM(vv, nnnn);
			g_MGLV_VRAMAddr += nnnn;
			break;
		}
		case MGLV_CMD_FILL_FRAME:	// 0B	vv				Fill a full frame with vv value
		{
			u8 vv = *++g_MGLV_Pointer;
			g_MGLV_VRAMAddr = VRAM_START_ADDR;
			MGLV_FillVRAM(vv, (u16)256 * 144);
			break;
		}

		//---------- COPY ----------
		case MGLV_CMD_COPY_4B:		// nC	vv[n]			Copy n bytes(1 - 15) from vv[n] data table
		{
			u8 n = *g_MGLV_Pointer >> 4;
			g_MGLV_Pointer++;
			MGLV_CopyVRAM_X(256-n, g_MGLV_Pointer);
			// MGLV_WriteVRAM(g_MGLV_Pointer, n);
			g_MGLV_Pointer += n;
			g_MGLV_VRAMAddr += n;
			goto mglvDecodeLoop;
		}
		case MGLV_CMD_COPY_8B:		// 0D	nn,vv[nn]		Copy nn bytes(1 - 255) from vv[nn] data table
		{
			u8 nn = *++g_MGLV_Pointer;
			g_MGLV_Pointer++;
			MGLV_CopyVRAM_X(256-nn, g_MGLV_Pointer);
			// MGLV_WriteVRAM(g_MGLV_Pointer, nn);
			g_MGLV_Pointer += nn;
			g_MGLV_VRAMAddr += nn;
			goto mglvDecodeLoop;
		}
		case MGLV_CMD_COPY_16B:		// 0E	nnnn,vv[nnnn]	Copy nnnn bytes(1 - 65535) from vv[nnnn] data table
		{
			u16 nnnn = *(u16*)++g_MGLV_Pointer;
			g_MGLV_Pointer += 2;
			MGLV_WriteVRAM(g_MGLV_Pointer, nnnn);
			g_MGLV_Pointer += nnnn;
			g_MGLV_VRAMAddr += nnnn;
			goto mglvDecodeLoop;
		}
		case MGLV_CMD_COPY_FRAME:	// 0F	vv[]			Copy a full frame from data table(raw frame)
		{
			g_MGLV_VRAMAddr = VRAM_START_ADDR;
			MGLV_WriteVRAM(++g_MGLV_Pointer, (u16)256 * 144);
			g_MGLV_Pointer += (u16)256 * 144 - 1;
			break;
		}
		}
		g_MGLV_Pointer++;
	}
}
