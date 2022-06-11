// ____________________________ 
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄   ▄▄▄  ▄▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██▄▀ ▀█▄  ██  
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██   ▄▄█▀ ▀█▄█
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// PSG handler module
//
// References:
//  - AY-3-8910 Programmable Sound Generator Data Manual
//     http://map.grauw.nl/resources/sound/generalinstrument_ay-3-8910.pdf
//  - YM2149 Programmable Software-Controlled Sound Generator Manual
//     http://map.grauw.nl/resources/sound/yamaha_ym2149.pdf
//  - The Revised MSX Red Book - PSG
//     https://www.angelfire.com/art2/unicorndreams/msx/RR-PSG.html
//─────────────────────────────────────────────────────────────────────────────
#include "psg.h"

//=============================================================================
// READ-ONLY DATA
//=============================================================================

#if (PSG_USE_NOTES)
const u16 g_PSG_Notes[] = {
//	C      C#     D      D#     E      F      F#     G      G#     A      A#     B
//	Do     Do#    Re     Re#    Mi     Fa     Fa#    Sol    Sol#   La     La#    Si        Octave
	0xD5D, 0xC9C, 0xBE7, 0xB3C, 0xA9B, 0xA02, 0x973, 0x8EB, 0x86B, 0x7F2, 0x780, 0x714, // 1
	0x6AF, 0x64E, 0x5F4, 0x59E, 0x54E, 0x501, 0x4BA, 0x476, 0x436, 0x3F9, 0x3C0, 0x38A, // 2
	0x357, 0x327, 0x2FA, 0x2CF, 0x2A7, 0x281, 0x25D, 0x23B, 0x21B, 0x1FD, 0x1E0, 0x1C5, // 3
	0x1AC, 0x194, 0x17D, 0x168, 0x153, 0x140, 0x12E, 0x11D, 0x10D, 0x0FE, 0x0F0, 0x0E3, // 4
	0x0D6, 0x0CA, 0x0BE, 0x0B4, 0x0AA, 0x0A0, 0x097, 0x08F, 0x087, 0x07F, 0x078, 0x071, // 5
	0x06B, 0x065, 0x05F, 0x05A, 0x055, 0x050, 0x04C, 0x047, 0x043, 0x040, 0x03C, 0x039, // 6
	0x035, 0x032, 0x030, 0x02D, 0x02A, 0x028, 0x026, 0x024, 0x022, 0x020, 0x01E, 0x01C, // 7
	0x01B, 0x019, 0x018, 0x016, 0x015, 0x014, 0x013, 0x012, 0x011, 0x010, 0x00F, 0x00E, // 8
};
#endif

//=============================================================================
// MEMORY DATA
//=============================================================================

#if (PSG_ACCESS == PSG_INDIRECT)
struct PSG_Data g_PSG_Regs;
#if (PSG_CHIP == PSG_BOTH)
struct PSG_Data g_PSG2_Regs;
#endif
#endif

#if (PSG_USE_RESUME)
u8 g_PSG_VolumeBackup[3];
#endif

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Set the value of a given register
void PSG_SetRegister(u8 reg, u8 value)
{
#if (PSG_ACCESS == PSG_INDIRECT)
	
	u8* ptr = (u8*)g_PSG_Regs;
	ptr += reg;
	*ptr = value;

#else // if (PSG_ACCESS == PSG_DIRECT)

	PSG_VAR_REG = reg;
	PSG_VAR_WRITE = value;

#endif
}

//-----------------------------------------------------------------------------
// Get the value of a given register
u8 PSG_GetRegister(u8 reg)
{
#if (PSG_ACCESS == PSG_INDIRECT)

	u8* ptr = (u8*)g_PSG_Regs;
	ptr += reg;
	return *ptr;

#else // if (PSG_ACCESS == PSG_DIRECT)

	PSG_VAR_REG = reg;
	return PSG_VAR_READ;

#endif
}

#if (PSG_USE_EXTRA)

//-----------------------------------------------------------------------------
// Set the tone period of a given channel (tone generator control register)
void PSG_SetTone(u8 chan, u16 period)
{
#if (PSG_ACCESS == PSG_INDIRECT)

	u8* ptr = (u8*)g_PSG_Regs;
	ptr += (chan * 2);
	*ptr = (u8)period;
	*++ptr = (u8)(period >> 8);

#else // if (PSG_ACCESS == PSG_DIRECT)

	u8 reg = (chan * 2);
	PSG_VAR_REG = reg;
	PSG_VAR_WRITE = (u8)period;
	PSG_VAR_REG = ++reg;
	PSG_VAR_WRITE = (u8)(period >> 8);

#endif
}

//-----------------------------------------------------------------------------
// Set the noise period (noise generator control register)
void PSG_SetNoise(u8 period)
{
#if (PSG_ACCESS == PSG_INDIRECT)

	u8* ptr = (u8*)g_PSG_Regs + PSG_REG_NOISE;
	*ptr = period;

#else // if (PSG_ACCESS == PSG_DIRECT)

	PSG_VAR_REG = PSG_REG_NOISE;
	PSG_VAR_WRITE = period;

#endif
}

//-----------------------------------------------------------------------------
// Setup mixer by enabling tune and noise generators for each channel (mixer control enable register)
void PSG_SetMixer(u8 mix)
{
#if (PSG_ACCESS == PSG_INDIRECT)

	u8* ptr = (u8*)g_PSG_Regs + PSG_REG_MIXER;
	*ptr = ~mix; // with PSG_INDIRECT bit #6 & #7 are handled in PSG_Apply() function

#else // if (PSG_ACCESS == PSG_DIRECT)

	PSG_VAR_REG = PSG_REG_MIXER;
	PSG_VAR_WRITE = ~mix;

#endif
}

//-----------------------------------------------------------------------------
// Set the volume of a given channel (Amplitude control register)
void PSG_SetVolume(u8 chan, u8 vol)
{
#if (PSG_ACCESS == PSG_INDIRECT)

	u8* ptr = (u8*)g_PSG_Regs + PSG_REG_AMP_A;
	ptr += chan;
	*ptr = vol;

#else // if (PSG_ACCESS == PSG_DIRECT)

	PSG_VAR_REG = PSG_REG_AMP_A + chan;
	PSG_VAR_WRITE = vol;

#endif
}

//-----------------------------------------------------------------------------
// Set the envelope period (Envelope priod control register)
void PSG_SetEnvelope(u16 period)
{
#if (PSG_ACCESS == PSG_INDIRECT)

	g_PSG_Regs.Envelope = period;

#else // if (PSG_ACCESS == PSG_DIRECT)

	u8 reg = PSG_REG_ENV;
	PSG_VAR_REG = reg;
	PSG_VAR_WRITE = (u8)period;
	PSG_VAR_REG = ++reg;
	PSG_VAR_WRITE = (u8)(period >> 8);

#endif
}

//-----------------------------------------------------------------------------
// Set the envelope shape (Envelope shape control register)
void PSG_SetShape(u8 shape)
{
#if (PSG_ACCESS == PSG_INDIRECT)

	g_PSG_Regs.Shape = shape;

#else // if (PSG_ACCESS == PSG_DIRECT)

	PSG_VAR_REG = PSG_REG_SHAPE;
	PSG_VAR_WRITE = shape;

#endif
}

//-----------------------------------------------------------------------------
//
void PSG_EnableTone(u8 chan, u8 val)
{
#if (PSG_ACCESS == PSG_INDIRECT)
	u8 mix = g_PSG_Regs.Mixer;
#else // if (PSG_ACCESS == PSG_DIRECT)
	PSG_VAR_REG = PSG_REG_MIXER;
	u8 mix = PSG_VAR_READ;
#endif

	u8 bit = 1 << chan;
	mix &= ~bit;
	
	if(val == 0)
		mix |= bit;
	
#if (PSG_ACCESS == PSG_INDIRECT)
	g_PSG_Regs.Mixer = mix;
#else // if (PSG_ACCESS == PSG_DIRECT)
	PSG_VAR_WRITE = mix;
#endif
}

//-----------------------------------------------------------------------------
//
void PSG_EnableNoise(u8 chan, u8 val)
{
#if (PSG_ACCESS == PSG_INDIRECT)
	u8 mix = g_PSG_Regs.Mixer;
#else // if (PSG_ACCESS == PSG_DIRECT)
	PSG_VAR_REG = PSG_REG_MIXER;
	u8 mix = PSG_VAR_READ;
#endif

	u8 bit = 8 << chan;
	mix &= ~bit;
	
	if(val == 0)
		mix |= bit;
	
#if (PSG_ACCESS == PSG_INDIRECT)
	g_PSG_Regs.Mixer = mix;
#else // if (PSG_ACCESS == PSG_DIRECT)
	PSG_VAR_WRITE = mix;
#endif
}

//-----------------------------------------------------------------------------
//
void PSG_EnableEnvelope(u8 chan, u8 val)
{
#if (PSG_ACCESS == PSG_INDIRECT)
	u8 vol = g_PSG_Regs.Volume[chan];
#else // if (PSG_ACCESS == PSG_DIRECT)
	PSG_VAR_REG = PSG_REG_AMP_A + chan;
	u8 vol = PSG_VAR_READ;
#endif

	vol &= 0x0F;
	
	if(val != 0)
		vol |= PSG_F_ENV;
	
#if (PSG_ACCESS == PSG_INDIRECT)
	g_PSG_Regs.Volume[chan] = vol;
#else // if (PSG_ACCESS == PSG_DIRECT)
	PSG_VAR_WRITE = vol;
#endif
}

#endif // (PSG_USE_EXTRA)

//-----------------------------------------------------------------------------
//
void PSG_Mute()
{
#if (PSG_ACCESS == PSG_INDIRECT)

	loop(i, 3)
	{
		#if (PSG_USE_RESUME)
		g_PSG_VolumeBackup[i] = g_PSG_Regs.Volume[i];
		#endif
		g_PSG_Regs.Volume[i] = 0;
	}

#else // if (PSG_ACCESS == PSG_DIRECT)

	loop(i, 3)
	{
		PSG_VAR_REG = PSG_REG_AMP_A + i;
		#if (PSG_USE_RESUME)
		g_PSG_VolumeBackup[i] = PSG_VAR_READ;
		#endif
		PSG_VAR_WRITE = 0;
	}

#endif
}

#if (PSG_USE_RESUME)
//-----------------------------------------------------------------------------
// Resume PSG sound
void PSG_Resume()
{
#if (PSG_ACCESS == PSG_INDIRECT)

	loop(i, 3)
	{
		g_PSG_Regs.Volume[i] = g_PSG_VolumeBackup[i];
	}

#else // if (PSG_ACCESS == PSG_DIRECT)

	loop(i, 3)
	{
		PSG_VAR_REG = PSG_REG_AMP_A + i;
		PSG_VAR_WRITE = g_PSG_VolumeBackup[i];
	}

#endif
}
#endif


#if (PSG_ACCESS == PSG_INDIRECT)
//-----------------------------------------------------------------------------
// Send data to PSG registers #1 to #13
// @note					Must be executed on each V-Blank interruption
void PSG_Apply()
{
__asm
	// Update mixer register wanted value with I/O 2-bits from the current mixer register value
	ld		A, (#_g_PSG_Regs + PSG_REG_MIXER)
	and		#0b00111111				// Keep register value but the higher 2-bits
	ld		B, A					// Store in B
	ld		A, #PSG_REG_MIXER		// Select R#7
	out		(#PSG_PORT_REG), A
	in		A, (#PSG_PORT_READ)		// Read R#7
	and		#0b11000000				// Keep the higher 2-bits of R#7
	or		B						// Merge
	ld		(#_g_PSG_Regs + PSG_REG_MIXER), A

	// Registers value copy loop
	ld		HL, #_g_PSG_Regs		// Data to copy to PSG registers
	ld		C, #PSG_PORT_WRITE		// Setup outi register
	xor		A						// Initialize register number
	// R#0-12
	.rept 13
		out		(PSG_PORT_REG), A	// port_reg <- reg_num
		outi						// port_data <- data[i++]
		inc		A					// 
	.endm
	// R#13
	out		(PSG_PORT_REG), A		// port_reg <- reg_num
	ld		A, (HL)					// 
	and		A						// 
	jp		M, PSG_End				// don't copy R#13 if value is negative
	out		(PSG_PORT_WRITE), A		// port_data <- data[i]
PSG_End:
	or		#0x80					// 
	ld		(HL), A					// 

#if (PSG_CHIP == PSG_BOTH)
	// Update mixer register wanted value with I/O 2-bits from the current mixer register value
	ld		A, (#_g_PSG2_Regs + PSG_REG_MIXER)
	and		#0b00111111				// Keep register value but the higher 2-bits
	ld		B, A					// Store in B
	ld		A, #PSG_REG_MIXER		// Select R#7
	out		(#PSG2_PORT_REG), A
	in		A, (#PSG2_PORT_READ)		// Read R#7
	and		#0b11000000				// Keep the higher 2-bits of R#7
	or		B						// Merge
	ld		(#_g_PSG2_Regs + PSG_REG_MIXER), A

	// Registers value copy loop
	ld		HL, #_g_PSG2_Regs		// Data to copy to PSG registers
	ld		C, #PSG2_PORT_WRITE		// Setup outi register
	xor		A						// Initialize register number
	// R#0-12
	.rept 13
		out		(PSG2_PORT_REG), A	// port_reg <- reg_num
		outi						// port_data <- data[i++]
		inc		A					// 
	.endm
	// R#13
	out		(PSG2_PORT_REG), A		// port_reg <- reg_num
	ld		A, (HL)					// 
	and		A						// 
	jp		M, PSG2_End				// don't copy R#13 if value is negative
	out		(PSG2_PORT_WRITE), A	// port_data <- data[i]
PSG2_End:
	or		#0x80					// 
	ld		(HL), A					// 
#endif

__endasm;
}

#endif // (PSG_ACCESS == PSG_INDIRECT)