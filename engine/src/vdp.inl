//_____________________________________________________________________________
//   ▄▄   ▄ ▄  ▄▄▄ ▄▄ ▄                                                        
//  ██ ▀ ██▀█ ▀█▄  ▀█▄▀                                                        
//  ▀█▄▀ ██ █ ▄▄█▀ ██ █                                                        
//_____________________________________________________________________________
/**
 * @file Features to manage the VDP - Inline functions
 *
 * References:
 * - V9938 Technical Data Book Programmer's Guide
 * - https://www.msx.org/wiki/Category:VDP_Registers
 * - http://map.grauw.nl/articles/
 */

//-----------------------------------------------------------------------------
//
// VDP COMMANDS
//
//-----------------------------------------------------------------------------

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
	g_VDP_Command.DX = dx;
	g_VDP_Command.DY = dy;
	g_VDP_Command.NX = nx;
	g_VDP_Command.NY = ny;
	g_VDP_Command.CLR = *addr;
	g_VDP_Command.ARG = 0; 
	g_VDP_Command.CMD = VDP_CMD_HMMC;
	VPD_CommandSetupR36();
	VPD_CommandWriteLoop(addr);
}

//-----------------------------------------------------------------------------
// Function: VDP_CommandYMMM
// High speed move VRAM to VRAM, Y coordinate only. [MSX2/2+/TR]
//
// Parameters:
//   sy - Y coordinate of the source area
//   dx - X coordinate of the destination area (must be multiple of 2 in GM4 and GM6 mode, and multiple of 4 in GM5)
//   dy - Y coordinate of the destination area
//   ny - Height of the area to move
//   dir - Direction of the move (from the destination)
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
// Function: VDP_CommandHMMM
// High speed move VRAM to VRAM. [MSX2/2+/TR]
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
	g_VDP_Command.SX = sx;
	g_VDP_Command.SY = sy;
	g_VDP_Command.DX = dx;
	g_VDP_Command.DY = dy;
	g_VDP_Command.NX = nx;
	g_VDP_Command.NY = ny;
	g_VDP_Command.ARG = 0; 
	g_VDP_Command.CMD = VDP_CMD_HMMM;
	VPD_CommandSetupR32();
}

//-----------------------------------------------------------------------------
// Function: VDP_CommandHMMV
// High speed move VDP to VRAM. [MSX2/2+/TR]
//
// Parameters:
//   dx   - X coordinate of the destination area (must be multiple of 2 in GM4 and GM6 mode, and multiple of 4 in GM5)
//   dy   - Y coordinate of the destination area
//   nx   - Width of the area (same limitation as DX)
//   ny   - Height of the area
//   col  - Color data
inline void VDP_CommandHMMV(u16 dx, u16 dy, u16 nx, u16 ny, u8 col)
{
	g_VDP_Command.DX = dx; 
	g_VDP_Command.DY = dy; 
	g_VDP_Command.NX = nx; 
	g_VDP_Command.NY = ny; 
	g_VDP_Command.CLR = col; 
	g_VDP_Command.ARG = 0; 
	g_VDP_Command.CMD = VDP_CMD_HMMV;
	VPD_CommandSetupR36();
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
	g_VDP_Command.DX = dx;
	g_VDP_Command.DY = dy;
	g_VDP_Command.NX = nx;
	g_VDP_Command.NY = ny;
	g_VDP_Command.CLR = *addr;
	g_VDP_Command.ARG = 0; 
	g_VDP_Command.CMD = VDP_CMD_LMMC + op;
	VPD_CommandSetupR36();
	VPD_CommandWriteLoop(addr);
}

//-----------------------------------------------------------------------------
// Function: VDP_CommandLMCM
// Logical move VRAM to CPU (not fonctional). [MSX2/2+/TR]
inline void VDP_CommandLMCM()
{
}

//-----------------------------------------------------------------------------
// Function: VDP_CommandLMMM
// Logical move VRAM to VRAM. [MSX2/2+/TR]
//
// Parameters:
//   sx   - X coordinate of the source area
//   sy   - Y coordinate of the source area
//   dx   - X coordinate of the destination area
//   dy   - Y coordinate of the destination area
//   nx   - Width of the area
//   ny   - Height of the area
//   op   - Color blend operation
inline void VDP_CommandLMMM(u16 sx, u16 sy, u16 dx, u16 dy, u16 nx, u16 ny, u8 op)
{
	g_VDP_Command.SX = sx;
	g_VDP_Command.SY = sy;
	g_VDP_Command.DX = dx;
	g_VDP_Command.DY = dy;
	g_VDP_Command.NX = nx;
	g_VDP_Command.NY = ny;
	g_VDP_Command.ARG = 0; 
	g_VDP_Command.CMD = VDP_CMD_LMMM + op;
	VPD_CommandSetupR32();
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
	g_VDP_Command.DX = dx; 
	g_VDP_Command.DY = dy; 
	g_VDP_Command.NX = nx; 
	g_VDP_Command.NY = ny; 
	g_VDP_Command.CLR = col; 
	g_VDP_Command.ARG = 0; 
	g_VDP_Command.CMD = VDP_CMD_LMMV + op;
	VPD_CommandSetupR36();
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