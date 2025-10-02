// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄ ▄ ▄▄▄  ▄▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ ██ █ ██▄▀
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▀█▀  ██▄▀ ██
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────
// Features to manage the VDP - Inline functions
//─────────────────────────────────────────────────────────────────────────────
#pragma once

//-----------------------------------------------------------------------------
//
// VDP COMMANDS
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Function: VDP_CommandHMMC_Arg
// High speed move CPU to VRAM. [MSX2/2+/TR]
inline void VDP_CommandHMMC_Arg(const u8* addr, u16 dx, u16 dy, u16 nx, u16 ny, u8 arg)
{
	g_VDP_Command.DX = dx;
	g_VDP_Command.DY = dy;
	g_VDP_Command.NX = nx;
	g_VDP_Command.NY = ny;
	g_VDP_Command.CLR = *addr;
	g_VDP_Command.ARG = arg; 
	g_VDP_Command.CMD = VDP_CMD_HMMC;
	VPD_CommandSetupR36();
	VPD_CommandWriteLoop(addr);
}

//-----------------------------------------------------------------------------
// Function: VDP_CommandHMMC
// High speed move CPU to VRAM. [MSX2/2+/TR]
//
// Parameters:
//   addr - Address of the source data
//   dx   - X coordinate of the destination area (must be multiple of 2 in GM4 and GM6 mode, and multiple of 4 in GM5)
//   dy   - Y coordinate of the destination area
//   nx   - Width of the destinatio area (same limitation as DX)
//   ny   - Height of the destination area
inline void VDP_CommandHMMC(const u8* addr, u16 dx, u16 dy, u16 nx, u16 ny)
{
	VDP_CommandHMMC_Arg(addr, dx, dy, nx, ny, 0);
}

//-----------------------------------------------------------------------------
// Function: VDP_CommandYMMM
// High speed move VRAM to VRAM, Y coordinate only. [MSX2/2+/TR]
// This function copies data from the coordinates (dx, sy)-(255, sy+ny) 
// to (dx, dy)-(255,dy+ny).
//
// Parameters:
//   sy - Y coordinate of the source area
//   dx - X coordinate of the destination area (must be multiple of 2 in GM4 and GM6 mode, and multiple of 4 in GM5)
//   dy - Y coordinate of the destination area
//   ny - Height of the area to move
//   dir - Direction of the move from the destination:
//                   0 when sy < dy
//                   1 when sy > dy
inline void VDP_CommandYMMM(u16 sy, u16 dx, u16 dy, u16 ny, u8 dir)
{
	g_VDP_Command.SY = sy;
	g_VDP_Command.DX = dx;
	g_VDP_Command.DY = dy;
	g_VDP_Command.NY = ny;
	g_VDP_Command.ARG = dir; 
	g_VDP_Command.CMD = VDP_CMD_YMMM;
	VPD_CommandSetupR32();
}

//-----------------------------------------------------------------------------
// Function: VDP_CommandHMMM_Arg
// High speed move VRAM to VRAM. [MSX2/2+/TR]
//
// This function copies rectangular sections from VRAM to VRAM.
//
// By default, the copy is done from (sx,sy)-(sx+nx,sy+ny) to (dx,dy)-(dx+nx,dy+ny),
// that is, left to right and top to bottom. However, this order can be
// changed by specifying values in the argument register parameter (see below).
// Also, this function by default copies data from and to the standard MSX2/2+/TR
// VRAM addresses, that is, below 128kb. To deal with addresses above that (in
// the rare case of a MSX with 192kb VRAM), there is an argument register value
// for that too.
// 
//
// Parameters:
//   sx   - X coordinate of the source area (must be multiple of 2 in GM4 and GM6 mode, and multiple of 4 in GM5)
//   sy   - Y coordinate of the source area
//   dx   - X coordinate of the destination area (same limitation as SX)
//   dy   - Y coordinate of the destination area
//   nx   - Width of the area (same limitation as SX)
//   ny   - Height of the area
//   arg  - Argument register:
//		VDP_ARG_DIY_DOWN (0)	copies from line sy to line sy + ny
//		VDP_ARG_DIY_UP (4)	copies from line sy - ny to line sy
//		VDP_ARG_DIX_RIGHT (0)	copies from column sx to sx + nx
//		VDP_ARG_DIX_LEFT (8)	copies from column sx - nx to sx
//              16                      copies from extended VRAM (between 128kb and 192kb)
//              32                      copies to extended VRAM (between 128kb and 192kb)
inline void VDP_CommandHMMM_Arg(u16 sx, u16 sy, u16 dx, u16 dy, u16 nx, u16 ny, u8 arg)
{
	g_VDP_Command.SX = sx;
	g_VDP_Command.SY = sy;
	g_VDP_Command.DX = dx;
	g_VDP_Command.DY = dy;
	g_VDP_Command.NX = nx;
	g_VDP_Command.NY = ny;
	g_VDP_Command.ARG = arg; 
	g_VDP_Command.CMD = VDP_CMD_HMMM;
	VPD_CommandSetupR32();
}

//-----------------------------------------------------------------------------
// Function: VDP_CommandHMMM
// High speed move VRAM to VRAM. [MSX2/2+/TR]
//
// This function copies data from the coordinates (sx, sy)-(sx+nx, sy+ny) 
// to (dx, dy)-(dx+nx,dy+ny).
//
// Parameters:
//   sx   - X coordinate of the source area (must be multiple of 2 in GM4 and GM6 mode, and multiple of 4 in GM5)
//   sy   - Y coordinate of the source area
//   dx   - X coordinate of the destination area (same limitation as SX)
//   dy   - Y coordinate of the destination area
//   nx   - Width of the area (same limitation as SX)
//   ny   - Height of the area
inline void VDP_CommandHMMM(u16 sx, u16 sy, u16 dx, u16 dy, u16 nx, u16 ny)
{
	VDP_CommandHMMM_Arg(sx, sy, dx, dy, nx, ny, 0);
}

//-----------------------------------------------------------------------------
// Function: VDP_CommandHMMV_Arg
// High speed move VDP to VRAM. [MSX2/2+/TR]
// 
// This function fills a retangular area with the byte
// col (bear in mind the bit depth of the mode you're using).
//
// By default, the filling is done from (sx,sy)-(sx+nx,sy+ny) to (dx,dy)-(dx+nx,dy+ny),
// that is, left to right and top to bottom. However, this order can be
// changed by specifying values in the argument register parameter (see below).
// Also, this function by default filling data inside standard MSX2/2+/TR
// VRAM addresses, that is, below 128kb. To deal with addresses above that (in
// the rare case of a MSX with 192kb VRAM), there is an argument register value
// for that too;
//
// Parameters:
//   sx   - X coordinate of the source area (must be multiple of 2 in GM4 and GM6 mode, and multiple of 4 in GM5)
//   sy   - Y coordinate of the source area
//   dx   - X coordinate of the destination area (same limitation as SX)
//   dy   - Y coordinate of the destination area
//   nx   - Width of the area (same limitation as SX)
//   ny   - Height of the area
//   col  - Byte to fill the area
//   arg  - Argument register:
//		VDP_ARG_DIY_DOWN (0)	fills from line sy to line sy + ny
//		VDP_ARG_DIY_UP (4)	fills from line sy - ny to line sy
//		VDP_ARG_DIX_RIGHT (0)	fills from column sx to sx + nx
//		VDP_ARG_DIX_LEFT (8)	fills from column sx - nx to sx
//              16                      fills extended VRAM (between 128kb and 192kb)
//
inline void VDP_CommandHMMV_Arg(u16 dx, u16 dy, u16 nx, u16 ny, u8 col, u8 arg)
{
	g_VDP_Command.DX = dx; 
	g_VDP_Command.DY = dy; 
	g_VDP_Command.NX = nx; 
	g_VDP_Command.NY = ny; 
	g_VDP_Command.CLR = col; 
	g_VDP_Command.ARG = arg; 
	g_VDP_Command.CMD = VDP_CMD_HMMV;
	VPD_CommandSetupR36();
}

//-----------------------------------------------------------------------------
// Function: VDP_CommandHMMV
// High speed move VDP to VRAM. [MSX2/2+/TR]
//
// This function fills a retangular area (dx,dy)-(dx+nx,dy+ny) with the byte
// col (bear in mind the bit depth of the mode you're using), from left to
// right, top to bottom.
//
// Parameters:
//   dx   - X coordinate of the destination area (must be multiple of 2 in GM4 and GM6 mode, and multiple of 4 in GM5)
//   dy   - Y coordinate of the destination area
//   nx   - Width of the area (same limitation as DX)
//   ny   - Height of the area
//   col  - Color data
inline void VDP_CommandHMMV(u16 dx, u16 dy, u16 nx, u16 ny, u8 col)
{
	VDP_CommandHMMV_Arg(dx, dy, nx, ny, col, 0);
}

//-----------------------------------------------------------------------------
// Function: VDP_CommandLMMC_Arg
// Logical move CPU to VRAM. [MSX2/2+/TR]
inline void VDP_CommandLMMC_Arg(const u8* addr, u16 dx, u16 dy, u16 nx, u16 ny, u8 op, u8 arg)
{
	g_VDP_Command.DX = dx;
	g_VDP_Command.DY = dy;
	g_VDP_Command.NX = nx;
	g_VDP_Command.NY = ny;
	g_VDP_Command.CLR = *addr;
	g_VDP_Command.ARG = arg; 
	g_VDP_Command.CMD = VDP_CMD_LMMC + op;
	VPD_CommandSetupR36();
	VPD_CommandWriteLoop(addr);
}

//-----------------------------------------------------------------------------
// Function: VDP_CommandLMMC
// Logical move CPU to VRAM. [MSX2/2+/TR]
//
// Parameters:
//   addr - Address of the source data
//   dx   - X coordinate of the destination area
//   dy   - Y coordinate of the destination area
//   nx   - Width of the area
//   ny   - Height of the area
//   op   - Color blend operation
inline void VDP_CommandLMMC(const u8* addr, u16 dx, u16 dy, u16 nx, u16 ny, u8 op)
{
	VDP_CommandLMMC_Arg(addr, dx, dy, nx, ny, op, 0);
}

//-----------------------------------------------------------------------------
// Function: VDP_CommandLMCM
// Logical move VRAM to CPU (not implemented). [MSX2/2+/TR]
inline void VDP_CommandLMCM()
{
}

//-----------------------------------------------------------------------------
// Function: VDP_CommandLMMM_Arg
// Logical move VRAM to VRAM. [MSX2/2+/TR]
inline void VDP_CommandLMMM_Arg(u16 sx, u16 sy, u16 dx, u16 dy, u16 nx, u16 ny, u8 op, u8 arg)
{
	g_VDP_Command.SX = sx;
	g_VDP_Command.SY = sy;
	g_VDP_Command.DX = dx;
	g_VDP_Command.DY = dy;
	g_VDP_Command.NX = nx;
	g_VDP_Command.NY = ny;
	g_VDP_Command.ARG = arg; 
	g_VDP_Command.CMD = VDP_CMD_LMMM + op;
	VPD_CommandSetupR32();
}

//-----------------------------------------------------------------------------
// Function: VDP_CommandLMMM
// Logical move VRAM to VRAM. [MSX2/2+/TR]
//
// This function copies rectangular sections from VRAM to VRAM, applying
// a logical operator between the value on source and the value on destination.
//
// Specifying AND operation means that the final pixel will be the source pixel AND the destination pixel.
// Same goes for OR and XOR.
//
// NOT disregards the pixel at destination, and copies a negated pixel from the source.
//
// All logic operators preceded by a "T" (TAND, TOR, TXOR, TNOT) behaves as the regular versions,
// but they keep the destination pixel untouched if the source pixel is transparent (usually 0)
//
// The TIMP operation copies data from source to value, except transparent ones - useful for
// superimposing two images. IMP copies everything, including the transparent pixels - so
// consider using the faster command VDP_CommandHMMM instead)
//
// Parameters:
//   sx   - X coordinate of the source area
//   sy   - Y coordinate of the source area
//   dx   - X coordinate of the destination area
//   dy   - Y coordinate of the destination area
//   nx   - Width of the area
//   ny   - Height of the area
//   op   - Color blend operation
//		VDP_OP_IMP	(0x00) copies everything (consider using the faster function HMMM)
//		VDP_OP_AND	(0x01) applies an AND operation between source and destination
//		VDP_OP_OR	(0x02) applies an OR operation between source and destination
//		VDP_OP_XOR	(0x03) applies a XOR operation between source and destination
//		VDP_OP_NOT	(0x04) applies a NOT operation in the source pixel
//		VDP_OP_TIMP	(0x08) copies only non-transparent pixels (superimpose)
//		VDP_OP_TAND	(0x09) same as AND, but dont change pixels transparent at the source
//		VDP_OP_TOR	(0x0A) same as OR, but dont change pixels transparent at the source
//		VDP_OP_TXOR	(0x0B) same as XOR, but dont change pixels transparent at the source
//		VDP_OP_TNOT	(0x0C) same as NOT, but dont change pixels transparent at the source
inline void VDP_CommandLMMM(u16 sx, u16 sy, u16 dx, u16 dy, u16 nx, u16 ny, u8 op)
{
	VDP_CommandLMMM_Arg(sx, sy, dx, dy, nx, ny, op, 0);
}

//-----------------------------------------------------------------------------
// Function: VDP_CommandLMMV_Arg
// Logical move VDP to VRAM. [MSX2/2+/TR]
inline void VDP_CommandLMMV_Arg(u16 dx, u16 dy, u16 nx, u16 ny, u8 col, u8 op, u8 arg)
{
	g_VDP_Command.DX = dx; 
	g_VDP_Command.DY = dy; 
	g_VDP_Command.NX = nx; 
	g_VDP_Command.NY = ny; 
	g_VDP_Command.CLR = col; 
	g_VDP_Command.ARG = arg; 
	g_VDP_Command.CMD = VDP_CMD_LMMV + op;
	VPD_CommandSetupR36();
}

//-----------------------------------------------------------------------------
// Function: VDP_CommandLMMV
// Logical move VDP to VRAM. [MSX2/2+/TR]
//
// Parameters:
//   dx   - X coordinate of the destination area
//   dy   - Y coordinate of the destination area
//   nx   - Width of the area
//   ny   - Height of the area
//   col  - Color data
//   op   - Color blend operation
inline void VDP_CommandLMMV(u16 dx, u16 dy, u16 nx, u16 ny, u8 col, u8 op)
{
	VDP_CommandLMMV_Arg(dx, dy, nx, ny, col, op, 0);
}

//-----------------------------------------------------------------------------
// Function: VDP_CommandLINE
// Draw straight line in VRAM. [MSX2/2+/TR]
//
// Parameters:
//   dx   - X coordinate of the destination area
//   dy   - Y coordinate of the destination area
//   nx   - Width of the area
//   ny   - Height of the area
//   col  - Color data
//   arg  - Command argument
//   op   - Color blend operation
inline void VDP_CommandLINE(u16 dx, u16 dy, u16 nx, u16 ny, u8 col, u8 arg, u8 op)
{
	g_VDP_Command.DX = dx;
	g_VDP_Command.DY = dy;
	g_VDP_Command.NX = nx;
	g_VDP_Command.NY = ny;
	g_VDP_Command.CLR = col;
	g_VDP_Command.ARG = arg;
	g_VDP_Command.CMD = VDP_CMD_LINE + op;
	VPD_CommandSetupR36();
}

//-----------------------------------------------------------------------------
// Function: VDP_CommandSRCH
// Search for the specific color in VRAM to the right or left of the starting point (not fonctional). [MSX2/2+/TR]
//
// Parameters:
//   sx   - X coordinate of the source area
//   sy   - Y coordinate of the source area
//   col  - Color data
//   arg  - Command argument
inline void VDP_CommandSRCH(u16 sx, u16 sy, u8 col, u8 arg)
{
	g_VDP_Command.SX = sx;
	g_VDP_Command.SY = sy;
	g_VDP_Command.CLR = col; 
	g_VDP_Command.ARG = arg; 
	g_VDP_Command.CMD = VDP_CMD_SRCH;
	VPD_CommandSetupR32();
}

//-----------------------------------------------------------------------------
// Function: VDP_CommandPSET
// Draw a dot in VRAM. [MSX2/2+/TR]
//
// Parameters:
//   dx   - X coordinate of the destination area
//   dy   - Y coordinate of the destination area
//   col  - Color data
//   op   - Color blend operation
inline void VDP_CommandPSET(u16 dx, u16 dy, u8 col, u8 op)
{
	g_VDP_Command.DX = dx;
	g_VDP_Command.DY = dy;
	g_VDP_Command.CLR = col;
	g_VDP_Command.ARG = 0;
	g_VDP_Command.CMD = VDP_CMD_PSET + op;
	VPD_CommandSetupR36();
}

//-----------------------------------------------------------------------------
// Function: VDP_CommandPOINT
// Read the color of the specified dot located in VRAM. [MSX2/2+/TR]
//
// Parameters:
//   sx   - X coordinate of the source area
//   sy   - Y coordinate of the source area
inline u8 VDP_CommandPOINT(u16 sx, u16 sy)
{
	g_VDP_Command.SX = sx;
	g_VDP_Command.SY = sy;
	g_VDP_Command.CMD = VDP_CMD_POINT;
	VPD_CommandSetupR32();
	VDP_CommandWait();
	return VDP_ReadStatus(7);
}

//-----------------------------------------------------------------------------
// Function: VDP_CommandSTOP
// Abort the current command. [MSX2/2+/TR]
inline void VDP_CommandSTOP()
{
	VDP_RegWrite(46, VDP_CMD_STOP);
}

//-----------------------------------------------------------------------------
// Function: VDP_CommandCustomR32
// Set a command from a given buffer starting mapped from VDP register R32 (15 bytes buffer)
/*inline void VDP_CommandCustomR32(VDP_Command* data)
{
    Mem_Copy((u8*)data, (u8*)g_VDP_Command, 15);
    VPD_CommandSetupR32();
}*/

//-----------------------------------------------------------------------------
// Function: VDP_CommandCustomR36
// Set a command from a given buffer starting mapped from VDP register R36 (11 bytes buffer)
/*inline void VDP_CommandCustomR36(VDP_Command36* data)
{
    Mem_Copy((u8*)data, (u8*)g_VDP_Command+4, 11);
    VPD_CommandSetupR36();
}*/