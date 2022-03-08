// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄ ▄ ▄▄▄  ▄▄▄ 
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ ██ █ ██▄▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▀  ██▄▀ ██  
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
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

//-----------------------------------------------------------------------------
// DEFINES
//-----------------------------------------------------------------------------

// VDP Registers Flags

#define F_VDP_REG	0x80	// VDP register write port (bit 7=1 in second write)
#define F_VDP_VRAM	0x00	// VRAM address register (bit 7=0 in second write)
#define F_VDP_WRIT	0x40	// bit 6: read/write access (1=write)
#define F_VDP_READ	0x00	// bit 6: read/write access (0=read)

#define VDP_REG(_r) (F_VDP_REG | _r)


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
u8 g_VDP_STASAV[10];
struct VDP_Data    g_VDP_Data;
struct VDP_Command g_VDP_Command;
struct VDP_Sprite  g_VDP_Sprite;

u16 g_ScreenLayoutLow;			//< Address of the Pattern Layout Table (Name)
u16 g_ScreenColorLow;			//< Address of the Color Table
u16 g_ScreenPatternLow;			//< Address of the Pattern Generator Table
u16 g_SpriteAtributeLow;		//< Address of the Sprite Attribute Table
u16 g_SpritePatternLow;			//< Address of the Sprite Pattern Generator Table
u16 g_SpriteColorLow;			//< Address of the Sprite Color Table
#if (VDP_VRAM_ADDR == VDP_VRAM_ADDR_17)
	u8  g_ScreenLayoutHigh;		//< Address of the Pattern Layout Table (Name)
	u8  g_ScreenColorHigh;		//< Address of the Color Table
	u8  g_ScreenPatternHigh;	//< Address of the Pattern Generator Table
	u8  g_SpriteAtributeHigh;	//< Address of the Sprite Attribute Table
	u8  g_SpritePatternHigh;	//< Address of the Sprite Pattern Generator Table
	u8  g_SpriteColorHigh;		//< Address of the Sprite Color Table
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

#if (VDP_USE_MODE_T1)
//-----------------------------------------------------------------------------
// Data structure to initialize Text 1 screen mode
static const u8 modeT1[] = 
{
	0x00, VDP_REG(0), // Text Mode, No External Video
	0xD0, VDP_REG(1), // 16K, Enable Disp., Disable Int.
	0x02, VDP_REG(2), // Address of Name Table in VRAM = 0800h
	//    VDP_REG(3), // (not used) Color is defined in Reg. 7
	0x00, VDP_REG(4), // Address of Pattern Table in VRAM = 0000h
	//    VDP_REG(5), // (not used)
	//    VDP_REG(6), // (not used)
	0xF5, VDP_REG(7), // White Text on Light Blue Background
};

// Initialize Text 1 screen mode. Use MSX 1 method without incremental register writing.
void VDP_SetModeText1()
{
	// Backup vdp register @todo Can be optimized
	for(u8 i = 0; i < 10; i += 2)
		g_VDP_REGSAV[modeT1[i + 1] & 0x7F] = modeT1[i];

	__asm
		ld		hl, #_modeT1
		ld		bc, #(10 * 0x100 + P_VDP_ADDR) // [num|0x99]
		otir
	__endasm;

	g_ScreenLayoutLow   = 0x0800;
	g_ScreenPatternLow  = 0x0000;

	#if (VDP_VRAM_ADDR == VDP_VRAM_ADDR_17)
		g_ScreenLayoutHigh   = 0x00;
		g_ScreenPatternHigh  = 0x00;
	#endif
}
#endif // VDP_USE_MODE_T1

#if (VDP_USE_MODE_MC)
//-----------------------------------------------------------------------------
// Data structure to initialize Multi Color screen mode
static const u8 modeMC[] = 
{
	0x00, VDP_REG(0), // Multicolor Mode, No External Video
	0xCB, VDP_REG(1), // 16K, Enable Disp., Disable Int., 16x16 Sprites, Mag. On
	0x05, VDP_REG(2), // Address of Name Table in VRAM = 1400h
	//    VDP_REG(3), // (not used)
	0x01, VDP_REG(4), // Address of Pattern Table in VRAM = 0800h
	0x20, VDP_REG(5), // Address of Sprite Attribute Table in VRAM = 1000h
	0x00, VDP_REG(6), // Address of Sprite Pattern Table in VRAM = 0000h
	0x04, VDP_REG(7), // Backdrop Color = Dark Blue
};

// Initialize Multi Color screen mode. Use MSX 1 method without incremental register writing.
void VDP_SetModeMultiColor()
{
	// Backup vdp register @todo Can be optimized
	for(u8 i = 0; i < 14; i += 2)
		g_VDP_REGSAV[modeMC[i + 1] & 0x7F] = modeMC[i];

	__asm
		ld		hl, #_modeMC
		ld		bc, #(14 * 0x100 + P_VDP_ADDR) // [num|0x99]
		otir
	__endasm;

	g_ScreenLayoutLow   = 0x1400;
	g_ScreenPatternLow  = 0x0800;
	g_SpriteAtributeLow = 0x1000;
	g_SpritePatternLow  = 0x0000;

	#if (VDP_VRAM_ADDR == VDP_VRAM_ADDR_17)
		g_ScreenLayoutHigh   = 0x00;
		g_ScreenPatternHigh  = 0x00;
		g_SpriteAtributeHigh = 0x00;
		g_SpritePatternHigh  = 0x00;
	#endif
}
#endif // VDP_USE_MODE_MC

#if (VDP_USE_MODE_G1)
//-----------------------------------------------------------------------------
// Data structure to initialize Graphic 1 screen mode
static const u8 modeG1[] = 
{
	0x00, VDP_REG(0), // Graphics 1 Mode, No External Video
	0xC0, VDP_REG(1), // 16K, Enable display, Disable Int, 8x8 Srpites, Mag. off
	0x05, VDP_REG(2), // Address of Name Table in VRAM = 1400h
	0x80, VDP_REG(3), // Address of Color Table in VRAM = 2000h
	0x01, VDP_REG(4), // Address of Pattern Table in VRAM = 0800h
	0x20, VDP_REG(5), // Address of Sprite Attribute Table in VRAM = 1000h
	0x00, VDP_REG(6), // Address of Sprite Pattern Table in VRAM = 0000h
	0x01, VDP_REG(7), // Backdrop Color = Black
};

// Initialize Graphic 1 screen mode. Use MSX 1 method without incremental register writing.
void VDP_SetModeGraphic1()
{
	// Backup vdp register @todo Can be optimized
	for(u8 i = 0; i < 16; i += 2)
		g_VDP_REGSAV[modeG1[i + 1] & 0x7F] = modeG1[i];

	__asm
		ld		hl, #_modeG1
		ld		bc, #(16 * 0x100 + P_VDP_ADDR) // [num|0x99]
		otir
	__endasm;

	g_ScreenLayoutLow   = 0x1400;
	g_ScreenColorLow    = 0x2000;
	g_ScreenPatternLow  = 0x0800;
	g_SpriteAtributeLow = 0x1000;
	g_SpritePatternLow  = 0x0000;

	#if (VDP_VRAM_ADDR == VDP_VRAM_ADDR_17)
		g_ScreenLayoutHigh   = 0x00;
		g_ScreenColorHigh    = 0x00;
		g_ScreenPatternHigh  = 0x00;
		g_SpriteAtributeHigh = 0x00;
		g_SpritePatternHigh  = 0x00;
	#endif
}
#endif // VDP_USE_MODE_G1

#if (VDP_USE_MODE_G2)
//-----------------------------------------------------------------------------
// Data structure to initialize Graphic 2 screen mode
static const u8 modeG2[] = 
{
	0x02, VDP_REG(0), // Graphics 2 Mode, No External Video
	0xC2, VDP_REG(1), // 16K, Enable Disp., Disable Int., 16x16 Sprites, Mag. Off
	0x0E, VDP_REG(2), // Address of Name Table in VRAM = 3800h
	0xFF, VDP_REG(3), // Address of Color Table in VRAM = 2000h
	0x03, VDP_REG(4), // Address of Pattern Table in VRAM = 0000h
	0x76, VDP_REG(5), // Address of Sprite Attribute Table in VRAM = 3B00h
	0x03, VDP_REG(6), // Address of Sprite Pattern Table in VRAM = 1800h
	0x0F, VDP_REG(7), // Backdrop Color = White
};

// Initialize Graphic 2 screen mode. Use MSX 1 method without incremental register writing.
void VDP_SetModeGraphic2()
{
	// Backup vdp register @todo Can be optimized
	for(u8 i = 0; i < 16; i += 2)
		g_VDP_REGSAV[modeG2[i + 1] & 0x7F] = modeG2[i];

	__asm
		ld		hl, #_modeG2
		ld		bc, #(16 * 0x100 + P_VDP_ADDR) // [num|0x99]
		otir
	__endasm;

	g_ScreenLayoutLow   = 0x3800;
	g_ScreenColorLow    = 0x2000;
	g_ScreenPatternLow  = 0x0000;
	g_SpriteAtributeLow = 0x3B00;
	g_SpritePatternLow  = 0x1800;

	#if (VDP_VRAM_ADDR == VDP_VRAM_ADDR_17)
		g_ScreenLayoutHigh   = 0x00;
		g_ScreenColorHigh    = 0x00;
		g_ScreenPatternHigh  = 0x00;
		g_SpriteAtributeHigh = 0x00;
		g_SpritePatternHigh  = 0x00;
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
	#if ((MSX_VERSION == MSX_1) || (MSX_VERSION == MSX_12))
		VDP_FillVRAM_16K(0, 0x0000, 0x4000);  // Clear 16 KB of VRAM
	#else
		VDP_FillVRAM(0, 0x0000, 0, 0x8000); // Clear VRAM by 32 KB step
		VDP_FillVRAM(0, 0x8000, 0, 0x8000); // Clear VRAM by 32 KB step
		VDP_FillVRAM(0, 0x0000, 1, 0x8000); // Clear VRAM by 32 KB step
		VDP_FillVRAM(0, 0x8000, 1, 0x8000); // Clear VRAM by 32 KB step
	#endif
}

//-----------------------------------------------------------------------------
// Write data from RAM to VRAM (16KB VRAM)
// @param		src			Source data address in RAM
// @param		dest		Destiation address in VRAM (14bits address form 16KB VRAM)
// @param		count		Nomber of byte to copy in VRAM
void VDP_WriteVRAM_16K(const u8* src, u16 dest, u16 count) __sdcccall(0)
{
	#if ((MSX_VERSION > MSX_1) && !(MSX_VERSION == MSX_12))
		VDP_RegWrite(14, 0); // VDP register 14 must be reset to 0
	#endif
	src, dest, count;
	__asm
		push	ix
		ld		ix, #0
		add		ix, sp
		// Setup address register 
		ld		a, 6 (ix)
		di //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// RegPort = (dest & 0xFF)
		ld		a, 7 (ix)
		and		a, #0x3F
		add		a, #F_VDP_WRIT
		ei //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// RegPort = ((dest >> 8) & 0x3F) + F_VDP_WRIT

#if ((MSX_VERSION == MSX_1) || (MSX_VERSION == MSX_12))

		// Setup fast 16-bits loop
		ld		l, 4 (ix)				// source address
		ld		h, 5 (ix)
		ld		e, 8 (ix)				// count
		ld		d, 9 (ix)
		ld		c, #P_VDP_DATA	
		ld		b, e					// number of loops is in DE
		dec		de						// calculate DB value (destroys B, D and E)
		inc		d
		// Fast loop	
	wrt16_loop_start:
		outi							// out(c) ; hl++ ; b--
		jp		nz, wrt16_loop_start
		dec		d
		jp		nz, wrt16_loop_start		// b = 0 = 256

#else // if (MSX_VERSION >= MSX_2)

		// while(count--) DataPort = *src++;
		ld		l, 4 (ix)				// source address
		ld		h, 5 (ix)
		ld		c, #P_VDP_DATA			// data register
		// Handle count LSB
		ld		a, 8 (ix)				// count LSB
		cp		a, #0
		jp		z, wrt16_loop_init		// skip LSB
		ld		b, a					// send (count & 0x00FF) bytes
		otir
		// Handle count MSB
	wrt16_loop_init:
		ld		a, 9 (ix)				// count MSB
	wrt16_loop_start:
		cp		a, #0
		jp		z, wrt16_loop_end			// finished
		ld		b, #0					// send 256 bytes packages
		otir
		dec		a
		jp		wrt16_loop_start

#endif

	wrt16_loop_end:
		pop	ix
	__endasm;
}

//-----------------------------------------------------------------------------
// Fill VRAM area with a given value
// @param		value		Byte value to copy in VRAM
// @param		dest		Destiation address in VRAM (14 bits address form 16 KB VRAM)
// @param		count		Nomber of byte to copy in VRAM
void VDP_FillVRAM_16K(u8 value, u16 dest, u16 count) __sdcccall(0)
{
	#if ((MSX_VERSION > MSX_1) && !(MSX_VERSION == MSX_12))
		VDP_RegWrite(14, 0); // VDP register 14 must be reset to 0
	#endif
	dest, value, count;
	__asm
		push	ix
		ld		ix, #0
		add		ix, sp
		// Setup address register
		ld		a, 5 (ix)
		di //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// RegPort = (dest & 0x00FF);
		ld		a, 6 (ix)
		and		a, #0x3F
		add		a, #F_VDP_WRIT
		ei //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// RegPort = ((dest >> 8) & 0x3F) + F_VDP_WRIT;
		// while(count--) DataPort = value;
		ld		e, 7 (ix)				// count
		ld		d, 8 (ix)
		ld		a, 4 (ix)				// value
		// fast 16-bits loop
		ld		b, e					// number of loops is in DE
		dec		de						// calculate DB value (destroys B, D and E)
		inc		d
	fll16_loop_start:
		out		(P_VDP_DATA), a			// fill
		djnz	fll16_loop_start
		dec		d
		jp		nz, fll16_loop_start

		pop	ix
	__endasm;
}

//-----------------------------------------------------------------------------
// Read data from VRAM to RAM
// @param		src			Source address in VRAM (14bits address form 16KB VRAM)
// @param		dst			Desitation data address in RAM
// @param		count		Nomber of byte to copy from VRAM
void VDP_ReadVRAM_16K(u16 src, u8* dest, u16 count) __sdcccall(0)
{
	#if ((MSX_VERSION > MSX_1) && !(MSX_VERSION == MSX_12))
		VDP_RegWrite(14, 0); // VDP register 14 must be reset to 0
	#endif
	src, dest, count;
	__asm
		push	ix
		ld		ix, #0
		add		ix, sp
		// Setup address register 	
		ld		a, 4 (ix)
		di //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// AddrPort = (srcLow & 0x00FF)
		ld		a, 5 (ix)
		and		a, #0x3F
		add		a, #F_VDP_READ
		ei //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// AddrPort = ((srcLow >> 8) & 0x3F) + F_VDP_READ
		
#if ((MSX_VERSION == MSX_1) || (MSX_VERSION == MSX_12))

		// Setup fast 16-bits loop
		ld		l, 6 (ix)				// source address
		ld		h, 7 (ix)
		ld		e, 8 (ix)				// count
		ld		d, 9 (ix)
		ld		c, #P_VDP_DATA	
		ld		b, e					// number of loops is in DE
		dec		de						// calculate DB value (destroys B, D and E)
		inc		d
		// Fast loop	
	rd16_loop_start:
		ini								// in(c) ; hl++ ; b--
		jp		nz, rd16_loop_start
		dec		d
		jp		nz, rd16_loop_start		// b = 0 = 256
		
#else // if (MSX_VERSION >= MSX_2)

		// while(count--) *src++ = DataPort;
		ld		l, 6 (ix)				// source address
		ld		h, 7 (ix)
		ld		c, #P_VDP_DATA			// data register
		// Handle count LSB
		ld		a, 8 (ix)				// count LSB
		cp		a, #0
		jp		z, rd16_loop_init			// skip LSB
		ld		b, a					// retreive (count & 0x00FF) bytes
		inir		
		// Handle count MSB		
	rd16_loop_init:
		ld		a, 9 (ix)				// count MSB
	rd16_loop_start:
		cp		a, #0
		jp		z, rd16_loop_end			// finished
		ld		b, #0					// retreive 256 bytes packages
		inir
		dec		a
		jp		rd16_loop_start

#endif

	rd16_loop_end:
		pop	ix
	__endasm;
}

//-----------------------------------------------------------------------------
// Read a value from VRAM
u8 VDP_Peek_16K(u16 dest)
{
	#if ((MSX_VERSION > MSX_1) && !(MSX_VERSION == MSX_12))
		VDP_RegWrite(14, 0); // VDP register 14 must be reset to 0
	#endif
	dest; // HL
	__asm
		ld		b, a
		// Setup address register 	
		ld		a, l
		di //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// AddrPort = (srcLow & 0x00FF)
		ld		a, h
		and		a, #0x3F
		add		a, #F_VDP_READ
		out		(P_VDP_ADDR), a			// AddrPort = ((srcLow >> 8) & 0x3F) + F_VDP_READ
		// Write data 	
		ei //~~~~~~~~~~~~~~~~~~~~~~~~~~
		in		a, (P_VDP_DATA)			// value = DataPort
	__endasm;
}

//-----------------------------------------------------------------------------
// Write a value to VRAM
void VDP_Poke_16K(u8 val, u16 dest)
{
	#if ((MSX_VERSION > MSX_1) && !(MSX_VERSION == MSX_12))
		VDP_RegWrite(14, 0); // VDP register 14 must be reset to 0
	#endif
	val;  // A
	dest; // DE
	__asm
		ld		b, a
		// Setup address register 	
		ld		a, e
		di //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// AddrPort = (srcLow & 0x00FF)
		ld		a, d
		and		a, #0x3F
		add		a, #F_VDP_WRIT
		out		(P_VDP_ADDR), a			// AddrPort = ((srcLow >> 8) & 0x3F) + F_VDP_WRIT
		// Write data 	
		ld		a, b
		ei //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_DATA), a			// DataPort = value
	__endasm;
}


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

//-----------------------------------------------------------------------------
// Incremental VDP registers writing
// @param	src		Address of the data to be write into the registers
// @param	count	Number of registers to be write
// @param	reg		First register to be write (will be automaticaly incremented at each write)
void VDP_RegIncWrite(u16 src, u8 count, u8 reg) __sdcccall(0)
{
	src, count, reg;
	
	__asm
		push	ix
		ld		ix, #0
		add		ix, sp

		// Backup VDP registers
		//ld		de, #(_g_VDP_REGSAV + 0) // first reg
		ld		hl, #_g_VDP_REGSAV
		ld		b, #0
		ld		c, 7(ix)				// first register
		add		hl, bc
		ld		d, h
		ld		e, l

		ld		b, #0
		ld		c, 6(ix)				// size
		ld		l, 4(ix)				// source address
		ld		h, 5(ix)
		ldir

		// Setup incremental VDP port writing
		ld		a, 7(ix)				// first register
		di //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a
		ld		a, #VDP_REG(17)
		ei //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a

		// Do incremental VDP port writing
		ld		b, 6(ix)				// size
		ld		c, #P_VDP_IREG
		ld		l, 4(ix)				// source address
		ld		h, 5(ix)
		otir

		pop		ix
	__endasm;
}

#define OUTI_1	\
	ei			\
	outi

#define OUTI_2	\
	OUTI_1		\
	outi

#define OUTI_3	\
	OUTI_2		\
	outi

#define OUTI_4	\
	OUTI_3		\
	outi

#define OUTI_5	\
	OUTI_4		\
	outi

#define OUTI_6	\
	OUTI_5	\
	outi

#define OUTI_7	\
	OUTI_6		\
	outi

#define OUTI_8	\
	OUTI_7		\
	outi

#define OUTI_9	\
	OUTI_8		\
	outi

#define OUTI_10	\
	OUTI_9		\
	outi

//#define OUTI(_x)	OUTI_##_x
#define OUTI(_x)	\
	.rept _x		\
		outi		\
	.endm			\
	ei

// Fast incremental write to VDP register with backup to RAM
#define ASM_REG_WRITE_INC_BK(_addr, _reg, _count)	\
	__asm											\
		ld		hl, #(_##_addr)						\
		ld		de, #(_g_VDP_REGSAV + _reg)			\
		ld		bc, #(_count)						\
		ldir										\
	__endasm;										\
	ASM_REG_WRITE_INC(_addr, _reg, _count)

// Fast incremental write to VDP register
#define ASM_REG_WRITE_INC(_addr, _reg, _count)		\
	__asm											\
		ld		a, #(_reg)							\
		di											\
		out		(P_VDP_ADDR), a						\
		ld		a, #VDP_REG(17)						\
		ei											\
		out		(P_VDP_ADDR), a						\
		ld		hl, #(_##_addr)						\
		ld		c, #P_VDP_IREG						\
		di											\
		OUTI(_count) ; 'ei' included				\
	__endasm

//-----------------------------------------------------------------------------
//
// SCREEN MODE SETTING
//
//-----------------------------------------------------------------------------

#if (VDP_USE_MODE_T2)
//-----------------------------------------------------------------------------
// Data structure to initialize Text 2 screen mode
static const u8 modeT2[] = 
{
	0x04, // R#0
	0x70, // R#1
	0x03, // R#2
	0x27, // R#3
	0x02, // R#4
	0x36, // R#5
	0x07, // R#6
	0xF4, // R#7
	0x08, // R#8
	0x02, // R#9
};
// Initialize Text 2 screen mode. Use MSX 2 incremental VDP registers writing
void VDP_SetModeText2()
{
	ASM_REG_WRITE_INC_BK(modeT2, 0, 10);

	g_ScreenLayoutLow   = 0x0000;
	g_ScreenPatternLow  = 0x1000;
	g_SpriteAtributeLow = 0x1B00;
	g_SpritePatternLow  = 0x3800;

	#if (VDP_VRAM_ADDR == VDP_VRAM_ADDR_17)
		g_ScreenLayoutHigh   = 0x00;
		g_ScreenPatternHigh  = 0x00;
		g_SpriteAtributeHigh = 0x00;
		g_SpritePatternHigh  = 0x00;
	#endif
}
#endif // VDP_USE_MODE_T2

#if (VDP_USE_MODE_G3)
//-----------------------------------------------------------------------------
// Data structure to initialize Graphic 3 screen mode
static const u8 modeG3[] = 
{
	0x04, // R#0
	0x60, // R#1
	0x06, // R#2
	0xFF, // R#3
	0x03, // R#4
	0x3F, // R#5
	0x07, // R#6
	0x04, // R#7
	0x08, // R#8
	0x02, // R#9
};
// Initialize Graphic 3 screen mode. Use MSX 2 incremental VDP registers writing
void VDP_SetModeGraphic3()
{
	ASM_REG_WRITE_INC_BK(modeG3, 0, 10);

	g_ScreenLayoutLow   = 0x1800;
	g_ScreenColorLow    = 0x2000;
	g_ScreenPatternLow  = 0x0000;
	g_SpriteAtributeLow = 0x1E00;
	g_SpritePatternLow  = 0x3800;

	#if (VDP_VRAM_ADDR == VDP_VRAM_ADDR_17)
		g_ScreenLayoutHigh   = 0x00;
		g_ScreenColorHigh    = 0x00;
		g_ScreenPatternHigh  = 0x00;
		g_SpriteAtributeHigh = 0x00;
		g_SpritePatternHigh  = 0x00;
	#endif
}
#endif // VDP_USE_MODE_G3

#if (VDP_USE_MODE_G4)
//-----------------------------------------------------------------------------
// Data structure to initialize Graphic 4 screen mode
static const u8 modeG4[] = 
{
	0x06, // R#0
	0x60, // R#1
	0x1F, // R#2
	0xFF, // R#3
	0x03, // R#4
	0xEF, // R#5
	0x0F, // R#6
	0x04, // R#7
	0x08, // R#8
	0x82, // R#9
};
// Initialize Graphic 4 screen mode. Use MSX 2 incremental VDP registers writing
void VDP_SetModeGraphic4()
{
	ASM_REG_WRITE_INC_BK(modeG4, 0, 10);
}
#endif // VDP_USE_MODE_G4

#if (VDP_USE_MODE_G5)
//-----------------------------------------------------------------------------
// Data structure to initialize Graphic 5 screen mode
static const u8 modeG5[] = 
{
	0x08, // R#0
	0x60, // R#1
	0x1F, // R#2
	0xFF, // R#3
	0x03, // R#4
	0xEF, // R#5
	0x0F, // R#6
	0x04, // R#7
	0x08, // R#8
	0x82, // R#9
};
// Initialize Graphic 5 screen mode. Use MSX 2 incremental VDP registers writing
void VDP_SetModeGraphic5()
{
	ASM_REG_WRITE_INC_BK(modeG5, 0, 10);
}
#endif // VDP_USE_MODE_G5

#if (VDP_USE_MODE_G6)
//-----------------------------------------------------------------------------
// Data structure to initialize Graphic 6 screen mode
static const u8 modeG6[] = 
{
	0x0A, // R#0
	0x60, // R#1
	0x1F, // R#2
	0xFF, // R#3
	0x03, // R#4
	0xF7, // R#5
	0x1E, // R#6
	0x04, // R#7
	0x08, // R#8
	0x82, // R#9
};
// Initialize Graphic 6 screen mode. Use MSX 2 incremental VDP registers writing
void VDP_SetModeGraphic6()
{
	ASM_REG_WRITE_INC_BK(modeG6, 0, 10);
}
#endif // VDP_USE_MODE_G6

#if (VDP_USE_MODE_G7)
//-----------------------------------------------------------------------------
// Data structure to initialize Graphic 7 screen mode
static const u8 modeG7[] = 
{
	0x0E, // R#0
	0x60, // R#1
	0x1F, // R#2
	0xFF, // R#3
	0x03, // R#4
	0xF7, // R#5
	0x1E, // R#6
	0x04, // R#7
	0x08, // R#8
	0x82, // R#9
};
// Initialize Graphic 7 screen mode. Use MSX 2 incremental VDP registers writing
void VDP_SetModeGraphic7()
{
	ASM_REG_WRITE_INC_BK(modeG7, 0, 10);
}
#endif // VDP_USE_MODE_G7

//-----------------------------------------------------------------------------
//
// SETTING FUNCTIONS
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Read a given status register then reset status register to default (0) [MSX2/2+/TR]
// @param		stat		Status register index (0-9)
u8 VDP_ReadStatus(u8 stat) __FASTCALL
{
	stat;
	__asm
//		ld		l, stat					// FastCall
		ld		a, l
		di //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a
		ld		a, #VDP_REG(15)
		out		(P_VDP_ADDR), a
		in		a, (P_VDP_STAT)
		ld		l, a					// return value
		// Reset R#15 to default status register S#0
		xor		a
		out		(P_VDP_ADDR), a
		ld		a, #VDP_REG(15)
		ei //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR),a
	__endasm;
}

//-----------------------------------------------------------------------------
// Enable/disable grayscale [MSX2/2+/TR]
// @param		enable		True to enable, false do disable
void VDP_SetGrayScale(bool enable)
{
	u8 reg = g_VDP_REGSAV[8];
	reg &= ~R08_BW;
	if(enable)
		reg |= R08_BW;
	VDP_RegWriteBak(8, reg);
}

//-----------------------------------------------------------------------------
// Change VDP frequency (50/60 Hz) [MSX2/2+/TR]
// @param		freq		Can be VDP_FREQ_50HZ or VDP_FREQ_60HZ
void VDP_SetFrequency(u8 freq)
{
	u8 reg = g_VDP_REGSAV[9];
	reg &= ~R09_NT;
	reg |= freq;
	VDP_RegWriteBak(9, reg);
}

//-----------------------------------------------------------------------------
// Enable/disable horizontal-blank interruption [MSX2/2+/TR]
// @param		enable		True to enable, false do disable
void VDP_EnableHBlank(bool enable)
{
	u8 reg = g_VDP_REGSAV[0];
	reg &= ~R00_IE1;
	if(enable)
		reg |= R00_IE1;
	VDP_RegWriteBak(0, reg);
}

//-----------------------------------------------------------------------------
// Set the horizontal-blank interruption line (in pixel) [MSX2/2+/TR]
// @param		line		Line number where the interruption will occure
void VDP_SetHBlankLine(u8 line)
{
	VDP_RegWrite(19, line);
}

//-----------------------------------------------------------------------------
// Set the vertical rendeing offset (in pixel) [MSX2/2+/TR]
// @param		offset		Number of lines of offset from the "normal" top of the screen
void VDP_SetVerticalOffset(u8 offset)
{
	VDP_RegWrite(23, offset);
}

//-----------------------------------------------------------------------------
// Adjustment of the display location on the screen [MSX2/2+/TR]
void VDP_SetAdjustOffset(u8 offset)
{
	VDP_CommandWait(); // @todo Check if it's really needed. Cf. https://www.msx.org/wiki/VDP_Display_Registers#Control_Register_18
	VDP_RegWrite(18, offset);
}

//-----------------------------------------------------------------------------
// Set a new color palette from index 1 to 15 [MSX2/2+/TR]
// @param		pal			Address of the palette in RAM (table of 15 * 2 bytes)
// 						Format : [0:5|green:3|0|red:3|0|blue:3]
void VDP_SetPalette(const u8* pal) __FASTCALL
{
	pal;
	__asm
//		ld		hl, pal				// FastCall
		ld		a, #1
		di  //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a
		ld		a, #VDP_REG(16)
		ei  //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a

		ld		c, #P_VDP_PAL
		ld		b, #30
		otir
	__endasm;
}

#if (USE_DEFAULT_PALETTE)
const u16 VDP_DefaulPalette[15] = {
	RGB16(0, 0, 0),
	RGB16(1, 6, 1),
	RGB16(3, 7, 3),
	RGB16(1, 1, 7),
	RGB16(2, 3, 7),
	RGB16(5, 1, 1),
	RGB16(2, 6, 7),
	RGB16(7, 1, 1),
	RGB16(7, 3, 3),
	RGB16(6, 6, 1),
	RGB16(6, 6, 4),
	RGB16(1, 4, 1),
	RGB16(6, 2, 5),
	RGB16(5, 5, 5),
	RGB16(7, 7, 7)
};
void VDP_SetDefaultPalette()
{
	VDP_SetPalette((u8*)VDP_DefaulPalette);
}
#endif // USE_DEFAULT_PALETTE


#if (USE_MSX1_PALETTE)
const u16 VDP_MSX1Palette[15] = {
	RGB16(0, 0, 0),
	RGB16(1, 5, 1),
	RGB16(3, 6, 3),
	RGB16(2, 2, 6),
	RGB16(3, 3, 7),
	RGB16(5, 2, 2),
	RGB16(2, 6, 7),
	RGB16(6, 2, 2),
	RGB16(6, 3, 3),
	RGB16(5, 5, 2),
	RGB16(6, 6, 3),
	RGB16(1, 4, 1),
	RGB16(5, 2, 5),
	RGB16(5, 5, 5),
	RGB16(7, 7, 7)
};
void VDP_SetMSX1Palette()
{
	VDP_SetPalette((u8*)VDP_MSX1Palette);
}
#endif // USE_MSX1_PALETTE

//-----------------------------------------------------------------------------
// Set a given color entry in the palette [MSX2/2+/TR]
// @param		index		Index of the palette entry (0-15)
// @param		color		16 bits color value (@see RGB16 macro)
//							Format : [0:5|green:3|0|red:3|0|blue:3]
void VDP_SetPaletteEntry(u8 index, u16 color)
{
	g_VDP_RegPort = index;
	g_VDP_RegPort = VDP_REG(16);
	g_VDP_PalPort = color & 0x00FF;
	g_VDP_PalPort = color >> 8;
}

//-----------------------------------------------------------------------------
// Set line count for the current screen mode [MSX2/2+/TR]
// @param		lines		Can be VDP_LINE_192 or VDP_LINE_212
void VDP_SetLineCount(u8 lines)
{
	u8 reg = g_VDP_REGSAV[9];
	reg &= ~R09_LN;
	reg |= lines;
	VDP_RegWriteBak(9, reg);
}

//-----------------------------------------------------------------------------
// Enable or disable interlace mode [MSX2/2+/TR]
// @param		enable		True to enable, false do disable
void VDP_SetInterlace(bool enable)
{
	u8 reg = g_VDP_REGSAV[9];
	reg &= ~R09_IL;
	if(enable)
		reg |= R09_IL;
	VDP_RegWriteBak(9, reg);
}

//-----------------------------------------------------------------------------
// Enable automatic page switch on even/odd frames [MSX2/2+/TR]
// @param		enable		True to enable, false do disable
void VDP_SetPageAlternance(bool enable)
{
	u8 reg = g_VDP_REGSAV[9];
	reg &= ~R09_EO;
	if(enable)
		reg |= R09_EO;
	VDP_RegWriteBak(9, reg);
}

//-----------------------------------------------------------------------------
//
// VRAM ACCESS FUNCTIONS
//
//-----------------------------------------------------------------------------

#if (VDP_VRAM_ADDR == VDP_VRAM_ADDR_17)

//-----------------------------------------------------------------------------
// Write data from RAM to VRAM
// @param		src			Source data address in RAM
// @param		destLow		Destiation address in VRAM (16 LSB of 17-bits VRAM address)
// @param		destHigh	Destiation address in VRAM (1 MSB of 17-bits VRAM address)
// @param		count		Nomber of byte to copy in VRAM
void VDP_WriteVRAM(const u8* src, u16 destLow, u8 destHigh, u16 count) __sdcccall(0)
{
	src;      // IX+5 IX+4
	destLow;  // IX+7 IX+6
	destHigh; // IX+8
	count;    // IX+10 IX+9
	__asm
		push	ix
		ld		ix, #0
		add		ix, sp
		// Setup address register 
		ld		a, 8 (ix)
		add		a, a
		add		a, a
		ld		c, a
		ld		a, 7 (ix)
		rlca
		rlca
		and		a, #0x03
		add		a, c
		di //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// RegPort = (page << 2) + (dest >> 14)
		ld		a, #VDP_REG(14)
		out		(P_VDP_REG), a			// RegPort = VDP_REG(14)
		ld		a, 6 (ix)
		out		(P_VDP_ADDR), a			// RegPort = (dest & 0xFF)
		ld		a, 7 (ix)
		and		a, #0x3f
		add		a, #F_VDP_WRIT
		ei //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// RegPort = ((dest >> 8) & 0x3F) + F_VDP_WRIT
		// while(count--) DataPort = *src++;
		ld		l, 4 (ix)				// source address
		ld		h, 5 (ix)
		ld		c, #P_VDP_DATA			// data register
		// Handle count LSB
		ld		a, 9 (ix)				// count LSB
		cp		a, #0
		jp		z, wrt_loop_init		// skip LSB
		ld		b, a					// send (count & 0x00FF) bytes
		otir
		// Handle count MSB
	wrt_loop_init:
		ld		a, 10 (ix)				// count MSB
	wrt_loop_start:
		cp		a, #0
		jp		z, wrt_loop_end			// finished
		ld		b, #0					// send 256 bytes packages
		otir
		dec		a
		jp		wrt_loop_start

	wrt_loop_end:
		pop	ix
	__endasm;
}

//-----------------------------------------------------------------------------
// Fill VRAM area with a given value
// @param		value		Byte value to copy in VRAM
// @param		destLow		Destiation address in VRAM (16 LSB of 17-bits VRAM address)
// @param		destHigh	Destiation address in VRAM (1 MSB of 17-bits VRAM address)
// @param		count		Nomber of byte to copy in VRAM
void VDP_FillVRAM(u8 value, u16 destLow, u8 destHigh, u16 count) __sdcccall(0)
{
	value;		// IX+4
	destLow;	// IX+6 IX+5
	destHigh;	// IX+7
	count;		// IX+9 IX+8
	__asm
		push	ix
		ld		ix, #0
		add		ix, sp
		// Setup address register 
		ld		a, 7 (ix)
		add		a, a
		add		a, a
		ld		c, a
		ld		a, 6 (ix)
		rlca
		rlca
		and		a, #0x03
		add		a, c
		di //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// RegPort = (page << 2) + (dest >> 14);
		ld		a, #VDP_REG(14)
		out		(P_VDP_ADDR), a			// RegPort = VDP_REG(14);
		ld		a, 5 (ix)
		out		(P_VDP_ADDR), a			// RegPort = (dest & 0x00FF);
		ld		a, 6 (ix)
		and		a, #0x3F
		add		a, #F_VDP_WRIT
		ei //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// RegPort = ((dest >> 8) & 0x3F) + F_VDP_WRIT;
		// while(count--) DataPort = value;
		ld		e, 8 (ix)				// count
		ld		d, 9 (ix)
		ld		a, 4 (ix)				// value
		// fast 16-bits loop
		ld		b, e					// number of loops is in DE
		dec		de						// calculate DB value (destroys B, D and E)
		inc		d
	fll_loop_start:
		out		(P_VDP_DATA), a			// fill
		djnz	fll_loop_start
		dec		d
		jp		nz, fll_loop_start

		pop	ix
	__endasm;
}

//-----------------------------------------------------------------------------
// Read data from VRAM to RAM
// @param		srcLow		Source address in VRAM (16 LSB of 17-bits VRAM address)
// @param		srcHigh		Source address in VRAM (1 MSB of 17-bits VRAM address)
// @param		dst			Desitation data address in RAM
// @param		count		Nomber of byte to copy from VRAM
void VDP_ReadVRAM(u16 srcLow, u8 srcHigh, u8* dest, u16 count) __sdcccall(0)
{
	srcLow, srcHigh, dest, count;
	__asm
		push	ix
		ld		ix, #0
		add		ix, sp
		// Setup address register 	
		ld		a, 6 (ix)
		add		a, a
		add		a, a
		ld		c, a
		ld		a, 5 (ix)
		rlca
		rlca
		and		a, #0x03
		add		a, c
		di //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// AddrPort = (srcHigh << 2) + (srcLow >> 14)
		ld		a, #VDP_REG(14)
		out		(P_VDP_REG), a			// RegPort  = (u8)(VDP_REG(14))
		ld		a, 4 (ix)
		out		(P_VDP_ADDR), a			// AddrPort = (srcLow & 0xFF)
		ld		a, 5 (ix)
		and		a, #0x3f
		add		a, #F_VDP_READ
		ei //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a			// AddrPort = ((srcLow >> 8) & 0x3F) + F_VDP_READ
		// while(count--) *src++ = DataPort;
		ld		l, 7 (ix)				// source address
		ld		h, 8 (ix)
		ld		c, #P_VDP_DATA			// data register
		// Handle count LSB
		ld		a, 9 (ix)				// count LSB
		cp		a, #0
		jp		z, rd_loop_init			// skip LSB
		ld		b, a					// retreive (count & 0x00FF) bytes
		inir		
		// Handle count MSB		
	rd_loop_init:
		ld		a, 10 (ix)				// count MSB
	rd_loop_start:
		cp		a, #0
		jp		z, rd_loop_end			// finished
		ld		b, #0					// retreive 256 bytes packages
		inir
		dec		a
		jp		rd_loop_start
		
	rd_loop_end:
		pop	ix
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

#if (MSX_VERSION >= MSX_2)

//-----------------------------------------------------------------------------
// Wait for previous VDP command to be finished
void VDP_CommandWait()
{
	__asm
	wait_vdp_ready:
		// Set current status ragister to S#2
		ld		a, #2
		di //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_REG), a
		ld		a, #VDP_REG(15)
		out		(P_VDP_REG), a
		in		a, (P_VDP_STAT)		// get S#2 value
		rra							// check CE (bit#0)
		// Reset current status register to S#0 before enabling interruptions
		ld		a, #0				// don't use XOR here to keep C flag from RRA alive
		out		(P_VDP_REG), a
		ld		a, #VDP_REG(15)
		ei //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_REG), a
		jp		c, wait_vdp_ready	// CE==1 ? command not finished
	__endasm;
}

//-----------------------------------------------------------------------------
// Send VDP command (form registres 32 to 46)
void VPD_CommandSetupR32()
{
	VDP_CommandWait();
	ASM_REG_WRITE_INC(g_VDP_Command, 32, 15);
}

//-----------------------------------------------------------------------------
// Send VDP command (form registres 36 to 46)
void VPD_CommandSetupR36()
{
	VDP_CommandWait();
	ASM_REG_WRITE_INC(g_VDP_Command + 4, 36, 11);
}

//-----------------------------------------------------------------------------
// Write to VRAM command loop
// @param		addr		The address in RAM from where read data to be copied in VRAM
// 						(First byte of data was sent in the command execution)
void VPD_CommandWriteLoop(const u8* addr) __FASTCALL
{
	addr;
	
	__asm
 //		ld		hl, address			// FastCall
		// Set indirect register write to R#44
		ld  	a, #VDP_REG(44)
		di //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out 	(P_VDP_REG), a
		ld  	a, #VDP_REG(17)
		out 	(P_VDP_REG), a		
		// Set current status ragister to S#2
		ld  	a, #2
		out 	(P_VDP_REG), a
		ld  	a, #VDP_REG(15)
		out 	(P_VDP_REG), a
		// Setup outi loop (value of register B don't matter)
		inc 	hl
		ld		c, #P_VDP_IREG
	write_loop:
		// Read S#2 to check CE flag (no need to check TR (bit#7) while write loop is longer than worse case VDP write duration (~29cc))
		in  	a, (P_VDP_STAT)
		rra							// check CE (bit#0)
		jp		nc, write_finished	// CE==0 ? command finished
		outi						// write a byte from HL to port VDP_IREG
		jp		write_loop

	write_finished:
		// Reset current status register to S#0
		xor 	a	
		out 	(P_VDP_REG), a
		ld  	a, #VDP_REG(15)
		ei //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out 	(P_VDP_REG), a
	__endasm;
}

#endif // (MSX_VERSION >= MSX_2)

//=============================================================================
//
//  █▀▄▀█ █▀ ▀▄▀   ▀█ ▄█▄   █▀▀ █ █ █▄ █ █▀▀ ▀█▀ █ █▀█ █▄ █ █▀
//  █ ▀ █ ▄█ █ █   █▄  ▀    █▀  █▄█ █ ▀█ █▄▄  █  █ █▄█ █ ▀█ ▄█
//
//=============================================================================

#if (MSX_VERSION >= MSX_2P)



#endif // (MSX_VERSION >= MSX_2P)

//=============================================================================
//
//  █▀▀ █▀█ █▀▄▀█ █▀▄▀█ █▀█ █▄ █   █▀▀ █ █ █▄ █ █▀▀ ▀█▀ █ █▀█ █▄ █ █▀
//  █▄▄ █▄█ █ ▀ █ █ ▀ █ █▄█ █ ▀█   █▀  █▄█ █ ▀█ █▄▄  █  █ █▄█ █ ▀█ ▄█
//
//=============================================================================

//-----------------------------------------------------------------------------
// Set the current screen mode
// @param 		mode		The new screen mode to set (@see VDP_MODE enum)
void VDP_SetMode(const u8 mode)
{
	g_VDP_Data.Mode = mode;
	switch(mode)
	{
#if (VDP_USE_MODE_T1)
	// case VDP_MODE_SCREEN0:
	// case VDP_MODE_SCREEN0_W40:
	case VDP_MODE_TEXT1:
		VDP_SetModeText1();
		return;
#endif // VDP_USE_MODE_T1
	
#if (VDP_USE_MODE_MC)
	// case VDP_MODE_SCREEN3:
	case VDP_MODE_MULTICOLOR:
		VDP_SetModeMultiColor();
		return;
#endif // VDP_USE_MODE_MC

#if (VDP_USE_MODE_G1)
	// case VDP_MODE_SCREEN1:
	case VDP_MODE_GRAPHIC1:
		VDP_SetModeGraphic1();
		return;
#endif // VDP_USE_MODE_G1

#if (VDP_USE_MODE_G2)
	// case VDP_MODE_SCREEN2:
	case VDP_MODE_GRAPHIC2:
		VDP_SetModeGraphic2();
		return;
#endif // VDP_USE_MODE_G2

#if (MSX_VERSION >= MSX_2)

#if (VDP_USE_MODE_T2)
	// case VDP_MODE_SCREEN0_W80:
	case VDP_MODE_TEXT2:
		VDP_SetModeText2();
		return;
#endif // VDP_USE_MODE_T2²

#if (VDP_USE_MODE_G3)
	// case VDP_MODE_SCREEN4:
	case VDP_MODE_GRAPHIC3:
		VDP_SetModeGraphic3();
		return;
#endif // VDP_USE_MODE_G3
	
#if (VDP_USE_MODE_G4)
	// case VDP_MODE_SCREEN5:
	case VDP_MODE_GRAPHIC4:
		VDP_SetModeGraphic4();
		return;
#endif // VDP_USE_MODE_G4
	
#if (VDP_USE_MODE_G5)
	// case VDP_MODE_SCREEN6:
	case VDP_MODE_GRAPHIC5:
		VDP_SetModeGraphic5();
		return;
#endif // VDP_USE_MODE_G5
	
#if (VDP_USE_MODE_G6)
	// case VDP_MODE_SCREEN7:
	case VDP_MODE_GRAPHIC6:
		VDP_SetModeGraphic6();
		return;
#endif // VDP_USE_MODE_G6
		
#if (VDP_USE_MODE_G7)
	// case VDP_MODE_SCREEN8:
	case VDP_MODE_GRAPHIC7:
		VDP_SetModeGraphic7();
		return;
#endif // VDP_USE_MODE_G7
		
#if (VDP_USE_MODE_G5)
	// case VDP_MODE_SCREEN9:
	case VDP_MODE_SCREEN9_40:
		VDP_SetModeGraphic5();
		// @todo Further setting needed
		return;
#endif // VDP_USE_MODE_G5
	
#if (VDP_USE_MODE_G4)
	case VDP_MODE_SCREEN9_80:
		VDP_SetModeGraphic4();
		// @todo Further setting needed
		return;
#endif // VDP_USE_MODE_G4

#endif // (MSX_VERSION >= MSX_2)

#if (MSX_VERSION >= MSX_2P)

#if (VDP_USE_MODE_G7)
	case VDP_MODE_SCREEN10:
		VDP_SetModeGraphic7();
		// @todo Further setting needed
		return;

	case VDP_MODE_SCREEN11:
		VDP_SetModeGraphic7();
		// @todo Further setting needed
		return;

	case VDP_MODE_SCREEN12:
		VDP_SetModeGraphic7();
		// @todo Further setting needed
		return;
#endif // VDP_USE_MODE_G7

#endif // (MSX_VERSION >= MSX_2P)
	}
}

//-----------------------------------------------------------------------------
// Tell if the given screen mode is a bitmap mode (text mode otherwise)
bool VDP_IsBitmapMode(const u8 mode)
{
#if (MSX_VERSION == MSX_1)
	return false;
#else
	return mode >= VDP_MODE_GRAPHIC4;
#endif
}

//-----------------------------------------------------------------------------
// Get VDP version
//	@return					0: TMS9918A, 1: V9938, 2: V9958, x: VDP ID
// @note Code by Grauw (http://map.grauw.nl/sources/vdp_detection.php)
u8 VDP_GetVersion() __naked
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
		di
		out		(P_VDP_ADDR), a
		ld		a, #VDP_REG(15)
		out		(P_VDP_ADDR), a
		in		a, (P_VDP_ADDR)			// read s#1
		and		#0b00111110				// get VDP ID
		rrca
		ex		af, af'
		xor		a						// select s#0 as required by BIOS
		out		(P_VDP_ADDR), a
		ld		a, #VDP_REG(15)
		ei
		out		(P_VDP_ADDR), a
		ex		af, af'
		ret		nz						// return VDP ID for V9958 or higher
		inc		a						// return 1 for V9938
		ret

	// Test if the VDP is a TMS9918A.
	//
	// The VDP ID number was only introduced in the V9938, so we have to use a
	// different method to detect the TMS9918A. We wait for the vertical blanking
	// interrupt flag, and then quickly read status register 2 and expect bit 6
	// (VR, vertical retrace flag) to be set as well. The TMS9918A has only one
	// status register, so bit 6 (5S, 5th sprite flag) will return 0 in stead.
	//
	// f <- z: TMS9918A, nz: V99X8
	VDP_IsTMS9918A:
		in		a, (P_VDP_ADDR)			// read s#0, make sure interrupt flag is reset
		di
	VDP_IsTMS9918A_Wait:
		in		a,(P_VDP_ADDR)			// read s#0
		and		a						// wait until interrupt flag is set
		jp		p, VDP_IsTMS9918A_Wait
		ld		a, #2					// select s#2 on V9938
		out		(P_VDP_ADDR), a
		ld		a, #VDP_REG(15)			// (this mirrors to r#7 on TMS9918 VDPs)
		out		(P_VDP_ADDR), a
		in		a, (P_VDP_ADDR)			// read s#2 / s#0
		ex		af, af'
		xor		a						// select s#0 as required by BIOS
		out		(P_VDP_ADDR), a
		ld		a, #VDP_REG(15)
		out		(P_VDP_ADDR), a
		ld		a, (0xF3E6)
		out		(P_VDP_ADDR), a			// restore r#7 if it mirrored (small flash visible)
		ld		a, #VDP_REG(7)
		ei
		out		(P_VDP_ADDR), a
		ex		af, af'
		and		#0b01000000				// check if bit 6 was 0 (s#0 5S) or 1 (s#2 VR)
		ret
	
	__endasm;
}

//-----------------------------------------------------------------------------
// Set register value
void VDP_RegWrite(u8 reg, u8 value)
{
	reg;	// A
	value;	// L

	__asm
		ld		h, a					// Register number
		ld		a, l					// Value
		di //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a
		ld		a, h
		add		#0x80					// @todo Can be optimize by including 80h in the register number ; 33cc -> 25cc (warning for MSX1 VRAM write timing)
		ei //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a
	__endasm;
}

//-----------------------------------------------------------------------------
// Set register value after backuping previous
void VDP_RegWriteBak(u8 reg, u8 value)
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
		di //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a
		ld		a, c
		add		#0x80					// @todo Can be optimize by including 80h in the register number ; 33cc -> 25cc (warning for MSX1 VRAM write timing)
		ei //~~~~~~~~~~~~~~~~~~~~~~~~~~
		out		(P_VDP_ADDR), a
	__endasm;
}

//-----------------------------------------------------------------------------
// Fast write to VDP register
// @todo out-out timing is 25 cc, less than worse case on MSX1 for G1 et G2!
#define ASM_REG_WRITE(_reg, _val)					\
	__asm											\
		ld		a, _val								\
		di											\
		out		(P_VDP_ADDR), a						\
		ld		a, #VDP_REG(_reg)					\
		ei											\
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

//-----------------------------------------------------------------------------
// Read default S#0 register
u8 VDP_ReadDefaultStatus()
{
	__asm
		in		a, (P_VDP_STAT)
		ld		l, a
	__endasm;
}

//-----------------------------------------------------------------------------
// Enable/disable vertical interruption
void VDP_EnableVBlank(bool enable)
{
	u8 reg = g_VDP_REGSAV[1];
	reg &= ~R01_IE0;
	if(enable)
		reg |= R01_IE0;	
	VDP_RegWriteBak(1, reg);
}

//-----------------------------------------------------------------------------
// Enable/disable screen display
void VDP_EnableDisplay(bool enable)
{
	u8 reg = g_VDP_REGSAV[1];
	reg &= ~R01_BL;
	if(enable)
		reg |= R01_BL;	
	VDP_RegWriteBak(1, reg);
}

//-----------------------------------------------------------------------------
// Set current VRAM page
void VDP_SetPage(u8 page)
{
	u8 reg = g_VDP_REGSAV[2];
	reg &= 0x9F;
	reg |= page << 5;
	VDP_RegWriteBak(2, reg);
}

//-----------------------------------------------------------------------------
// Set text and border default color
void VDP_SetColor(u8 color)
{
	VDP_RegWrite(7, color);
}

//-----------------------------------------------------------------------------
// Set layout table VRAM address
void VDP_SetLayoutTable(VADDR addr)
{
	g_ScreenLayoutLow = (u16)addr;

	u8 reg;
	reg = (u8)(addr >> 10);
	#if (MSX_VERSION >= MSX_2)
		switch(g_VDP_Data.Mode)
		{
		case VDP_MODE_TEXT2:
			reg |= 0b11;
			break;
		case VDP_MODE_GRAPHIC6:
		case VDP_MODE_GRAPHIC7:
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
	switch(g_VDP_Data.Mode)
	{
	#if (MSX_VERSION >= MSX_2)
	case VDP_MODE_TEXT2:
		reg |= 0b111;
		break;
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
	switch(g_VDP_Data.Mode)
	{
	#if (MSX_VERSION >= MSX_2)
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

#if (MSX_VERSION >= MSX_2)
//-----------------------------------------------------------------------------
// Enable/disable sprite rendering
void VDP_EnableSprite(u8 enable)
{
	u8 reg = g_VDP_REGSAV[8];
	reg &= ~R08_SPD;
	if(!enable)
		reg |= R08_SPD;
	VDP_RegWriteBak(8, reg);
}
#endif

//-----------------------------------------------------------------------------
// Set sprite parameters
void VDP_SetSpriteFlag(u8 flag)
{
	u8 reg = g_VDP_REGSAV[1];
	reg &= ~(R01_ST | R01_MAG);
	if(flag & R01_ST)
		reg |= R01_ST;
	if(flag & R01_MAG)
		reg |= R01_MAG;
	VDP_RegWriteBak(1, reg);
}

//-----------------------------------------------------------------------------
// Set sprite attribute table address
// @param		addr		VRAM address where to place the table (16 or 17-bits long depending on VDP_VRAM_ADDR definition)
//							Address must be a multiple of 80h for MSX1 screen modes and  multiple of 200h for MSX2 ones.
void VDP_SetSpriteAttributeTable(VADDR addr)
{
	g_SpriteAtributeLow = (u16)addr;
	
	u8 reg;
	reg = (u8)(addr >> 7);
	#if (MSX_VERSION >= MSX_2)
		switch(g_VDP_Data.Mode)
		{
		case VDP_MODE_GRAPHIC3:
		case VDP_MODE_GRAPHIC4:
		case VDP_MODE_GRAPHIC5:
		case VDP_MODE_GRAPHIC6:
		case VDP_MODE_GRAPHIC7:
			reg |= 0b111;
			break;
		};
	#endif
	VDP_RegWrite(5, reg);

	#if (VDP_VRAM_ADDR == VDP_VRAM_ADDR_17)
		reg = (u8)(addr >> 15);
		VDP_RegWrite(11, reg);

		g_SpriteAtributeHigh = addr >> 16;
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
// Set sprite table address (bit#16 to bit#1)
// @param		pattern		1-bit right shifted VRAM address (bit#16 to bit#1)
// @param		attrib		1-bit right shifted VRAM address (bit#16 to bit#1)
void VDP_SetSpriteTables(VADDR pattern, VADDR attrib)
{
	VDP_SetSpritePatternTable(pattern);
	VDP_SetSpriteAttributeTable(attrib);
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

	u16 low = g_SpriteAtributeLow;
	low += (index * 4);
	VDP_WriteVRAM((u8*)&g_VDP_Sprite, low, g_SpriteAtributeHigh, 4);
}

//-----------------------------------------------------------------------------
// Set sprite attribute for Sprite Mode 2
void VDP_SetSprite(u8 index, u8 x, u8 y, u8 shape)
{
	g_VDP_Sprite.Y = y;				// Y coordinate on screen (all lower priority sprite will be disable if equal to 216 or 0xD0)
    g_VDP_Sprite.X = x;				// X coordinate of the sprite
    g_VDP_Sprite.Pattern = shape;	// Pattern index

	u16 low = g_SpriteAtributeLow;
	low += (index * 4);
	VDP_WriteVRAM((u8*)&g_VDP_Sprite, low, g_SpriteAtributeHigh, 3);
}

#if (MSX_VERSION >= MSX_2)
//-----------------------------------------------------------------------------
// Set sprite attribute for Sprite Mode 2 and fill color table with color data
void VDP_SetSpriteExMultiColor(u8 index, u8 x, u8 y, u8 shape, const u8* ram)
{
	u16 col = g_SpriteColorLow;
	col += (index * 16);
	VDP_WriteVRAM(ram, col, g_SpriteColorHigh, 16);

	g_VDP_Sprite.X = x;				// Y coordinate on screen (all lower priority sprite will be disable if equal to 216 or 0xD0)
	g_VDP_Sprite.Y = y;				// X coordinate of the sprite
	g_VDP_Sprite.Pattern = shape;	// Pattern index
	u16 attr = g_SpriteAtributeLow;
	attr += (index * 4);
	VDP_WriteVRAM((u8*)&g_VDP_Sprite, attr, g_SpriteAtributeHigh, 3);
}

//-----------------------------------------------------------------------------
// Set sprite attribute for Sprite Mode 2 and fill color table with unique color
void VDP_SetSpriteExUniColor(u8 index, u8 x, u8 y, u8 shape, u8 color)
{
	u16 col = g_SpriteColorLow;
	col += (index * 16);
	VDP_FillVRAM(color, col, g_SpriteColorHigh, 16);

	g_VDP_Sprite.X = x;				// Y coordinate on screen (all lower priority sprite will be disable if equal to 216 or 0xD0)
	g_VDP_Sprite.Y = y;				// X coordinate of the sprite
	g_VDP_Sprite.Pattern = shape;	// Pattern index
	u16 attr = g_SpriteAtributeLow;
	attr += (index * 4);
	VDP_WriteVRAM((u8*)&g_VDP_Sprite, attr, g_SpriteAtributeHigh, 3);
}
#endif // (MSX_VERSION >= MSX_2)

//-----------------------------------------------------------------------------
// Update sprite position
void VDP_SetSpritePosition(u8 index, u8 x, u8 y)
{
	g_VDP_Sprite.Y = y;				// Y coordinate on screen (all lower priority sprite will be disable if equal to 216 or 0xD0)
    g_VDP_Sprite.X = x;				// X coordinate of the sprite

	u16 low = g_SpriteAtributeLow;
	low += (index * 4);
	VDP_WriteVRAM((u8*)&g_VDP_Sprite, low, g_SpriteAtributeHigh, 2);
}

//-----------------------------------------------------------------------------
// Update sprite position Y
void VDP_SetSpritePositionY(u8 index, u8 y)
{
	g_VDP_Sprite.Y = y;				// Y coordinate on screen (all lower priority sprite will be disable if equal to 216 or 0xD0)

	u16 low = g_SpriteAtributeLow;
	low += (index * 4);
	VDP_WriteVRAM((u8*)&g_VDP_Sprite, low, g_SpriteAtributeHigh, 1);
}

//-----------------------------------------------------------------------------
// Update sprite pattern
void VDP_SetSpritePattern(u8 index, u8 shape)
{
    g_VDP_Sprite.Pattern = shape;	// Pattern index

	u16 low = g_SpriteAtributeLow + 2;
	low += (index * 4);
	VDP_WriteVRAM((u8*)&g_VDP_Sprite.Pattern, low, g_SpriteAtributeHigh, 1);
}

//-----------------------------------------------------------------------------
// Update sprite pattern (Shader mode 1)
void VDP_SetSpriteColorSM1(u8 index, u8 color)
{
    g_VDP_Sprite.Color = color;	// Color index (Sprite Mode 1 only) + Early clock

	u16 low = g_SpriteAtributeLow + 3;
	low += (index * 4);
	VDP_WriteVRAM((u8*)&g_VDP_Sprite.Color, low, g_SpriteAtributeHigh, 1);
}

#if (MSX_VERSION >= MSX_2)
//-----------------------------------------------------------------------------
// Update sprite color (Uni-color)
void VDP_SetSpriteUniColor(u8 index, u8 color)
{
	u16 col = g_SpriteColorLow;
	col += (index * 16);
	VDP_FillVRAM(color, col, g_SpriteColorHigh, 16);
}

//-----------------------------------------------------------------------------
// Update sprite color (Multi-color)
void VDP_SetSpriteMultiColor(u8 index, const u8* ram)
{
	u16 col = g_SpriteColorLow;
	col += (index * 16);
	VDP_WriteVRAM(ram, col, g_SpriteColorHigh, 16);	
}

//-----------------------------------------------------------------------------
//
void VDP_SetSpriteData(u8 index, const u8* data)
{
	u16 low = g_SpriteAtributeLow;
	low += (index * 4);
	VDP_WriteVRAM(data, low, g_SpriteAtributeHigh, 3);
}
#endif // (MSX_VERSION >= MSX_2)

//-----------------------------------------------------------------------------
//
void VDP_DisableSpritesFrom(u8 index)
{
	u8 y = VDP_SPRITE_DISABLE_SM1;
	#if (MSX_VERSION >= MSX_2)
		if(g_VDP_Data.Mode >= VDP_MODE_TEXT2) // MSX2 modes
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
		di
		outi
		outi
		outi
		ei
		outi
	__endasm;
}
*/


//=============================================================================
//
//   G R A P H   M O D E   2
//
//=============================================================================

#if (VDP_USE_MODE_G2 || VDP_USE_MODE_G3)

//-----------------------------------------------------------------------------
// Fill the full screen with a given pattern value
void VDP_FillScreen_GM2(u8 value)
{
	VDP_FillVRAM(value, g_ScreenLayoutLow, g_ScreenLayoutHigh, 32*24);
}

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
	for(u8 y = 0; y < ny; ++y)
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
	for(u8 y = 0; y < ny; ++y)
	{
		VDP_FillVRAM(value, dst, g_ScreenLayoutHigh, nx);
		dst += 32;
	}
}

#endif