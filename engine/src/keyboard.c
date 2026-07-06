// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │   ██  ██▀▄ ██▀▄ ██ █ ██▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄██▄ ██ █ ██▀  ▀█▄█ ▀█▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘            ▀▀  
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// User input handler using direct access to ports
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================

#include "core.h"
#include "keyboard.h"
#include "system_port.h"
#include "bios_var.h"

//=============================================================================
// Direct access to keyboard
//=============================================================================

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
	u8* inputBufferNew = g_InputBufferNew;
	u8* inputBufferOld = g_InputBufferOld;
	
	for (u8 i = INPUT_KB_UPDATE_MIN; i <= INPUT_KB_UPDATE_MAX; ++i)	
	{
		*inputBufferOld++ = *inputBufferNew;
		*inputBufferNew++ = Keyboard_Read(i);
	}
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

#endif // (INPUT_KB_UPDATE)

#if (INPUT_KB_AS_JOYSTICK)

// Emulate joystick input using keyboard keys (see Joystick_Read)
//
// Return:
//   Keyboard state (bit=0: pressed)
// : xxBARLDU
// :   │││││└─ Up
// :   ││││└── Down
// :   │││└─── Left
// :   ││└──── Right
// :   │└───── Trigger A (space)
// :   └────── Trigger B (N)
u8 Keyboard_ReadAsJoystick() __NAKED __PRESERVES(e, h, l, iyl, iyh)
{
__asm
// Get keyboard row 8
	in		a, (P_PPI_C)
	and		#0b11110000				// only change bits 0-3
	ld		d, a					// backup PPI state
	or		#8						// set row number to 8
	out		(P_PPI_C), a

	in		a, (P_PPI_B)			// read row into A				[RDUL...1]
	rrca							//								[1RDUL...]
	rrca							//								[.1RDUL..]
	ld		b, a					// backup
	and		#0b00000100				// keep only Left				[-----L--]
	ld		c, a					// backup Left

	ld		a, b					// restore						[.1RDUL..]
	rrca							//								[..1RDUL.]
	rrca							//								[...1RDUL]
	ld		b, a					// backup
	and		#0b00011000				// keep only (1) and Right		[---1R---]
	or		a, c					// combine Left and Right		[---1RL--]
	ld		c, a					// backup (1) and Right

	ld		a, b					// restore						[...1RDUL]
	rrca							//								[L...1RDU]
	and		#0b00000011				// keep only Down & Up			[------DU]
	or		a, c					// combine with Up and Down		[---1RLDU]
	ld		b, a					// backup

// Get keyboard row 4
	ld		a, d					// restore PPI state
	or		#4						// set row number to 4
	out		(P_PPI_C), a
	in		a, (P_PPI_B)			// read row into A				[RQPONMLK]
	and		#0b00111000				// keep only N					[--PON---]
	rlca							//								[-PON----]
	rlca							//								[PON-----]

// Combine
	or		a, b					// Combine with previous result	[PON1RLDU]
	ret

__endasm;
}

#endif // (INPUT_KB_AS_JOYSTICK)