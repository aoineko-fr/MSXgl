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

#if ((PSG_ACCESS == PSG_DIRECT) && (PSG_DEVICE_NUM > 1))
// Register port of each device (data write port is always the next one)
const u8 g_PSG_Ports[] =
{
#if (PSG_DEVICE & PSG_DEVICE_INTERN)
	P_PSG_REGS,
#endif
#if (PSG_DEVICE & PSG_DEVICE_EXTERN)
	P_PSG_EXT_REGS,
#endif
#if (PSG_DEVICE & PSG_DEVICE_DARKY)
	P_DARKY_PSG1_IDX,
	P_DARKY_PSG2_IDX,
#endif
#if (PSG_DEVICE & PSG_DEVICE_OPNA)
	P_OPNA_ADDR0,
#endif
};
#endif

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
u8 g_PSG_Regs[PSG_DEVICE_NUM][16];
#endif

#if (PSG_USE_PAUSE)
u8 g_PSG_VolumeBackup[PSG_DEVICE_NUM * 3];
#endif

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Initialize the PSG
void PSG_Initialize()
{
	#if (PSG_DEVICE & PSG_DEVICE_DARKY)
	io_Darky_Enable = 170;  // Activate Darky's 2x ePSG
	#endif
	#if (PSG_DEVICE & PSG_DEVICE_OPNA)
	io_OPNA_Reg0 = OPNA_REG_PRESCALER_6; // Set prescaler to 1/6 (SSG clock = φM/4)
	io_OPNA_Data0 = 0;
	#endif
}

//-----------------------------------------------------------------------------
// Set the value of a given register
void PSG_SetRegister(u8 reg, u8 value) __NAKED __PRESERVES(d, e, iyl, iyh)
{
	reg;	// A
	value;	// L

	#if (PSG_ACCESS == PSG_INDIRECT)
	__asm
		// g_PSG_Regs[reg] = value;
		ld		c, l					// C = value
		ld		hl, #_g_PSG_Regs
		add		a, l
		ld		l, a
		adc		a, h
		sub		l
		ld		h, a					// HL = &g_PSG_Regs[reg]
		ld		(hl), c
		ret
	__endasm;
	#elif (PSG_DEVICE_NUM == 1)
	__asm
		// PSG_VAR_REG = reg; PSG_VAR_WRITE = value;
		out		(PSG_PORT_REG), a
		ld		a, l
		out		(PSG_PORT_WRITE), a
		ret
	__endasm;
	#else
	__asm
		// io = g_PSG_Ports[reg >> 4]; io = reg; io++; io = value;
		ld		b, l					// B = value
		ld		c, a					// C = reg
		rrca
		rrca
		rrca
		rrca
		and		#0x0F					// A = device index
		ld		hl, #_g_PSG_Ports
		add		a, l
		ld		l, a
		adc		a, h
		sub		l
		ld		h, a					// HL = &g_PSG_Ports[dev]
		ld		a, c
		and		#0x0F					// A = register number
		ld		c, (hl)					// C = register port
		out		(c), a
		inc		c						// C = data port
		out		(c), b
		ret
	__endasm;
	#endif
}

//-----------------------------------------------------------------------------
// Get the value of a given register
u8 PSG_GetRegister(u8 reg) __NAKED __PRESERVES(d, e, iyl, iyh)
{
	reg;	// A

	#if (PSG_ACCESS == PSG_INDIRECT)
	__asm
		// return g_PSG_Regs[reg];
		ld		hl, #_g_PSG_Regs
		add		a, l
		ld		l, a
		adc		a, h
		sub		l
		ld		h, a					// HL = &g_PSG_Regs[reg]
		ld		a, (hl)
		ret
	__endasm;
	#elif (PSG_DEVICE_NUM == 1)
	__asm
		// PSG_VAR_REG = reg; return PSG_VAR_READ;
		out		(PSG_PORT_REG), a
		in		a, (PSG_PORT_READ)
		ret
	__endasm;
	#else
	__asm
		// io = g_PSG_Ports[reg >> 4]; io = reg; io += 2; return io;
		ld		c, a					// C = reg
		rrca
		rrca
		rrca
		rrca
		and		#0x0F					// A = device index
		ld		b, a					// B = device index
		ld		hl, #_g_PSG_Ports
		add		a, l
		ld		l, a
		adc		a, h
		sub		l
		ld		h, a					// HL = &g_PSG_Ports[dev]
		ld		a, c
		and		#0x0F					// A = register number
		ld		c, (hl)					// C = register port
		out		(c), a
		inc		c						// C = data port
	#if (PSG_DEVICE & PSG_DEVICE_OPNA)
		ld		a, b
		cp		#PSG_OPNA_IDX
		jr		z, 10$					// OPNA: read port = data port
	#endif
		inc		c						// C = read port
	10$:
		in		a, (c)
		ret
	__endasm;
	#endif
}

#if (PSG_USE_EXTRA)

//-----------------------------------------------------------------------------
// Set the tone period of a given channel (tone generator control register)
void PSG_SetTone(u8 chan, u16 period)
{
#if (PSG_ACCESS == PSG_INDIRECT)

	u8* ptr = &g_PSG_Regs[0][chan * 2];
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

	g_PSG_Regs[0][PSG_REG_NOISE] = period;

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

	g_PSG_Regs[0][PSG_REG_MIXER] = ~mix; // with PSG_INDIRECT bit #6 & #7 are handled in PSG_Apply() function

#else // if (PSG_ACCESS == PSG_DIRECT)

	PSG_VAR_REG = PSG_REG_MIXER;
	PSG_VAR_WRITE = (PSG_VAR_READ & 0b11000000) | (~mix & 0b00111111); // Keep I/O port direction bits #6 & #7

#endif
}

//-----------------------------------------------------------------------------
// Set the volume of a given channel (Amplitude control register)
void PSG_SetVolume(u8 chan, u8 vol)
{
#if (PSG_ACCESS == PSG_INDIRECT)

	g_PSG_Regs[0][PSG_REG_AMP_A + chan] = vol;

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

	g_PSG_Regs[0][PSG_REG_ENV] = (u8)period;
	g_PSG_Regs[0][PSG_REG_ENV + 1] = (u8)(period >> 8);

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

	g_PSG_Regs[0][PSG_REG_SHAPE] = shape;

#else // if (PSG_ACCESS == PSG_DIRECT)

	PSG_VAR_REG = PSG_REG_SHAPE;
	PSG_VAR_WRITE = shape;

#endif
}

//-----------------------------------------------------------------------------
//
void PSG_EnableTone(u8 chan, bool val)
{
#if (PSG_ACCESS == PSG_INDIRECT)
	u8 mix = g_PSG_Regs[0][PSG_REG_MIXER];
#else // if (PSG_ACCESS == PSG_DIRECT)
	PSG_VAR_REG = PSG_REG_MIXER;
	u8 mix = PSG_VAR_READ;
#endif

	u8 bit = 1 << chan;
	mix &= ~bit;

	if (val == 0)
		mix |= bit;

#if (PSG_ACCESS == PSG_INDIRECT)
	g_PSG_Regs[0][PSG_REG_MIXER] = mix;
#else // if (PSG_ACCESS == PSG_DIRECT)
	PSG_VAR_WRITE = mix;
#endif
}

//-----------------------------------------------------------------------------
//
void PSG_EnableNoise(u8 chan, bool val)
{
#if (PSG_ACCESS == PSG_INDIRECT)
	u8 mix = g_PSG_Regs[0][PSG_REG_MIXER];
#else // if (PSG_ACCESS == PSG_DIRECT)
	PSG_VAR_REG = PSG_REG_MIXER;
	u8 mix = PSG_VAR_READ;
#endif

	u8 bit = 8 << chan;
	mix &= ~bit;

	if (val == 0)
		mix |= bit;

#if (PSG_ACCESS == PSG_INDIRECT)
	g_PSG_Regs[0][PSG_REG_MIXER] = mix;
#else // if (PSG_ACCESS == PSG_DIRECT)
	PSG_VAR_WRITE = mix;
#endif
}

//-----------------------------------------------------------------------------
//
void PSG_EnableEnvelope(u8 chan, bool val)
{
#if (PSG_ACCESS == PSG_INDIRECT)
	u8 vol = g_PSG_Regs[0][PSG_REG_AMP_A + chan];
#else // if (PSG_ACCESS == PSG_DIRECT)
	PSG_VAR_REG = PSG_REG_AMP_A + chan;
	u8 vol = PSG_VAR_READ;
#endif

	vol &= 0x0F;

	if (val != 0)
		vol |= PSG_F_ENV;

#if (PSG_ACCESS == PSG_INDIRECT)
	g_PSG_Regs[0][PSG_REG_AMP_A + chan] = vol;
#else // if (PSG_ACCESS == PSG_DIRECT)
	PSG_VAR_WRITE = vol;
#endif
}

//-----------------------------------------------------------------------------
// Get the tone period of a given channel
u16 PSG_GetTone(u8 chan)
{
	u8 reg = chan * 2;
	u8 low = PSG_GetRegister(reg);
	return ((u16)PSG_GetRegister(++reg) << 8) | low;
}

//-----------------------------------------------------------------------------
// Get the envelope period
u16 PSG_GetEnvelope()
{
	u8 low = PSG_GetRegister(PSG_REG_ENV);
	return ((u16)PSG_GetRegister(PSG_REG_ENV + 1) << 8) | low;
}

#endif // (PSG_USE_EXTRA)

//-----------------------------------------------------------------------------
// Silent all PSG devices
void PSG_Mute()
{
	u8 reg = PSG_REG_AMP_A;
	loop (i, PSG_DEVICE_NUM * 3)
	{
		#if (PSG_USE_PAUSE)
		g_PSG_VolumeBackup[i] = PSG_GetRegister(reg);
		#endif
		PSG_SetRegister(reg, 0);
		reg++;
		if ((reg & 0x0F) > PSG_REG_AMP_C) // Next device
			reg += 0x10 - 3;
	}
}

#if (PSG_USE_PAUSE)
//-----------------------------------------------------------------------------
// Resume all PSG devices sound
void PSG_Resume()
{
	u8 reg = PSG_REG_AMP_A;
	loop (i, PSG_DEVICE_NUM * 3)
	{
		PSG_SetRegister(reg, g_PSG_VolumeBackup[i]);
		reg++;
		if ((reg & 0x0F) > PSG_REG_AMP_C) // Next device
			reg += 0x10 - 3;
	}
}
#endif

#if (PSG_USE_CHANNEL)
//-----------------------------------------------------------------------------
// Get the current volume of a given channel (0-2: 1st device, 3-5: 2nd device, etc.)
u8 PSG_GetChannelVolume(u8 chan)
{
	u8 dev = 0;
#if (PSG_DEVICE_NUM > 1)
	while (chan >= 3)
	{
		chan -= 3;
		dev += 0x10;
	}
#endif
	u8 mix = PSG_GetRegister(dev | PSG_REG_MIXER);
	u8 amp = PSG_GetRegister(dev | (PSG_REG_AMP_A + chan));

	u8 off = 0b00001001 << chan; // Tone and noise disable bits of the channel
	if ((mix & off) == off)
		return 0;
	if (amp & PSG_F_ENV)
		return 15;
	return amp & 0x0F;
}
#endif

#if (PSG_ACCESS == PSG_INDIRECT)
//-----------------------------------------------------------------------------
// Send data to registers #0 to #13 of all PSG devices
// @note					Must be executed on each V-Blank interruption
void PSG_Apply() __NAKED __PRESERVES(d, e, iyl, iyh)
{
__asm
// Send the registers buffer of one device
.macro PSG_APPLY_DEVICE buf, preg, pwrite, pread, ?skip
	// Update mixer register wanted value with I/O 2-bits from the current mixer register value
	ld		a, (buf + PSG_REG_MIXER)
	and		#0b00111111				// Keep register value but the higher 2-bits
	ld		b, a					// Store in B
	ld		a, #PSG_REG_MIXER		// Select R#7
	out		(preg), a
	in		a, (pread)				// Read R#7
	and		#0b11000000				// Keep the higher 2-bits of R#7
	or		b						// Merge
	ld		(buf + PSG_REG_MIXER), a

	// Registers value copy loop
	ld		hl, #buf				// Data to copy to PSG registers
	ld		c, #pwrite				// Setup outi register
	xor		a						// Initialize register number
	// R#0-12
	.rept 13
	out		(preg), a				// port_reg <- reg_num
	outi							// port_data <- data[i++]
	inc		a
	.endm
	// R#13
	out		(preg), a				// port_reg <- reg_num
	ld		a, (hl)
	and		a
	jp		m, skip					// Don't copy R#13 if value is negative
	out		(pwrite), a				// port_data <- data[i]
skip:
	or		#0x80
	ld		(hl), a
.endm

#if (PSG_DEVICE & PSG_DEVICE_INTERN)
	PSG_APPLY_DEVICE ^/_g_PSG_Regs+PSG_INTERN_IDX*16/, P_PSG_REGS, P_PSG_DATA, P_PSG_STAT
#endif
#if (PSG_DEVICE & PSG_DEVICE_EXTERN)
	PSG_APPLY_DEVICE ^/_g_PSG_Regs+PSG_EXTERN_IDX*16/, P_PSG_EXT_REGS, P_PSG_EXT_DATA, P_PSG_EXT_STAT
#endif
#if (PSG_DEVICE & PSG_DEVICE_DARKY)
	PSG_APPLY_DEVICE ^/_g_PSG_Regs+PSG_DARKY_IDX*16/, P_DARKY_PSG1_IDX, P_DARKY_PSG1_DATA, P_DARKY_PSG1_READ
	PSG_APPLY_DEVICE ^/_g_PSG_Regs+(PSG_DARKY_IDX+1)*16/, P_DARKY_PSG2_IDX, P_DARKY_PSG2_DATA, P_DARKY_PSG2_READ
#endif
#if (PSG_DEVICE & PSG_DEVICE_OPNA)
	PSG_APPLY_DEVICE ^/_g_PSG_Regs+PSG_OPNA_IDX*16/, P_OPNA_ADDR0, P_OPNA_DATA0, P_OPNA_DATA0
#endif
	ret
__endasm;
}
#endif // (PSG_ACCESS == PSG_INDIRECT)
