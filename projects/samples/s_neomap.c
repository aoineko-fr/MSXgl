// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  ROM mapper sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL						"\x01\x02\x03\x04\x05\x06"

#if (ROM_MAPPER == ROM_NEO8)
	#define SET_SEGMENT(s)			SET_BANK_SEGMENT(4, s)
	#define SET_SEGMENT_LOW(s)		SET_BANK_SEGMENT_LOW(4, s)
	#define SET_SEGMENT_HIGH(s)		SET_BANK_SEGMENT_HIGH(4, s)
	#define GET_SEGMENT()			GET_BANK_SEGMENT(4)
	#define BANK_NUM				6
	#define BANK_SIZE				0x2000
	#define TEST_BANK				4
	#define TEST_ADDR				0x8000
#elif (ROM_MAPPER == ROM_NEO16)
	#define SET_SEGMENT(s)			SET_BANK_SEGMENT(2, s)
	#define SET_SEGMENT_LOW(s)		SET_BANK_SEGMENT_LOW(2, s)
	#define SET_SEGMENT_HIGH(s)		SET_BANK_SEGMENT_HIGH(2, s)
	#define GET_SEGMENT()			GET_BANK_SEGMENT(2)
	#define BANK_NUM				3
	#define BANK_SIZE				0x4000
	#define TEST_BANK				2
	#define TEST_ADDR				0x8000
#endif

#define MIRROR_MASK				0b0000011111111110

struct SignEntry
{
	u16       Segment;
	const c8* Sign;
};


//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font/font_mgl_sample6.h"

// Animation characters
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

// Sample data
__at(0x00030000) const c8 g_S003Data[] = "S003";
__at(0x00110000) const c8 g_S011Data[] = "S011";
__at(0x01110000) const c8 g_S111Data[] = "S111";
__at(0x01120000) const c8 g_S000Data[] = "S112";
__at(0x00FF0000) const c8 g_S0FFData[] = "S0FF";
__at(0x01000000) const c8 g_S100Data[] = "S100";
__at(0x01230000) const c8 g_S123Data[] = "S123";
__at(0x01FF0000) const c8 g_S1FFData[] = "S1FF";

const struct SignEntry g_Signs[] =
{
	{ 0x0003, "S003" },
	{ 0x0011, "S011" },
	{ 0x0111, "S111" },
	{ 0x0112, "S112" },
	{ 0x00FF, "S0FF" },
	{ 0x0100, "S100" },
	{ 0x0123, "S123" },
	{ 0x01FF, "S1FF" },
};

const u16 g_SwitchAddr[BANK_NUM] =
{
#if (ROM_MAPPER == ROM_NEO8)
	0x5000,
	0x5800,
	0x6000,
	0x6800,
	0x7000,
	0x7800,
#elif (ROM_MAPPER == ROM_NEO16)
	0x5000,
	0x6000,
	0x7000,
#endif
};


//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
//
bool CheckSign(const u8* addr, const u8* sign)
{
	loop(i, 4)
		if(*addr++ != *sign++)
			return FALSE;
	return TRUE;
}

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
	case ROM_ASCII8:     return "ASCII-8";
	case ROM_ASCII16:    return "ASCII-16";
	case ROM_KONAMI:     return "Konami";
	case ROM_KONAMI_SCC: return "Konami SCC";
	case ROM_NEO8:       return "NEO-8";
	case ROM_NEO16:      return "NEO-16";
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
	case ROM_24K:	return "24K";
	case ROM_32K:	return "32K";
	case ROM_48K:	return "48K";
	case ROM_64K:	return "64K";
	case ROM_128K:	return "128K";
	case ROM_256K:	return "256K";
	case ROM_512K:	return "512K";
	case ROM_1M:	return "1M";
	case ROM_2M:	return "2M";
	case ROM_4M:	return "4M";
	case ROM_8M:	return "8M";
	case ROM_16M:	return "16M";
	case ROM_32M:	return "32M";
	case ROM_64M:	return "64M";
	};
	return "Unknow";
}

//-----------------------------------------------------------------------------
// Display from address
void DisplayMemory(u16 addr)
{
	const u8* ptr = (const u8*)addr; 
	loop(i, 28)
		Print_DrawChar(*ptr++);
}

//-----------------------------------------------------------------------------
// Interrupt handler
void VDP_InterruptHandler()
{

}

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	VDP_SetMode(VDP_MODE_SCREEN0); // Initialize screen mode 0 (text)
	VDP_ClearVRAM();

#if (ROM_MAPPER == ROM_NEO8)
	SET_BANK_SEGMENT(0, 4); // Bank 0 (0000-1FFF) = Segment 4
	SET_BANK_SEGMENT(1, 5); // Bank 1 (2000-3FFF) = Segment 5
	SET_BANK_SEGMENT(2, 0); // Bank 2 (4000-5FFF) = Segment 0
	SET_BANK_SEGMENT(3, 1); // Bank 3 (6000-7FFF) = Segment 1
	SET_BANK_SEGMENT(4, 2); // Bank 4 (8000-9FFF) = Segment 2
	SET_BANK_SEGMENT(5, 3); // Bank 5 (A000-BFFF) = Segment 3
#elif (ROM_MAPPER == ROM_NEO16)
	SET_BANK_SEGMENT(0, 2); // Bank 0 (0000-3FFF) = Segment 2
	SET_BANK_SEGMENT(1, 0); // Bank 1 (4000-7FFF) = Segment 0
	SET_BANK_SEGMENT(2, 1); // Bank 2 (8000-BFFF) = Segment 1
#endif

	// Title
	Print_SetTextFont(g_Font_MGL_Sample6, 1); // Initialize font
	Print_DrawText(MSX_GL " NEO mapper sample"); // Display title
	Print_DrawLineH(0, 1, 40);

	// Information
	Print_SetPosition(0, 2);
	Print_DrawFormat("%s\n\n", TARGET_NAME);
	Print_DrawFormat("Type:    %s\n", GetTargetType(TARGET_TYPE));
	Print_DrawFormat("Mapper:  %s\n", GetROMMapper(ROM_MAPPER));
	Print_DrawFormat("Size:    %s\n", GetROMSize(ROM_SIZE));
	Print_DrawFormat("Seg num: %i\n", ROM_SEGMENTS);
	// Slot selection
	loop(i, 4)
	{
		u8 slotId = Sys_GetPageSlot(i);
		Print_SetPosition(20, 4+i);
		Print_DrawFormat("Page[%i]: ", i);
		Print_Slot(slotId);
	}
	Print_DrawLineH(0, 8, 40);

	Print_SetPosition(0, 8);
	Print_DrawText("\nBank test:");
	loop(b, BANK_NUM)
	{
		u16 s = 0;
		switch(b)
		{
			case 0:
				s = GET_BANK_SEGMENT(0);
				break;
			case 1:
				s = GET_BANK_SEGMENT(1);
				break;
			case 2:
				s = GET_BANK_SEGMENT(2);
				break;
		#if (ROM_MAPPER == ROM_NEO8)
			case 3:
				s = GET_BANK_SEGMENT(3);
				break;
			case 4:
				s = GET_BANK_SEGMENT(4);
				break;
			case 5:
				s = GET_BANK_SEGMENT(5);
				break;
		#endif
		}
		Print_DrawFormat("\n\x7""[%i]->%03i: ", b, s);
		DisplayMemory(BANK_SIZE * b);
	}

	Print_DrawText("\n\nAccess test:");
	Print_DrawText("\n\x7SET_BANK_SEGMENT()  ");
	SET_SEGMENT(0);
	loop(i, numberof(g_Signs))
	{
		const struct SignEntry* e = &g_Signs[i];
		SET_SEGMENT(e->Segment); // Set segment in bank
		Print_DrawText(CheckSign((const u8*)TEST_ADDR, e->Sign) ? "\xC " : "\xB ");
	}

	Print_DrawText("\n\x7""16b access          ");
	SET_SEGMENT(0);
	loop(i, numberof(g_Signs))
	{
		const struct SignEntry* e = &g_Signs[i];
		POKEW(g_SwitchAddr[TEST_BANK], e->Segment); // Set segment in bank
		Print_DrawText(CheckSign((const u8*)TEST_ADDR, e->Sign) ? "\xC " : "\xB ");
	}

	Print_DrawText("\n\x7""16b access + mask   ");
	SET_SEGMENT(0);
	loop(i, numberof(g_Signs))
	{
		const struct SignEntry* e = &g_Signs[i];
		u16 addr = g_SwitchAddr[TEST_BANK] | MIRROR_MASK;
		POKEW(addr, e->Segment); // Set segment in bank
		Print_DrawText(CheckSign((const u8*)TEST_ADDR, e->Sign) ? "\xC " : "\xB ");
	}

	Print_DrawText("\n\x7""8b access           ");
	SET_SEGMENT(0);
	loop(i, numberof(g_Signs))
	{
		const struct SignEntry* e = &g_Signs[i];
		u16 addr = g_SwitchAddr[TEST_BANK];
		switch(i)
		{
			case 0: // 0x0003
				SET_SEGMENT_LOW(0x03);
				break;
			case 1: // 0x0011
				SET_SEGMENT_LOW(0x11);
				break;
			case 2: // 0x0111
				SET_SEGMENT_HIGH(0x01);
				break;
			case 3: // 0x0112
				SET_SEGMENT_LOW(0x12);
				break;
			case 4: // 0x00FF
				SET_SEGMENT_HIGH(0x00);
				SET_SEGMENT_LOW(0xFF);
				break;
			case 5: // 0x0100
				SET_SEGMENT_HIGH(0x01);
				SET_SEGMENT_LOW(0x00);
				break;
			case 6: // 0x0123
				SET_SEGMENT_LOW(0x23);
				break;
			case 7: // 0x01FF
				SET_SEGMENT_LOW(0xFF);
				break;
		}
		Print_DrawText(CheckSign((const u8*)TEST_ADDR, e->Sign) ? "\xC " : "\xB ");
	}


	u8 count = 0;
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		Halt();

		Print_SetPosition(39, 0);
		Print_DrawChar(g_ChrAnim[count++ & 0x03]);
	}
}