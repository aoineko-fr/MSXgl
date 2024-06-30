// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  System module sample
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"

//=============================================================================
// DEFINES
//=============================================================================
#define SLOT_Y 3

// Library's logo
#define MSX_GL "\x01\x02\x03\x04\x05\x06"

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts data
#include "font/font_mgl_sample6.h"

// Character animation data
const c8 g_ChrAnim[] = { '|', '\\', '-', '/' };

// Slot box
const c8* g_SlotTop			= "\x18\x17\x17\x17\x17\x17\x17\x17\x19"; 
const c8* g_SlotMid			= "\x16\x20\x20\x20\x20\x20\x20\x20\x16"; 
const c8* g_SlotBot			= "\x1A\x17\x17\x17\x17\x17\x17\x17\x1B"; 
// Selected slot	
const c8* g_SlotTopSel		= "\x98\x97\x97\x97\x97\x97\x97\x97\x99"; 
const c8* g_SlotMidSel		= "\x96\x20\x20\x20\x20\x20\x20\x20\x96"; 
const c8* g_SlotBotSel		= "\x9A\x97\x97\x97\x97\x97\x97\x97\x9B"; 
// Expended slot box
const c8* g_SlotExTop		= "\x18\x17\x12\x17\x12\x17\x12\x17\x19"; 
const c8* g_SlotExMid		= "\x16\x20\x16\x20\x16\x20\x16\x20\x16"; 
const c8* g_SlotExBot		= "\x1A\x17\x11\x17\x11\x17\x11\x17\x1B"; 
// Selected slot
const c8* g_SlotExTopSel	= "\x98\x97\x99"; 
const c8* g_SlotExMidSel	= "\x96\x20\x96"; 
const c8* g_SlotExBotSel	= "\x9A\x97\x9B"; 

//=============================================================================
// MEMORY DATA
//=============================================================================

u8 g_VDP;

//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
//
void Print_DrawSlot(u8 slot)
{
	Print_DrawInt(Sys_SlotGetPrimary(slot));
	if(Sys_SlotIsExpended(slot))
	{
		Print_DrawChar('-');
		Print_DrawInt(Sys_SlotGetSecondary(slot));
	}
}

//-----------------------------------------------------------------------------
//
void Print_DrawVersion(u16 ver)
{
	Print_DrawInt(ver >> 12);
	Print_DrawChar('.');
	Print_DrawInt((ver >> 6) & 0x3F);
	Print_DrawChar('.');
	Print_DrawInt(ver & 0x3F);
}

//-----------------------------------------------------------------------------
//
void InterSlotWritePage3(u8 slot, u16 addr, u8 value) __sdcccall(0)
{
	slot;  // IX+4
	addr;  // IX+6 IX+5
	value; // IX+7
	__asm
		di
		push	ix
		ld		ix, #0
		add		ix, sp
		ld		d, 4(ix)		// D=[E.|..|SS|PP] Get slot ID
		ld		l, 5(ix)
		ld		h, 6(ix)		// HL=Write address
		ld		a, 7(ix)		
		ld		i, a			// I=Write value
		
		// Backup primary slot
		in		a, (P_PPI_A)	// A=[P3|P2|P1|P0]
		ld		b, a			// B=[P3|P2|P1|P0] Save current primary slot in B

		// Switch primary slot
		ld		a, d			// A=[E.|..|SS|PP]
		rrca
		rrca					// A=[PP|E.|..|SS]
		and		#0b11000000		// A=[PP|00|00|00]
		ld		e, a			// E=[PP|00|00|00]
		ld		a, b			// A=[P3|P2|P1|P0]
		and		#0b00111111		// A=[00|P2|P1|P0]
		or		e				// A=[PP|P2|P1|P0] remplace ?? par le slot primaire (00,01,10 ou 11)
		out		(P_PPI_A), a	//                 write new primary slot

		// Backup secondary slot (of the new primary slot)
		ld		a, (M_SLTSL)	// A=[~3|~2|~1|~0]
		cpl						// A=[S3|S2|S1|S0]
		ld		c, a			// C=[S3|S2|S1|S0] Save current secondary slot in C

		// Switch secondary slot
		ld		a, d			// A=[E.|..|SS|PP]
		rlca
		rlca					// A=[..|SS|PP|E.]
		rlca
		rlca					// A=[SS|PP|E.|..]
		and		#0b11000000		// A=[SS|00|00|00]
		ld		e, a			// E=[SS|00|00|00]
		ld		a, c			// A=[S3|S2|S1|S0]
		and		#0b00111111		// A=[00|S2|S1|S0]
		or		e				// A=[SS|S2|S1|S0] remplace ?? par le slot secondaire (00,01,10 ou 11)
		ld		(M_SLTSL), a	//				   write new seconday slot

		// Write
		ld		a, i
		ld		(hl), a

		// Restore		
		ld		a, c			// C=[S3|S2|S1|S0]
		ld		(M_SLTSL), a
		ld		a, b			// B=[P3|P2|P1|P0]
		out		(P_PPI_A), a

		pop		ix
		ei
	__endasm;
}

//-----------------------------------------------------------------------------
//
u8 InterSlotReadPage3(u8 slot, u16 addr)
{
	slot;	// A  -> D
	addr;	// DE -> HL
	__asm
		di
		ld		d, a			// D=[E.|..|SS|PP] Get slot ID
		ld		l, d
		ld		h, e			// HL=Read address

		// Backup primary slot
		in		a, (P_PPI_A)	// A=[P3|P2|P1|P0]
		ld		b, a			// B=[P3|P2|P1|P0] Save current primary slot in B

		// Switch primary slot
		ld		a, d			// A=[E.|..|SS|PP]
		rrca
		rrca					// A=[PP|E.|..|SS]
		and		#0b11000000		// A=[PP|00|00|00]
		ld		e, a			// E=[PP|00|00|00]
		ld		a, b			// A=[P3|P2|P1|P0]
		and		#0b00111111		// A=[00|P2|P1|P0]
		or		e				// A=[PP|P2|P1|P0] Remplace ?? par le slot primaire (00,01,10 ou 11)
		out		(P_PPI_A), a

		// Backup secondary slot (of the new primary slot)
		ld		a, (M_SLTSL)	// A=[~3|~2|~1|~0]
		cpl						// A=[S3|S2|S1|S0]
		ld		c, a			// C=[S3|S2|S1|S0] Save current secondary slot in C

		// Switch secondary slot
		ld		a, d			// A=[E.|..|SS|PP]
		rlca					
		rlca					// A=[..|SS|PP|E.]
		rlca
		rlca					// A=[SS|PP|E.|..]
		and		#0b11000000		// A=[SS|00|00|00]
		ld		e, a			// E=[SS|00|00|00]
		ld		a, c			// A=[S3|S2|S1|S0]
		and		#0b00111111		// A=[S3|00|00|00]
		or		e				// A=[SS|S2|S1|S0] Remplace ?? par le slot secondaire (00,01,10 ou 11)
		ld		(M_SLTSL), a
		// Read
		ld		a, (hl)
		ld		l, a			// L=Return value

		// Restore		
		ld		a, c
		ld		(M_SLTSL), a
		ld		a, b
		out		(P_PPI_A), a

		ei
	__endasm;
}

//-----------------------------------------------------------------------------
//
u8 IsSlotPageRAM(u8 slotId, u8 page)
{
	u16 addr = page * 0x4000;
	if(page < 3)
	{
		u8 data = Bios_InterSlotRead(slotId, addr);

		Bios_InterSlotWrite(slotId, addr, ~data);
		
		if(Bios_InterSlotRead(slotId, addr) == data)
			return 0;

		Bios_InterSlotWrite(slotId, addr, data);
	}
	else
	{
		u8 data = InterSlotReadPage3(slotId, addr);

		InterSlotWritePage3(slotId, addr, ~data);
		
		if(InterSlotReadPage3(slotId, addr) == data)
			return 0;

		InterSlotWritePage3(slotId, addr, data);
	}
	return 1;
}

//-----------------------------------------------------------------------------
//
const c8* GetSlotName(u8 slotId, u8 page)
{
	if((slotId == g_EXPTBL[0]) && (page == 0))
		return "M-R";

	if((slotId == g_EXPTBL[0]) && (page == 1))
		return "BAS";

	if((slotId == g_EXBRSA) && (page == 0))
		return "SUB";

	if((slotId == g_MASTER) && (page == 1))
		return "DSK";
		
	if(IsSlotPageRAM(slotId, page))
		return "RAM";
	
	if(page < 3)
	{
		u16 addr = page * 0x4000;
		if(Bios_InterSlotRead(slotId, addr) == 'A')
			if(Bios_InterSlotRead(slotId, ++addr) == 'B')
				return "ROM";
	}
		
	u8 prim = slotId & 0x03;
	u8 sec = (slotId >> 2) & 0x03;
	u16 addr = M_SLTATR + 16 * prim + 4 * sec + page;
	u8 app = *((u8*)addr);
	if(app & 0x80)
		return " B ";
	if(app & 0x40)
		return " D ";
	if(app & 0x20)
		return " S ";
	
	return " ? ";
}

//-----------------------------------------------------------------------------
//
void DisplayHeader()
{
	Print_Clear();
	Print_SetPosition(0, 0);
	Print_DrawText(MSX_GL " SYSTEM INFORMATION");
	Print_DrawLineH(0, 1, 40);
}

//-----------------------------------------------------------------------------
//
void DisplayFooter()
{
	Print_SetPosition(0, 23);
	Print_DrawText("F1:Info F2:Slot F3:Mem ESC:Exit");
}

//-----------------------------------------------------------------------------
//
void DisplayInfo()
{
	DisplayHeader();

	Print_DrawLineV(19, 2, 20);

	//-------------------------------------------------------------------------
	Print_SetPosition(0, 3);
	Print_DrawText("System");
	// MSX version
	Print_DrawText("\n- Version: ");
	switch(g_MSXVER)
	{
		case 0: Print_DrawText("MSX1"); break;
		case 1: Print_DrawText("MSX2"); break;
		case 2: Print_DrawText("MSX2+"); break;
		case 3: Print_DrawText("TurboR"); break;
		default: Print_DrawText("Unknow"); break;
	}
	Print_DrawText("\n- Font:    ");
	switch(g_ROMVersion.CharacterSet)
	{
		case 0: Print_DrawText("Jap"); break;
		case 1: Print_DrawText("Int"); break;
		case 2: Print_DrawText("Rus"); break;
		default: Print_DrawText("Unknow"); break;
	}
	Print_DrawText("\n- Keyb:    ");
	switch(g_ROMVersion.KeyboardType)
	{
		case 0: Print_DrawText("Jap"); break;
		case 1: Print_DrawText("Int"); break;
		case 2: Print_DrawText("Fra"); break;
		case 3: Print_DrawText("UK"); break;
		case 4: Print_DrawText("Ger"); break;
		case 5: Print_DrawText("USSR"); break;
		case 6: Print_DrawText("Spain"); break;
		default: Print_DrawText("Unknow"); break;
	}
	
	//-------------------------------------------------------------------------
	// VDP version
	Print_DrawText("\n\nVideo");
	Print_DrawText("\n- VDP:     ");
	switch(g_VDP)
	{
		case 0: Print_DrawText("TMS9918A"); break;
		case 1: Print_DrawText("V9938"); break;
		case 2: Print_DrawText("V9958"); break;
		default: Print_DrawText("Unknow"); break;
	}
	// VDP frequency
	Print_DrawText("\n- Freq:    ");
	switch(g_ROMVersion.VSF)
	{
		case 0: Print_DrawText("60 Hz"); break;
		case 1: Print_DrawText("50 Hz"); break;
	}
	// VRAM size
	Print_DrawText("\n- VRAM:    ");
	switch(GET_VRAM_SIZE())
	{
		case 0: Print_DrawText("16 KB"); break;
		case 1: Print_DrawText("64 KB"); break;
		case 2: Print_DrawText("128 KB"); break;
		case 3: Print_DrawText("192 KB"); break;
	}
	// VRAM size
	Print_DrawText("\n- IO R/W:  ");
	Print_DrawHex8(g_VDP_DR);
	Print_DrawChar('/');
	Print_DrawHex8(g_VDP_DW);
	
	//-------------------------------------------------------------------------
	// Memory
	Print_DrawText("\n\nMemory");
	Print_DrawText("\n- Heap:    ");
	Print_DrawHex16(Mem_GetHeapAddress());
	Print_DrawText("\n- Stack:   ");
	Print_DrawHex16(Mem_GetStackAddress());
	Print_DrawText("\n- Free:    ");
	Print_DrawInt(Mem_GetHeapSize());
	Print_DrawText(" B");
	
	//-------------------------------------------------------------------------
	// Slots
	u8 X = 21;
	u8 Y = 3;
	Print_DrawTextAt(X, Y++, "Current Slots");
	Print_DrawTextAt(X, Y++, "- Page #0:  ");
	Print_DrawSlot(Sys_GetPageSlot(0));
	Print_DrawTextAt(X, Y++, "- Page #1:  ");
	Print_DrawSlot(Sys_GetPageSlot(1));
	Print_DrawTextAt(X, Y++, "- Page #2:  ");
	Print_DrawSlot(Sys_GetPageSlot(2));
	Print_DrawTextAt(X, Y++, "- Page #3:  ");
	Print_DrawSlot(Sys_GetPageSlot(3));

	//-------------------------------------------------------------------------
	// ROMs
	Y++;
	Print_DrawTextAt(X, Y++, "ROM");
	Print_DrawTextAt(X, Y++, "- Main:     ");
	Print_DrawSlot(g_MNROM);
	Print_DrawTextAt(X, Y++, "- Sub:      ");
	Print_DrawSlot(g_EXBRSA);
	Print_DrawTextAt(X, Y++, "- Disk:     ");
	Print_DrawSlot(g_MASTER);

	//-------------------------------------------------------------------------
	// Lib
	Y++;
	Print_DrawTextAt(X, Y++, "Lib");
	Print_DrawTextAt(X, Y++, "- "MSX_GL":   ");
	Print_DrawVersion(VERSION_CURRENT);
	Print_DrawTextAt(X, Y++, "- SDCC:     ");
	Print_DrawVersion(SDCC_VERSION_CURRENT);
	#if (MSX_VERSION & MSX_1)
	Print_DrawTextAt(X, Y++, "- Target:   MSX1");
	#elif (MSX_VERSION & MSX_2)         
	Print_DrawTextAt(X, Y++, "- Target:   MSX2");
	#elif (MSX_VERSION & MSX_2P)        
	Print_DrawTextAt(X, Y++, "- Target:   MSX2+");
	#elif (MSX_VERSION & MSX_TR)        
	Print_DrawTextAt(X, Y++, "- Target:   MSX TR");
	#endif

	DisplayFooter();
}

//-----------------------------------------------------------------------------
//
void DisplaySlots()
{
	DisplayHeader();

	// Draw frames
	Print_DrawTextAt(0, SLOT_Y + 2, "FFFF\n\n\nC000\nBFFF\n\n\n8000\n7FFF\n\n\n4000\n3FFF\n\n\n0000");
	for(u8 slot = 0; slot < 4; ++slot)
	{
		Print_DrawTextAt((slot * 9) + 6, SLOT_Y, "Slot");
		Print_DrawInt(slot);
		if(Sys_IsSlotExpanded(slot))
		{
			for(u8 sub = 0; sub < 4; ++sub)
			{
				Print_SetPosition((slot * 9) + 5 + (sub * 2), SLOT_Y + 1);
				Print_DrawInt(sub);
			}
			for(u8 page = 0; page < 4; ++page)
			{
				Print_DrawTextAt((slot * 9) + 4, (page * 4) + SLOT_Y + 2, g_SlotExTop);
				Print_DrawTextAt((slot * 9) + 4, (page * 4) + SLOT_Y + 3, g_SlotExMid);
				Print_DrawTextAt((slot * 9) + 4, (page * 4) + SLOT_Y + 4, g_SlotExMid);
				Print_DrawTextAt((slot * 9) + 4, (page * 4) + SLOT_Y + 5, g_SlotExBot);
			}
		}
		else
		{
			for(u8 page = 0; page < 4; ++page)
			{
				u8 pageSlot = Sys_GetPageSlot(3 - page);
				if(pageSlot == SLOT(slot))
				{
					Print_DrawTextAt((slot * 9) + 4, (page * 4) + SLOT_Y + 2, g_SlotTopSel);
					Print_DrawTextAt((slot * 9) + 4, (page * 4) + SLOT_Y + 3, g_SlotMidSel);
					Print_DrawTextAt((slot * 9) + 4, (page * 4) + SLOT_Y + 4, g_SlotMidSel);
					Print_DrawTextAt((slot * 9) + 4, (page * 4) + SLOT_Y + 5, g_SlotBotSel);
				}
				else
				{
					Print_DrawTextAt((slot * 9) + 4, (page * 4) + SLOT_Y + 2, g_SlotTop);
					Print_DrawTextAt((slot * 9) + 4, (page * 4) + SLOT_Y + 3, g_SlotMid);
					Print_DrawTextAt((slot * 9) + 4, (page * 4) + SLOT_Y + 4, g_SlotMid);
					Print_DrawTextAt((slot * 9) + 4, (page * 4) + SLOT_Y + 5, g_SlotBot);
				}
			}
		}
	}

	// Find slot type
	for(u8 page = 0; page < 4; ++page)
	{
		u8 pageSlot = Sys_GetPageSlot(3 - page);
		for(u8 slot = 0; slot < 4; ++slot)
		{
			if(Sys_IsSlotExpanded(slot))
			{
				for(u8 sub = 0; sub < 4; ++sub)
				{
					if(pageSlot == SLOTEX(slot, sub))
					{
						Print_SetPosition((slot * 9) + 4 + (sub * 2), (page * 4) + SLOT_Y + 2);
						Print_DrawText(g_SlotExTopSel);
						Print_SetPosition((slot * 9) + 4 + (sub * 2), (page * 4) + SLOT_Y + 3);
						Print_DrawText(g_SlotExMidSel);
						Print_SetPosition((slot * 9) + 4 + (sub * 2), (page * 4) + SLOT_Y + 4);
						Print_DrawText(g_SlotExMidSel);
						Print_SetPosition((slot * 9) + 4 + (sub * 2), (page * 4) + SLOT_Y + 5);
						Print_DrawText(g_SlotExBotSel);
					}
					Print_SetPosition((slot * 9) + 4 + (sub * 2), (page * 4) + SLOT_Y + 3 + (sub & 0x1));
					Print_DrawText(GetSlotName(SLOTEX(slot, sub), 3 - page));
				}
			}
			else
			{
				Print_SetPosition((slot * 9) + 7, (page * 4) + SLOT_Y + 3);
				Print_DrawText(GetSlotName(SLOT(slot), 3 - page));
			}
		}
	}

	DisplayFooter();
}

//-----------------------------------------------------------------------------
//
void DisplayMem()
{
	DisplayHeader();

	Print_SetPosition(0, 3);
	
	// Memory
	Print_DrawText("Memory\n");
	Print_DrawFormat("- Heap:    %x\n", Mem_GetHeapAddress());
	Print_DrawFormat("- Stack:   %x\n", Mem_GetStackAddress());
	Print_DrawFormat("- Free:    %i bytes\n", Mem_GetHeapSize());

	// Static allocator
	Print_DrawText("\nDynamic allocator\n");

	Mem_DynamicInitializeHeap(1024);
	Print_DrawFormat("- Init 1024 bytes from heap\n");

	void* p1 = Mem_DynamicAlloc(100);
	Print_DrawFormat("- Alloc 100 bytes: p1=%x (%i)\n", p1, p1 ? Mem_GetDynamicSize(p1) : 0);

	void* p2 = Mem_DynamicAlloc(500);
	Print_DrawFormat("- Alloc 500 bytes: p2=%x (%i)\n", p2, p2 ? Mem_GetDynamicSize(p2) : 0);

	Mem_DynamicFree(p1);
	Print_DrawFormat("- Free p1\n");

	void* p3 = Mem_DynamicAlloc(101);
	Print_DrawFormat("- Alloc 101 bytes: p3=%x (%i)\n", p3, p3 ? Mem_GetDynamicSize(p3) : 0);

	void* p4 = Mem_DynamicAlloc(100);
	Print_DrawFormat("- Alloc 100 bytes: p4=%x (%i)\n", p4, p4 ? Mem_GetDynamicSize(p4) : 0);

	void* p5 = Mem_DynamicAlloc(500);
	Print_DrawFormat("- Alloc 500 bytes: p5=%x (%i)\n", p5, p5 ? Mem_GetDynamicSize(p5) : 0);
	
	Mem_HeapFree(1024); // Release dynamic allocator heap allocation

	DisplayFooter();
}


//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	g_VDP = VDP_GetVersion();

	VDP_SetMode(VDP_MODE_SCREEN0);
	VDP_FillVRAM_16K(0, 0x0000, 0x4000); // Clear VRAM

	Print_SetTextFont(g_Font_MGL_Sample6, 1);
	Print_SetColor(0xF, 0x0);

	callback cb = DisplayInfo;
	cb();

	u8 count = 0;
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		Halt();
		// EnableInterrupt();

		Print_SetPosition(39, 0);
		Print_DrawChar(g_ChrAnim[count++ & 0x03]);

		u8 row = Keyboard_Read(KEY_ROW(KEY_F1));
		if(IS_KEY_PRESSED(row, KEY_F1))
		{
			cb = DisplayInfo;
			cb();
		}
		else if(IS_KEY_PRESSED(row, KEY_F2))
		{
			cb = DisplaySlots;
			cb();
		}
		else if(IS_KEY_PRESSED(row, KEY_F3))
		{
			cb = DisplayMem;
			cb();
		}
	}

	Bios_Exit(0);
}