// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// PCCM Ninja-Tap Driver v2.0 for MSX (Primary & C-lib routine)
// Copyright 1994 1995 Knight's Chamber, all rights reserved.
// 	Written by Firm Knight.
//─────────────────────────────────────────────────────────────────────────────
#include "ninjatap.h"

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// INCLUDE_ASM(NTap_Dummy, "ninjatap.asm");

void NTap_Dummy()
{
	__asm

	#if (NTAP_DRIVER & NTAP_DRIVER_GIGAMIX)
		#include "ninjatap.asm"
	#endif

	#if (NTAP_DRIVER & NTAP_DRIVER_SHINOBI)
		#include "ninjatap_st.asm"
	#endif

	#if (NTAP_DRIVER & NTAP_DRIVER_MSXGL)
	//======================================
	// Connection Check
	//[E]	None
	//[R]	(NTAP Info)
	//[M]	af,bc,de,hl,ix

	//NTAP Info
	//ofset	-1 Result(Internal Work)
	//	+0 Max Player
	//	+1 ID(Port1),0-15	(Dummy)
	//	+2 ID(Port2),NotAP=255	(Dummy)

	MGL_CKNTAP::
		di
		ld		b, #0	// b=Max Player
		ld		de, #0xBF0A
		call	MGL_CHECK	// Port1 Check
		ld		c, a
		ld		e, #0x4A
		call	MGL_CHECK	// Port2 Check
		rlca
		or		c
	// Result Save
		ld		c, a
		ld		(_g_NTap_Buffer), bc
	// Restore Port (6,7,8=H)
		ld		de, #0xFF3F
		jp		MGL_PORSEL

	//======================================
	// Connection Check Sub.
	MGL_CHECK:
		call	MGL_PORSEL
		inc		b
		and		#0xCA
		out		(0xA1), a	// 678=L
		ex		af, af'		;'
		ld		a, #14
		out		(0xA0), a
		in		a, (0xA2)
		and		#0x20		// 7=H ?
		ret		z

		ld		a, #15
		out		(0xA0), a
		ex		af, af'		;'
		or		#0x30
		out		(0xA1), a	// 8=H
		ld		a, #14
		out		(0xA0), a
		in		a, (0xA2)
		and		#0x20		// 7=L ?
		jr		nz, MGL_CHECK1

		inc		a
		inc		b
		inc		b
		inc		b
		ret

	// No NTAP
	MGL_CHECK1:
		xor		a
		ret

	//======================================
	// Sub Routine
	// [E]	D: Mask, E: Flags
	MGL_PORSEL:
		ld		a, #15
		out		(0xA0), a
		in		a, (0xA2)
		and		d
		or		e
		out		(0xA1), a
		ret
	#endif

	__endasm;
}

//=============================================================================
// MEMORY DATA
//=============================================================================

u8 g_NTap_Info;
u8 g_NTap_Buffer[2];
u8 g_NTap_Data[8];
#if (NTAP_USE_PREVIOUS)
u8 g_NTap_Prev[8];
#endif

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
//
void NTap_BufferToInfo()
{
	g_NTap_Info = g_NTap_Buffer[1];
	if(g_NTap_Buffer[0] & 1)
		g_NTap_Info |= NTAP_TYPE_NINJA << 4;
	if(g_NTap_Buffer[0] & 2)
		g_NTap_Info |= NTAP_TYPE_NINJA << 6;
}

#if (NTAP_DRIVER & NTAP_DRIVER_MSXGL)
//-----------------------------------------------------------------------------
// Check the presence of Ninja Tap in the joystick ports.
u8 NTap_CheckMGL()
{
	__asm
		call	MGL_CKNTAP
	__endasm;

	NTap_BufferToInfo();
	return g_NTap_Info;
}
#endif

#if (NTAP_DRIVER & NTAP_DRIVER_GIGAMIX)
//-----------------------------------------------------------------------------
// Check the presence of Ninja Tap in the joystick ports.
u8 NTap_CheckDM()
{
	__asm
		call	CKNTAP
	__endasm;

	NTap_BufferToInfo();
	return g_NTap_Info;
}
#endif

#if (NTAP_DRIVER & NTAP_DRIVER_SHINOBI)
//-----------------------------------------------------------------------------
// Check the presence of Ninja Tap in the joystick ports. Danjovic version.
u8 NTap_CheckST()
{
	__asm
		call	ST_CHECKPLYRS
		ld		a, b
		ld		(_g_NTap_Info+0), a
	__endasm;

	g_NTap_Buffer[0] = 0;
	if(g_NTap_Info & NTAP_PORT1_MASK)
		g_NTap_Buffer[0] |= 0x01;
	if(g_NTap_Info & NTAP_PORT2_MASK)
		g_NTap_Buffer[0] |= 0x02;

	g_NTap_Buffer[1] = g_NTap_Info & 0x0F;

	return g_NTap_Info;
}
#endif

//-----------------------------------------------------------------------------
// Update the status of all joysticks whether they are connected to a Ninja Tap or directly to the port.
void NTap_Update()
{
	#if (NTAP_USE_PREVIOUS)
	loop(i, 8)
		g_NTap_Prev[i] = g_NTap_Data[i];
	#endif

	__asm
		ld		hl, #_g_NTap_Data
		call	GTNTAP
	__endasm;
}