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
// DEFINES
//=============================================================================

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// INCLUDE_ASM(NTap_Dummy, "ninjatap.asm");

void NTap_Dummy()
{
	__asm
		#include "ninjatap.asm"

	CHECKPLYRS::
	// Return the number of players connected
	// [E] None
	// [R] C Tap connected info
	//     B number of players connected
		call	CHECKTAPS
		ld		b, #0
		ld		hl, #TBPLRS
		add		hl, bc
		ld		b, (hl)
		ret
		
	TBPLRS: // unknown count as 1
		.db  8, 8, 5, 5, 8, 8, 5, 5, 5, 5, 2, 2, 5, 5, 2, 2
		//   a1 a0 b1 b0   Tap a, Tapb   Count
		//   0  0  0  0     4      4       8
		//   0  0  0  1     4      4       8
		//   0  0  1  0     4      1       5
		//   0  0  1  1     4      1       5
		//   0  1  0  0     4      4       8
		//   0  1  0  1     4      4       8
		//   0  1  1  0     4      1       5
		//   0  1  1  1     4      1       5
		//   1  0  0  0     1      4       5
		//   1  0  0  1     1      4       5
		//   1  0  1  0     1      1       2
		//   1  0  1  1     1      1       2
		//   1  1  0  0     1      4       5
		//   1  1  0  1     1      4       5
		//   1  1  1  0     1      1       2
		//   1  1  1  1     1      1       2

	CHECKTAPS:
	// Return Ninja/Shinobi Tap info
	// [E] None
	// [R] C Tap connected info
	//      7  6  5  4  3  2  1  0
	//      0  0  0  0  a1 a0 b1 b0
	//
	//  a[1..0], b[1..0] : Status of ports A and B
	//  1 1 No Tap detected      ( no feedback on either pin 6 or 8)
	//  0 1 Ninja Tap detected   ( feedback Pin 7 = ~Pin 8 )
	//  0 0 Shinobi Tap detected ( feedback Pin 7 = ~Pin 6 . ~Pin 8)
	//  1 0 Unknown/unexpected   ( feedback Pin 7 = ~Pin 6 )

	// --write--  ---Pin 7 (read)-----
	// PIN6  PIN8 No-Tap Ninja  Shinobi Unknown
	//  LOW  LOW   HIGH   HIGH   HIGH    HIGH
	//  LOW  HIGH  HIGH   LOW    LOW     HIGH    -> to bit a1/b1   First Read
	//  HIGH LOW   HIGH   HIGH   LOW     LOW    -> to bit a0/b0   Second Read
	//  HIGH HIGH  HIGH   LOW    LOW     LOW   (can be disregarded)
	//
	CKANYTAP:
		di                  // 
		ld		c, #0       // 
		// port A
		ld		de, #0xBF3A // start with Joysel port A, pins 8=1, 7=1, 6=0 (read a1)
		call	FBCHECK2    // return Cy = State of pin 7
		rl		c           // c = 0 0 0 0 0 0 0 a1
		ld		de, #0x8F0F // repeat with Joysel port A, pins 8=0, 7=1, 6=1 (read a0)
		call	FBCHECK2    // feedback check at pin 7
		rl		c           // c = 0 0 0 0 0 0 a1 a0
		// port b
		ld		de, #0xBF7A // repeat with Joysel port B, pins 8=1, 7=1, 6=0 (read b1)
		call	FBCHECK2    // return Cy = State of pin 7
		rl		c           // c = 0 0 0 0 0 a1 a0 b1
		ld		de, #0x8F4F // repeat with Joysel port B, pins 8=0, 7=1, 6=1 (read b0)
		call	FBCHECK2    // feedback check at pin 7
		rl		c           // c = 0 0 0 0 a1 a0 b1 b0
		ei
		ret                 // and B = B+1

	//	
	// Set bits on Joystick ports and return state of pin 7
	// [E]  DE => Reg 15 bit mask, D = bit clear mask, E = bit set mask 
	// [R] Cy = State of pin 7
	//
	FBCHECK2:
		call	PORSEL2
		ld		a, #14      // select register 14 
		out		(0xA0), a
		in		a,(0xA2)    // read state of joystick port a = x x B A R L D U
		rla                 // a =  x B A R L D U, cy = x
		rla                 // a =  B A R L D U x, cy = x
		rla                 // a =  A R L D U x x, cy = B
		ret

	//======================================
	// Sub Routine
	PORSEL2:
		ld		a, #15
		out		(0xA0), a
		in		a, (0xA2)
		and		d           // bit clear mask 
		or		e           // bit set mask
		out		(0xA1), a   // return A = state of Reg15 
		ret
	__endasm;
}

//=============================================================================
// MEMORY DATA
//=============================================================================

u8 g_NTap_Info[3];
u8 g_NTap_Data[8];
#if (NTAP_USE_PREVIOUS)
u8 g_NTap_Prev[8];
#endif

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Check the presence of Ninja Tap in the joystick ports.
u8 NTap_Check()
{
	__asm
		call	CKNTAP
	__endasm;
	return NTap_GetInfo();
}

//-----------------------------------------------------------------------------
// Check the presence of Ninja Tap in the joystick ports. Danjovic version.
u8 NTap_Check2()
{
	__asm
		call	CHECKPLYRS
		ld		a, b
		ld		(_g_NTap_Info+1), a
	__endasm;
}

//-----------------------------------------------------------------------------
// Get Ninja Tap information.
u8 NTap_GetInfo()
{
	u8 ret = g_NTap_Info[1];
	if(g_NTap_Info[0] & 1)
		ret |= 0x10;
	if(g_NTap_Info[0] & 2)
		ret |= 0x40;
	return ret;
}

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