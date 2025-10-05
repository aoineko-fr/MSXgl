// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄ ▄ ▄▄▄  ▄▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ ██ █ ██▄▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▀  ██▄▀ ██  
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Features to manage the VDP
//
// References:
// - V9938 Technical Data Book Programmer's Guide
// - https://www.msx.org/wiki/Category:VDP_Registers
// - http://map.grauw.nl/articles/
//─────────────────────────────────────────────────────────────────────────────

//-----------------------------------------------------------------------------
// INCLUDES
//-----------------------------------------------------------------------------
#include "vdp.h"
#include "system_port.h"
#include "color.h"
#include "bios_mainrom.h"

//-----------------------------------------------------------------------------
// DEFINES
//-----------------------------------------------------------------------------

// VDP Registers Flags
#if (VDP_USE_16X16_SPRITE)
#define VDP_SPRITE_COLORS			16
#else
#define VDP_SPRITE_COLORS			8
#endif

// Handle interruptions disabling
#if (VDP_ISR_SAFE_MODE == VDP_ISR_SAFE_ALL)
	#define VDP_DI_ALL				di
	#define VDP_EI_ALL				ei
	#define VDP_DI_DEF
	#define VDP_EI_DEF
#elif (VDP_ISR_SAFE_MODE == VDP_ISR_SAFE_DEFAULT)
	#define VDP_DI_ALL
	#define VDP_EI_ALL
	#define VDP_DI_DEF				di
	#define VDP_EI_DEF				ei
#else // (VDP_ISR_SAFE_MODE == VDP_ISR_SAFE_NONE)
	#define VDP_DI_ALL
	#define VDP_EI_ALL
	#define VDP_DI_DEF
	#define VDP_EI_DEF
#endif

#if (VDP_ISR_SAFE_MODE == VDP_ISR_SAFE_NONE)
	#define VDP_DI
	#define VDP_EI
#else
	#define VDP_DI					di
	#define VDP_EI					ei
#endif

// #if ((MSX_VERSION & MSX_1) && (VDP_USE_MODE_G1 || VDP_USE_MODE_G2 || VDP_USE_MODE_MC)) // Worst case 29 cc
// #elif (VDP_USE_MODE_T1 || VDP_USE_MODE_T2) // Worst case 20 cc
// #else // Worst case 15 cc
// #endif

//-----------------------------------------------------------------------------
// PROTOTYPES
//-----------------------------------------------------------------------------

// Set screen mode to Text 1
void VDP_SetModeText1();

// Set screen mode to Multi-color
void VDP_SetModeMultiColor();

// Set screen mode to Graphic 1
void VDP_SetModeGraphic1();

// Set screen mode to Graphic 2
void VDP_SetModeGraphic2();

#if (MSX_VERSION >= MSX_2)

// Set screen mode to Text 2
void VDP_SetModeText2();

// Set screen mode to Graphic 3
void VDP_SetModeGraphic3();

// Set screen mode to Graphic 4
void VDP_SetModeGraphic4();

// Set screen mode to Graphic 5
void VDP_SetModeGraphic5();

// Set screen mode to Graphic 6
void VDP_SetModeGraphic6();

// Set screen mode to Graphic 7
void VDP_SetModeGraphic7();

#endif // (MSX_VERSION >= MSX_2)

//-----------------------------------------------------------------------------
// DATAS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// VDP Registers

u8 g_VDP_REGSAV[28];

VDP_Data    g_VDP_Data;

#if (VDP_USE_COMMAND)
VDP_Command g_VDP_Command;
#endif

#if (VDP_USE_SPRITE)
VDP_Sprite  g_VDP_Sprite;
#endif

u16 g_ScreenLayoutLow;		// Address of the Name Table
u16 g_ScreenColorLow;		// Address of the Color Table
u16 g_ScreenPatternLow;		// Address of the Pattern Table
#if (VDP_USE_SPRITE)
u16 g_SpriteAttributeLow;	// Address of the Sprite Attribute Table
u16 g_SpritePatternLow;		// Address of the Sprite Pattern Table
u16 g_SpriteColorLow;		// Address of the Sprite Color Table
#endif
#if (VDP_VRAM_ADDR == VDP_VRAM_ADDR_17)
u8  g_ScreenLayoutHigh;		// Address of the Name Table
u8  g_ScreenColorHigh;		// Address of the Color Table
u8  g_ScreenPatternHigh;	// Address of the Pattern Table
#if (VDP_USE_SPRITE)
u8  g_SpriteAttributeHigh;	// Address of the Sprite Attribute Table
u8  g_SpritePatternHigh;	// Address of the Sprite Pattern Table
u8  g_SpriteColorHigh;		// Address of the Sprite Color Table
#endif
#endif

#if (VDP_AUTO_INIT)
bool g_VDPInitilized = FALSE;	// Flag to check if VDP module initialization already occurs
#endif

//=============================================================================
//
//  █▀▄▀█ █▀ ▀▄▀   ▄█   █▀▀ █ █ █▄ █ █▀▀ ▀█▀ █ █▀█ █▄ █ █▀
//  █ ▀ █ ▄█ █ █    █   █▀  █▄█ █ ▀█ █▄▄  █  █ █▄█ █ ▀█ ▄█
//
//=============================================================================

//-----------------------------------------------------------------------------
//
// SCREEN MODE SETTING
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Set screen mode flag [MSX1/2/2+/TR]
// VRAM tables address must be set to fit the new screen mode
//
// Parameters:
//   flag - Screen binary flag
void VDP_SetModeFlag(u8 flag)
{
	// VDP register #1
	u8 reg1 = g_VDP_REGSAV[1];
	reg1 &= 0b11100111;
	if (flag & 0b00001)
		reg1 |= 0b00010000;
	if (flag & 0b00010)
		reg1 |= 0b00001000;
	VDP_RegWriteBak(1, reg1);

	// VDP register #0
	u8 reg0 = g_VDP_REGSAV[0];
	reg0 &= 0b11110001;
	flag >>= 1;
	flag &= 0b00001110;
	reg0 |= flag;
	VDP_RegWriteBak(0, reg0);
}

#if (VDP_USE_MODE_T1)
//-----------------------------------------------------------------------------
// Initialize Text 1 screen mode.
void VDP_SetModeText1()
{
	VDP_SetModeFlag(VDP_T1_MODE);
	VDP_SetLayoutTable(VDP_T1_ADDR_NT);
	VDP_SetPatternTable(VDP_T1_ADDR_PT);
}
#endif // VDP_USE_MODE_T1

#if (VDP_USE_MODE_MC)
//-----------------------------------------------------------------------------
// Initialize Multi Color screen mode.
void VDP_SetModeMultiColor()
{
	VDP_SetModeFlag(VDP_MC_MODE);
	VDP_SetLayoutTable(VDP_MC_ADDR_NT);
	VDP_SetPatternTable(VDP_MC_ADDR_PT);
	#if (VDP_USE_SPRITE)
	VDP_SetSpriteAttributeTable(VDP_MC_ADDR_SAT);
	VDP_SetSpritePatternTable(VDP_MC_ADDR_SPT);
	#endif
}
#endif // VDP_USE_MODE_MC

#if (VDP_USE_MODE_G1)
//-----------------------------------------------------------------------------
// Initialize Graphic 1 screen mode.
void VDP_SetModeGraphic1()
{
	VDP_SetModeFlag(VDP_G1_MODE);
	VDP_SetLayoutTable(VDP_G1_ADDR_NT);
	VDP_SetColorTable(VDP_G1_ADDR_CT);
	VDP_SetPatternTable(VDP_G1_ADDR_PT);
	#if (VDP_USE_SPRITE)
	VDP_SetSpriteAttributeTable(VDP_G1_ADDR_SAT);
	VDP_SetSpritePatternTable(VDP_G1_ADDR_SPT);
	#endif
}
#endif // VDP_USE_MODE_G1

#if (VDP_USE_MODE_G2)
//-----------------------------------------------------------------------------
// Initialize Graphic 2 screen mode.
void VDP_SetModeGraphic2()
{
	VDP_SetModeFlag(VDP_G2_MODE);
	VDP_SetLayoutTable(VDP_G2_ADDR_NT);
	VDP_SetColorTable(VDP_G2_ADDR_CT);
	VDP_SetPatternTable(VDP_G2_ADDR_PT);
	#if (VDP_USE_SPRITE)
	VDP_SetSpriteAttributeTable(VDP_G2_ADDR_SAT);
	VDP_SetSpritePatternTable(VDP_G2_ADDR_SPT);
	#endif
}
#endif // VDP_USE_MODE_G2

//-----------------------------------------------------------------------------
//
// VRAM ACCESS FUNCTIONS
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Clear the VRAM content
void VDP_ClearVRAM()
{
	#if (MSX_VERSION & MSX_1)
		VDP_FillVRAM_16K(0, 0x0000, 0x4000);  // Clear 16 KB of VRAM
	#else
		VDP_FillVRAM(0, 0x0000, 0, 0x8000); // Clear VRAM by 32 KB step
		VDP_FillVRAM(0, 0x8000, 0, 0x8000); // Clear VRAM by 32 KB step
		VDP_FillVRAM(0, 0x0000, 1, 0x8000); // Clear VRAM by 32 KB step
		VDP_FillVRAM(0, 0x8000, 1, 0x8000); // Clear VRAM by 32 KB step
	#endif
}

#if ((VDP_USE_VRAM16K) || (MSX_VERSION & MSX_1))

//-----------------------------------------------------------------------------
// Write data from RAM to VRAM [MSX1/2/2+/TR]
//
// Parameters:
//   src   - Source data address in RAM
//   dest  - Destination address in VRAM (14bits address for 16KB VRAM)
//   count - Number of byte to copy in VRAM
void VDP_WriteVRAM_16K(const u8* src, u16 dest, u16 count)
{
	src;   // HL
	dest;  // DE
	count; // SP+2 => IY+0

	__asm

	#if ((VDP_USE_VALIDATOR) && !(MSX_VERSION & MSX_1))
		// Reset VRAM address bit 14 to 16 (in R#14)
		xor		a
		out		(P_VDP_REG), a
		ld		a, #VDP_REG(14)
		out		(P_VDP_REG), a
	#endif

		ld		iy, #2
		add		iy, sp
		// Setup address register 
		ld		a, e
		VDP_DI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// RegPort = (dest & 0xFF)
		ld		a, d
		and		a, #0x3F
		or		a, #F_VDP_WRIT
		VDP_EI_DEF //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// RegPort = ((dest >> 8) & 0x3F) + F_VDP_WRIT

	#if ((MSX_VERSION & MSX_1) && (VDP_USE_MODE_G1 || VDP_USE_MODE_G2 || VDP_USE_MODE_MC)) // Worst case 29 cc (use 30 cc loop with "outi + jp nz")

		// Setup fast 16-bits loop
		ld		e, 0(iy)				// count
		ld		d, 1(iy)
		ld		c, #P_VDP_DATA	
		ld		b, e					// number of loops is in DE
		dec		de						// calculate DB value (destroys B, D and E)
		inc		d
		// Fast loop	
	wrt16_loop_start:
		outi							// out(c) ; hl++ ; b--
		jp		nz, wrt16_loop_start
		dec		d
		jp		nz, wrt16_loop_start	// b = 0 = 256

	#else // Worst case 20 cc (use 23 cc loop with "otir")

		// while (count--) DataPort = *src++;
		ld		c, #P_VDP_DATA			// data register
		// Handle count LSB
		ld		a, 0(iy)				// count LSB
		and		a
		jp		z, wrt16_loop_init		// skip LSB
		ld		b, a					// send (count & 0x00FF) bytes
		otir
		// Handle count MSB
	wrt16_loop_init:
		ld		a, 1(iy)				// count MSB
	wrt16_loop_start:
		and		a
		jp		z, wrt16_loop_end		// finished
		ld		b, #0					// send 256 bytes packages
		otir
		dec		a
		jp		wrt16_loop_start

	#endif

	wrt16_loop_end:
		VDP_EI_ALL //~~~~~~~~~~~~~~~~~~~~~~~~~~
	__endasm;
}

//-----------------------------------------------------------------------------
// Fill VRAM area with a given value [MSX1/2/2+/TR]
//
// Parameters:
//   value	- Byte value to copy in VRAM
//   dest	- Destination address in VRAM (14 bits address form 16 KB VRAM)
//   count	- Number of byte to copy in VRAM
void VDP_FillVRAM_16K(u8 value, u16 dest, u16 count) __NAKED // Stack: 4 bytes
{
	value; // A
	dest;  // DE
	count; // SP+2

	__asm
		ld		c, a					// backup value

	#if ((VDP_USE_VALIDATOR) && !(MSX_VERSION & MSX_1))
		// Reset VRAM address bit 14 to 16 (in R#14)
		xor		a
		out		(P_VDP_REG), a
		ld		a, #VDP_REG(14)
		out		(P_VDP_REG), a
	#endif

		// Setup destination address (LSB)
		ld		a, e
		VDP_DI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// RegPort = (dest & 0x00FF);

		// Setup destination address (MSB)
		ld		a, d
		and		a, #0x3F				// reset 2 MSB bits
		or		a, #F_VDP_WRIT			// add write flag
		out		(P_VDP_ADDR), a			// RegPort = ((dest >> 8) & 0x3F) + F_VDP_WRIT;

		// Retreive loop parameters
		pop		iy						// get return address 
		pop		de						// get loop count
		ld		a, c					// retreive value

		// Setup fast 16-bits loop
		ld		b, e					// calculate DB value... B is count LSB 
		dec		de
		inc		d						// D is count's MSB + 1 except when LSB is equal 0 (in this case D equal count's MSB)
	
	fll16_loop_start:
		// fast 16-bits loop
		out		(P_VDP_DATA), a			// fill VRAM							12 cc
	#if ((MSX_VERSION & MSX_1) && (VDP_USE_MODE_G1 || VDP_USE_MODE_G2 || VDP_USE_MODE_MC)) // Worst case 29 cc (use 31 cc loop)
		nop								//										5 cc
	#endif
		djnz	fll16_loop_start		// Iner 8-bits loop						14/9 cc
		dec		d
		jp		nz, fll16_loop_start	// Outer 8-bits loop

	fll16_loop_end:
		VDP_EI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		jp		(iy)
	__endasm;
}

#if (VDP_USE_FASTFILL)
//-----------------------------------------------------------------------------
// Fill VRAM area with a given value [MSX1/2/2+/TR]
//
// Parameters:
//   value	- Byte value to copy in VRAM
//   dest	- Destination address in VRAM (14 bits address form 16 KB VRAM)
//   count	- Number of byte to copy in VRAM
void VDP_FastFillVRAM_16K(u8 value, u16 dest, u16 count) __NAKED // Stack: 4 bytes
{
	value; // A
	dest;  // DE
	count; // SP+2

	__asm
		ld		l, a						// backup fill value

	#if ((VDP_USE_VALIDATOR) && !(MSX_VERSION & MSX_1))
		// Reset VRAM address bit 14 to 16 (in R#14)
		xor		a
		out		(P_VDP_REG), a
		ld		a, #VDP_REG(14)
		out		(P_VDP_REG), a
	#endif

		// Setup destination address (LSB)
		ld		a, e						// destination VRAM address LSB
		VDP_DI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a				// RegPort = (dest & 0x00FF);

		// Setup destination address (MSB)
		ld		a, d						// destination VRAM address MSB
		and		a, #0x3F					// reset 2 MSB bits
		or		a, #F_VDP_WRIT				// add write flag
		out		(P_VDP_ADDR), a				// RegPort = ((dest >> 8) & 0x3F) + F_VDP_WRIT;

		// Get parameters
		pop		iy							// 16 cc	Get return address
		pop		de							// 11 cc	Get fill count
		ld		h, e						//  5 cc	Backup count LSB

		// Shift Right 4 (56 T-Cycles, 14 bytes)
		ld		a, e						//  5 cc
		srl		d							// 10 cc
		rra									//  5 cc
		srl		d							// 10 cc
		rra									//  5 cc
		srl		d							// 10 cc
		rra									//  5 cc
		srl		d							// 10 cc
		rra									//  5 cc
		ld		e, a						//  5 cc	DE >> 4

		// Setup 16-bits fast loop
		ld		b, e						//  5 cc	Calculate DB value... B is count LSB 
		dec		de							//  7 cc
		inc		d							//  5 cc	D is count's MSB + 1 except when LSB is equal 0 (in this case D equal count's MSB)

		ld		a, h						//  5 cc	Retrieve count LSB
		and		#0x0F						//  8 cc
		jr		z, fastfill16_jump			// 13/8 cc

	fastfill16_setup:
		// Setup fast LDIR loop
		xor		a							//  5 cc
		sub		h							//  5 cc
		and		#15							//  8 cc
		add		a							//  5 cc

		exx									//  5 cc
		add		a, #fastfill16_loop 		//  8 cc	Keep Carry
		ld		l, a						//  5 cc
		ld		a, #0						//  8 cc
		adc		a, #fastfill16_loop >> 8	//  8 cc	Use Carry
		ld		h, a						//  5 cc
		push	hl							// 12 cc
		exx									//  5 cc

		ld		a, l						//  5 cc	Retreive fill value
		inc		b							//  5 cc

		ret									// 11 cc	Jump to "fastfill16_loop + 15 - count:LSB"

	fastfill16_jump:
		ld		a, l						//  5 cc	Retreive fill value

	fastfill16_loop:
		// fast 16-bits loop
		.rept 16
		out		(P_VDP_DATA), a				// 14 cc	Fill VRAM
		.endm
		djnz	fastfill16_loop				// 14/9 cc	Inner 8-bits loop

		dec		d							//  5 cc
		jp		nz, fastfill16_loop			// 11 cc	Outer 8-bits loop

	fastfill16_end:
		VDP_EI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		jp		(iy)						// 10 cc

	__endasm;
}
#endif // (VDP_USE_FASTFILL)

//-----------------------------------------------------------------------------
// Read data from VRAM to RAM [MSX1/2/2+/TR]
//
// Parameters:
//   src	- Source address in VRAM (14bits address form 16KB VRAM)
//   dst	- Desitation data address in RAM
//   count	- Number of byte to copy from VRAM
void VDP_ReadVRAM_16K(u16 src, u8* dest, u16 count)
{
	src;   // HL
	dest;  // DE
	count; // SP+2 => IY+0

	__asm

	#if ((VDP_USE_VALIDATOR) && !(MSX_VERSION & MSX_1))
		// Reset VRAM address bit 14 to 16 (in R#14)
		xor		a
		out		(P_VDP_REG), a
		ld		a, #VDP_REG(14)
		out		(P_VDP_REG), a
	#endif

		ld		iy, #2
		add		iy, sp
		// Setup address register 	
		ld		a, l
		VDP_DI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// AddrPort = (srcLow & 0x00FF)
		ld		a, h
		and		a, #0x3F				// + F_VDP_READ
		VDP_EI_DEF //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// AddrPort = ((srcLow >> 8) & 0x3F) + F_VDP_READ

	#if ((MSX_VERSION & MSX_1) && (VDP_USE_MODE_G1 || VDP_USE_MODE_G2 || VDP_USE_MODE_MC)) // Worst case 29 cc (use 29 cc loop)

		// Setup fast 16-bits loop
		ex		de, hl
		ld		e, 0(iy)				// count
		ld		d, 1(iy)
		ld		c, #P_VDP_DATA	
		ld		b, e					// number of loops is in DE
		dec		de						// calculate DB value (destroys B, D and E)
		inc		d
		// Fast loop	
	rd16_loop_start:
		ini								// in(c) ; hl++ ; b--					18 cc
		jp		nz, rd16_loop_start		//										11 cc
		dec		d
		jp		nz, rd16_loop_start		// b = 0 = 256

	#else // Worst case 20 cc (use 23 cc loop)

		// while (count--) *src++ = DataPort;
		ex		de, hl
		ld		c, #P_VDP_DATA			// data register
		// Handle count LSB
		ld		a, 0(iy)				// count LSB
		and		a
		jp		z, rd16_loop_init		// skip LSB
		ld		b, a					// retreive (count & 0x00FF) bytes
		inir							// 										23 cc
		// Handle count MSB		
	rd16_loop_init:
		ld		a, 1(iy)				// count MSB
	rd16_loop_start:
		and		a
		jp		z, rd16_loop_end		// finished
		ld		b, #0					// retreive 256 bytes packages
		inir							// 										23 cc
		dec		a
		jp		rd16_loop_start

	#endif

	rd16_loop_end:
		VDP_EI_ALL //~~~~~~~~~~~~~~~~~~~~~~~~~~
	__endasm;
}

//-----------------------------------------------------------------------------
// Read a value from VRAM [MSX1/2/2+/TR]
//
// Parameters:
//   dest	- Desitation address in VRAM (14bits address form 16KB VRAM)
//
// Return:
//   Value read in VRAM
u8 VDP_Peek_16K(u16 dest) __NAKED __PRESERVES(b, c, d, e, iyl, iyh)
{
	dest; // HL

	__asm

	#if ((VDP_USE_VALIDATOR) && !(MSX_VERSION & MSX_1))
		// Reset VRAM address bit 14 to 16 (in R#14)
		xor		a
		out		(P_VDP_REG), a
		ld		a, #VDP_REG(14)
		out		(P_VDP_REG), a
	#endif

		// Set destination address bits 0~7 to port #1
		ld		a, l
		VDP_DI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// AddrPort = (srcLow & 0x00FF)

		// Set destination address bits 8~13 to port #1
		ld		a, h
		and		a, #0x3F				// + F_VDP_READ
		out		(P_VDP_ADDR), a			// 12cc		AddrPort = ((srcLow >> 8) & 0x3F) + F_VDP_READ

		// Wait for VDP to be ready
	#if ((MSX_VERSION & MSX_1) && (VDP_USE_MODE_G1 || VDP_USE_MODE_G2 || VDP_USE_MODE_MC)) // Worst case 29cc (interval 29cc)
		add		hl, hl					// 12cc		For 29cc wait
	#elif ((VDP_USE_MODE_T1) || (VDP_USE_MODE_T2)) // Worst case 20cc (interval 22cc)
		nop								//  5cc		For 20cc wait
	#endif

		// Read data
		VDP_EI //~~~~~~~~~~~~~~~~~~~~~~ //  5cc
		in		a, (P_VDP_DATA)			// 12cc		value = DataPort

		ret
	__endasm;
}

//-----------------------------------------------------------------------------
// Write a value to VRAM [MSX1/2/2+/TR]
//
// Parameters:
//   val	- Value to write in VRAM
//   dest	- Desitation address in VRAM (14bits address form 16KB VRAM)
void VDP_Poke_16K(u8 val, u16 dest) __PRESERVES(c, h, l, iyl, iyh)
{
	val;  // A
	dest; // DE

	__asm
		ld		b, a					// Backup A register

	#if ((VDP_USE_VALIDATOR) && !(MSX_VERSION & MSX_1))
		// Reset VRAM address bit 14 to 16 (in R#14)
		xor		a
		out		(P_VDP_REG), a
		ld		a, #VDP_REG(14)
		out		(P_VDP_REG), a
	#endif

		// Set destination address bits 0~7 to port #1
		ld		a, e
		VDP_DI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// AddrPort = (srcLow & 0x00FF)

		// Set destination address bits 8~13 to port #1
		ld		a, d
		and		a, #0x3F
		or		a, #F_VDP_WRIT
		out		(P_VDP_ADDR), a			// 12cc		AddrPort = ((srcLow >> 8) & 0x3F) + F_VDP_WRIT

		// Wait for VDP to be ready
	#if ((MSX_VERSION & MSX_1) && (VDP_USE_MODE_G1 || VDP_USE_MODE_G2 || VDP_USE_MODE_MC)) // Worst case 29cc
		inc 	de						//  7cc		For 29cc wait
	#endif

		// Write data 	
		ld		a, b					//  5cc
		VDP_EI //~~~~~~~~~~~~~~~~~~~~~~ //  5cc
		out		(P_VDP_DATA), a			// 12cc		DataPort = value

	__endasm;
}

#endif // ((VDP_USE_VRAM16K) || (MSX_VERSION & MSX_1))


//=============================================================================
//
//  █▀▄▀█ █▀ ▀▄▀   ▀█   █▀▀ █ █ █▄ █ █▀▀ ▀█▀ █ █▀█ █▄ █ █▀
//  █ ▀ █ ▄█ █ █   █▄   █▀  █▄█ █ ▀█ █▄▄  █  █ █▄█ █ ▀█ ▄█
//
//=============================================================================

#if (MSX_VERSION >= MSX_2)

//-----------------------------------------------------------------------------
//
// HELPER FUNCTIONS
//
//-----------------------------------------------------------------------------

// N times outi 
#define OUTI(_n)	\
	.rept _n-1		\
		outi		\
	.endm			\
	VDP_EI			\
	outi

//-----------------------------------------------------------------------------
// Fast write to VDP register
// @todo out-out timing is 25 cc, less than worse case on MSX1 for G1 et G2!
#define ASM_REG_WRITE(_reg, _val)					\
	__asm											\
		ld		a, _val								\
		VDP_DI										\
		out		(P_VDP_ADDR), a						\
		ld		a, HASH(VDP_REG(_reg))				\
		VDP_EI										\
		out		(P_VDP_ADDR), a						\
	__endasm

//-----------------------------------------------------------------------------
// Fast write to VDP register with backup to RAM
#define ASM_REG_WRITE_BK(_reg, _val)				\
	__asm											\
		ld		a, _val								\
		ld		(_g_VDP_REGSAV + 7), a				\
	__endasm;										\
	ASM_REG_WRITE(_reg, _val)

// Fast incremental write to VDP register with backup to RAM.
#define ASM_REG_WRITE_INC_BK(_addr, _reg, _count)	\
	__asm											\
		ld		hl, HASH(_##_addr)					\
		ld		de, HASH(_g_VDP_REGSAV + _reg)		\
		ld		bc, HASH(_count)					\
		ldir										\
	__endasm;										\
	ASM_REG_WRITE_INC(_addr, _reg, _count)

// Fast incremental write to VDP register. Address in a label.
#define ASM_REG_WRITE_INC(_addr, _reg, _count)		\
	__asm											\
		ld		a, HASH(_reg)						\
		VDP_DI										\
		out		(P_VDP_ADDR), a						\
		ld		a, HASH(VDP_REG(17))				\
		VDP_EI_DEF									\
		out		(P_VDP_ADDR), a						\
		ld		hl, HASH(_##_addr)					\
		ld		c, HASH(P_VDP_IREG)					\
		VDP_DI_DEF									\
		OUTI(_count) ; 'ei' included				\
	__endasm

// Fast incremental write to VDP register. Address in HL register.
#define ASM_REG_WRITE_INC_HL(_reg, _count)			\
	__asm											\
		ld		a, HASH(_reg)						\
		VDP_DI										\
		out		(P_VDP_ADDR), a						\
		ld		a, HASH(VDP_REG(17))				\
		VDP_EI_DEF									\
		out		(P_VDP_ADDR), a						\
		ld		c, HASH(P_VDP_IREG)					\
		VDP_DI_DEF									\
		OUTI(_count) ; 'ei' included				\
	__endasm

//-----------------------------------------------------------------------------
//
// SCREEN MODE SETTING
//
//-----------------------------------------------------------------------------

#if (VDP_USE_MODE_T2)
//-----------------------------------------------------------------------------
// Initialize Text 2 screen mode.
void VDP_SetModeText2()
{
	VDP_SetModeFlag(VDP_T2_MODE);
	VDP_SetLayoutTable(VDP_T2_ADDR_NT);
	VDP_SetColorTable(VDP_T2_ADDR_CT);
	VDP_SetPatternTable(VDP_T2_ADDR_PT);
}
#endif // VDP_USE_MODE_T2

#if (VDP_USE_MODE_G3)
//-----------------------------------------------------------------------------
// Initialize Graphic 3 screen mode.
void VDP_SetModeGraphic3()
{
	VDP_SetModeFlag(VDP_G3_MODE);
	VDP_SetLayoutTable(VDP_G3_ADDR_NT);
	VDP_SetColorTable(VDP_G3_ADDR_CT);
	VDP_SetPatternTable(VDP_G3_ADDR_PT);
	#if (VDP_USE_SPRITE)
	VDP_SetSpriteAttributeTable(VDP_G3_ADDR_SAT);
	VDP_SetSpritePatternTable(VDP_G3_ADDR_SPT);
	#endif
}
#endif // VDP_USE_MODE_G3

#if (VDP_USE_MODE_G4)
//-----------------------------------------------------------------------------
// Initialize Graphic 4 screen mode.
void VDP_SetModeGraphic4()
{
	VDP_SetModeFlag(VDP_G4_MODE);
	VDP_SetLayoutTable(VDP_G4_ADDR_NT);
	#if (VDP_USE_SPRITE)
	VDP_SetSpriteAttributeTable(VDP_G4_ADDR_SAT);
	VDP_SetSpritePatternTable(VDP_G4_ADDR_SPT);
	#endif
}
#endif // VDP_USE_MODE_G4

#if (VDP_USE_MODE_G5)
//-----------------------------------------------------------------------------
// Initialize Graphic 5 screen mode.
void VDP_SetModeGraphic5()
{
	VDP_SetModeFlag(VDP_G5_MODE);
	VDP_SetLayoutTable(VDP_G5_ADDR_NT);
	#if (VDP_USE_SPRITE)
	VDP_SetSpriteAttributeTable(VDP_G5_ADDR_SAT);
	VDP_SetSpritePatternTable(VDP_G5_ADDR_SPT);
	#endif
}
#endif // VDP_USE_MODE_G5

#if (VDP_USE_MODE_G6)
//-----------------------------------------------------------------------------
// Initialize Graphic 6 screen mode.
void VDP_SetModeGraphic6()
{
	VDP_SetModeFlag(VDP_G6_MODE);
	VDP_SetLayoutTable(VDP_G6_ADDR_NT);
	#if (VDP_USE_SPRITE)
	VDP_SetSpriteAttributeTable(VDP_G6_ADDR_SAT);
	VDP_SetSpritePatternTable(VDP_G6_ADDR_SPT);
	#endif
}
#endif // VDP_USE_MODE_G6

#if (VDP_USE_MODE_G7)
//-----------------------------------------------------------------------------
// Initialize Graphic 7 screen mode.
void VDP_SetModeGraphic7()
{
	VDP_SetModeFlag(VDP_G7_MODE);
	VDP_SetLayoutTable(VDP_G7_ADDR_NT);
	#if (VDP_USE_SPRITE)
	VDP_SetSpriteAttributeTable(VDP_G7_ADDR_SAT);
	VDP_SetSpritePatternTable(VDP_G7_ADDR_SPT);
	#endif
}
#endif // VDP_USE_MODE_G7

//-----------------------------------------------------------------------------
//
// SETTING FUNCTIONS
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Read a given status register then reset status register to default (0) [MSX2/2+/TR]
//
// Parameters:
//   stat - Status register number (0-9)
u8 VDP_ReadStatus(u8 stat) __NAKED __PRESERVES(b, c, d, e, h, iyl, iyh)
{
	stat; // A

	__asm
	#if (VDP_USE_RESTORE_S0)
		VDP_DI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a
		ld		a, #VDP_REG(15)
		out		(P_VDP_ADDR), a
		in		a, (P_VDP_STAT)
		ld		l, a					// return value
		// Reset R#15 to default status register S#0
		xor		a
		out		(P_VDP_ADDR), a
		ld		a, #VDP_REG(15)
		VDP_EI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR),a
		ld		a, l
	#else
		VDP_DI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a
		ld		a, #VDP_REG(15)
		out		(P_VDP_ADDR), a
		VDP_EI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		in		a, (P_VDP_STAT)			// return value
	#endif
		ret
	__endasm;
}

//-----------------------------------------------------------------------------
// Adjustment of the display location on the screen [MSX2/2+/TR]
void VDP_SetAdjustOffset(u8 offset)
{
	#if (VDP_USE_COMMAND)
	VDP_CommandWait(); // @todo Check if it's really needed. Cf. https://www.msx.org/wiki/VDP_Display_Registers#Control_Register_18
	#endif
	VDP_RegWrite(18, offset);
}

//-----------------------------------------------------------------------------
// Set a new color palette from index 1 to 15 [MSX2/2+/TR]
// @param		pal			Address of the palette in RAM (table of 15 * 2 bytes)
// 						Format : [0:5|green:3|0|red:3|0|blue:3]
void VDP_SetPalette(const u8* pal) __FASTCALL __PRESERVES(d, e, iyl, iyh)
{
	pal; // HL

	__asm
	#if (VDP_USE_PALETTE16)
		xor		a // Start from index 0
	#else
		ld		a, #1 // Start from index 1
	#endif
		// VDP_DI  //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a
		ld		a, #VDP_REG(16)
		VDP_EI_DEF  //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a

		ld		c, #P_VDP_PAL
	#if (VDP_USE_PALETTE16)
		ld		b, #32 // Copy 16 x 2 bytes
	#else
		ld		b, #30 // Copy 15 x 2 bytes
	#endif
		otir
		VDP_EI_ALL  //~~~~~~~~~~~~~~~~~~~~~~~~~~
	__endasm;
}

#if (VDP_USE_DEFAULT_PALETTE)
#include "palette/pal_msx2.h"
void VDP_SetDefaultPalette()
{
	VDP_SetPalette((u8*)g_PaletteMSX2);
}
#endif // VDP_USE_DEFAULT_PALETTE


#if (VDP_USE_MSX1_PALETTE)
#include "palette/pal_msx1.h"
void VDP_SetMSX1Palette()
{
	VDP_SetPalette((u8*)g_PaletteMSX1);
}
#endif // VDP_USE_MSX1_PALETTE

#include "system.h"

//-----------------------------------------------------------------------------
// Set a given color entry in the palette [MSX2/2+/TR]
// @param		index		Index of the palette entry (0-15)
// @param		color		16 bits color value (@see RGB16 macro)
//							Format : [0:5|green:3|0|red:3|0|blue:3]
void VDP_SetPaletteEntry(u8 index, u16 color)
{
	index; // A
	color; // DE
	__asm
		VDP_DI
		out	(_g_VDP_RegPort), a
		ld	a, #VDP_REG(16)
		out	(_g_VDP_RegPort), a
		ld	a, e
		out	(_g_VDP_PalPort), a
		ld	a, d
		VDP_EI
		out	(_g_VDP_PalPort), a
	__endasm;
}

//-----------------------------------------------------------------------------
//
// VRAM ACCESS FUNCTIONS
//
//-----------------------------------------------------------------------------

#if (VDP_VRAM_ADDR == VDP_VRAM_ADDR_17)

//-----------------------------------------------------------------------------
// Write data from RAM to VRAM [MSX2/2+/TR]
//
// Parameters:
//   src		- Source data address in RAM
//   destLow	- Destination address in VRAM (16 LSB of 17-bits VRAM address)
//   destHigh	- Destination address in VRAM (1 MSB of 17-bits VRAM address)
//   count		- Number of byte to copy in VRAM
void VDP_WriteVRAM_128K(const u8* src, u16 destLow, u8 destHigh, u16 count)
{
	src;      // HL
	destLow;  // DE
	destHigh; // SP+2 => IY+0
	count;    // SP+3 => IY+1

	__asm
		ld		iy, #2
		add		iy, sp
		// Setup address register 
		ld		a, 0(iy)
		add		a, a
		add		a, a
		ld		c, a
		ld		a, d
		rlca
		rlca
		and		a, #0x03
		add		a, c
		VDP_DI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// RegPort = (page << 2) + (dest >> 14)
		ld		a, #VDP_REG(14)
		out		(P_VDP_REG), a			// RegPort = VDP_REG(14)
		ld		a, e
		out		(P_VDP_ADDR), a			// RegPort = (dest & 0xFF)
		ld		a, d
		and		a, #0x3F
		or		a, #F_VDP_WRIT
		VDP_EI_DEF //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// RegPort = ((dest >> 8) & 0x3F) + F_VDP_WRIT
		// while (count--) DataPort = *src++;
		ld		c, #P_VDP_DATA			// data register
		// Handle count LSB
		ld		a, 1(iy)				// count LSB
		and		a
		jp		z, wrt_loop_init		// skip LSB
		ld		b, a					// send (count & 0x00FF) bytes
		otir
		// Handle count MSB
	wrt_loop_init:
		ld		a, 2(iy)				// count MSB
	wrt_loop_start:
		and		a
		jp		z, wrt_loop_end			// finished
		ld		b, #0					// send 256 bytes packages
		otir
		dec		a
		jp		wrt_loop_start

	wrt_loop_end:
		VDP_EI_ALL //~~~~~~~~~~~~~~~~~~~~~~~~~~
	__endasm;
}

//-----------------------------------------------------------------------------
// Fill VRAM area with a given value [MSX2/2+/TR]
//
// Parameters:
//   value		- Byte value to copy in VRAM
//   destLow	- Destination address in VRAM (16 LSB of 17-bits VRAM address)
//   destHigh	- Destination address in VRAM (1 MSB of 17-bits VRAM address)
//   count		- Number of byte to copy in VRAM
void VDP_FillVRAM_128K(u8 value, u16 destLow, u8 destHigh, u16 count)
{
	value;		// A => B
	destLow;	// DE
	destHigh;	// SP+2 => IY+0
	count;		// SP+3 => IY+1

	__asm
		ld		b, a					// Backup input value 
		ld		iy, #2
		add		iy, sp
		// Setup address register 
		ld		a, 0(iy)
		add		a, a
		add		a, a
		ld		c, a
		ld		a, d
		rlca
		rlca
		and		a, #0x03
		add		a, c
		VDP_DI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// RegPort = (page << 2) + (dest >> 14);
		ld		a, #VDP_REG(14)
		out		(P_VDP_ADDR), a			// RegPort = VDP_REG(14);
		ld		a, e
		out		(P_VDP_ADDR), a			// RegPort = (dest & 0x00FF);
		ld		a, d
		and		a, #0x3F
		or		a, #F_VDP_WRIT
		VDP_EI_DEF //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// RegPort = ((dest >> 8) & 0x3F) + F_VDP_WRIT;
		// while (count--) DataPort = value;
		ld		e, 1(iy)				// count
		ld		d, 2(iy)
		ld		a, b					// value
		// fast 16-bits loop
		ld		b, e					// number of loops is in DE
		dec		de						// calculate DB value (destroys B, D and E)
		inc		d
	fll_loop_start:
		out		(P_VDP_DATA), a			// fill			12cc
		djnz	fll_loop_start			//				14cc
		dec		d
		jp		nz, fll_loop_start

	fll_loop_end:
		VDP_EI_ALL //~~~~~~~~~~~~~~~~~~~~~~~~~~
	__endasm;
}

//-----------------------------------------------------------------------------
// Read data from VRAM to RAM [MSX2/2+/TR]
//   srcLow		- Source address in VRAM (16 LSB of 17-bits VRAM address)
//   srcHigh	- Source address in VRAM (1 MSB of 17-bits VRAM address)
//   dest		- Desitation data address in RAM
//   count		- Number of byte to copy from VRAM
void VDP_ReadVRAM_128K(u16 srcLow, u8 srcHigh, u8* dest, u16 count)
{
	srcLow;		// HL
	srcHigh;	// SP+2 => IY+0
	dest;		// SP+3 => IY+1
	count;		// SP+5 => IY+3

	__asm
		ld		iy, #2
		add		iy, sp
		// Setup address register 	
		ld		a, 0(iy)				// R#14 A16 (move to bit 2)
		add		a, a
		add		a, a
		ld		c, a					// Backup A16
		ld		a, h					// R#14 A14-A15 (move to bit 1&0)
		rlca
		rlca
		and		a, #0x03
		add		a, c					// Merge R#14 A14-A16
		VDP_DI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// AddrPort = (srcHigh << 2) + (srcLow >> 14)
		ld		a, #VDP_REG(14)
		out		(P_VDP_REG), a			// RegPort  = (u8)(VDP_REG(14))
		ld		a, l
		out		(P_VDP_ADDR), a			// AddrPort = (srcLow & 0xFF)
		ld		a, h
		and		a, #0x3F				// + F_VDP_READ
		VDP_EI_DEF //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// AddrPort = ((srcLow >> 8) & 0x3F) + F_VDP_READ
		// while (count--) *src++ = DataPort;
		ld		l, 1(iy)				// source address
		ld		h, 2(iy)
		ld		c, #P_VDP_DATA			// data register
		// Handle count LSB
		ld		a, 3(iy)				// count LSB
		and		a
		jp		z, rd_loop_init			// skip LSB
		ld		b, a					// retreive (count & 0x00FF) bytes
		inir		
		// Handle count MSB		
	rd_loop_init:
		ld		a, 4(iy)				// count MSB
	rd_loop_start:
		and		a
		jp		z, rd_loop_end			// finished
		ld		b, #0					// retreive 256 bytes packages
		inir
		dec		a
		jp		rd_loop_start
		
	rd_loop_end:
		VDP_EI_ALL //~~~~~~~~~~~~~~~~~~~~~~~~~~
	__endasm;
}

//-----------------------------------------------------------------------------
// Write a value to VRAM [MSX2/2+/TR]
//
// Parameters:
//   val		- Value to write in VRAM
//   destLow	- Destination address in VRAM (16 LSB of 17-bits VRAM address)
//   destHigh	- Destination address in VRAM (1 MSB of 17-bits VRAM address)
void VDP_Poke_128K(u8 val, u16 destLow, u8 destHigh)
{
	val;      // A
	destLow;  // DE
	destHigh; // SP+2 => IY+0

	__asm
		ld		b, a

		ld		iy, #2
		add		iy, sp

		// Set destination address bits 14~16 to R#14
		ld		a, 0(iy)
		add		a, a
		add		a, a
		ld		c, a
		ld		a, d
		rlca
		rlca
		and		a, #0x03
		add		a, c
		VDP_DI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_REG), a			// RegPort = (destHigh << 2) + (destLow >> 14);
		ld		a, #VDP_REG(14)
		out		(P_VDP_REG), a			// RegPort = VDP_REG(14);

		// Set destination address bits 0~7 to port #1
		ld		a, e
		out		(P_VDP_ADDR), a			// AddrPort = (destLow & 0x00FF);

		// Set destination address bits 8~13 to port #1
		ld		a, d
		and		a, #0x3F
		or		a, #F_VDP_WRIT
		out		(P_VDP_ADDR), a			// 12cc		AddrPort = ((destLow >> 8) & 0x3F) + F_VDP_WRIT;

		// Write data
		ld		a, b					// 21cc
		VDP_EI //~~~~~~~~~~~~~~~~~~~~~~ //  5cc
		out		(P_VDP_DATA), a			// 12cc		DataPort = val

	__endasm;
}

//-----------------------------------------------------------------------------
// Read a value from VRAM [MSX2/2+/TR]
//
// Parameters:
//   srcLow		- Source address in VRAM (16 LSB of 17-bits VRAM address)
//   srcHigh	- Source address in VRAM (1 MSB of 17-bits VRAM address)
//
// Return:
//   Value read in VRAM
u8 VDP_Peek_128K(u16 srcLow, u8 srcHigh) __NAKED
{
	srcLow;		// HL
	srcHigh;	// SP+2

	__asm
		pop		iy				// Get return address from stack
		dec		sp				// Adjust stack pointer
		pop		de				// Get srcHigh from stack

		// Set destination address bits 14~16 to R#14
		ld		a, d
		add		a, a
		add		a, a
		ld		c, a
		ld		a, h
		rlca
		rlca
		and		a, #0x03
		add		a, c
		VDP_DI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_REG), a			// RegPort = (destHigh << 2) + (destLow >> 14);
		ld		a, #VDP_REG(14)
		out		(P_VDP_REG), a			// RegPort = VDP_REG(14);

		// Set destination address bits 0~7 to port #1
		ld		a, l
		out		(P_VDP_ADDR), a			// AddrPort = (srcLow & 0x00FF)

		// Set destination address bits 8~13 to port #1
		ld		a, h
		and		a, #0x3F				// + F_VDP_READ
		out		(P_VDP_ADDR), a			// 12cc		AddrPort = ((srcLow >> 8) & 0x3F) + F_VDP_READ

		// Wait for VDP to be ready
	#if ((VDP_USE_MODE_T1) || (VDP_USE_MODE_T2)) // MSX2 T1&T2 mode have 20cc wait
		nop								//  5cc		For 20cc wait
	#endif

		// Read data
		VDP_EI //~~~~~~~~~~~~~~~~~~~~~~ //  5cc
		in		a, (P_VDP_DATA)			// 12cc		value = DataPort

		// Return
		jp		(iy)
	__endasm;
}


#endif // (VDP_VRAM_ADDR == VDP_VRAM_ADDR_17)

#endif // (MSX_VERSION >= MSX_2)

//=============================================================================
//
//  █ █ █▀▄ █▀█   █▀▀ █▀█ █▀▄▀█ █▀▄▀█ ▄▀█ █▄ █ █▀▄ █▀
//  ▀▄▀ █▄▀ █▀▀   █▄▄ █▄█ █ ▀ █ █ ▀ █ █▀█ █ ▀█ █▄▀ ▄█
//
//=============================================================================

#if ((VDP_USE_COMMAND) && (MSX_VERSION >= MSX_2))

//-----------------------------------------------------------------------------
// Wait for previous VDP command to be finished
void VDP_CommandWait() __PRESERVES(b, c, d, e, h, l, iyl, iyh)
{
	__asm
	wait_vdp_ready:
	#if (VDP_USE_RESTORE_S0)
		// Set current status register to S#2
		ld		a, #2
		VDP_DI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_REG), a
		ld		a, #VDP_REG(15)
		out		(P_VDP_REG), a
		in		a, (P_VDP_STAT)		// get S#2 value
		rra							// check CE (bit#0)
		// Reset current status register to S#0 before enabling interruptions
		ld		a, #0				// don't use XOR here to keep C flag from RRA alive
		out		(P_VDP_REG), a
		ld		a, #VDP_REG(15)
		VDP_EI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_REG), a
	#else
		// Set current status register to S#2
		ld		a, #2
		VDP_DI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_REG), a
		ld		a, #VDP_REG(15)
		out		(P_VDP_REG), a
		VDP_EI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		in		a, (P_VDP_STAT)		// get S#2 value
		rra							// check CE (bit#0)
	#endif
		jp		c, wait_vdp_ready	// CE==1 ? command not finished
	__endasm;
}

//-----------------------------------------------------------------------------
// Send VDP command (from register 32 to 46)
void VPD_CommandSetupR32()
{
	VDP_CommandWait();
	ASM_REG_WRITE_INC(g_VDP_Command, 32, 15);
}

//-----------------------------------------------------------------------------
// Send VDP command (from register 36 to 46)
void VPD_CommandSetupR36()
{
	VDP_CommandWait();
	ASM_REG_WRITE_INC(g_VDP_Command + 4, 36, 11);
}

//-----------------------------------------------------------------------------
// Write to VRAM command loop
// @param		addr		The address in RAM from where read data to be copied in VRAM
// 						(First byte of data was sent in the command execution)
void VPD_CommandWriteLoop(const u8* addr) __FASTCALL __PRESERVES(d, e, iyl, iyh)
{
	addr; // HL
	
	__asm
		// Set indirect register write to R#44
		ld  	a, #VDP_REG(44)
		VDP_DI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out 	(P_VDP_REG), a
		ld  	a, #VDP_REG(17)
		out 	(P_VDP_REG), a
		// Set current status register to S#2
		ld  	a, #2
		out 	(P_VDP_REG), a
		ld  	a, #VDP_REG(15)
		out 	(P_VDP_REG), a
		// Setup outi loop (value of register B don't matter)
		inc 	hl
		ld		c, #P_VDP_IREG
	write_loop:
		// Read S#2 to check CE flag (no need to check TR (bit#7) while write loop is longer than worse case VDP write duration (~29cc))
		in		a, (P_VDP_STAT)
		rra							// check CE (bit#0)
		jp		nc, write_finished	// CE==0 ? command finished
		outi						// write a byte from HL to port VDP_IREG
		jp		write_loop

	write_finished:
		// Reset current status register to S#0
	#if (VDP_USE_RESTORE_S0)
		xor 	a	
		out 	(P_VDP_REG), a
		ld  	a, #VDP_REG(15)
		VDP_EI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out 	(P_VDP_REG), a
	#else
		VDP_EI //~~~~~~~~~~~~~~~~~~~~~~~~~~
	#endif
	__endasm;
}

#endif // ((VDP_USE_COMMAND) && (MSX_VERSION >= MSX_2))

#if ((MSX_VERSION >= MSX_2) && (VDP_USE_CUSTOM_CMD))

//-----------------------------------------------------------------------------
// Send VDP custom command through buffer (form registres 32 to 46). [MSX2/2+/TR]
void VDP_CommandCustomR32(const VDP_Command* data)
{
	data; // HL
	VDP_CommandWait(); // don't modify HL
	ASM_REG_WRITE_INC_HL(32, 15);
}

//-----------------------------------------------------------------------------
// Send VDP custom command through buffer (form registres 36 to 46). [MSX2/2+/TR]
void VDP_CommandCustomR36(const VDP_Command36* data)
{
	data; // HL
	VDP_CommandWait(); // don't modify HL
	ASM_REG_WRITE_INC_HL(36, 11);
}

#endif

//=============================================================================
//
//  █▀▄▀█ █▀ ▀▄▀   ▀█ ▄█▄   █▀▀ █ █ █▄ █ █▀▀ ▀█▀ █ █▀█ █▄ █ █▀
//  █ ▀ █ ▄█ █ █   █▄  ▀    █▀  █▄█ █ ▀█ █▄▄  █  █ █▄█ █ ▀█ ▄█
//
//=============================================================================

#if (MSX_VERSION >= MSX_2P)

//-----------------------------------------------------------------------------
// Set the horizontal rendeing offset (in pixel) [MSX2+/TR]
void VDP_SetHorizontalOffset(u16 offset)
{
	u8 reg = (8 - (offset & 0xFF)) & 0x07;
	VDP_RegWrite(27, reg);
	reg = (offset + 7 ) >> 3;
	VDP_RegWrite(26, reg);
}

#endif


//=============================================================================
//
//  █▀▀ █▀█ █▀▄▀█ █▀▄▀█ █▀█ █▄ █   █▀▀ █ █ █▄ █ █▀▀ ▀█▀ █ █▀█ █▄ █ █▀
//  █▄▄ █▄█ █ ▀ █ █ ▀ █ █▄█ █ ▀█   █▀  █▄█ █ ▀█ █▄▄  █  █ █▄█ █ ▀█ ▄█
//
//=============================================================================

//-----------------------------------------------------------------------------
// Initialize VDP module [MSX1/2/2+/TR]
// Retreive BIOS initiale value.
// This function is called automatically when VDP_SetMode() is called if the macro VDP_AUTO_INIT equals 1.
void VDP_Initialize()
{
__asm
	// Copy 8 bytes from M_RG0SAV to g_VDP_REGSAV+0 (0-7)
	ld		hl, #M_RG0SAV
	ld		de, #_g_VDP_REGSAV+0
	ld		bc, #8
	ldir
__endasm;

#if (MSX_VERSION >= MSX_2)
__asm
	// Copy 16 bytes from M_RG08SAV to g_VDP_REGSAV+8 (8-23)
	ld		hl, #M_RG08SAV
	ld		de, #_g_VDP_REGSAV+8
	ld		bc, #16
	ldir
__endasm;
#endif

#if (MSX_VERSION >= MSX_2P)
__asm
	// Copy 3 bytes from M_RG25SAV to g_VDP_REGSAV+25 (25-27)
	ld		hl, #M_RG25SAV
	ld		de, #_g_VDP_REGSAV+25
	ld		bc, #3
	ldir
__endasm;
#endif

	#if (VDP_AUTO_INIT)
	g_VDPInitilized = TRUE;
	#endif
}


//-----------------------------------------------------------------------------
// Set screen mode [MSX1/2/2+/TR]
//
// Parameters:
//   mode - The new screen mode to set (see <VDP_MODE> enumeration)
void VDP_SetMode(const u8 mode)
{
	#if (VDP_AUTO_INIT)
	if (!g_VDPInitilized)
		VDP_Initialize();
	#endif

	#if (MSX_VERSION >= MSX_2P)
	VDP_RegWriteBak(25, 0);
	#endif

	g_VDP_Data.Mode = mode;
	switch (mode)
	{
//.............................................................................
// MSX1
#if (VDP_USE_MODE_T1)
	// case VDP_MODE_SCREEN0:
	// case VDP_MODE_SCREEN0_W40:
	case VDP_MODE_TEXT1:
		VDP_SetModeText1();
		break;
#endif
	
#if (VDP_USE_MODE_MC)
	// case VDP_MODE_SCREEN3:
	case VDP_MODE_MULTICOLOR:
		VDP_SetModeMultiColor();
		break;
#endif

#if (VDP_USE_MODE_G1)
	// case VDP_MODE_SCREEN1:
	case VDP_MODE_GRAPHIC1:
		VDP_SetModeGraphic1();
		break;
#endif

#if (VDP_USE_MODE_G2)
	// case VDP_MODE_SCREEN2:
	case VDP_MODE_GRAPHIC2:
		VDP_SetModeGraphic2();
		break;
#endif

//.............................................................................
// MSX2
#if (MSX_VERSION >= MSX_2)

#if (VDP_USE_MODE_T2)
	// case VDP_MODE_SCREEN0_W80:
	case VDP_MODE_TEXT2:
		VDP_SetModeText2();
		break;
#endif

#if (VDP_USE_MODE_G3)
	#if (VDP_USE_UNDOCUMENTED)
	case VDP_MODE_GRAPHIC3_MIRROR_0:
	case VDP_MODE_GRAPHIC3_MIRROR_01:
	case VDP_MODE_GRAPHIC3_MIRROR_02:
	#endif
	// case VDP_MODE_SCREEN4:
	case VDP_MODE_GRAPHIC3:
		VDP_SetModeGraphic3();
		break;
#endif
	
	
#if (VDP_USE_MODE_G4)
	// case VDP_MODE_SCREEN5:
	case VDP_MODE_GRAPHIC4:
	case VDP_MODE_SCREEN9_40: // @todo Further setting needed?
		VDP_SetModeGraphic4();
		break;
#endif
	
#if (VDP_USE_MODE_G5)
	// case VDP_MODE_SCREEN6:
	case VDP_MODE_GRAPHIC5:
	case VDP_MODE_SCREEN9_80: // @todo Further setting needed?
		VDP_SetModeGraphic5();
		break;
#endif
	
#if (VDP_USE_MODE_G6)
	// case VDP_MODE_SCREEN7:
	case VDP_MODE_GRAPHIC6:
		VDP_SetModeGraphic6();
		break;
#endif
		
#if (VDP_USE_MODE_G7)
	// case VDP_MODE_SCREEN8:
	case VDP_MODE_GRAPHIC7:		
		VDP_SetModeGraphic7();
		#if (MSX_VERSION >= MSX_2P)
		VDP_SetYJK(VDP_YJK_OFF);
		#endif
		break;
#endif

#endif // (MSX_VERSION >= MSX_2)

//.............................................................................
// MSX2+
#if ((MSX_VERSION >= MSX_2P) && (VDP_USE_MODE_G7))
	case VDP_MODE_SCREEN10:
	case VDP_MODE_SCREEN11:
		VDP_SetModeGraphic7();
		VDP_SetYJK(VDP_YJK_YAE);
		break;

	case VDP_MODE_SCREEN12:
		VDP_SetModeGraphic7();
		VDP_SetYJK(VDP_YJK_ON);
		break;
#endif // ((MSX_VERSION >= MSX_2P) && (VDP_USE_MODE_G7))
	}

//.............................................................................
// Default VDP setting
#if (VDP_USE_DEFAULT_SETTINGS)
	VDP_EnableDisplay(TRUE);
	VDP_EnableVBlank(TRUE);
#if (!(MSX_VERSION & MSX_1))
	#if (VDP_INIT_50HZ == VDP_INIT_ON)
		VDP_SetFrequency(VDP_FREQ_50HZ);
	#elif (VDP_INIT_50HZ == VDP_INIT_OFF)
		VDP_SetFrequency(VDP_FREQ_60HZ);
	#elif (VDP_INIT_50HZ == VDP_INIT_AUTO)
		u8 freq = Sys_Is50Hz() ? VDP_FREQ_50HZ : VDP_FREQ_60HZ;
		VDP_SetFrequency(freq);
	#endif
	#if (VDP_USE_SPRITE)
		VDP_EnableSprite(TRUE);
	#else
		VDP_DisableSprite();
	#endif
	if (VDP_IsBitmapMode(mode)) // Activate 212 lines for bitmap mode
		VDP_SetLineCount(VDP_LINE_212);
	else
		VDP_SetLineCount(VDP_LINE_192);
	VDP_SetInterlace(FALSE);
	VDP_SetPageAlternance(FALSE);
#endif // (MSX_VERSION >= MSX_2)
#endif // (VDP_USE_DEFAULT_SETTINGS)
}

//-----------------------------------------------------------------------------
// Get VDP version
//	@return					0: TMS9918A, 1: V9938, 2: V9958, x: VDP ID
// @note Code by Grauw (http://map.grauw.nl/sources/vdp_detection.php)
u8 VDP_GetVersion() __NAKED
{
	__asm
	// Detect VDP version
	//
	// a <- 0: TMS9918A, 1: V9938, 2: V9958, x: VDP ID
	// f <- z: TMS9918A, nz: other
	VDP_GetVersionAsm:
		call	VDP_IsTMS9918A			// use a different way to detect TMS9918A
		ret		z
		ld		a, #1					// select s#1
		VDP_DI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a
		ld		a, #VDP_REG(15)
		out		(P_VDP_ADDR), a
		in		a, (P_VDP_ADDR)			// read s#1
		and		#0b00111110				// get VDP ID
		rrca
		ex		af, af'					; '
		xor		a						// select s#0 as required by BIOS
		out		(P_VDP_ADDR), a
		ld		a, #VDP_REG(15)
		VDP_EI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a
		ex		af, af'					; '
		ret		nz						// return VDP ID for V9958 or higher
		inc		a						// return 1 for V9938
		ret

	// Test if the VDP is a TMS9918A.
	//
	// The VDP ID number was only introduced in the V9938, so we have to use a
	// different method to detect the TMS9918A. We wait for the vertical blanking
	// interrupt flag, and then quickly read status register 2 and expect bit 6
	// (VR, vertical retrace flag) to be set as well. The TMS9918A has only one
	// status register, so bit 6 (5S, 5th sprite flag) will return 0 instead.
	//
	// f <- z: TMS9918A, nz: V99x8
	VDP_IsTMS9918A:
		xor 	a						// reset s#0
		VDP_DI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out 	(P_VDP_REG), a
		ld  	a, #VDP_REG(15)
		out 	(P_VDP_REG), a

		in		a, (P_VDP_ADDR)			// read s#0, make sure interrupt flag is reset
	VDP_IsTMS9918A_Wait:
		in		a,(P_VDP_ADDR)			// read s#0
		and		a						// wait until interrupt flag is set
		jp		p, VDP_IsTMS9918A_Wait
		ld		a, #2					// select s#2 on V9938
		out		(P_VDP_ADDR), a
		ld		a, #VDP_REG(15)			// (this mirrors to r#7 on TMS9918 VDPs)
		out		(P_VDP_ADDR), a
		in		a, (P_VDP_ADDR)			// read s#2 / s#0
		ex		af, af'					; '
		xor		a						// select s#0 as required by BIOS
		out		(P_VDP_ADDR), a
		ld		a, #VDP_REG(15)
		out		(P_VDP_ADDR), a
		ld		a, (0xF3E6)
		out		(P_VDP_ADDR), a			// restore r#7 if it mirrored (small flash visible)
		ld		a, #VDP_REG(7)
		VDP_EI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a
		ex		af, af'					; '
		and		#0b01000000				// check if bit 6 was 0 (s#0 5S) or 1 (s#2 VR)
		ret
	
	__endasm;
}

//-----------------------------------------------------------------------------
// Set register value [MSX1/2/2+/TR]
//
// Parameters:
//   reg   - Register number
//   value - Value to set
void VDP_RegWrite(u8 reg, u8 value) __PRESERVES(b, c, d, e, iyl, iyh)
{
	reg;	// A
	value;	// L

	__asm
		ld		h, a					// Register number
		ld		a, l					// Value
		VDP_DI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a
		ld		a, h
		add		#0x80					// @todo Can be optimize by including 80h in the register number ; 33cc -> 25cc (warning for MSX1 VRAM write timing)
		VDP_EI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a
	__endasm;
}

//-----------------------------------------------------------------------------
// Set register value after backuping previous value [MSX1/2/2+/TR]
//
// Parameters:
//   reg   - Register number
//   value - Value to set
void VDP_RegWriteBak(u8 reg, u8 value) __PRESERVES(d, e, iyl, iyh)
{
	reg;	// A
	value;	// L

	__asm
		ld		c, a					// Register number
		ld		a, l					// Value
		// Backup
		ld		b, #0
		ld		hl, #_g_VDP_REGSAV
		add		hl, bc
		ld		(hl), a
		// Write to register
		VDP_DI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a
		ld		a, c
		add		#0x80					// @todo Can be optimize by including 80h in the register number ; 33cc -> 25cc (warning for MSX1 VRAM write timing)
		VDP_EI //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a
	__endasm;
}

//-----------------------------------------------------------------------------
// Set masked register value after backuping previous value. [MSX1/2/2+/TR]
//
// Parameters:
//   reg  - Register number
//   mask - Previous value reset mask
//   flag - Value to set
void VDP_RegWriteBakMask(u8 reg, u8 mask, u8 flag)
{
	u8 value = g_VDP_REGSAV[reg];
	value &= mask;
	value |= flag;
	VDP_RegWriteBak(reg, value);
}

//-----------------------------------------------------------------------------
// Read default S#0 register [MSX1/2/2+/TR]
u8 VDP_ReadDefaultStatus() __NAKED __PRESERVES(b, c, d, e, h, l, iyl, iyh)
{
	__asm
		in		a, (P_VDP_STAT)
		ret
	__endasm;
}

#if ((VDP_USE_MODE_G4 || VDP_USE_MODE_G5 || VDP_USE_MODE_G6 || VDP_USE_MODE_G7))

//-----------------------------------------------------------------------------
// Set current VRAM page
void VDP_SetPage(u8 page)
{
	u8 reg = page << 5;
	reg |= 0b11111;
	VDP_RegWriteBak(2, reg);
}

#endif // ((VDP_USE_MODE_G4 || VDP_USE_MODE_G5 || VDP_USE_MODE_G6 || VDP_USE_MODE_G7))

//-----------------------------------------------------------------------------
// Set layout table VRAM address
void VDP_SetLayoutTable(VADDR addr)
{
	g_ScreenLayoutLow = (u16)addr;

	u8 reg;
	reg = (u8)(addr >> 10);
	#if (MSX_VERSION >= MSX_2)
		switch (g_VDP_Data.Mode)
		{
		case VDP_MODE_TEXT2:
			reg |= 0b11;
			break;
		case VDP_MODE_GRAPHIC6:
		case VDP_MODE_GRAPHIC7:
		#if (MSX_VERSION >= MSX_2P)
		case VDP_MODE_SCREEN10:
		case VDP_MODE_SCREEN11:
		case VDP_MODE_SCREEN12:
		#endif
			reg >>= 1;
		case VDP_MODE_GRAPHIC4:
		case VDP_MODE_GRAPHIC5:
			reg |= 0b11111;
			break;
		};	
	#endif
	VDP_RegWrite(2, reg);

	#if (VDP_VRAM_ADDR == VDP_VRAM_ADDR_17)
		g_ScreenLayoutHigh = addr >> 16;
	#endif
}

//-----------------------------------------------------------------------------
// Set color table VRAM address
void VDP_SetColorTable(VADDR addr)
{
	g_ScreenColorLow = (u16)addr;
	
	u8 reg;
	reg = (u8)(addr >> 6);
	switch (g_VDP_Data.Mode)
	{
	#if (MSX_VERSION >= MSX_2)
	case VDP_MODE_TEXT2:
		reg |= 0b111;
		break;
	#if (VDP_USE_UNDOCUMENTED)
	case VDP_MODE_GRAPHIC3_MIRROR_0:
		reg |= 0b0011111;
		break;
	case VDP_MODE_GRAPHIC3_MIRROR_01:
		reg |= 0b0111111;
		break;
	case VDP_MODE_GRAPHIC3_MIRROR_02:
		reg |= 0b1011111;
		break;
	#endif
	case VDP_MODE_GRAPHIC3:
	#endif
	case VDP_MODE_GRAPHIC2:
		reg |= 0b1111111;
		break;
	};	
	VDP_RegWrite(3, reg);

	#if (VDP_VRAM_ADDR == VDP_VRAM_ADDR_17)
		reg = (u8)(addr >> 14);
		VDP_RegWrite(10, reg);

		g_ScreenColorHigh = addr >> 16;
	#endif
}

//-----------------------------------------------------------------------------
// Set pattern table VRAM address
void VDP_SetPatternTable(VADDR addr)
{
	g_ScreenPatternLow = (u16)addr;

	u8 reg;
	reg = (u8)(addr >> 11);
	switch (g_VDP_Data.Mode)
	{
	#if (MSX_VERSION >= MSX_2)
	#if (VDP_USE_UNDOCUMENTED)
	case VDP_MODE_GRAPHIC3_MIRROR_01:
		reg |= 0b01;
		break;
	case VDP_MODE_GRAPHIC3_MIRROR_02:
		reg |= 0b10;
		break;
	#endif
	case VDP_MODE_GRAPHIC3:
	#endif
	case VDP_MODE_GRAPHIC2:
		reg |= 0b11;
	};
	VDP_RegWrite(4, reg);
	
	#if (VDP_VRAM_ADDR == VDP_VRAM_ADDR_17)
		g_ScreenPatternHigh = addr >> 16;
	#endif
}


//-----------------------------------------------------------------------------
//
//   S P R I T E S
//
//-----------------------------------------------------------------------------
#if (VDP_USE_SPRITE)

//-----------------------------------------------------------------------------
// Set sprite attribute table address
// @param		addr		VRAM address where to place the table (16 or 17-bits long depending on VDP_VRAM_ADDR definition)
//							Address must be a multiple of 80h for MSX1 screen modes and  multiple of 200h for MSX2 ones.
void VDP_SetSpriteAttributeTable(VADDR addr)
{
	g_SpriteAttributeLow = (u16)addr;
	
	u8 reg;
	reg = (u8)(addr >> 7);
	#if (MSX_VERSION >= MSX_2)
		switch (g_VDP_Data.Mode)
		{
		case VDP_MODE_GRAPHIC3:
		case VDP_MODE_GRAPHIC4:
		case VDP_MODE_GRAPHIC5:
		case VDP_MODE_GRAPHIC6:
		case VDP_MODE_GRAPHIC7:
		#if (MSX_VERSION >= MSX_2P)
		case VDP_MODE_SCREEN10:
		case VDP_MODE_SCREEN11:
		case VDP_MODE_SCREEN12:
		#endif
		#if (VDP_USE_UNDOCUMENTED)
		case VDP_MODE_GRAPHIC3_MIRROR_0:
		case VDP_MODE_GRAPHIC3_MIRROR_01:
		case VDP_MODE_GRAPHIC3_MIRROR_02:
		#endif
			reg |= 0b111;
			break;
		};
	#endif
	VDP_RegWrite(5, reg);

	#if (VDP_VRAM_ADDR == VDP_VRAM_ADDR_17)
		reg = (u8)(addr >> 15);
		VDP_RegWrite(11, reg);

		g_SpriteAttributeHigh = addr >> 16;
	#endif

	addr -= 0x200;
	g_SpriteColorLow = (u16)addr;
	#if (VDP_VRAM_ADDR == VDP_VRAM_ADDR_17)
		g_SpriteColorHigh = addr >> 16;
	#endif
}

//-----------------------------------------------------------------------------
// Set sprite pattern table address
// @param		addr		VRAM address where to place the table (16 or 17-bits long depending on VDP_VRAM_ADDR definition)
//							Address must be a multiple of 800h.
void VDP_SetSpritePatternTable(VADDR addr)
{
	g_SpritePatternLow  = (u16)addr;
	#if (VDP_VRAM_ADDR == VDP_VRAM_ADDR_17)
		g_SpritePatternHigh = addr >> 16;
	#endif

	u8 reg = (u8)(addr >> 11);
	VDP_RegWrite(6, reg);

}

//-----------------------------------------------------------------------------
// Load pattern data into VRAM
void VDP_LoadSpritePattern(const u8* addr, u8 index, u8 count)
{
	u16 low = g_SpritePatternLow;
	low += (index * 8);
	VDP_WriteVRAM(addr, low, g_SpritePatternHigh, count * 8);
}

//-----------------------------------------------------------------------------
// Set sprite attribute for Sprite Mode 1 (MSX1)
void VDP_SetSpriteSM1(u8 index, u8 x, u8 y, u8 shape, u8 color)
{
	g_VDP_Sprite.Y = y;				// Y coordinate on screen (all lower priority sprite will be disable if equal to 216 or 0xD0)
    g_VDP_Sprite.X = x;				// X coordinate of the sprite
    g_VDP_Sprite.Pattern = shape;	// Pattern index
    g_VDP_Sprite.Color = color;		// Color index (Sprite Mode 1 only) + Early clock

	u16 low = g_SpriteAttributeLow;
	low += (index * 4);
	VDP_WriteVRAM((u8*)&g_VDP_Sprite, low, g_SpriteAttributeHigh, 4);
}

//-----------------------------------------------------------------------------
// Set sprite attribute for Sprite Mode 2
void VDP_SetSprite(u8 index, u8 x, u8 y, u8 shape)
{
	g_VDP_Sprite.Y = y;				// Y coordinate on screen (all lower priority sprite will be disable if equal to 216 or 0xD0)
    g_VDP_Sprite.X = x;				// X coordinate of the sprite
    g_VDP_Sprite.Pattern = shape;	// Pattern index

	u16 low = g_SpriteAttributeLow;
	low += (index * 4);
	VDP_WriteVRAM((u8*)&g_VDP_Sprite, low, g_SpriteAttributeHigh, 3);
}

#if (MSX_VERSION >= MSX_2)
//-----------------------------------------------------------------------------
// Set sprite attribute for Sprite Mode 2 and fill color table with color data
void VDP_SetSpriteExMultiColor(u8 index, u8 x, u8 y, u8 shape, const u8* ram)
{
	u16 col = g_SpriteColorLow;
	col += (index * 16);
	VDP_WriteVRAM(ram, col, g_SpriteColorHigh, VDP_SPRITE_COLORS);

	g_VDP_Sprite.X = x;				// Y coordinate on screen (all lower priority sprite will be disable if equal to 216 or 0xD0)
	g_VDP_Sprite.Y = y;				// X coordinate of the sprite
	g_VDP_Sprite.Pattern = shape;	// Pattern index
	u16 attr = g_SpriteAttributeLow;
	attr += (index * 4);
	VDP_WriteVRAM((u8*)&g_VDP_Sprite, attr, g_SpriteAttributeHigh, 3);
}

//-----------------------------------------------------------------------------
// Set sprite attribute for Sprite Mode 2 and fill color table with unique color
void VDP_SetSpriteExUniColor(u8 index, u8 x, u8 y, u8 shape, u8 color)
{
	u16 col = g_SpriteColorLow;
	col += (index * 16);
	VDP_FillVRAM(color, col, g_SpriteColorHigh, VDP_SPRITE_COLORS);

	g_VDP_Sprite.X = x;				// Y coordinate on screen (all lower priority sprite will be disable if equal to 216 or 0xD0)
	g_VDP_Sprite.Y = y;				// X coordinate of the sprite
	g_VDP_Sprite.Pattern = shape;	// Pattern index
	u16 attr = g_SpriteAttributeLow;
	attr += (index * 4);
	VDP_WriteVRAM((u8*)&g_VDP_Sprite, attr, g_SpriteAttributeHigh, 3);
}
#endif // (MSX_VERSION >= MSX_2)

//-----------------------------------------------------------------------------
// Update sprite position
void VDP_SetSpritePosition(u8 index, u8 x, u8 y)
{
	g_VDP_Sprite.Y = y;				// Y coordinate on screen (all lower priority sprite will be disable if equal to 216 or 0xD0)
    g_VDP_Sprite.X = x;				// X coordinate of the sprite

	u16 low = g_SpriteAttributeLow;
	low += (index * 4);
	VDP_WriteVRAM((u8*)&g_VDP_Sprite, low, g_SpriteAttributeHigh, 2);
}

//-----------------------------------------------------------------------------
// Update sprite position X
void VDP_SetSpritePositionX(u8 index, u8 x)
{
	u16 low = g_SpriteAttributeLow;
	low += (index * 4);
	VDP_Poke(x, ++low, g_SpriteAttributeHigh);
}

//-----------------------------------------------------------------------------
// Update sprite position Y
void VDP_SetSpritePositionY(u8 index, u8 y)
{
	u16 low = g_SpriteAttributeLow;
	low += (index * 4);
	VDP_Poke(y, low, g_SpriteAttributeHigh);
}

//-----------------------------------------------------------------------------
// Update sprite pattern
void VDP_SetSpritePattern(u8 index, u8 shape)
{
	u16 low = g_SpriteAttributeLow + 2;
	low += (index * 4);
	VDP_Poke(shape, low, g_SpriteAttributeHigh);
}

//-----------------------------------------------------------------------------
// Update sprite pattern (Shader mode 1)
void VDP_SetSpriteColorSM1(u8 index, u8 color)
{
	u16 low = g_SpriteAttributeLow + 3;
	low += (index * 4);
	VDP_Poke(color, low, g_SpriteAttributeHigh);
}

#if (MSX_VERSION >= MSX_2)
//-----------------------------------------------------------------------------
// Update sprite color (Uni-color)
void VDP_SetSpriteUniColor(u8 index, u8 color)
{
	u16 col = g_SpriteColorLow;
	col += (index * 16);
	VDP_FillVRAM(color, col, g_SpriteColorHigh, VDP_SPRITE_COLORS);
}

//-----------------------------------------------------------------------------
// Update sprite color (Multi-color)
void VDP_SetSpriteMultiColor(u8 index, const u8* ram)
{
	u16 col = g_SpriteColorLow;
	col += (index * 16);
	VDP_WriteVRAM(ram, col, g_SpriteColorHigh, VDP_SPRITE_COLORS);	
}

//-----------------------------------------------------------------------------
//
void VDP_SetSpriteData(u8 index, const u8* data)
{
	u16 low = g_SpriteAttributeLow;
	low += (index * 4);
	VDP_WriteVRAM(data, low, g_SpriteAttributeHigh, 3);
}
#endif // (MSX_VERSION >= MSX_2)

//-----------------------------------------------------------------------------
//
void VDP_DisableSpritesFrom(u8 index)
{
	u8 y = VDP_SPRITE_DISABLE_SM1;
	#if (MSX_VERSION >= MSX_2)
		if (g_VDP_Data.Mode >= VDP_MODE_MSX2) // MSX2 modes
			y = VDP_SPRITE_DISABLE_SM2;
	#endif
	VDP_SetSpritePositionY(index, y);
}

/*
//-----------------------------------------------------------------------------
// Set sprite attribute
void VDP_SendSpriteAttribute(u8 index) __FASTCALL
{
	VDP_CommandWait();
	__asm
		// Compute sprite attribute VRAM address
		ld		a, l			// Sprite index
		rlca
		rlca
		ld		hl, (#M_ATRBAS)
		add		a, l
		ld		l, a
		adc		a, h 
		sub		l 
		ld		h, a 
		
		ld		hl, #(_g_VDP_Sprite)
		ld		c, #P_VDP_DATA 
		VDP_DI
		outi
		outi
		outi
		VDP_EI
		outi
	__endasm;
}
*/

#endif // (VDP_USE_SPRITE)

//=============================================================================
//
//   T E X T   B L I N K
//
//=============================================================================

#if (VDP_USE_MODE_T2 && (MSX_VERSION >= MSX_2))

const u8 g_VDPBitValue[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

//-----------------------------------------------------------------------------
// Set blink attribute for a the given tile 
void VDP_SetBlinkTile(u8 x, u8 y)
{
	u16 addr = g_ScreenColorLow + (y * 10) + (x / 8);
	u8 chunk = VDP_Peek(addr, g_ScreenColorHigh);
	chunk |= g_VDPBitValue[7 - (x % 8)];
	VDP_Poke(chunk, addr, g_ScreenColorHigh);
}

#endif // (VDP_USE_MODE_T2 && (MSX_VERSION >= MSX_2))

//=============================================================================
//
//   G R A P H   M O D E   2
//
//=============================================================================

#if (VDP_USE_MODE_G2 || VDP_USE_MODE_G3)

//-----------------------------------------------------------------------------
// Load patterns in all 3 screen sections
void VDP_LoadPattern_GM2(const u8* src, u8 count, u8 offset)
{
	u16 dst = g_ScreenPatternLow + (offset * 8);
	VDP_WriteVRAM(src, dst, g_ScreenPatternHigh, count * 8);
	dst += 0x800;
	VDP_WriteVRAM(src, dst, g_ScreenPatternHigh, count * 8);
	dst += 0x800;
	VDP_WriteVRAM(src, dst, g_ScreenPatternHigh, count * 8);
}

//-----------------------------------------------------------------------------
// Load colors in all 3 screen sections
void VDP_LoadColor_GM2(const u8* src, u8 count, u8 offset)
{
	u16 dst = g_ScreenColorLow + (offset * 8);
	VDP_WriteVRAM(src, dst, g_ScreenColorHigh, count * 8);
	dst += 0x800;
	VDP_WriteVRAM(src, dst, g_ScreenColorHigh, count * 8);
	dst += 0x800;
	VDP_WriteVRAM(src, dst, g_ScreenColorHigh, count * 8);
}

//-----------------------------------------------------------------------------
// Copy patterns to a rectangle
void VDP_WriteLayout_GM2(const u8* src, u8 dx, u8 dy, u8 nx, u8 ny)
{
	u16 dst = g_ScreenLayoutLow + (dy * 32) + dx;
	for (u8 y = 0; y < ny; ++y)
	{
		VDP_WriteVRAM(src, dst, g_ScreenLayoutHigh, nx);
		src += nx;
		dst += 32;
	}
}

//-----------------------------------------------------------------------------
// Fill a rectangle with a given value
void VDP_FillLayout_GM2(u8 value, u8 dx, u8 dy, u8 nx, u8 ny)
{
	u16 dst = g_ScreenLayoutLow + (dy * 32) + dx;
	for (u8 y = 0; y < ny; ++y)
	{
		VDP_FillVRAM(value, dst, g_ScreenLayoutHigh, nx);
		dst += 32;
	}
}

#endif