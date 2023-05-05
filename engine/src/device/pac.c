// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄   ▄▄   ▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀ ██▄█ ██ ▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██   ██ █ ▀█▄▀
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Panasoft PAC and FM-PAC's SRAM handler
//─────────────────────────────────────────────────────────────────────────────
#include "pac.h"
#include "bios.h"

//=============================================================================
// DEFINES
//=============================================================================

#if (PAC_USE_SIGNATURE)
extern const u32 g_AppSignature;
#endif

//=============================================================================
// READ-ONLY DATA
//=============================================================================

//=============================================================================
// MEMORY DATA
//=============================================================================

// Number of detected PAC cartridge
u8 g_PAC_Num;

// Detected PACs' slot ID
u8 g_PAC_Slot[PAC_DEVICE_MAX];

// Selected PAC's slot ID
u8 g_PAC_Current;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
//
void PAC_ReadInterSlot(u8 slot, u16 addr, u8* data, u16 size)
{
	for(u16 i = 0; i < size; ++i)
		*data++ = Bios_InterSlotRead(slot, addr++);
	EnableInterrupt();
}

//-----------------------------------------------------------------------------
//
void PAC_WriteInterSlot(u8 slot, u16 addr, const u8* data, u16 size)
{
	for(u16 i = 0; i < size; ++i)
		Bios_InterSlotWrite(slot, addr++, *data++);
	EnableInterrupt();
}

//-----------------------------------------------------------------------------
//
void PAC_CheckSlot(u8 slot)
{
	if(g_PAC_Num >= PAC_DEVICE_MAX)
		return;

	// Backup values
	u8 bakE = Bios_InterSlotRead(slot, 0x5FFE);
	u8 bakF = Bios_InterSlotRead(slot, 0x5FFF);
	u8 bakD = Bios_InterSlotRead(slot, 0x5FFD);

	// Check ROM
	Bios_InterSlotWrite(slot, 0x5FFF, 0);				// Desactivate SRAM
	Bios_InterSlotWrite(slot, 0x5FFD, ~bakD);
	if(Bios_InterSlotRead(slot, 0x5FFD) == bakD)
	{
		// Check SRAM
		Bios_InterSlotWrite(slot, 0x5FFE, 'M');		// Activate SRAM
		Bios_InterSlotWrite(slot, 0x5FFF, 'i');
		Bios_InterSlotWrite(slot, 0x5FFD, ~bakD);
		if(Bios_InterSlotRead(slot, 0x5FFD) != bakD)
			g_PAC_Slot[g_PAC_Num++] = slot;
	}

	// Restore values
	Bios_InterSlotWrite(slot, 0x5FFD, bakD);
	Bios_InterSlotWrite(slot, 0x5FFE, bakE);
	Bios_InterSlotWrite(slot, 0x5FFF, bakF);
}

//-----------------------------------------------------------------------------
//
bool PAC_Search()
{
	// Initialize
	g_PAC_Num = 0;
	for(u8 i = 0; i < PAC_DEVICE_MAX; ++i)
		g_PAC_Slot[i] = SLOT_NOTFOUND;

	// Parse slot and subslot
	for(u8 slot = 0; slot < 4; ++slot)
	{
		if(g_EXPTBL[slot] & SLOT_EXP)
		{
			for(u8 sub = 0; sub < 4; ++sub)
			{
				u8 slotId = SLOTEX(slot, sub);
				PAC_CheckSlot(slotId);
			}
		}
		else
			PAC_CheckSlot(slot);
	}

	return g_PAC_Num > 0;
}

//-----------------------------------------------------------------------------
//
bool PAC_Initialize()
{
	if(!PAC_Search())
		return FALSE;

	PAC_Select(0);
	PAC_Activate(TRUE);

	return TRUE;
}

//-----------------------------------------------------------------------------
//
void PAC_Activate(bool bEnable)
{
	if(bEnable)
	{
		Bios_InterSlotWrite(g_PAC_Current, 0x5FFE, 'M'); // 0x4D
		Bios_InterSlotWrite(g_PAC_Current, 0x5FFF, 'i'); // 0x69
	}
	else
	{
		Bios_InterSlotWrite(g_PAC_Current, 0x5FFF, 0);
	}
	EnableInterrupt();
}

//-----------------------------------------------------------------------------
//
void PAC_Write(u8 page, const u8* data, u16 size)
{
	#if (PAC_USE_VALIDATOR)
	if(page > 7)
		return;
	if((page == 7) && (size > 1022))
		return;
	if(size > 1024)
		return;
	#endif

	u16 addr = 0x4000 + page * 1024;
	#if (PAC_USE_SIGNATURE)
	const u8* sign = (const u8*)&g_AppSignature;
	for(u16 i = 0; i < 4; ++i)
		Bios_InterSlotWrite(g_PAC_Current, addr++, *sign++);
	#endif

	PAC_WriteInterSlot(g_PAC_Current, addr, data, size);
}

//-----------------------------------------------------------------------------
//
void PAC_Read(u8 page, u8* data, u16 size)
{
	#if (PAC_USE_VALIDATOR)
	if(page > 7)
		return;
	if((page == 7) && (size > 1022))
		return;
	if(size > 1024)
		return;
	#endif

	u16 addr = 0x4000 + page * 1024;
	#if (PAC_USE_SIGNATURE)
	addr += 4;
	#endif

	PAC_ReadInterSlot(g_PAC_Current, addr, data, size);
}

//-----------------------------------------------------------------------------
//
void PAC_Format(u8 page)
{
	#if (PAC_USE_VALIDATOR)
	if(page > 7)
		return;
	#endif

	u16 addr = 0x4000 + page * 1024;

	u16 size = 1024;
	if(page == 7)
		size -= 2;

	for(u16 i = 0; i < size; ++i)
		Bios_InterSlotWrite(g_PAC_Current, addr++, PAC_EMPTY_CHAR);
	EnableInterrupt();
}

//-----------------------------------------------------------------------------
//
u8 PAC_Check(u8 page)
{
	#if (PAC_USE_VALIDATOR)
	if(page > 7)
		return PAC_CHECK_ERROR;
	#endif

	u16 addr = 0x4000 + page * 1024;

	#if (PAC_USE_SIGNATURE)
	bool bSigned = TRUE;
	const u8* sign = (const u8*)&g_AppSignature;
	for(u16 i = 0; i < 4; ++i)
	{
		if(Bios_InterSlotRead(g_PAC_Current, addr + i) != sign[i])
		{
			bSigned = FALSE;
			break;
		}
	}
	if(bSigned)
		return PAC_CHECK_APP;
	#endif

	u16 size = 1024;
	if(page == 7)
		size -= 2;

	for(u16 i = 0; i < size; ++i)
		if(Bios_InterSlotRead(g_PAC_Current, addr++) != PAC_EMPTY_CHAR)
			return PAC_CHECK_UNDEF;

	return PAC_CHECK_EMPTY;
}







/*void PAC_Search()
{
__asm

	ld		a, #SLOT_NOTFOUND
	ld		(_g_PAC_Slot), a		// Initialize FM-PAC slot

SCANSLT:
	xor		a						// start from slot 0
	ld		hl, #0xFCC1
.loop:
	ld		c, (hl)
	or		c
.loop2:
	exx
	push	af

	ld		h, #0x40				// switch page 1
	call	0x0024

	call	.chk_FM

	pop		af
	exx

	add		a, #0b00000100			// increase subslot number
	ld		c, a					// C = new slot+subslot
	bit		7, a					// check if slot expanded
	jr		z, .next				// if not expanded go for next slot
	and		#0b00001100				// check if already checked all subslots
	ld		a, c
	jr		nz, .loop2				// still some subslots left
.next:
	inc		hl
	ld		a, c
	inc		a
	and		#0b00000011
	ret		z						// return if all slots & subslots are checked!

	jr		.loop

.chk_FM:
	ld		de, (0x5FFE)

	ld		hl, #0x694D				// enable FM-PAC SRAM
	ld		(0x5FFE), hl

	ld		hl, #0x4000

	ld		a, (hl)
	inc		(hl)
	cp		(hl)
	ret		z

	dec		(hl)					// return previous value to RAM

	ld		(0x5FFE), hl			// disable SRAM

	ld		a,(hl)
	inc		(hl)
	cp		(hl)
	jr		nz, .exit

	pop		hl
	pop		af

	ld		(_g_PAC_Slot), a		// save FM-PAC slot
	ret

.exit:
	dec		(hl)

	ld		(0x5FFE), de			// return previous value to 5FFE
	ret

__endasm;	
}*/
