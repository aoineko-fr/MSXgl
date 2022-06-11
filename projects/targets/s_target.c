// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL		"\x01\x02\x03\x04\x05\x06"

// Mapper page
#define BANK_ADDR	0x8000
#define MAPPER_Y	8
#define MAPPER_NUM	6
#if (MAPPER_NUM > ROM_SEGMENTS)
	#undef  MAPPER_NUM
	#define MAPPER_NUM	ROM_SEGMENTS
#endif
#if (ROM_MAPPER > ROM_PLAIN)
	#if (ROM_MAPPER == ROM_ASCII16)
		void PrintSegment2Data(u8 x, u8 y) __banked;
		#define BANKED_BANK 1
		#define BANKED_SEG 2
		#define BANKED_CALL(x, y) PrintSegment2Data(x, y)
	#else	
		void PrintSegment4Data(u8 x, u8 y) __banked;
		#define BANKED_BANK 2
		#define BANKED_SEG 4
		#define BANKED_CALL(x, y) PrintSegment4Data(x, y)
	#endif
#endif

// Slots page
#define SLOT_Y		3

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts data
#include "font\font_mgl_sample6.h"

// Data in bank 2 & 3
#if (ROM_MAPPER != ROM_PLAIN)
#if (ROM_MAPPER == ROM_ASCII16)
__at(0x8000) const c8 g_DataBank1[] = "Segment #1 (default in Bank #1)";
#else
__at(0x8000) const c8 g_DataBank2[] = "Segment #2 (default in Bank #2)";
__at(0xA000) const c8 g_DataBank3[] = "Segment #3 (default in Bank #3)";
#endif
#endif

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
u8  g_Buffer[128];
u8  g_PageSlot[4];
#if (ROM_MAPPER != ROM_PLAIN)
u8  g_DisplayASCII = 0;
u8  g_Segment = 0;
u16 g_Address = BANK_ADDR;
#endif
u8  g_Page = 1;

//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

#if (TARGET_TYPE == TYPE_ROM)
//-----------------------------------------------------------------------------
/// ISR for 48/64K ROM
void VDP_InterruptHandler()
{
}
#endif

//-----------------------------------------------------------------------------
//
void Print_Slot(u8 slot)
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
		
	return " ? ";
}

//-----------------------------------------------------------------------------
//
const c8* GetTargetType(u8 target)
{
	switch(target)
	{
	case TYPE_BIN: return "BASIC";
	case TYPE_ROM: return "ROM";
	case TYPE_DOS: return "DOS";
	};
	return "Unknow";
}

//-----------------------------------------------------------------------------
//
const c8* GetROMMapper(u8 mapper)
{
	switch(mapper)
	{
	case ROM_PLAIN:      return "Plain";
	case ROM_ASCII8:     return "ASCII8";
	case ROM_ASCII16:    return "ASCII16";
	case ROM_KONAMI:     return "Konami";
	case ROM_KONAMI_SCC: return "Konami SCC";
	};
	return "Unknow";
}

//-----------------------------------------------------------------------------
//
const c8* GetROMSize(u8 size)
{
	switch(size)
	{
	case ROM_8K:	return "8K";
	case ROM_16K:	return "16K";
	case ROM_32K:	return "32K";
	case ROM_48K:	return "48K";
	case ROM_64K:	return "64K";
	case ROM_128K:	return "128K";
	case ROM_256K:	return "256K";
	case ROM_512K:	return "512K";
	case ROM_1024K:	return "1M";
	case ROM_2048K:	return "2M";
	case ROM_4096K:	return "4M";
	};
	return "Unknow";
}

//-----------------------------------------------------------------------------
//
void DisplayHeader()
{
	// Print_Clear();
	VDP_FillVRAM(' ' - g_PrintData.CharFirst + g_PrintData.PatternOffset, g_ScreenLayoutLow, g_ScreenLayoutHigh, 40 * 24);
	Print_SetPosition(0, 0);
	Print_DrawText(MSX_GL "   Target Sample");
	Print_DrawLineH(0, 1, 40);
}

//-----------------------------------------------------------------------------
//
void DisplayFooter()
{
	Print_DrawLineH(0, 22, 40);
	Print_SetPosition(0, 23);
	if(g_Page == 0) // Info
	{
		Print_DrawText("F1:Slot ");
		#if (ROM_MAPPER != ROM_PLAIN)
			if(g_DisplayASCII)
				Print_DrawText("F2:Hexa ");
			else
				Print_DrawText("F2:ASCII ");
			Print_DrawText("\x81\x80:Addr \x82:Seg");
		#endif
	}
	else // Slot
	{
		Print_DrawText("F1:Info ");
	}
	
	
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


#if (ROM_MAPPER != ROM_PLAIN)

//-----------------------------------------------------------------------------
//
void UpdateMapper()
{
	Print_SetPosition(32, MAPPER_Y);
	Print_DrawText("\x81 ");
	Print_DrawHex16(g_Address);
	Print_DrawText(" \x80\n");

	Print_SetPosition(1, MAPPER_Y + 2);
	Print_DrawChar((g_Segment == 0) ? ' ' : '\x8E');
	Print_SetPosition(1, MAPPER_Y + 1 + MAPPER_NUM);
	Print_DrawChar((g_Segment == ROM_SEGMENTS - MAPPER_NUM) ? ' ' : '\x8F');


	for(u8 i = 0; i < MAPPER_NUM; i++)
	{
		Print_SetPosition(2, MAPPER_Y + 2 + i);
		Print_DrawInt(g_Segment + i);
		if(g_Segment + i < 100)
			Print_DrawChar(' ');

		Print_SetPosition(6, MAPPER_Y + 2 + i);
		#if (ROM_MAPPER == ROM_ASCII16)
			SET_BANK_SEGMENT(1, g_Segment + i);
		#else
			SET_BANK_SEGMENT(2, g_Segment + i);
		#endif
		u8 max = g_DisplayASCII ? 33 : 11;
		for(u8 j = 0; j < max; j++)
		{
			u8 chr = ((const u8*)(g_Address))[j];
			if(g_DisplayASCII)
				Print_DrawChar(chr);
			else
			{
				Print_DrawHex8(chr);
				Print_DrawChar(' ');
			}
		}
		Print_Return();
	}
}
#endif // (ROM_MAPPER != ROM_PLAIN)


//-----------------------------------------------------------------------------
//
void DiplayInfo()
{
	DisplayHeader();

	Print_SetPosition(0, 2);
	Print_DrawText(TARGET_NAME);

	Print_DrawFormat("\nType:   %s", GetTargetType(TARGET_TYPE));
	
	#if (TARGET_TYPE == TYPE_ROM)
		Print_DrawFormat("\nMapper: %s", GetROMMapper(ROM_MAPPER));
		
		Print_DrawFormat("\nSize:   %s", GetROMSize(ROM_SIZE));
		#if (ROM_MAPPER != ROM_PLAIN)
			Print_DrawFormat(" (%i)", ROM_SEGMENTS);
		#endif
	#endif

	Print_DrawFormat("\nAddr:   %4x~%4x", Sys_GetFirstAddr(), Sys_GetLastAddr());

	for(u8 i = 0; i < 4; i++)
	{
		g_PageSlot[i] = Sys_GetPageSlot(i);
		Print_SetPosition(20, 3+i);
		Print_DrawFormat("Page[%i]: ", i);
		Print_Slot(g_PageSlot[i]);
		Print_SetPosition(33, 3+i);
		Print_DrawFormat("(%s)", GetSlotName(g_PageSlot[i], i));
	}

	#if (ROM_MAPPER != ROM_PLAIN)

		Print_SetPosition(0, MAPPER_Y);
		#if (ROM_MAPPER == ROM_ASCII16)
			Print_DrawText("Segments in Bank #1");
		#else
			Print_DrawText("Segments in Bank #2");
		#endif
		
		Print_SetPosition(0, MAPPER_Y + 1);
		Print_DrawText("\x18\x17\x17\x17\x17\x12");
		Print_DrawCharX('\x17', 33);
		Print_DrawChar('\x19');
		
		for(u8 i = 0; i < MAPPER_NUM; i++)
		{
			Print_SetPosition(0, MAPPER_Y + 2 + i);
			Print_DrawChar('\x16');
			g_PrintData.CursorX = 5;
			Print_DrawChar('\x16');
			g_PrintData.CursorX = 39;
			Print_DrawChar('\x16');
		}

		Print_SetPosition(0, MAPPER_Y + 2 + MAPPER_NUM);
		Print_DrawText("\x1A\x17\x17\x17\x17\x11");
		Print_DrawCharX('\x17', 33);
		Print_DrawChar('\x1B');

		UpdateMapper();

		// #if (ROM_MAPPER == ROM_ASCII16)
			// SET_BANK_SEGMENT(1, BANKED_SEG);
		// #else
			// SET_BANK_SEGMENT(2, BANKED_SEG);
		// #endif		
		Print_SetPosition(0, MAPPER_Y + 4 + MAPPER_NUM);
		Print_DrawText("Banked call:");
		BANKED_CALL(1, MAPPER_Y + 5 + MAPPER_NUM);

	#endif

	DisplayFooter();
}

//-----------------------------------------------------------------------------
// Program entry point
void SwitchPage()
{
	g_Page = 1 - g_Page;
	if(g_Page == 0)
		DiplayInfo();
	else
		DisplaySlots();
}

//=============================================================================
// MAIN LOOP
//=============================================================================
//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	// Initialize
	VDP_SetMode(VDP_MODE_SCREEN0);
	VDP_EnableVBlank(TRUE);
	Print_SetTextFont(g_Font_MGL_Sample6, 1);

	SwitchPage();
	
	u8 count = 0;
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		Print_SetPosition(39, 0);
		Print_DrawChar(g_ChrAnim[count++ & 0x03]);
		
		// Handle user input
		u8 row6 = Keyboard_Read(6);
		u8 row8 = Keyboard_Read(8);

		if(IS_KEY_PRESSED(row6, KEY_F1))
			SwitchPage();
		
		#if (ROM_MAPPER != ROM_PLAIN)
		if(g_Page == 0)
		{
			if(IS_KEY_PRESSED(row6, KEY_F2))
			{
				g_DisplayASCII = 1 - g_DisplayASCII;
				UpdateMapper();
			}
			if(IS_KEY_PRESSED(row8, KEY_RIGHT))
			{
				g_Address++;
				UpdateMapper();
			}
			if(IS_KEY_PRESSED(row8, KEY_LEFT))
			{
				if(g_Address > BANK_ADDR)
					g_Address--;
				UpdateMapper();
			}
			if(IS_KEY_PRESSED(row8, KEY_DOWN))
			{
				if(g_Segment < ROM_SEGMENTS-MAPPER_NUM)
					g_Segment++;
				UpdateMapper();
			}
			if(IS_KEY_PRESSED(row8, KEY_UP))
			{
				if(g_Segment > 0)
					g_Segment--;
				UpdateMapper();
			}
		}
		#endif // (ROM_MAPPER != ROM_PLAIN)

		Halt();
	}
	
	#if(TARGET & ROM_ISR)
	/* BIOS not present in page 0 */
	Sys_SetPage0Slot(g_EXPTBL[0]);
	#endif
	Bios_Exit(0);
}