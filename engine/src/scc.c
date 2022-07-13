// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄  ▄▄   ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ██ ▀ ██ ▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀█▄▀ ▀█▄▀
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// PSG handler module
//
// References:
//  - Konami SCC Sound Chip
//     http://bifi.msxnet.org/msxnet/tech/scc.html
//─────────────────────────────────────────────────────────────────────────────
#include "scc.h"
#include "system.h"
#include "bios.h"

//=============================================================================
// DEFINES
//=============================================================================

//=============================================================================
// READ-ONLY DATA
//=============================================================================

#if (SCC_USE_EXTA)
const u16 g_SCC_WaveformAddr[5] = { SCC_ADDR_WAVE_1, SCC_ADDR_WAVE_2, SCC_ADDR_WAVE_3, SCC_ADDR_WAVE_4, SCC_ADDR_WAVE_5 };
#endif

#if (SCC_USE_RESUME)
u8 g_SCC_MixerBackup;
#endif

//=============================================================================
// MEMORY DATA
//=============================================================================

#if ((SCC_SLOT_MODE == SCC_SLOT_USER) || (SCC_SLOT_MODE == SCC_SLOT_AUTO))
	u8 g_SCC_SlotId = SLOT_NOTFOUND;
#endif

//=============================================================================
// FUNCTIONS
//=============================================================================

#if (SCC_SLOT_MODE == SCC_SLOT_AUTO)

//-----------------------------------------------------------------------------
//
bool SCC_CheckSlotID(u8 slotId)
{
	// Check ROM
	u8 val = Bios_InterSlotRead(slotId, 0x8000);
	Bios_InterSlotWrite(slotId, 0x8000, ~val);
	if(Bios_InterSlotRead(slotId, 0x8000) != val) // Is RAM?
	{
		Bios_InterSlotWrite(slotId, 0x8000, val); // backup RAM value
		return FALSE;
	}

	// Select SCC
	Bios_InterSlotWrite(slotId, 0x9000, 0x3F);

	// Check SCC RAM
	val = Bios_InterSlotRead(slotId, 0x9800);
	Bios_InterSlotWrite(slotId, 0x9800, ~val);
	if(Bios_InterSlotRead(slotId, 0x9800) != val) // Is SCC RAM?
	{
		Bios_InterSlotWrite(slotId, 0x9800, val); // backup SCC RAM value
		return TRUE;
	}

	return FALSE;
}

//-----------------------------------------------------------------------------
//
u8 SCC_AutoDetect()
{
	return Sys_CheckSlot(SCC_CheckSlotID);
}

//-----------------------------------------------------------------------------
// Auto-detecting the SCC slot id
/*u8 SCC_AutoDetect() __naked
{
__asm
	
SCC_Detect:
	in		a,(#0xA8)               // read prim. slotregister
	rra
	rra
	rra
	rra
	and		#0b00000011             // A = prim.slot page 2
	ld		b, #0
	ld		c, a
	ld		hl, #M_EXPTBL
	add		hl, bc
	bit		#7, (hl)                // page 2-slot expanded ?
	jr		z, SCC_Test
	ld		hl, #M_SLTTBL
	add		hl, bc
	ld		a, (hl)                 // A = sec.sel.reg. of page 2-slot
	rra
	rra
	and		#0b00001100             // bit 1/2 = sec.slot page 2
	or		c
	set		#7, a                   // compose sec.slot-code
SCC_Test:
	push	af                      // save page 2-slot on the stack
	ld		a, (M_EXPTBL)           // 1st slot to test

SCC_TestSlot:        
	push	af                      // save test-slot on the stack
	ld		h, #0x80
	call	#R_ENASLT               // switch slot-to-test in 8000-bfffh
	ld		hl, #0x9000
	ld		b, (hl)                 // save contents of address 9000h
	ld		(hl), #0x3F             // activate SCC (if present)

	xor		a
	ld		(0xBFFE), a	            // SCC+ patch for bluemsx

	ld		h, #0x9C                // address of SCC-register mirrors
	ld		de, #0x9800             // 9800h = address of SCC-registers
SCC_TestReg:         
	ld		a, (de)
	ld		c, a                    // save contents of address 98xxh
	ld		a, (hl)                 // read byte from address 9cxxh
	cpl                             // and invert it
	ld		(de), a                 // write inverted byte to 98xxh
	cp		(hl)                    // same value on 9cxxh ?
	ld		a, c
	ld		(de), a                 // restore value on 98xxh
	jr		nz, SCC_NextSlot        // unequal -> no SCC -> continue search
	inc		hl
	inc		de                      // next test-addresses
	bit		#7, l                   // 128 addresses (registers) tested ?
	jr		z, SCC_TestReg          // no -> repeat mirror-test
	ld		a, b
	ld		(#0x9000), a            // restore value on 9000h
	pop		bc                      // retrieve slotcode (=SCC-slot) from stack
	jr		SCC_Done                // SCC found, restore page 2-slot & return

SCC_NextSlot:
	ld		a, b
	ld		(#0x9000), a            // restore value on 9000h
	pop		bc                      // retrieve slotcode from stack
	bit		#7, b                   // test-slot = sec.slot ?
	jr		z, SCC_NextPrim
	ld		a, b
	add		a, #4                   // increase sec.slotnumber
	bit		#4, a                   // sec.slot = 4 ?
	jr		z, SCC_TestSlot
SCC_NextPrim:
	ld		a, b
	and		#0b00000011
	cp		#3                      // prim.slot = 3 ?
	jr		z, SCC_NoSCC
	inc		a                       // increase prim.slotnumber
	ld		d, #0
	ld		e, a
	ld		hl, #M_EXPTBL
	add		hl, de
	or		(hl)                    // combine slot-expansion with slotcode
	jr		SCC_TestSlot

SCC_NoSCC:           
	ld		b, #0xFF                // code for no SCC
SCC_Done:            
	pop		af                      // retrieve page 2-slot from stack
	push	bc
	ld		h, #0x80
	call	#R_ENASLT               // restore original page 2-slot
	pop		bc
	ei
	
	ld		a, b                    // return value
	ret

__endasm;
}*/

//-----------------------------------------------------------------------------
//
/*u8 SCC_AutoDetect() __naked
{
__asm

	call	SCCDETECT
	ret		nc // SCC found (result in A)
	ld		a, #SLOT_NOTFOUND
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
	ld		(hl), e
	ld		a, d
	ld		(#0x9000), a
	scf
EXIT:
	pop		hl
	pop		bc
	ld		a, c
	ret
__endasm;
}*/
#endif // (SCC_SLOT_MODE == SCC_SLOT_AUTO)

//-----------------------------------------------------------------------------
// Initialize SCC module
bool SCC_Initialize()
{
	#if (SCC_SLOT_MODE == SCC_SLOT_AUTO)
		g_SCC_SlotId = SCC_AutoDetect();
		if(g_SCC_SlotId == SLOT_NOTFOUND)
			return FALSE;
	#elif (SCC_SLOT_MODE == SCC_SLOT_USER)
		g_SCC_SlotId = SLOT_2; // Non-expanded secondary slot cartridge 
	#endif
	
	SCC_Select();
	return TRUE;
}

//-----------------------------------------------------------------------------
// Select sound chip on SCC cartridge
void SCC_Select()
{
	#if (SCC_SLOT_MODE == SCC_SLOT_DIRECT)
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
	#if (SCC_SLOT_MODE == SCC_SLOT_DIRECT)
		POKE(0x9800 + reg, value);
	#else
		Bios_InterSlotWrite(SCC_SLOT, 0x9800 + reg, value);
	#endif

	#if (SCC_USE_RESUME)
	if(reg == SCC_REG_MIXER)
		g_SCC_MixerBackup = value;
	#endif
}

//-----------------------------------------------------------------------------
// Get the value of a given register
u8 SCC_GetRegister(u8 reg)
{
	#if (SCC_SLOT_MODE == SCC_SLOT_DIRECT)
		return PEEK(0x9800 + reg);
	#else
		return Bios_InterSlotRead(SCC_SLOT, 0x9800 + reg);
	#endif
}

//-----------------------------------------------------------------------------
// Mute the SCC sound chip
void SCC_Mute()
{
	#if (SCC_SLOT_MODE == SCC_SLOT_DIRECT)
		POKE(SCC_ADDR_MIXER, 0);
	#else
		Bios_InterSlotWrite(SCC_SLOT, SCC_ADDR_MIXER, 0x00);
	#endif
}

#if (SCC_USE_RESUME)
//-----------------------------------------------------------------------------
// Resume SCC sound
void SCC_Resume()
{
	#if (SCC_SLOT_MODE == SCC_SLOT_DIRECT)
		POKE(SCC_ADDR_MIXER, g_SCC_MixerBackup);
	#else
		Bios_InterSlotWrite(SCC_SLOT, SCC_ADDR_MIXER, g_SCC_MixerBackup);
	#endif
}
#endif


//-----------------------------------------------------------------------------
// EXTRA FEATURES
//-----------------------------------------------------------------------------
#if (SCC_USE_EXTA)
	
//-----------------------------------------------------------------------------
// Load a full waveform
void SCC_LoadWaveform(u8 channel, const u8* data)
{
	u16 addr = g_SCC_WaveformAddr[channel];
	#if (SCC_SLOT_MODE == SCC_SLOT_DIRECT)
		Mem_Copy(data, addr, 32);
	#else
		loopx(32)
			Bios_InterSlotWrite(SCC_SLOT, addr++, *data++);
	#endif
}

//-----------------------------------------------------------------------------
// Set channel frquency
void SCC_SetFrequency(u8 channel, u16 freq)
{
	u16 addr = SCC_ADDR_FREQ_1 + (channel * 2);
	#if (SCC_SLOT_MODE == SCC_SLOT_DIRECT)
		Mem_Copy(&freq, addr, 2);
	#else
		Bios_InterSlotWrite(SCC_SLOT, addr++, (u8)freq);
		Bios_InterSlotWrite(SCC_SLOT, addr, freq >> 8);
	#endif
	
}

#endif // (SCC_USE_EXTA)
