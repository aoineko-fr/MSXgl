// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄ ▄  ▄▄   ▄▄   ▄▄   ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ █▄██ █▄██ █▄██ █ ██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▀   ▄█▀  ▄█▀  ▄█▀ ▀▄█▀
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// References:
// - 
//─────────────────────────────────────────────────────────────────────────────
#include "v9990.h"
#include "v9990_reg.h"

#if (V9_INT_PROTECT)
	#define V9_DI					di
	#define V9_EI					ei
#else
	#define V9_DI
	#define V9_EI
#endif

// V9990 screen mode configuration
//  R#06, R#07, Port#07
const u8 g_V9_ModeConfig[V9_MODE_MAX][3] = 
{
	// Pattern mode 1      (P1)
	{
		(u8)(V9_R06_MODE_P1 + V9_R06_CLOCK_4 + V9_R06_WIDH_512 + V9_R06_BPP_4),
		0,
		V9_P07_XTAL1,
	},

	// Pattern mode 2      (P2)
	{
		(u8)(V9_R06_MODE_P2 + V9_R06_CLOCK_2 + V9_R06_WIDH_1024 + V9_R06_BPP_4),
		0,
		V9_P07_XTAL1,
	},

	// Bitmap 256 * 212    (B0) (undocumented mode)
	{
		(u8)(V9_R06_MODE_BITMAP + V9_R06_CLOCK_4 + V9_R06_WIDH_256 + V9_R06_BPP_8),
		0,
		V9_P07_MCKIN,
	},

	// Bitmap 256 * 212    (B1)
	{
		(u8)(V9_R06_MODE_BITMAP + V9_R06_CLOCK_4 + V9_R06_WIDH_256 + V9_R06_BPP_8),
		0,
		V9_P07_XTAL1,
	},

	// Bitmap 384 * 240    (B2)
	{
		(u8)(V9_R06_MODE_BITMAP + V9_R06_CLOCK_2 + V9_R06_WIDH_512 + V9_R06_BPP_8),
		0,
		V9_P07_MCKIN,
	},

	// Bitmap 512 * 212    (B3)
	{
		(u8)(V9_R06_MODE_BITMAP + V9_R06_CLOCK_2 + V9_R06_WIDH_512 + V9_R06_BPP_8),
		0,
		V9_P07_XTAL1,
	},

	// Bitmap 768 * 212    (B4)
	{
		(u8)(V9_R06_MODE_BITMAP + V9_R06_CLOCK_1 + V9_R06_WIDH_1024 + V9_R06_BPP_4),
		0,
		V9_P07_MCKIN,
	},

	// Bitmap 640 * 400    (B5)
	{
		(u8)(V9_R06_MODE_BITMAP + V9_R06_CLOCK_1 + V9_R06_WIDH_1024 + V9_R06_BPP_4),
		V9_R07_HSCN,
		V9_P07_XTAL1,
	},

	// Bitmap 640 * 480    (B6)
	{
		(u8)(V9_R06_MODE_BITMAP + V9_R06_CLOCK_1 + V9_R06_WIDH_1024 + V9_R06_BPP_4),
		(u8)(V9_R07_HSCN + V9_R07_C25),
		V9_P07_XTAL1,
	},

	// Bitmap 1024 * 212   (B7) (undocumented mode)
	{
		(u8)(V9_R06_MODE_BITMAP + V9_R06_CLOCK_1 + V9_R06_WIDH_1024 + V9_R06_BPP_4),
		0,
		V9_P07_XTAL1,
	},
};

// V9990 color mode configuration
//  R#06, R#13
const u8 g_V9_ColorConfig[V9_COLOR_BMP_MAX][2] = 
{
	// V9_COLOR_BP2			Color palette (4 colors out of 32768 colors)
	{ V9_R06_BPP_2, V9_R13_PLTM_PAL },
	// V9_COLOR_BP4			Color palette (16 colors out of 32768 colors)
	{ V9_R06_BPP_4, V9_R13_PLTM_PAL },
	// V9_COLOR_BP6			Color palette (64 colors out of 32768 colors)
	{ V9_R06_BPP_8, V9_R13_PLTM_PAL },
	// V9_COLOR_BD8			Direct RGB [G:3|R:3|B:2] (256 colors)
	{ V9_R06_BPP_8, V9_R13_PLTM_256 },
	// V9_COLOR_BYJK		YJK Decoder (19268 colors)
	{ V9_R06_BPP_8, V9_R13_PLTM_YJK },
	// V9_COLOR_BYJKP		YJK Decoder + Color palette (12599 colors + 16 colors out of 32768 colors)
	{ V9_R06_BPP_8, V9_R13_PLTM_YJK + V9_R13_YAE},
	// V9_COLOR_BYUV		YUV Decoder (19268 colors)
	{ V9_R06_BPP_8, V9_R13_PLTM_YUV },
	// V9_COLOR_BYUVP		YUV Decoder + Color palette (12599 colors + 16 colors out of 32768 colors)
	{ V9_R06_BPP_8, V9_R13_PLTM_YUV + V9_R13_YAE},
	// V9_COLOR_BD16		Direct RGB [YS|G:5|R:2][R:3|B:5] (32768 colors)
	{ V9_R06_BPP_16, V9_R13_PLTM_PAL },
};

//-----------------------------------------------------------------------------
//
void V9_SetPort(u8 port, u8 value) __PRESERVES(b, d, e, h, iyl, iyh)
{
	port;	// A
	value;	// L

	__asm
	v9_set_port:
		ld		c, a				// Select port
		out		(c), l				// Set value
	__endasm;
}

//-----------------------------------------------------------------------------
//
u8 V9_GetPort(u8 port) __PRESERVES(b, d, e, h, l, iyl, iyh)
{
	port;	// A

	__asm
	v9_get_port:
		ld		c, a				// Select port
		in		a, (c)				// Get value
	__endasm;
}

//-----------------------------------------------------------------------------
//
void V9_SetRegister(u8 reg, u8 val) __PRESERVES(b, c, d, e, h, iyl, iyh)
{
	reg;	// A
	val;	// L

	__asm
	v9_set_reg:
		V9_DI
		out		(V9_P04), a				// Select register
		ld		a, l
		V9_EI
		out		(V9_P03), a				// Set value
	__endasm;
}

//-----------------------------------------------------------------------------
//
void V9_SetRegister16(u8 reg, u16 val) __PRESERVES(b, h, l, iyl, iyh)
{
	reg;	// A
	val;	// DE

	__asm
	v9_set_reg16:
		V9_DI
		out		(V9_P04), a				// Select register
		ld		a, l
		V9_EI
		ld		c, #V9_P03
		out		(c), e					// Set LSB value
		out		(c), d					// Set MSB value
	__endasm;
}

//-----------------------------------------------------------------------------
//
u8 V9_GetRegister(u8 reg) __PRESERVES(b, c, d, e, h, l, iyl, iyh)
{
	reg;	// A

	__asm
	v9_get_reg:
		V9_DI
		out		(V9_P04), a				// Select register
		V9_EI
		in		a, (V9_P03)				// Get value
	__endasm;
}

//-----------------------------------------------------------------------------
//
bool V9_Detect()
{
	__asm
	v9_detect:
		ld		a, #V9_REG(15) + V9_RII + V9_WII
		V9_DI
		out		(V9_P04), a				// Select R#15

		in		a, (V9_P03)				// Read value
		ld		b, a
		cpl								// Invert value
		out		(V9_P03), a				// Write value
		in		a, (V9_P03)				// Read value again
		xor		b
		jr		z, v9_notfound
	v9_found:							// Return A=0xFF
		ld		c, #V9_P03
		out		(c), b					// Restore value
	v9_notfound:						// Return A=0x00
		V9_EI
	__endasm;
}

//-----------------------------------------------------------------------------
// Set screen mode
void V9_SetScreenMode(u8 mode)
{
	const u8* ptr = (const u8*)&g_V9_ModeConfig[mode];
	V9_SetRegister(6, *ptr++);
	V9_SetRegister(7, *ptr++);
	V9_SetPort(V9_P07, *ptr);

	V9_SetRegister(8, V9_R08_DISP_ON | 0x02); // Enable display
	V9_SetRegister(13, 0x00); // Reset color mode
}

#if ((V9_USE_MODE_B0) || (V9_USE_MODE_B1) || (V9_USE_MODE_B2) || (V9_USE_MODE_B3) || (V9_USE_MODE_B4) || (V9_USE_MODE_B5) || (V9_USE_MODE_B6) || (V9_USE_MODE_B7))

//-----------------------------------------------------------------------------
// Set bitmap color mode.
//
// Parameters:
//   mode - New color mode to set. Can be any of the bitmap modes of <V9_COLOR_MODE> enum.
void V9_SetColorMode(u8 mode)
{
	const u8* ptr = (const u8*)&g_V9_ColorConfig[mode];
	V9_SetFlag(6, V9_R06_BPP_MASK, *ptr++);
	V9_SetFlag(13, (u8)(V9_R13_PLTM_MASK + V9_R13_YAE), *ptr);
}

#endif

//-----------------------------------------------------------------------------
//
void V9_SetScrollingY(u16 y)
{
	// Set R#17
	V9_SetRegister(17, y & 0xFF);

	// Set R#18
	u8 a = (y >> 8) & 0x1F;
	u8 b = V9_GetRegister(18) & 0xE0;
	V9_SetRegister(18, a | b);
}

//-----------------------------------------------------------------------------
//
void V9_SetScrollingX(u16 x)
{
	// Set R#19
	V9_SetRegister(19, x & 0x07);

	// Set R#20
	V9_SetRegister(20, (x >> 3) & 0xFF);
}

#if (V9_USE_MODE_P1)

//-----------------------------------------------------------------------------
//
void V9_SetScrollingBY(u16 y)
{
	// Set R#21
	V9_SetRegister(21, y & 0xFF);

	// Set R#22
	u8 a = (y >> 8) & 0x01;
	u8 b = V9_GetRegister(22) & 0xFE;
	V9_SetRegister(22, a | b);
}

//-----------------------------------------------------------------------------
//
void V9_SetScrollingBX(u16 x)
{
	// Set R#23
	V9_SetRegister(23, x & 0x07);

	// Set R#24
	V9_SetRegister(24, (x >> 3) & 0xFF);
}

#endif // (V9_USE_MODE_P1)

//-----------------------------------------------------------------------------
// Clean the whole VRAM (512 KB)
void V9_ClearVRAM() __PRESERVES(d, e, h, l, iyl, iyh)
{
	__asm
		V9_DI
		// Select address 0
		xor		a
		out		(V9_P04), a			// Select R#0
		out		(V9_P03), a			// R#0 = 0
		out		(V9_P03), a			// R#1 = 0
		out		(V9_P03), a			// R#2 = 0

		// Setup 512 KB loop (256x256x8)
		ld		b, a
		ld		c, a

		// Loop
	v9_clear_loop:
		out		(V9_P00), a
		out		(V9_P00), a
		out		(V9_P00), a
		out		(V9_P00), a
		out		(V9_P00), a
		out		(V9_P00), a
		out		(V9_P00), a
		out		(V9_P00), a
		djnz	v9_clear_loop
		dec		c
		jp		nz, v9_clear_loop

		V9_EI
	__endasm;
}

//-----------------------------------------------------------------------------
//
void V9_SetWriteAddress(u32 addr) __PRESERVES(b, iyl, iyh)
{
	addr;		// HL:DE
	__asm
	v9_set_write_addr:					//-cc-
		xor		a						//  5
		V9_DI
		out		(V9_P04), a				// 12	Select R#0 (with write increment)
		ld		c, #V9_P03				//  8
		out		(c), e					// 14	R#0 (lower address)
		out		(c), d					// 14	R#1 (center address)
		out		(c), l					// 14	R#2 (upper address)
	__endasm;							//-total: 67 cc
}

//-----------------------------------------------------------------------------
//
void V9_SetReadAddress(u32 addr) __PRESERVES(b, iyl, iyh)
{
	addr;		// HL:DE
	__asm
	v9_set_read_addr:					//-cc-
		ld		a, #3					//  8
		V9_DI
		out		(V9_P04), a				// 12	Select R#3 (with read increment)
		ld		c, #V9_P03				//  8
		out		(c), e					// 14	R#3 (lower address)
		out		(c), d					// 14	R#4 (center address)
		out		(c), l					// 14	R#5 (upper address)
	__endasm;							//-total: 70 cc
}

//-----------------------------------------------------------------------------
//
u8 V9_Peek_CurrentAddr() __PRESERVES(b, c, d, e, h, l, iyl, iyh)
{
	__asm
		V9_EI
		in		a, (V9_P00)
	__endasm;
}

//-----------------------------------------------------------------------------
//
u16 V9_Peek16_CurrentAddr() __PRESERVES(b, c, h, l, iyl, iyh)
{
	__asm
		in		a, (V9_P00)
		ld		e, a
		V9_EI
		in		a, (V9_P00)
		ld		d, a
	__endasm;
}

//-----------------------------------------------------------------------------
//
void V9_Poke_CurrentAddr(u8 val) __PRESERVES(b, c, d, e, h, l, iyl, iyh)
{
	val;		// A
	__asm
		V9_EI
		out		(V9_P00), a
	__endasm;
}

//-----------------------------------------------------------------------------
//
void V9_Poke16_CurrentAddr(u16 val) __PRESERVES(b, c, d, e, iyl, iyh)
{
	val;		// HL
	__asm
		ld		a, l
		out		(V9_P00), a				// Fill VRAM
		ld		a, h
		V9_EI
		out		(V9_P00), a				// Fill VRAM
	__endasm;
}

//-----------------------------------------------------------------------------
//
void V9_FillVRAM_CurrentAddr(u8 value, u16 count)
{
	value;	// A
	count;	// DE

	__asm
	// Setup fast 16-bits loop
	v9_fill_setup:
		ld		b, e					// Number of loops is in DE
		dec		de						// Calculate DB value (destroys B, D and E)
		inc		d
	// Fast 16-bits loop
	v9_fill_loop:
		out		(V9_P00), a				// Fill VRAM
		djnz	v9_fill_loop			// Iner 8-bits loop
		dec		d
		jp		nz, v9_fill_loop		// Outer 8-bits loop

		V9_EI
	__endasm;
}

//-----------------------------------------------------------------------------
//
void V9_FillVRAM16_CurrentAddr(u16 value, u16 count)
{
	value;	// HL
	count;	// DE

	__asm
	// Setup fast 16-bits loop
	v9_fill16_setup:
		ld		b, e					// Number of loops is in DE
		dec		de						// Calculate DB value (destroys B, D and E)
		inc		d
	// Fast 16-bits loop
	v9_fill16_loop:
		ld		a, l
		out		(V9_P00), a				// Fill VRAM
		ld		a, h
		out		(V9_P00), a				// Fill VRAM
		djnz	v9_fill16_loop			// Iner 8-bits loop
		dec		d
		jp		nz, v9_fill16_loop		// Outer 8-bits loop

		V9_EI
	__endasm;
}

//-----------------------------------------------------------------------------
//
void V9_WriteVRAM_CurrentAddr(const u8* src, u16 count)
{
	src;	// HL
	count;	// DE

	__asm
	// Setup fast 16-bits loop
	v9_write_setup:
		ld		b, e					// Number of loops is in DE
		dec		de						// Calculate DB value (destroys B, D and E)
		inc		d
		ld		c, #V9_P00
	// Fast 16-bits loop
	v9_write_loop:
		otir							// Write to VRAM (iner 8-bits loop)
		dec		d
		jp		nz, v9_write_loop		// Outer 8-bits loop

		V9_EI
	__endasm;
}

//-----------------------------------------------------------------------------
//
void V9_ReadVRAM_CurrentAddr(const u8* dest, u16 count)
{
	dest;	// HL
	count;	// DE

	__asm
	// Setup fast 16-bits loop
	v9_read_setup:
		ld		b, e					// Number of loops is in DE
		dec		de						// Calculate DB value (destroys B, D and E)
		inc		d
		ld		c, #V9_P00
	// Fast 16-bits loop
	v9_read_loop:
		inir							// Read from VRAM (iner 8-bits loop)
		dec		d
		jp		nz, v9_read_loop		// Outer 8-bits loop

		V9_EI
	__endasm;
}

#if ((V9_PALETTE_MODE == V9_PALETTE_GBR_16) || (V9_PALETTE_MODE == V9_PALETTE_YSGBR_16))

//-----------------------------------------------------------------------------
// Set the color of a given palette entry.
void V9_SetPaletteEntry(u8 index, u16 color)
{
	index; // A
	color; // DE
	       // D: [Ys|G|G|G|G|G|R|R] E: [R|R|R|B|B|B|B|B]

	__asm
		// Select R#14 (palette pointer register)
		ld		c, #V9_P04
		ld		b, #14
		V9_DI
		out		(c), b

		// Set palette pointer to right index's red channel [index:6|00]
		sla		a
		sla		a
		out		(V9_P03), a

	v9_set_pal_entry:
		// Send R + Ys data
		ld		a, d
		and		#0b00000011
		rlca
		rlca
		rlca
		ld		b, a

		ld		a, e
		and		#0b11100000
		rlca
		rlca
		rlca
		or		b

#if (V9_PALETTE_MODE == V9_PALETTE_YSGBR_16)
		bit		7, d
		jr		z, v9_set_pal_no_ys
		set		7, a
	v9_set_pal_no_ys:
#endif
		out		(V9_P01), a

		// Send G data
		ld		a, d
		and		#0b01111100
		rrca
		rrca
		out		(V9_P01), a

		// Send B data
		ld		a, e
		and		#0b00011111
		V9_EI
		out		(V9_P01), a
	__endasm;
}

#elif (V9_PALETTE_MODE == V9_PALETTE_RGB_24)

//-----------------------------------------------------------------------------
// Set the color of a given palette entry.
void V9_SetPaletteEntry(u8 index, const u8* color) __PRESERVES(h, l, iyl, iyh)
{
	index; // A
	color; // DE [R][G][B]

	__asm
		// Select R#14 (palette pointer register)
		ld		c, #V9_P04
		ld		b, #14
		V9_DI
		out		(c), b

		// Set palette pointer to right index's red channel [index:6|00]
		sla		a
		sla		a
		out		(V9_P03), a

	v9_set_pal_entry:
		// Send R + Ys data
		ld		a, (de)
		out		(V9_P01), a

		// Send G data
		inc		de
		ld		a, (de)
		out		(V9_P01), a

		// Send B data
		inc		de
		ld		a, (de)
		V9_EI
		out		(V9_P01), a
	__endasm;
}

#endif

#if ((V9_USE_MODE_B0) || (V9_USE_MODE_B1) || (V9_USE_MODE_B2) || (V9_USE_MODE_B3) || (V9_USE_MODE_B4) || (V9_USE_MODE_B5) || (V9_USE_MODE_B6) || (V9_USE_MODE_B7))

//-----------------------------------------------------------------------------
// Set the given cursor atribute (for bitmap modes).
//
// Parameters:
//   id - Cursor index (0 or 1).
//   x - Cursor X coordinate (from 0 to 1023 regardless of the screen mode).
//   y - Cursor Y coordinate (from 0 to 511 regardless of the screen mode).
//   color - Cursor color offset (2 bits).
//
// Note: Call to this function will enable cursor display.
void V9_SetCursorAttribute(u8 id, u16 x, u16 y, u8 color)
{
	u32 addr = 0x7FE00;
	if(id)
		addr += 8;
	V9_Poke(addr, y & 0xFF);
	addr += 2;
	V9_Poke(addr, y >> 8);
	addr += 2;
	V9_Poke(addr, x & 0xFF);
	addr += 2;
	V9_Poke(addr, ((x >> 8) & 0x3) + ((color & 0x3) << 6));
}

//-----------------------------------------------------------------------------
// Display/hide the given cursor
//
// Parameters:
//   id - Cursor index (0 or 1).
//   enable - TRUE to display or FALSE to hide.
void V9_SetCursorDisplay(u8 id, bool enable)
{
	u32 addr = 0x7FE06;
	if(id)
		addr += 8;
	u8 val = V9_Peek(addr);
	if(enable)
		val &= ~V9_CURSOR_DISABLE;
	else
		val |= V9_CURSOR_DISABLE;
	V9_Poke(addr, val);
}

#endif // ((V9_USE_MODE_B0) || (V9_USE_MODE_B1) || (V9_USE_MODE_B2) || (V9_USE_MODE_B3) || (V9_USE_MODE_B4) || (V9_USE_MODE_B5) || (V9_USE_MODE_B6) || (V9_USE_MODE_B7))