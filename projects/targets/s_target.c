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
#define MSX_GL			"\x01\x02\x03\x04\x05\x06"

// Mapper page
#define BANK_ADDR		0x8000
#define MAPPER_Y		10
#define MAPPER_NUM		8
#define MAPPER_ADDR_X	20
#define CHAR_ASCII		20
#define CHAR_HEXA		7
#define QUICK_JUMP		16

#if (MAPPER_NUM > ROM_SEGMENTS)
	#undef  MAPPER_NUM
	#define MAPPER_NUM	ROM_SEGMENTS
#endif

#if ((TARGET_TYPE == TYPE_ROM) && (ROM_MAPPER == ROM_NEO8))
	// void PrintSegment500Data(u8 x, u8 y) __banked;
	// #define BANKED_BANK 1
	// #define BANKED_SEG 500
	// #define BANKED_CALL(x, y)	PrintSegment500Data(x, y)
	#define TEST_BANK			4
	#define ROM_MAPPER_STR		"Segment in bank 4"
#elif ((TARGET_TYPE == TYPE_ROM) && (ROM_MAPPER == ROM_NEO16))
	// void PrintSegment500Data(u8 x, u8 y) __banked;
	// #define BANKED_BANK 1
	// #define BANKED_SEG 500
	// #define BANKED_CALL(x, y) 	PrintSegment500Data(x, y)
	#define TEST_BANK			2
	#define ROM_MAPPER_STR		"Segment in bank 2"
#elif ((TARGET_TYPE == TYPE_ROM) && (ROM_MAPPER >= ROM_MAPPER_16K))
	// void PrintSegment2Data(u8 x, u8 y) __banked;
	// #define BANKED_BANK 1
	// #define BANKED_SEG 2
	// #define BANKED_CALL(x, y)	PrintSegment2Data(x, y)
	#define TEST_BANK			1
	#define ROM_MAPPER_STR		"Segment in bank 1"
#elif ((TARGET_TYPE == TYPE_ROM) && (ROM_MAPPER > ROM_PLAIN))
	// void PrintSegment4Data(u8 x, u8 y) __banked;
	// #define BANKED_BANK 2
	// #define BANKED_SEG 4
	// #define BANKED_CALL(x, y)	PrintSegment4Data(x, y)
	#define TEST_BANK			2
	#define ROM_MAPPER_STR		"Segment in bank 2"
#else
	// void PrintSegment4Data(u8 x, u8 y) __banked;
	// #define BANKED_BANK 2
	// #define BANKED_SEG 4
	// #define BANKED_CALL(x, y)	PrintSegment4Data(x, y)
	#define TEST_BANK			0
	#define ROM_MAPPER_STR		""
#endif

// Function prototypes
void main();

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts data
#include "font/font_mgl_sample6.h"

// // Data in bank 2 & 3
// #if (ROM_MAPPER == ROM_ASCII16)
// __at(0x8000) const c8 g_DataBank1[] = "Segment #1 (default in Bank #1)";
// #elif ((ROM_MAPPER == ROM_ASCII8) || (ROM_MAPPER == ROM_KONAMI) || (ROM_MAPPER == ROM_KONAMI_SCC))
// __at(0x8000) const c8 g_DataBank2[] = "Segment #2 (default in Bank #2)";
// __at(0xA000) const c8 g_DataBank3[] = "Segment #3 (default in Bank #3)";
// #endif

// Character animation data
const c8 g_ChrAnim[] = { '|', '\\', '-', '/' };


//=============================================================================
// MEMORY DATA
//=============================================================================
u8  g_Buffer[128];
#if ((TARGET_TYPE == TYPE_ROM) || (ROM_MAPPER != ROM_PLAIN))
u8  g_DisplayASCII = 0;
i16 g_Segment = 0;
u16 g_Address = BANK_ADDR;
#endif

//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

#if ((TARGET_TYPE == TYPE_ROM) || (TARGET == TARGET_DOS0))
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
	if (Sys_SlotIsExpended(slot))
	{
		Print_DrawChar('-');
		Print_DrawInt(Sys_SlotGetSecondary(slot));
	}
}

//-----------------------------------------------------------------------------
//
const c8* GetTargetType(u8 target)
{
	switch (target)
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
	switch (mapper)
	{
	case ROM_PLAIN:      return "Plain";
	case ROM_ASCII8:     return "ASCII-8";
	case ROM_ASCII16:    return "ASCII-16";
	case ROM_KONAMI:     return "Konami";
	case ROM_KONAMI_SCC: return "Konami SCC";
	case ROM_NEO8:       return "NEO-8";
	case ROM_NEO16:      return "NEO-16";
	case ROM_YAMANOOTO:  return "Yamanooto";
	case ROM_ASCII16X:    return "ASCII16-X";
	};
	return "Unknow";
}

//-----------------------------------------------------------------------------
//
const c8* GetROMFrame(u8 size)
{
	switch (size)
	{
	case ROM_8K:	return "8K";
	case ROM_16K:	return "16K";
	case ROM_24K:	return "24K";
	case ROM_32K:	return "32K";
	case ROM_48K:	return "48K";
	case ROM_64K:	return "64K";
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
	Print_DrawText(MSX_GL " TARGET SAMPLE");
	Print_DrawLineH(0, 1, 40);
}

//-----------------------------------------------------------------------------
//
void DisplayFooter()
{
	Print_DrawLineH(0, 22, 40);
	Print_SetPosition(0, 23);
	#if ((TARGET_TYPE == TYPE_ROM) || (ROM_MAPPER != ROM_PLAIN))
		if (g_DisplayASCII)
			Print_DrawText("F1:Hexa ");
		else
			Print_DrawText("F1:ASCII ");
		Print_DrawText("\x81\x80:Addr \x82:Seg");
	#endif
}

#if ((TARGET_TYPE == TYPE_ROM) && (ROM_MAPPER != ROM_PLAIN))

//-----------------------------------------------------------------------------
//
void UpdateMapper()
{
	Print_SetPosition(MAPPER_ADDR_X, MAPPER_Y);
	Print_DrawText("\x81 ");
	Print_DrawHex16(g_Address);
	Print_DrawText(" \x80\n");

	Print_SetPosition(1, MAPPER_Y + 2);
	Print_DrawChar((g_Segment == 0) ? ' ' : '\x8E');
	Print_SetPosition(1, MAPPER_Y + 1 + MAPPER_NUM);
	Print_DrawChar((g_Segment == ROM_SEGMENTS - MAPPER_NUM) ? ' ' : '\x8F');

	for (u8 i = 0; i < MAPPER_NUM; i++)
	{
		u16 index = g_Segment + i;
		Print_SetPosition(2, MAPPER_Y + 2 + i);
		Print_DrawInt(index);
		if (index < 100)
			Print_DrawChar(' ');
		if (index < 1000)
			Print_DrawChar(' ');

		Print_SetPosition(7, MAPPER_Y + 2 + i);
		SET_BANK_SEGMENT(TEST_BANK, index);
		u8 max = g_DisplayASCII ? CHAR_ASCII : CHAR_HEXA;
		for (u8 j = 0; j < max; j++)
		{
			u8 chr = ((const u8*)(g_Address))[j];
			if (g_DisplayASCII)
				Print_DrawChar(chr);
			else
			{
				Print_DrawHex8(chr);
				if (j < max - 1)
					Print_DrawChar(' ');
			}
		}
		Print_Return();
	}

	for (u8 i = 0; i < MAPPER_BANKS; i++)
	{
		u16 seg = 0;
		switch (i)
		{
		case 0: seg = GET_BANK_SEGMENT(0); break;
		case 1: seg = GET_BANK_SEGMENT(1); break;
		case 2: seg = GET_BANK_SEGMENT(2); break;
		case 3: seg = GET_BANK_SEGMENT(3); break;
		case 4: seg = GET_BANK_SEGMENT(4); break;
		case 5: seg = GET_BANK_SEGMENT(5); break;
		};
		Print_SetPosition(30, MAPPER_Y + 4 + i);
		Print_DrawFormat("%c%i", (i == TEST_BANK) ? 0x80 : ' ', i);
		Print_SetPosition(35, MAPPER_Y + 4 + i);
		Print_DrawFormat("%i ", seg);
	}
}
#endif


//-----------------------------------------------------------------------------
// Display media target information
void DiplayInfo()
{
	DisplayHeader();

	Print_SetPosition(0, 2);
	Print_DrawText(TARGET_NAME);
	Print_DrawFormat("\n\nType:   %s", GetTargetType(TARGET_TYPE));
	#if (TARGET_TYPE == TYPE_ROM)
		Print_DrawFormat("\nMapper: %s", GetROMMapper(ROM_MAPPER));
		
		Print_DrawFormat("\nSize:   %iKB", ROM_SIZE);
		#if (ROM_MAPPER != ROM_PLAIN)
			Print_DrawFormat(" (%i)", (i16)ROM_SEGMENTS);
		#endif
	#endif
	Print_DrawFormat("\nAddr:   %4x~%4x", Sys_GetFirstAddr(), Sys_GetLastAddr());
	Print_DrawFormat("\nISR:    %c", TARGET_ISR ? 0x0C : 0x0B);

	Print_SetPosition(29, 4);
	Print_DrawText("Page  Slot");

	Print_SetPosition(29, 5);
	Print_DrawText("\x18\x17\x17\x17\x12\x17\x17\x17\x17\x17\x19");
	u8 mainPage = (u16)main >> 14;
	for (u8 i = 0; i < 4; i++)
	{
		Print_SetPosition(29, 6 + i);
		Print_DrawChar('\x16');
		g_PrintData.CursorX = 33;
		Print_DrawChar('\x16');
		g_PrintData.CursorX = 39;
		Print_DrawChar('\x16');

		u8 slotId = Sys_GetPageSlot(i);
		Print_SetPosition(30, 6 + i);
		Print_DrawFormat("%c%i", (i == mainPage) ? 0x80 : ' ', i);
		Print_SetPosition(35, 6 + i);
		Print_Slot(slotId);
	}
	Print_SetPosition(29, 6 + 4);
	Print_DrawText("\x1A\x17\x17\x17\x11\x17\x17\x17\x17\x17\x1B");

	#if ((TARGET_TYPE == TYPE_ROM) && (ROM_MAPPER != ROM_PLAIN))

		Print_SetPosition(0, MAPPER_Y);
		Print_DrawText(ROM_MAPPER_STR);
		
		Print_SetPosition(0, MAPPER_Y + 1);
		Print_DrawText("\x18\x17\x17\x17\x17\x17\x12");
		Print_DrawCharX('\x17', 20);
		Print_DrawChar('\x19');
		for (u8 i = 0; i < MAPPER_NUM; i++)
		{
			Print_SetPosition(0, MAPPER_Y + 2 + i);
			Print_DrawChar('\x16');
			g_PrintData.CursorX = 6;
			Print_DrawChar('\x16');
			g_PrintData.CursorX = 27;
			Print_DrawChar('\x16');
		}
		Print_SetPosition(0, MAPPER_Y + 2 + MAPPER_NUM);
		Print_DrawText("\x1A\x17\x17\x17\x17\x17\x11");
		Print_DrawCharX('\x17', 20);
		Print_DrawChar('\x1B');

		Print_SetPosition(29, MAPPER_Y + 2);
		Print_DrawText("Bank  Seg");

		Print_SetPosition(29, MAPPER_Y + 3);
		Print_DrawText("\x18\x17\x17\x17\x12\x17\x17\x17\x17\x17\x19");
		for (u8 i = 0; i < MAPPER_BANKS; i++)
		{
			Print_SetPosition(29, MAPPER_Y + 4 + i);
			Print_DrawChar('\x16');
			g_PrintData.CursorX = 33;
			Print_DrawChar('\x16');
			g_PrintData.CursorX = 39;
			Print_DrawChar('\x16');
		}
		Print_SetPosition(29, MAPPER_Y + 4 + MAPPER_BANKS);
		Print_DrawText("\x1A\x17\x17\x17\x11\x17\x17\x17\x17\x17\x1B");

		UpdateMapper();

		// Print_SetPosition(0, MAPPER_Y + 4 + MAPPER_NUM);
		// Print_DrawText("Banked call:");
		// BANKED_CALL(1, MAPPER_Y + 5 + MAPPER_NUM);

	#endif

	DisplayFooter();
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

	DiplayInfo();
	
	u8 count = 0;
	while (!Keyboard_IsKeyPressed(KEY_ESC))
	{
		Halt();

		Print_SetPosition(39, 0);
		Print_DrawChar(g_ChrAnim[count++ & 0x03]);
		
		// Handle user input
		u8 row6 = Keyboard_Read(6);
		u8 row8 = Keyboard_Read(8);

		#if ((TARGET_TYPE == TYPE_ROM) && (ROM_MAPPER != ROM_PLAIN))
			if (IS_KEY_PRESSED(row6, KEY_F1))
			{
				g_DisplayASCII = 1 - g_DisplayASCII;
				UpdateMapper();
			}
			if (IS_KEY_PRESSED(row8, KEY_RIGHT))
			{
				g_Address++;
				UpdateMapper();
			}
			if (IS_KEY_PRESSED(row8, KEY_LEFT))
			{
				if (g_Address > BANK_ADDR)
					g_Address--;
				UpdateMapper();
			}
			if (IS_KEY_PRESSED(row8, KEY_DOWN))
			{
				if (IS_KEY_PRESSED(row6, KEY_CTRL))
					g_Segment += QUICK_JUMP;
				else
					g_Segment++;
				if (g_Segment > ROM_SEGMENTS - MAPPER_NUM)
					g_Segment = ROM_SEGMENTS - MAPPER_NUM;
				UpdateMapper();
			}
			if (IS_KEY_PRESSED(row8, KEY_UP))
			{
				if (IS_KEY_PRESSED(row6, KEY_CTRL))
					g_Segment -= QUICK_JUMP;
				else
					g_Segment--;
				if (g_Segment < 0)
					g_Segment = 0;
				UpdateMapper();
			}
		#endif
	}
	
	#if ((TARGET_TYPE == TYPE_ROM) && (TARGET & ROM_ISR))
		// BIOS not present in page 0
		Sys_SetPage0Slot(g_EXPTBL[0]);
	#endif
	Bios_Exit(0);
}