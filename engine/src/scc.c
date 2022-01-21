// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄   ▄▄▄  ▄▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀ ▀█▄  ██  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██   ▄▄█▀ ▀█▄█
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
// PSG handler module
//
// References:
//  - The Revised MSX Red Book - PSG
//     https://www.angelfire.com/art2/unicorndreams/msx/RR-PSG.html
//─────────────────────────────────────────────────────────────────────────────
#include "scc.h"
#include "system.h"
#include "bios_mainrom.h"

//=============================================================================
// DEFINES
//=============================================================================

//=============================================================================
// READ-ONLY DATA
//=============================================================================


//=============================================================================
// MEMORY DATA
//=============================================================================

#if (SCC_SLOT_MODE != SCC_SLOT_FIXED)
	u8 g_SCC_SlotId;
#endif

//=============================================================================
// FUNCTIONS
//=============================================================================

#if (SCC_SLOT_MODE == SCC_SLOT_AUTO)
//-----------------------------------------------------------------------------
//
u8 SCC_AutoDetect() __naked
{
__asm
	
	call	SCCDETECT
	ret		nc // SCC found (result in A)
	ld		a, #SCC_SLOT_NOTFOUND
	ret

SCCDETECT:
//----------------------------------------------------
// SCCDETECT (Made by : NYYRIKKI)
// Input: (None)
// Output:
//   Success:
//     CF = NC
//     A  = SlotID
//     B  = Number of untested slots
//     (SCC SLOT on #8000-#BFFF)
//
//   Fail:
//     CF = C
//     B  = 0
//     (Random slot on #8000-#BFFF)
// Changes: All registers
//----------------------------------------------------
 
	ld		b, #16
 
SCCDETECT_NEXT:
//-----------------------------------------------------
// Call this address if you need to detect another SCC
// Input: B = Value returned by SCCDETECT (must be >0)
//-----------------------------------------------------
 
	ld		h, #0xFC
SLOTLOOP:
	dec		b
	ld		a, b
	and		#3
	add		a, #0xC1
	ld		l, a
	ld		a, (hl)
	and		#128
	or		b
	cp		#12
	call	nc, TESTSLOT
	ret		nc
	inc		b
	djnz	SLOTLOOP
	ret
 
TESTSLOT:
	xor		#12
	ld		c, a
	push	bc
	push	hl

	ld		h, #0x80
	call	#R_ENASLT

	ld		a, (#0x9000)
	ld		d, a
	ld		a, #0x3F
	ld		(#0x9000), a
	ld		hl, #0x9800
	ld		e, (hl)
	xor		a
	ld		(hl), a
	or		(hl)
	jr		nz, NOSCC
	dec		a
	ld		(hl), a
	ld		a, (hl)
	inc		a
	jr		nz, NOSCC
	ld		a, (#0x9000)
	xor		#0x3F
	jr		nz, EXIT
NOSCC:
	LD		(HL), E
	LD		A, D
	LD		(#0x9000), A
	SCF
EXIT:
	POP		HL
	POP		BC
	LD		A, C
	RET
__endasm;
}
#endif // (SCC_SLOT_MODE == SCC_SLOT_AUTO)

//-----------------------------------------------------------------------------
// Initialize SCC module
bool SCC_Initialize()
{
	#if (SCC_SLOT_MODE == SCC_SLOT_AUTO)
	g_SCC_SlotId = SCC_AutoDetect();
	if(g_SCC_SlotId == SCC_SLOT_NOTFOUND)
		return false;
	#elif (SCC_SLOT_MODE == SCC_SLOT_USER)
	g_SCC_SlotId = SLOT_2; // Non-expanded secondary slot cartridge 
	#endif
	
	SCC_Select();
	return true;
}

//-----------------------------------------------------------------------------
// Select sound chip on SCC cartridge
void SCC_Select()
{
	#if (SCC_SLOT_MODE == SCC_SLOT_PROGRAM)
	SET_BANK_SEGMENT(2, 0x3F);
	#else
	Bios_InterSlotWrite(SCC_SLOT, 0x9000, 0x3F);
	#endif
}

//-----------------------------------------------------------------------------
// Set the value of a given register
// Note: Only wavetable area can be read
void SCC_SetRegister(u8 reg, u8 value)
{
	#if (SCC_SLOT_MODE == SCC_SLOT_PROGRAM)
	POKE(0x9800 + reg, value);
	#else
	Bios_InterSlotWrite(SCC_SLOT, 0x9800 + reg, value);
	#endif
}

//-----------------------------------------------------------------------------
// Get the value of a given register
u8 SCC_GetRegister(u8 reg)
{
	#if (SCC_SLOT_MODE == SCC_SLOT_PROGRAM)
	return PEEK(0x9800 + reg);
	#else
	return Bios_InterSlotRead(SCC_SLOT, 0x9800 + reg);
	#endif
}

//-----------------------------------------------------------------------------
// Mute the SCC sound chip
void SCC_Mute()
{
	#if (SCC_SLOT_MODE == SCC_SLOT_PROGRAM)
	POKE(SCC_ADDR_MIXER, 0);
	#else
	Bios_InterSlotWrite(SCC_SLOT, SCC_ADDR_MIXER, 0x00);
	#endif
}
