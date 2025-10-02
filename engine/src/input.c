// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │   ██  ██▀▄ ██▀▄ ██ █ ██▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄██▄ ██ █ ██▀  ▀█▄█ ▀█▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘            ▀▀  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// User input handler using direct access to ports
//─────────────────────────────────────────────────────────────────────────────
#include "core.h"
#include "system_port.h"
#include "input.h"
#include "bios_var.h"
#include "psg_reg.h"

//-----------------------------------------------------------------------------
// Constant data
//-----------------------------------------------------------------------------

//=============================================================================
// General purpose port device detection
//=============================================================================
#if (1)//(INPUT_USE_DETECT)

//-----------------------------------------------------------------------------
// Detect device plugged in General purpose ports
u8 Input_Detect(enum INPUT_PORT port) __NAKED __PRESERVES(b, c, d, e, iyl, iyh)
{
	port; // A

__asm
	ld		h, a					// Pin 8 HIGH
	and		#0b11001111
	ld		l, a					// Pin 8 LOW

	ld		a, #PSG_REG_IO_PORT_B	// R#15
	INPUT_DI //-------- Disable interrupts --------
	out		(P_PSG_REGS), a			// Select port B (output)
	ld		a, l					// 
	out		(P_PSG_DATA), a			// Set Pin 8 LOW

	ld		a, #PSG_REG_IO_PORT_B	// R#15
	out		(P_PSG_REGS), a			// Select port B (output)
	ld		a, h					// 
	out		(P_PSG_DATA), a			// Set Pin 8 HIGH

	ld		a, #PSG_REG_IO_PORT_A	// R#14
	out		(P_PSG_REGS), a			// Select port A (input)
	INPUT_EI //-------- Enable interrupts --------
	in		a, (P_PSG_STAT)			//
	and		#0x3F

	ret
__endasm;
}

#endif

//=============================================================================
// Direct access to joystick
//=============================================================================
#if (INPUT_USE_JOYSTICK)

//-----------------------------------------------------------------------------
// Get the current joystick information (bit=0: pressed)
// Input  : JOY_PORT_1 or JOY_PORT_2
// Output : xxBARLDU
//            │││││└─ Up
//            ││││└── Down
//            │││└─── Left
//            ││└──── Right
//            │└───── Trigger A
//            └────── Trigger B
u8 Joystick_Read(u8 port) __NAKED __FASTCALL __PRESERVES(b, c, d, e, h, iyl, iyh)
{
	port; // L

__asm
	ld		a, #PSG_REG_IO_PORT_B	// R#15
	INPUT_DI //-------- Disable interrupts --------
	out		(P_PSG_REGS), a			// Select port B
#if (INPUT_HOLD_SIGNAL)
	in		a, (P_PSG_STAT)			// Read port B value
	res		6, a
	or		a, l					// Select witch joystick connector is connected to PSG Port A
	out		(P_PSG_DATA), a			// Write port B value
#else
	ld		a, l					// Select witch joystick connector is connected to PSG Port A
	out		(P_PSG_DATA), a			// Write port B value
#endif

	ld		a, #PSG_REG_IO_PORT_A	// R#14
	out		(P_PSG_REGS), a			// Select port A
	INPUT_EI //-------- Enable interrupts --------
	in		a, (P_PSG_STAT)			// Read port A value
	ld		l, a					// Return value

	ret
__endasm;
}

#if (INPUT_JOY_UPDATE)

u8 g_JoyStats[2] = { 0x3F, 0x3F };
u8 g_JoyStatsPrev[2] = { 0x3F, 0x3F };

//-----------------------------------------------------------------------------
// Update both joystick stats at once and store the result
void Joystick_Update()
{
	g_JoyStatsPrev[0] = g_JoyStats[0];
	g_JoyStats[0] = ~Joystick_Read(JOY_PORT_1);
	g_JoyStatsPrev[1] = g_JoyStats[1];
	g_JoyStats[1] = ~Joystick_Read(JOY_PORT_2);
}

#else // !(INPUT_JOY_UPDATE)

//-----------------------------------------------------------------------------
// Get current direction of the given joystick
// Input  : JOY_PORT_1 or JOY_PORT_2
/*u8 Joystick_GetDirection(u8 port) __FASTCALL
{
	u8 in = Joystick_Read(port);
	in = ~in;
	in &= JOY_INPUT_DIR_MASK;
	return in;
}*/

//-----------------------------------------------------------------------------
// Get current trigger status of the given joystick (0: released; 1: pressed)
// Input  : JOY_PORT_1 or JOY_PORT_2
//          JOY_INPUT_TRIGGER_A or JOY_INPUT_TRIGGER_B
/*inline u8 Joystick_GetTrigger(u8 port, u8 trigger)
{
	u8 in = Joystick_Read(port);
	return ((in & trigger) == 0);
}*/

#endif // (INPUT_JOY_UPDATE)

#endif // (INPUT_USE_JOYSTICK)


//=============================================================================
// Direct access to mouse
//=============================================================================
#if (INPUT_USE_MOUSE)

i8 g_Mouse_OffsetX;
i8 g_Mouse_OffsetY;
u8 g_Mouse_Buttons;

//-----------------------------------------------------------------------------
// 
void Mouse_Read(u8 port, Mouse_State* data) __NAKED __PRESERVES(iyl, iyh)
{
	port; // A
	data; // DE

__asm
	WAIT1 = 10	// Short delay value
	WAIT2 = 30	// Long delay value

// Routine to read the mouse by direct accesses (works on MSX1/2/2+/turbo R)
// Original function by FRS and/or MRC users (https://www.msx.org/wiki/Mouse/Trackball)
//
// Input:  HL = 09310h for mouse in port 1 (H = 10010011b, L = 00010000b)
//         HL = 0EC20h for mouse in port 2 (H = 11101100b, L = 00100000b)
// Output: D = X-offset, E = Y-offset (D = E = 255 if no mouse)
GTMOUS:
	ld		l, a					// Backup input port value 

	ld		a, (de)					// Get previous mouse button value
	ld		i, a

	// Get current Code/Kana LED state
	ld		a, #PSG_REG_IO_PORT_B	// Select PSG R#15
	INPUT_DI
	out		(P_PSG_REGS), a
	INPUT_EI
	in		a, (P_PSG_STAT)			// Read from PSG R#15
	and		#0x80					// Keep current Code/Kana LED state
	or		l
	ld		h, a
	ld		l, a

	// Read first X-offset nibble (bits 4-7)
	ld		b, #WAIT2				// Long delay for first read
	call	GTOFS2					// Get R#14 content
	ld		(de), a					// Store mouse button value
	and		#0x0F
	rlca
	rlca
	rlca
	rlca
	ld		c, a					// Backup bits 4-7 of the X-offset
	// Read second X-offset nibble (bits 0-3)
	ld		a, h
	and		#0b11001111
	ld		l, a
	call	GTOFST					// Read bits 0-3 of the X-offset
	and		#0x0F
	or		c
	inc		de
	ld		(de), a					// Store combined X-offset

	// Read first Y-offset nibble (bits 7-4)
	ld		l, h
	call	GTOFST					// Read bits 7-4 of the y-offset
	and		#0x0F
	rlca
	rlca
	rlca
	rlca
	ld		c, a
	// Read second Y-offset nibble (bits 0-3)
	ld		a, h
	and		#0b11001111
	ld		l, a
	call	GTOFST					// Read bits 3-0 of the y-offset
	and		#0x0F
	or		c
	inc		de
	ld		(de), a					// Store combined Y-offset

	ld		a, i
	inc		de
	ld		(de), a					// Store previous mouse button value

	ret

GTOFST:
	ld		b, #WAIT1
GTOFS2:
	ld		a, #PSG_REG_IO_PORT_B	// Select PSG R#15 for mouse
	INPUT_DI
	out		(P_PSG_REGS), a
	ld		a, l					// Port 1=10010011b, 2=11101100b
	out		(P_PSG_DATA), a			// Write to PSG R#15

	call	WAITMS					// Extra delay because the mouse is slow

	ld		a, #PSG_REG_IO_PORT_A	// Select PSG R#14 for mouse
	out		(P_PSG_REGS), a
	INPUT_EI
	in		a, (P_PSG_STAT)			// Read to PSG R#14
	ret

WAITMS:
	ld		a, b
WTTR:
	djnz	WTTR
	.db		0xED, 0x55				// Back if Z80 (RETN on Z80, NOP on R800)
	rlca
	rlca
	ld		b, a
WTTR2:
	djnz	WTTR2
	ld		b, a
WTTR3:
	djnz	WTTR3

	ret
__endasm;
}

#endif // (INPUT_USE_MOUSE)

//=============================================================================
// Direct access to keyboard
//=============================================================================
#if (INPUT_USE_KEYBOARD)

//-----------------------------------------------------------------------------
// Read keyboard matrix row
u8 Keyboard_Read(u8 row) __NAKED __FASTCALL __PRESERVES(b, c, d, e, h, iyl, iyh)
{
	row; // L

__asm
	in		a, (P_PPI_C)
	and		#0xF0					// only change bits 0-3
	or		l						// take row number from L
	out		(P_PPI_C), a
	in		a, (P_PPI_B)			// read row into A
	ld		l, a

	ret
__endasm;
}

#if (INPUT_KB_UPDATE)

// Buffer to store current frame keys state
u8* g_InputBufferNew = (u8*)g_NEWKEY;
// Buffer to store previous frame keys state
u8* g_InputBufferOld = (u8*)g_OLDKEY;

//-----------------------------------------------------------------------------
// Update all keyboard rows at once
void Keyboard_Update()
{
	for (u8 i = INPUT_KB_UPDATE_MIN; i <= INPUT_KB_UPDATE_MAX; ++i)	
	{
		g_InputBufferOld[i] = g_InputBufferNew[i];
		g_InputBufferNew[i] = Keyboard_Read(i);
	}
}

//-----------------------------------------------------------------------------
// Check if a given key is pressed
bool Keyboard_IsKeyPressed(u8 key)
{
	return (g_InputBufferNew[KEY_ROW(key)] & (1 << KEY_IDX(key))) == 0;
}

//-----------------------------------------------------------------------------
// Check if a given key is just pushed
bool Keyboard_IsKeyPushed(u8 key)
{
	u8 flag = 1 << KEY_IDX(key);
	u8 newKey = (g_InputBufferNew[KEY_ROW(key)] & flag) == 0;
	u8 oldKey = (g_InputBufferOld[KEY_ROW(key)] & flag) == 0;
	return newKey && !oldKey;
}

#else // if !(INPUT_KB_UPDATE)

//-----------------------------------------------------------------------------
// Check if a given key is pressed
u8 Keyboard_IsKeyPressed(u8 key)
{
	return (Keyboard_Read(KEY_ROW(key)) & (1 << KEY_IDX(key))) == 0;
}

#endif // (INPUT_KB_UPDATE)

#endif // (INPUT_USE_KEYBOARD)