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
#include "joystick.h"
#include "system_port.h"
#include "psg_reg.h"


//-----------------------------------------------------------------------------
// Constant data
//-----------------------------------------------------------------------------

//=============================================================================
// Direct access to joystick
//=============================================================================

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