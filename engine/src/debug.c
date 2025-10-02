// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// 
//─────────────────────────────────────────────────────────────────────────────
#include "debug.h"

//-----------------------------------------------------------------------------
// CONST DATA
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// VARIABLES
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// FUNCTIONS
//-----------------------------------------------------------------------------

//=============================================================================
// openMSX default debugger (no profiler)
// https://openmsx.org/manual/user.html#debugdevice
#if (DEBUG_EMULATOR(DEBUG_TOOL) == DEBUG_OPENMSX)

#include "string.h"

#define P_DEBUG_MODE		0x2E
#define P_DEBUG_DATA		0x2F
__sfr __at(P_DEBUG_MODE) g_PortDebugMode; // Mode Set Register. (Write only)
__sfr __at(P_DEBUG_DATA) g_PortDebugData; // Data Register. (Write only)

//-----------------------------------------------------------------------------
// Port 0x2E - Mode Set Register
//-----------------------------------------------------------------------------
//	7	6	5	4	3	2	1	0
//		│	│	│	└───┴───┴───┴── Mode-specific parameters
//		│	│	│						Single byte mode
//		│	│	│							bit#3	ASCII mode on/off
//		│	│	│							bit#2	decimal mode on/off
//		│	│	│							bit#1	binary mode on/off
//		│	│	│							bit#0	hexadecimal mode on/off
//		│	│	│						Multi byte mode
//		│	│	│							mode (0 = hex, 1 = binary, 2 = decimal, 3 = ASCII mode)
//		│	└───┴────────────────── Output mode (0 = OFF, 1 = single byte, 2 = multi byte)
//		└────────────────────────── Line feed mode (0 = line feed at mode change, 1 no line feed)
#define DEBUG_NOLINEFEED	0b01000000
#define DEBUG_MODE			0b00110000
#define DEBUG_MODE_OFF		0b00000000
#define DEBUG_MODE_SINGLE	0b00010000
#define DEBUG_MODE_MULTI	0b00100000
#define DEBUG_SINGLE_HEX	0b00000001
#define DEBUG_SINGLE_BIN	0b00000010
#define DEBUG_SINGLE_DEC	0b00000100
#define DEBUG_SINGLE_ASCII	0b00001000
#define DEBUG_MULTI_HEX		0b00000000
#define DEBUG_MULTI_BIN		0b00000001
#define DEBUG_MULTI_DEC		0b00000010
#define DEBUG_MULTI_ASCII	0b00000011

//-----------------------------------------------------------------------------
// Initialize Debug module
void DEBUG_INIT() {}

//-----------------------------------------------------------------------------
// Force a break point
void DEBUG_BREAK()
{
#if (DEBUG_TOOL == DEBUG_OPENMSX_P)
	g_PortDebugMode = 0xFF;
#endif
}

//-----------------------------------------------------------------------------
// Conditionnal break
void DEBUG_ASSERT(bool a)
{	
	if (!(a))
		DEBUG_BREAK();
}

//-----------------------------------------------------------------------------
// Display debug message
void DEBUG_LOG(const c8* msg)
{
#if (DEBUG_TOOL == DEBUG_OPENMSX_P)
	DEBUG_PRINT("%s", msg);
#else
	g_PortDebugMode = DEBUG_MODE_MULTI|DEBUG_MULTI_ASCII; // Multi byte ASCII mode
	while (*msg)
		g_PortDebugData = *msg++;
#endif
}

//-----------------------------------------------------------------------------
// Display debug message and a 8-bits value
void DEBUG_LOGNUM(const c8* msg, u8 num)
{
#if (DEBUG_TOOL == DEBUG_OPENMSX_P)
	DEBUG_PRINT("%s:%d", msg, num);
#else
	DEBUG_LOG(msg);
	g_PortDebugData = ':';
	g_PortDebugMode = DEBUG_NOLINEFEED|DEBUG_MODE_MULTI|DEBUG_MULTI_DEC; // Multi byte ASCII mode
	g_PortDebugData = num;
#endif
}

//-----------------------------------------------------------------------------
// Display debug formated message
void DEBUG_PRINT(const c8* format, ...)
{
#if (DEBUG_TOOL == DEBUG_OPENMSX_P)
    va_list args;
    va_start(args, format);
    u16 val = ((u16)args - sizeof(format));
	va_end(args);

	g_PortDebugMode = 1;
	g_PortDebugData = val & 0xFF; // low byte
	g_PortDebugData = val >> 8; // high byte
#else
	static c8 g_DebugBuffer[64];

	va_list args;
	va_start(args, format);
  	String_Format(g_DebugBuffer, format, &args);
	va_end(args);

	DEBUG_LOG(g_DebugBuffer);
#endif
}
#endif

//=============================================================================
// Support for Emulicious debugger/profiler
#if (DEBUG_TOOL == DEBUG_EMULICIOUS)

#include "vdp.h"
#include "input.h"

#define DEBUG_CMD_MAGIC				0x6464 // Emulator debug function request
#define DEBUG_CMD_PRINT_INLINE		0x0000 // Display the following string
#define DEBUG_CMD_PRINT_POINTER		0x0001 // Display string from following address (+ 16-bits bank number)
#define DEBUG_CMD_PRINT_REG_BC		0x0100 // Display string from address pointed by BC
#define DEBUG_CMD_PRINT_REG_DE		0x0101 // Display string from address pointed by DE
#define DEBUG_CMD_PRINT_REG_HL		0x0102 // Display string from address pointed by HL
#define DEBUG_CMD_PRINT_REG_AF		0x0103 // Display string from address pointed by AF
#define DEBUG_CMD_PRINT_REG_IX		0x0104 // Display string from address pointed by IX
#define DEBUG_CMD_PRINT_REG_IY		0x0105 // Display string from address pointed by IY
#define DEBUG_CMD_PRINT_REG_SP		0x0106 // Display string from address pointed by SP
#define DEBUG_CMD_PRINT_REG_PC		0x0107 // Display string from address pointed by PC
#define DEBUG_CMD_PRINT_FORMAT		0x0200 // Display a formatted string from address pointed by HL (parameters are in the stack)

//-----------------------------------------------------------------------------
// Initialize Debug module
void DEBUG_INIT()
{
#if (VDP_USE_COMMAND)
	// Fix assertion for un-initialized VDP buffer
	u8* ptr = (u8*)&g_VDP_Command;
	for (u8 i = 0; i < sizeof(g_VDP_Command); ++i)
		*(ptr++) = 0;
#endif
	
	// Fix assertion for BIOS-initialized key buffer
	#if (INPUT_KB_UPDATE)
	for (u8 i = INPUT_KB_UPDATE_MIN; i <= INPUT_KB_UPDATE_MAX; ++i)
		((u8*)g_NEWKEY)[i] = 0xFF;
	#endif
}

//-----------------------------------------------------------------------------
// Force a break point
void DEBUG_BREAK() __NAKED __PRESERVES(a, b, c, d, e, h, l, iyl, iyh)
{
__asm
	ld b, b
	ret
__endasm;
}

//-----------------------------------------------------------------------------
// Conditionnal break
void DEBUG_ASSERT(bool a)
{
	if (!(a))
		DEBUG_BREAK();
}

//-----------------------------------------------------------------------------
// Display debug message
void DEBUG_LOG(const c8* msg) __NAKED __PRESERVES(a, b, c, d, e, h, l, iyl, iyh)
{
	msg; // HL

__asm
	ld		d, d
	ret
	nop
	.dw		DEBUG_CMD_MAGIC // emulator debug function request
	.dw		DEBUG_CMD_PRINT_REG_HL // debug_printf function selected
__endasm;
}

//-----------------------------------------------------------------------------
// Display debug message and a 8-bits value
void DEBUG_LOGNUM(const c8* msg, u8 num)
{
	DEBUG_PRINT("%s: %i\n", msg, num);
}

//-----------------------------------------------------------------------------
// Display debug formated message
// from SMSlib - C programming library for the SMS/GG (part of devkitSMS - github.com/sverx/devkitSMS)
// https://github.com/sverx/devkitSMS/blob/master/SMSlib/src/SMSlib_debug.c
void DEBUG_PRINT(const c8 *format, ...) __NAKED __PRESERVES(a, b, c, iyh, iyl)
{
	format;

// basic debug_printf code kindly provided by toxa - thank you!
__asm
	ld		hl, #2
	add		hl, sp
	ld		e, (hl)
	inc		hl
	ld		d, (hl)
	inc		hl
	ex		de, hl  ; format string needs to be in HL
	ld		d, d
	ret
	nop
	.dw		DEBUG_CMD_MAGIC // emulator debug function request
	.dw		DEBUG_CMD_PRINT_FORMAT // debug_printf function selected
__endasm;
}

#endif