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
#define MSX_GL						"\x01\x02\x03\x04\x05\x06"

// Mapper page
#define VIEWER_Y					11
#define VIEWER_ADDR_X				20
#define CHAR_ASCII					20
#define CHAR_HEXA					7
#define QUICK_JUMP					16

#if (TARGET_TYPE == TYPE_ROM)

#if (ROM_MAPPER == ROM_PLAIN)
	#define VIEWER_LINE				4
#elif (ROM_SEGMENTS < 8)
	#define VIEWER_LINE				ROM_SEGMENTS
#else
	#define VIEWER_LINE				8
#endif

#if (ROM_MAPPER == ROM_PLAIN)
	#define VIEWER_ADDR				0x0000
#else
	#define VIEWER_ADDR				0x8000
#endif

#if (ROM_MAPPER == ROM_NEO8) // NEO-8 mapper
	#define TEST_BANK				4
	#define SEG_FREE				1
	#define SEG_ISR					4
	#define ROM_VIEWER_STR			"Segment in bank 4"
#elif (ROM_MAPPER == ROM_NEO16) // NEO-16 mapper
	#define TEST_BANK				2
	#define SEG_FREE				1
	#define SEG_ISR					2
	#define ROM_VIEWER_STR			"Segment in bank 2"
#elif (ROM_MAPPER >= ROM_MAPPER_16K) // 16K mapper
	#define TEST_BANK				1
	#define SEG_FREE				1
	#define SEG_ISR					-1
	#define ROM_VIEWER_STR			"Segment in bank 1"
#elif (ROM_MAPPER > ROM_PLAIN) // 8K mapper
	#define TEST_BANK				2
	#define SEG_FREE				1
	#define SEG_ISR					-1
	#define ROM_VIEWER_STR			"Segment in bank 2"
#else
	#define TEST_BANK				0
	#define ROM_VIEWER_STR			"Page"
#endif

#endif // (TARGET_TYPE == TYPE_ROM)

#define SEG_ADDR(s)					((u32)(s) << (u32)(16))

// Function prototypes
void main();

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts data
#include "font/font_mgl_sample6.h"

// ROM test data
#if (TARGET_TYPE == TYPE_ROM)

	const c8* g_ROMSignature = TARGET_SIGN;

#if (ROM_MAPPER == ROM_PLAIN)

	#if (ROM_START == 0)
	__at(SEG_ADDR(0)) const c8 g_SegmentISR[] = "#ISR PAGE";
	#endif

#else

	#if (SEG_ISR >= 0)
	__at(SEG_ADDR(SEG_ISR)) const c8 g_SegmentISR[] = "#ISR SEGMENT";
	#endif
	#if (ROM_SEGMENTS > 10)
	__at(SEG_ADDR(10)) const c8 g_Segment10[] = "#SEGMENT 10";
	#endif
	#if (ROM_SEGMENTS > 16)
	__at(SEG_ADDR(16)) const c8 g_Segment16[] = "#SEGMENT 16";
	#endif
	#if (ROM_SEGMENTS > 32)
	__at(SEG_ADDR(32)) const c8 g_Segment32[] = "#SEGMENT 32";
	#endif
	#if (ROM_SEGMENTS > 64)
	__at(SEG_ADDR(64)) const c8 g_Segment64[] = "#SEGMENT 64";
	#endif
	#if (ROM_SEGMENTS > 100)
	__at(SEG_ADDR(100)) const c8 g_Segment100[] = "#SEGMENT 100";
	#endif
	#if (ROM_SEGMENTS > 128)
	__at(SEG_ADDR(128)) const c8 g_Segment128[] = "#SEGMENT 128";
	#endif
	#if (ROM_SEGMENTS > 256)
	__at(SEG_ADDR(256)) const c8 g_Segment256[] = "#SEGMENT 256";
	#endif
	#if (ROM_SEGMENTS > 512)
	__at(SEG_ADDR(512)) const c8 g_Segment512[] = "#SEGMENT 512";
	#endif
	#if (ROM_SEGMENTS > 1000)
	__at(SEG_ADDR(1000)) const c8 g_Segment1000[] = "#SEGMENT 1000";
	#endif
	#if (ROM_SEGMENTS > 1024)
	__at(SEG_ADDR(1024)) const c8 g_Segment1024[] = "#SEGMENT 1024";
	#endif
	#if (ROM_SEGMENTS > 2048)
	__at(SEG_ADDR(2048)) const c8 g_Segment2048[] = "#SEGMENT 2048";
	#endif
	#if (ROM_SEGMENTS > 4096)
	__at(SEG_ADDR(4096)) const c8 g_Segment4096[] = "#SEGMENT 4096";
	#endif

	__at(SEG_ADDR(SEG_FREE)) const c8 g_SegmentFirst[] = "#FIRST FREE SEGMENT";
	__at(SEG_ADDR(ROM_SEGMENTS - 1)) const c8 g_SegmentLast[] = "#LAST SEGMENT";

#endif
#endif

// Character animation data
const c8 g_ChrAnim[] = { '|', '\\', '-', '/' };


//=============================================================================
// MEMORY DATA
//=============================================================================
u8  g_Buffer[128];
#if ((TARGET_TYPE == TYPE_ROM) || (ROM_MAPPER != ROM_PLAIN))
u8  g_DisplayASCII = 0;
i16 g_Segment = 0;
u16 g_Address = VIEWER_ADDR;
#endif

//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

#if ((TARGET_TYPE == TYPE_ROM) || (TARGET == TARGET_DOS0))
//-----------------------------------------------------------------------------
// ISR for 48/64K ROM
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
	case ROM_ASCII16X:   return "ASCII16-X";
	case ROM_POPOLON:    return "Popolon SCC";
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

#if (TARGET_TYPE == TYPE_ROM)

//-----------------------------------------------------------------------------
//
void UpdateViewer()
{
	Print_SetPosition(VIEWER_ADDR_X, VIEWER_Y);
	Print_DrawText("\x81 ");
	Print_DrawHex16(g_Address);
	Print_DrawText(" \x80\n");

 #if (ROM_MAPPER != ROM_PLAIN)
 	Print_SetPosition(1, VIEWER_Y + 2);
	Print_DrawChar((g_Segment == 0) ? ' ' : '\x8E');
	Print_SetPosition(1, VIEWER_Y + 1 + VIEWER_LINE);
	Print_DrawChar((g_Segment == ROM_SEGMENTS - VIEWER_LINE) ? ' ' : '\x8F');
 #endif

	for (u8 i = 0; i < VIEWER_LINE; i++)
	{
		u16 index = g_Segment + i;
		Print_SetPosition(2, VIEWER_Y + 2 + i);
		Print_DrawInt(index);
		if (index < 100)
			Print_DrawChar(' ');
		if (index < 1000)
			Print_DrawChar(' ');

		Print_SetPosition(7, VIEWER_Y + 2 + i);
 #if (ROM_MAPPER == ROM_PLAIN)
		const u8* ptr = (const u8*)g_Address + (i * 0x4000);
 #else
		SET_BANK_SEGMENT(TEST_BANK, index);
		const u8* ptr = (const u8*)g_Address;
 #endif
		u8 max = g_DisplayASCII ? CHAR_ASCII : CHAR_HEXA;
		for (u8 j = 0; j < max; j++)
		{
			u8 chr = ptr[j];
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

 #if (ROM_MAPPER != ROM_PLAIN)
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
		Print_SetPosition(30, VIEWER_Y + 4 + i);
		Print_DrawFormat("%c%i", (i == TEST_BANK) ? 0x80 : ' ', i);
		Print_SetPosition(35, VIEWER_Y + 4 + i);
		Print_DrawFormat("%i ", seg);
	}
#endif
}
#endif

//-----------------------------------------------------------------------------
// Check for ROM signature
bool FindSign()
{
#if (TARGET_TYPE == TYPE_ROM)
	const c8* sign = g_ROMSignature;
	const c8* rom = (const c8*)(((u16)main & 0xC000) + 16);
	loop (i, 8)
	{
		if (*rom++ != *sign++)
			return FALSE;
	}
	return TRUE;
#else
	return FALSE;
#endif
}

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
		if (ROM_SIZE >= 1024)
			Print_DrawFormat("\nSize:   %i MB", ROM_SIZE / 1024);
		else
			Print_DrawFormat("\nSize:   %i KB", ROM_SIZE);
		#if (ROM_MAPPER != ROM_PLAIN)
			Print_DrawFormat(" (%i)", (i16)ROM_SEGMENTS);
		#endif
	#endif
	Print_DrawFormat("\nAddr:   %4x~%4x", Sys_GetFirstAddr(), Sys_GetLastAddr());
	Print_DrawFormat("\nISR:    %c", GET_TARGET_ISR(TARGET) ? 0x0C : 0x0B);
	#if (TARGET_TYPE == TYPE_ROM)
	Print_DrawFormat("\nSign:   %c %s", FindSign() ? 0x0C : 0x0B, FindSign() ? g_ROMSignature : "");
	#endif

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

	#if (TARGET_TYPE == TYPE_ROM)

		Print_SetPosition(0, VIEWER_Y);
		Print_DrawText(ROM_VIEWER_STR);
		
		Print_SetPosition(0, VIEWER_Y + 1);
		Print_DrawText("\x18\x17\x17\x17\x17\x17\x12");
		Print_DrawCharX('\x17', 20);
		Print_DrawChar('\x19');
		for (u8 i = 0; i < VIEWER_LINE; i++)
		{
			Print_SetPosition(0, VIEWER_Y + 2 + i);
			Print_DrawChar('\x16');
			g_PrintData.CursorX = 6;
			Print_DrawChar('\x16');
			g_PrintData.CursorX = 27;
			Print_DrawChar('\x16');
		}
		Print_SetPosition(0, VIEWER_Y + 2 + VIEWER_LINE);
		Print_DrawText("\x1A\x17\x17\x17\x17\x17\x11");
		Print_DrawCharX('\x17', 20);
		Print_DrawChar('\x1B');

		#if (ROM_MAPPER == ROM_PLAIN)
		u8 num = 0;
		switch (ROM_FRAME)
		{
			case ROM_8K:  num = 1; break;
			case ROM_16K: num = 1; break;
			case ROM_24K: num = 2; break;
			case ROM_32K: num = 2; break;
			case ROM_48K: num = 3; break;
			case ROM_64K: num = 4; break;
		}
		for (u8 i = ROM_START; i < ROM_START + num; ++i)
		{
			Print_SetPosition(29, VIEWER_Y + 2 + i);
			Print_DrawHex16(i * 0x4000);
		}
		#else
		Print_SetPosition(29, VIEWER_Y + 2);
		Print_DrawText("Bank  Seg");

		Print_SetPosition(29, VIEWER_Y + 3);
		Print_DrawText("\x18\x17\x17\x17\x12\x17\x17\x17\x17\x17\x19");
		for (u8 i = 0; i < MAPPER_BANKS; i++)
		{
			Print_SetPosition(29, VIEWER_Y + 4 + i);
			Print_DrawChar('\x16');
			g_PrintData.CursorX = 33;
			Print_DrawChar('\x16');
			g_PrintData.CursorX = 39;
			Print_DrawChar('\x16');
		}
		Print_SetPosition(29, VIEWER_Y + 4 + MAPPER_BANKS);
		Print_DrawText("\x1A\x17\x17\x17\x11\x17\x17\x17\x17\x17\x1B");
		#endif

		UpdateViewer();

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

		#if (TARGET_TYPE == TYPE_ROM)
			if (IS_KEY_PRESSED(row6, KEY_F1))
			{
				g_DisplayASCII = 1 - g_DisplayASCII;
				UpdateViewer();
			}
			if (IS_KEY_PRESSED(row8, KEY_RIGHT))
			{
				if (IS_KEY_PRESSED(row6, KEY_CTRL))
					g_Address += QUICK_JUMP;
				else
					g_Address++;
				UpdateViewer();
			}
			else if (IS_KEY_PRESSED(row8, KEY_LEFT))
			{
				if (IS_KEY_PRESSED(row6, KEY_CTRL))
				{
					if (g_Address > VIEWER_ADDR + QUICK_JUMP)
						g_Address -= QUICK_JUMP;
					else
						g_Address = VIEWER_ADDR;
				}
				else if (g_Address > VIEWER_ADDR)
					g_Address--;
				UpdateViewer();
			}
			 #if (ROM_MAPPER != ROM_PLAIN)
			if (IS_KEY_PRESSED(row8, KEY_DOWN))
			{
				if (IS_KEY_PRESSED(row6, KEY_CTRL))
					g_Segment += QUICK_JUMP;
				else
					g_Segment++;
				if (g_Segment > ROM_SEGMENTS - VIEWER_LINE)
					g_Segment = ROM_SEGMENTS - VIEWER_LINE;
				UpdateViewer();
			}
			else if (IS_KEY_PRESSED(row8, KEY_UP))
			{
				if (IS_KEY_PRESSED(row6, KEY_CTRL))
					g_Segment -= QUICK_JUMP;
				else
					g_Segment--;
				if (g_Segment < 0)
					g_Segment = 0;
				UpdateViewer();
			}
			 #endif
		#endif
	}
	
	#if ((TARGET_TYPE == TYPE_ROM) && (TARGET & ROM_ISR))
		// BIOS not present in page 0
		Sys_SetPage0Slot(g_EXPTBL[0]);
	#endif
	BIOS_Exit(0);
}