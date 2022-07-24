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

//
struct V9_Address g_V9_Address;

const u8 g_V9_ModeConfig[] = 
{
	// Pattern mode 1      (P1)
	V9_R06_MODE_P1 + V9_R06_CLOCK_4 + V9_R06_WIDH_512 + V9_R06_BPP_4,
	0,
	V9_P07_XTAL1,

	// Pattern mode 2      (P2)
	V9_R06_MODE_P2 + V9_R06_CLOCK_2 + V9_R06_WIDH_1024 + V9_R06_BPP_4,
	0,
	V9_P07_XTAL1,

	// Bitmap 256 * 212    (B0) (undocumented mode)
	V9_R06_MODE_BITMAP + V9_R06_CLOCK_4 + V9_R06_WIDH_256 + V9_R06_BPP_8,
	0,
	V9_P07_MCKIN,

	// Bitmap 256 * 212    (B1)
	V9_R06_MODE_BITMAP + V9_R06_CLOCK_4 + V9_R06_WIDH_256 + V9_R06_BPP_8,
	0,
	V9_P07_XTAL1,

	// Bitmap 384 * 240    (B2)
	V9_R06_MODE_BITMAP + V9_R06_CLOCK_2 + V9_R06_WIDH_512 + V9_R06_BPP_8,
	0,
	V9_P07_MCKIN,

	// Bitmap 512 * 212    (B3)
	V9_R06_MODE_BITMAP + V9_R06_CLOCK_2 + V9_R06_WIDH_512 + V9_R06_BPP_8,
	0,
	V9_P07_XTAL1,

	// Bitmap 768 * 212    (B4)
	V9_R06_MODE_BITMAP + V9_R06_CLOCK_1 + V9_R06_WIDH_1024 + V9_R06_BPP_4,
	0,
	V9_P07_MCKIN,

	// Bitmap 640 * 400    (B5)
	V9_R06_MODE_BITMAP + V9_R06_CLOCK_1 + V9_R06_WIDH_1024 + V9_R06_BPP_4,
	V9_R07_HSCN,
	V9_P07_XTAL1,

	// Bitmap 640 * 480    (B6)
	V9_R06_MODE_BITMAP + V9_R06_CLOCK_1 + V9_R06_WIDH_1024 + V9_R06_BPP_4,
	V9_R07_HSCN + V9_R07_C25,
	V9_P07_XTAL1,

	// Bitmap 1024 * 212   (B7) (undocumented mode)
	V9_R06_MODE_BITMAP + V9_R06_CLOCK_1 + V9_R06_WIDH_1024 + V9_R06_BPP_4,
	0,
	V9_P07_XTAL1,
};

//-----------------------------------------------------------------------------
//
inline u8 V9_GetPort(u8 port)
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
inline void V9_SetPort(u8 port, u8 value)
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
void V9_SetRegister(u8 reg, u8 val)
{
	reg;	// A
	val;	// L
	__asm
	v9_set_reg:
		out		(V9_P04), a				// Select register
		ld		a, l
		out		(V9_P03), a				// Set value
	__endasm;
}

//-----------------------------------------------------------------------------
//
u8 V9_GetRegister(u8 reg)
{
	reg;	// A
	__asm
	v9_get_reg:
		out		(V9_P04), a				// Select register
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
	__endasm;
}

//-----------------------------------------------------------------------------
//
void V9_SetMode(u8 mode)
{
	u8* ptr = g_V9_ModeConfig + (mode * 3);
	V9_SetRegister(V9_R06, *ptr++);
	V9_SetRegister(V9_R07, *ptr++);
	V9_SetPort(V9_P07, *ptr);
}

//-----------------------------------------------------------------------------
//
void V9_ClearVRAM()
{
	__asm
		// Select address 0
		xor		a
		out		(V9_P04), a			// Select R#0
		out		(V9_P03), a			// R#0 = 0
		out		(V9_P03), a			// R#1 = 0
		out		(V9_P03), a			// R#2 = 0
		
		// Setup 64K loop
		ld		b, a
		ld		d, a
		ld		c, #V9_P00
		
		// Loop
	v9_clear_loop:
		out		(c), b
		djnz	v9_clear_loop
		dec		d
		jp		nz, v9_clear_loop
	__endasm;
}


/*

//-----------------------------------------------------------------------------
//
void V9_SetWriteAddress(struct V9_Address* addr)
{
	addr; // HL
	__asm
	v9_set_write_addr:					//-cc-
		xor		a						//  5	
		out		(V9_P04), a				// 12	Select R#0 (with write increment)
		ld		c, #V9_P03				//  8	
		outi							// 18	R#0 (lower address)
		outi							// 18	R#1 (center address)
		outi							// 18	R#2 (upper address)
	__endasm;							//-total: 79 cc
}

/*void V9_SetWriteAddress(u8 up, u16 low)
{
	up;		// A
	low;	// DE
	__asm
	v9_set_write_addr:						//-cc-
		ld		b, a					//  5
		xor		a						//  5
		out		(V9_P04), a				// 12	Select R#0 (with write increment)
		ld		c, #V9_P03				//  8
		out		(c), d					// 14	R#0 (lower address)
		out		(c), e					// 14	R#1 (center address)
		out		(c), b					// 14	R#2 (upper address)
	__endasm;							//-total: 72 cc
}/

//-----------------------------------------------------------------------------
//
void V9_FillVRAM(u8 value, struct V9_Address* dest, u16 count) __naked
{
	value;	// A
	dest;	// DE
	count;	// SP+2
	__asm
	// Setup destination address
	v9_fill_addr:
		ld		h, d					// DE -> HL
		ld		l, e
		ld		b, a					// Backup value
		xor		a
		out		(V9_P04), a				// Select R#0 (with write increment)
		ld		c, #V9_P03
		outi							// Set R#0 (lower address)
		outi							// Set R#1 (center address)
		outi							// Set R#2 (upper address)
	// Setup fast 16-bits loop
	v9_fill_setup:
		ld		a, b					// Retreive value
		pop		iy
		pop		de						// Get counter
		ld		b, e					// Compute DB value... B is count LSB 
		dec		de
		inc		d						// D is count's MSB + 1 except when LSB is equal 0 (in this case D equal count's MSB)
	// Fast 16-bits loop
	v9_fill_loop:
		out		(V9_P00), a				// Fill VRAM
		djnz	v9_fill_loop			// Iner 8-bits loop
		dec		d
		jr		nz, v9_fill_loop		// Outer 8-bits loop
		
		jp		(iy)
__endasm;
}

//-----------------------------------------------------------------------------
//
void V9_WriteVRAM(u8* src, struct V9_Address* dest, u16 count) __naked
{
	src;	// HL -> DE
	dest;	// DE -> HL
	count;	// SP+2
	__asm
	// Setup destination address
	v9_write_addr:
		ex		de, hl
		xor		a
		out		(V9_P04), a				// Select R#0 (with write increment)
		ld		c, #V9_P03
		outi							// Set R#0 (lower address)
		outi							// Set R#1 (center address)
		outi							// Set R#2 (upper address)
	// Setup fast 16-bits loop
	v9_write_setup:
		ld		a, b					// Retreive value
		pop		iy
		pop		de						// Get counter
		ld		b, e					// Compute DB value... B is count LSB 
		dec		de
		inc		d						// D is count's MSB + 1 except when LSB is equal 0 (in this case D equal count's MSB)
		ld		c, #V9_P00
	// Fast 16-bits loop
	v9_write_loop:
		outi							// Write to VRAM
		jr		nz, v9_write_loop		// Iner 8-bits loop
		dec		d
		jr		nz, v9_write_loop		// Outer 8-bits loop
		
		jp		(iy)
__endasm;
}

//-----------------------------------------------------------------------------
//
void V9_SetReadAddress(struct V9_Address* addr)
{
	addr; // HL
__asm
set_read_addr:
	ld		#V9_REG(3)				// Select R#3 (with write increment)
	out		(V9_P04), a
	ld		c, #V9_P03
	outi							// R#0 (lower address)
	outi							// R#1 (center address)
	outi							// R#2 (upper address)
__endasm;
}


//-----------------------------------------------------------------------------
//
void V9_ReadVRAM(struct V9_Address* src, u8* dest, u16 count)
{
	src;	// HL
	dest;	// DE
	count;	// SP+2
	// V9_SetWriteAddress(dest);
}*/