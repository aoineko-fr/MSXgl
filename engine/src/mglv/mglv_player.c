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


//=============================================================================
// READ-ONLY DATA
//=============================================================================

// File ident data
const u8  g_MGLV_Ident[3] = { 'M', 'G', 'V' };

//=============================================================================
// MEMORY DATA
//=============================================================================

const struct MGLV_Header* g_MGLV_Header;
const u8* g_MGLV_Start;
const u8* g_MGLV_Pointer;
u16 g_MGLV_VRAMAddr;

//=============================================================================
// FUNCTIONS
//=============================================================================

// Function: MGLV_Play
// Start movie playback.
bool MGLV_Play(const void* addr)
{
	g_MGLV_Header = (struct MGLV_Header*)addr;
	g_MGLV_Start = addr;
	#if (MGLV_HAS_HEADER)
		if(g_MGLV_Header->Flag & MGLV_IMAGE_HEADER)
			g_MGLV_Start += 8;
		else
			g_MGLV_Start += 4;
	#endif

	g_MGLV_Pointer = g_MGLV_Start; 
	g_MGLV_VRAMAddr = 0x0000;

	return TRUE;
}

// Function: MGLV_Decode
// Decode a frame of movie
void MGLV_Decode()
{
	while(1)
	{
		u8 cmd = *g_MGLV_Pointer & 0xF;
		switch(cmd)
		{
		case MGLV_CMD_END_VIDEO:	// 00					End of data
		{
			// if (loop)
				g_MGLV_Pointer = g_MGLV_Start;
			return;
		}
		case MGLV_CMD_END_SEGMENT:	// 01					End of segment(increment segment index and reset data pointer)
		{
			g_MGLV_Pointer = 0xA008;
			break;
		}
		case MGLV_CMD_END_LINE:		// 02					End of line
		{
			break;
		}
		case MGLV_CMD_END_TABLE:	// x3					End table and switch to next one
		{
			break;
		}
		case MGLV_CMD_SKIP_4B:		// n4					Skip n bytes(1 - 15)
		{
			u8 n = *g_MGLV_Pointer >> 4;
			g_MGLV_VRAMAddr += n;
			break;
		}
		case MGLV_CMD_SKIP_8B:		// 05	nn				Skip nn bytes(1 - 255)
		{
			u8 nn = *++g_MGLV_Pointer;
			g_MGLV_VRAMAddr += nn;
			break;
		}
		case MGLV_CMD_SKIP_16B:		// 06	nnnn			Skip nnnn bytes(1 - 65535)
		{
			u16 nnnn = *(u16*)++g_MGLV_Pointer;
			g_MGLV_VRAMAddr += nnnn;
			g_MGLV_Pointer++;
			break;
		}
		case MGLV_CMD_SKIP_FRAME:	// 07					Skip a frame / End of frame
		{
			g_MGLV_VRAMAddr = 0x0000;
			break;
		}
		case MGLV_CMD_FILL_4B:		// n8	vv				Fill n bytes(1 - 15) with vv value
		{
			u8 n = *g_MGLV_Pointer >> 4;
			u8 vv = *++g_MGLV_Pointer;
			VDP_FillVRAM_128K(vv, g_MGLV_VRAMAddr, 0, n);
			g_MGLV_VRAMAddr += n;
			break;
		}
		case MGLV_CMD_FILL_8B:		// 09	vv,nn			Fill nn bytes(1 - 255) with vv value
		{
			u8 vv = *++g_MGLV_Pointer;
			u8 nn = *++g_MGLV_Pointer;
			VDP_FillVRAM_128K(vv, g_MGLV_VRAMAddr, 0, nn);
			g_MGLV_VRAMAddr += nn;
			break;
		}
		case MGLV_CMD_FILL_16B:		// 0A	vv,nnnn			Fill nnnn bytes(1 - 65535) with vv value
		{
			u8 vv = *++g_MGLV_Pointer;
			u16 nnnn = *(u16*)++g_MGLV_Pointer;
			VDP_FillVRAM_128K(vv, g_MGLV_VRAMAddr, 0, nnnn);
			g_MGLV_Pointer++;
			g_MGLV_VRAMAddr += nnnn;
			break;
		}
		case MGLV_CMD_FILL_FRAME:	// 0B	vv				Fill a full frame with vv value
		{
			u8 vv = *++g_MGLV_Pointer;
			g_MGLV_VRAMAddr = 0x0000;
			VDP_FillVRAM_128K(vv, g_MGLV_VRAMAddr, 0, 256 * 144);
			break;
		}
		case MGLV_CMD_COPY_4B:		// nC	vv[n]			Copy n bytes(1 - 15) from vv[n] data table
		{
			u8 n = *g_MGLV_Pointer >> 4;
			VDP_WriteVRAM_128K(++g_MGLV_Pointer, g_MGLV_VRAMAddr, 0, n);
			g_MGLV_Pointer += n - 1;
			g_MGLV_VRAMAddr += n;
			break;
		}
		case MGLV_CMD_COPY_8B:		// 0D	nn,vv[nn]		Copy nn bytes(1 - 255) from vv[nn] data table
		{
			u8 nn = *++g_MGLV_Pointer;
			VDP_WriteVRAM_128K(++g_MGLV_Pointer, g_MGLV_VRAMAddr, 0, nn);
			g_MGLV_Pointer += nn - 1;
			g_MGLV_VRAMAddr += nn;
			break;
		}
		case MGLV_CMD_COPY_16B:		// 0E	nnnn,vv[nnnn]	Copy nnnn bytes(1 - 65535) from vv[nnnn] data table
		{
			u16 nnnn = *(u16*)++g_MGLV_Pointer;
			VDP_WriteVRAM_128K(++g_MGLV_Pointer, g_MGLV_VRAMAddr, 0, nnnn);
			g_MGLV_Pointer += nnnn - 1;
			g_MGLV_VRAMAddr += nnnn;
			break;
		}
		case MGLV_CMD_COPY_FRAME:	// 0F	vv[]			Copy a full frame from data table(raw frame)
		{
			g_MGLV_VRAMAddr = 0x0000;
			VDP_WriteVRAM_128K(++g_MGLV_Pointer, g_MGLV_VRAMAddr, 0, 256 * 144);
			g_MGLV_Pointer += 256 * 144 - 1;
			break;
		}
		}
		g_MGLV_Pointer++;
	}
}
