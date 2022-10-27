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
#include "obsonet.h"

//=============================================================================
// OPTIONS VALIDATION
//=============================================================================

//=============================================================================
// READ-ONLY DATA
//=============================================================================

//=============================================================================
// MEMORY DATA
//=============================================================================

u8 g_ONET_Slot;
u8 g_ONET_Bios;
u8 g_ONET_Version[3];
u8 g_ONET_MAC[6];

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// ObsoNET searching routine
u8 ONET_Initialize() __naked
{
	__asm

	NEXTSL = 0xFFFD // unused System variables address

		call	SRCHOBSO

		ld		a, (_g_ONET_Slot)
		cp		#0xFF
		ret		z

		// Select the ROM page 0 by using WRSLT (see Section 3.3):
		ld		a, (_g_ONET_Slot)
		ld		e, #0xC0
		ld		hl, #0x7FE0
		call	R_WRSLT
		ld		a, (_g_ONET_Slot)
		ld		e, #0
		ld		hl, #0x7FE2
		call	R_WRSLT

		push	ix

		ld		l, #0
		ld		a, (_g_ONET_Slot)
		ld		h, a
		push	hl
		pop		iy
		ld		ix, #ONET_GET_VERS
		call	R_CALSLT
		ld		(_g_ONET_Version+0), a
		ld		a, b
		ld		(_g_ONET_Version+1), a
		ld		a, c
		ld		(_g_ONET_Version+2), a

		ld		ix, #ONET_GET_HWAD
		call	R_CALSLT
		ld		(_g_ONET_MAC+0), hl
		ld		(_g_ONET_MAC+2), de
		ld		(_g_ONET_MAC+4), bc

		ei							// because CALSLT do DI
		pop		ix

		ld		a, (_g_ONET_Slot)	// Return value
		ret

	// SRCHOBSO - ObsoNET searching routine - by Nestor Soriano
	// ObsoNET (c) 2004
	// by Daniel Berdugo (hardware) and Nestor Soriano (software)
	// 
	// Input:   -
	// Output:  A  = ObsoNET slot (#FF if not found)
	//          Cy = 1 if the ObsoNET has BIOS
	//          ObsoNET slot connected on page 1
	//          (if not found, last available slot connected on page 1)
	SRCHOBSO:
		ld		a, #0xFF			// Initializes the NEXTSLOT routine
		ld		(NEXTSL), a

	SRCHLOOP:
		call	NEXTSLOT			// Obtains the next available slot...
		cp		#0xFF				// (if no more slots are availabe, terminates)
		ret		z

		push	af
		ld		h, #0x40			// ...and conects it.
		call	R_ENASLT
		pop		af

		ld		c, a
		call	CHKOBNET			// Checks if there is an ObsoNET...

		jr		nc, SRCHLOOP		// ...if not, go to the next slot.

		// MSXgl - start
		ld		a, c
		ld		(_g_ONET_Slot), a
		// MSXgl - end

		call	CHKBIOS				// Checks if it has BIOS and terminates.
		// MSXgl - start
		// ld		a, c
		ld		a, #0				// don't use xor here to preserve carry flag
		adc		a					// store carry of shift as subtimer
		ld		(_g_ONET_Bios), a
		// MSXgl - end
		ret

	// --- Checks the presence of an ObsoNET on page 1.
	//     Input:    -
	//     Output:   Cy = 1 if there is an ObsoNET on page 1, 0 if not
	//     Modifies: AF, B
	CHKOBNET:
		ld		a, (0x7FE0)			// Save value to restore it
		ld		b, a				// if there is no ObsoNET

		xor		a					// Switches registers bank 0
		ld		(0x7FE0), a

		ld		a, (0x7FEA)			// Reads registers 10 and 11 (8019ID0/1),
		cp		#0x50				// they must return #50 and #70 respectively
		jr		nz, NO_OBNET
		ld		a, (0x7FEB)
		cp		#0x70
		jr		nz, NO_OBNET

		ld		a, #0x80			// Switches registers bank 0
		ld		(0x7FE0), a

		ld		a,(0x7FEA)			// Reads registers 10 and 11 (unused,
		cp		#0x50				// they must NOT return #50 and #70)
		scf
		ret		nz
		ld		a, (0x7FEB)
		cp		#0x70
		scf
		ret		nz

	NO_OBNET:
		ld		a, b				// No ObsoNET found: restores contents
		ld		(0x7FE0), a			// of #7FE0 in case it is RAM
		or		a
		ret

	// --- Checks if there is BIOS in the ObsoNET of page 1.
	//     Input:    -
	//     Output:   Cy = 1 if there is BIOS, 0 if not
	//     Modifies: AF, HL, DE
	CHKBIOS:
		ld		a, #0xC0			// Connects ROM page 0
		ld		(0x7FE0), a
		xor		a
		ld		(0x7FE2), a

		ld		hl, #0x7FD0
		ld		de, #OBSTRING

	CHKBLOOP: // Strings comparison loop
		ld		a, (de)
		cp		(hl)
		scf
		ccf
		ret		nz

		or		a
		scf
		ret		z

		inc		hl
		inc		de
		jr		CHKBLOOP

	OBSTRING:
		.asciz	"ObsoNET"

	// --- NEXTSLOT:
	//     Returns in A the next slot available on the system every time it is called.
	//     When no more slots are available it returns #FF.
	//     To initialize it, #FF must be written in NEXTSL.
	//     Modifies: AF, BC, HL
	NEXTSLOT:
		ld		a, (NEXTSL)
		cp		#0xFF
		jr		nz, NXTSL1
		ld		a, (M_EXPTBL)			// First slot
		and		#0b10000000
		ld		(NEXTSL), a
		ret

	NXTSL1:
		ld		a, (NEXTSL)
		cp		#0b10001111
		jr		z, NOMORESL			// No more slots?
		cp		#0b00000011
		jr		z, NOMORESL
		bit		7, a
		jr		nz, SLTEXP

	SLTSIMP:
		and		#0b00000011			// Simple slot
		inc		a
		ld		c, a
		ld		b, #0
		ld		hl, #M_EXPTBL
		add		hl, bc
		ld		a, (hl)
		and		#0b10000000
		or		c
		ld		(NEXTSL), a
		ret

	SLTEXP:
		ld		c, a				// Expanded slot
		and		#0b00001100
		cp		#0b00001100
		ld		a, c
		jr		z, SLTSIMP
		add		#0b00000100
		ld		(NEXTSL), a
		ret

	NOMORESL:
		ld		a, #0xFF
		ret

	// NEXTSL:
		// .db		#0xFF				// Last returned slot
	__endasm;
}

//-----------------------------------------------------------------------------
// Initializes the ObsoNET card
void ONET_Reset()
{
	
}

//-----------------------------------------------------------------------------
// Obtains the network connection status
u8 ONET_GetStatus()
{
	
}

//-----------------------------------------------------------------------------
// Activates or deactivates ObsoNET
void ONET_Activate(bool activate)
{
	
}

//-----------------------------------------------------------------------------
// Activates or deactivates ObsoNET / Obtain the current state
bool ONET_IsActive()
{
	
}

//-----------------------------------------------------------------------------
// Configuration of the reception parameters
void ONET_SetReception(u8 flag)
{
	
}

//-----------------------------------------------------------------------------
// Configuration of the multicast mask
void ONET_SetMulticastMask(u16 addr)
{
	
}

//-----------------------------------------------------------------------------
// Configuration of the multicast mask
u16 ONET_GetMulticastMask()
{
	
}

//-----------------------------------------------------------------------------
// Obtains information about the oldest received packet
const c8* ONET_GetPacketInfo()
{
	
}

//-----------------------------------------------------------------------------
// Obtains the oldest captured incoming packet
u8 ONET_GetPacket(u16 addr, u16* size)
{
	
}

//-----------------------------------------------------------------------------
// Sends a packet to the network (Synchronous execution)
u8 ONET_SendPacketSynch(u16 addr, u16 size)
{
	
}

//-----------------------------------------------------------------------------
// Sends a packet to the network (Asynchronous execution)
u8 ONET_SendPacketAsynch(u16 addr, u16 size)
{
	
}

//-----------------------------------------------------------------------------
// Obtains the status of the packet transmission
u8 ONET_GetSendStatus()
{
	
}