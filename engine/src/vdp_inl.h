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
// Function: VDP_CommandHMMM_Arg
// High speed move VRAM to VRAM. [MSX2/2+/TR]
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
// Logical move VRAM to CPU (not fonctional). [MSX2/2+/TR]
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
/*inline void VDP_CommandCustomR32(struct VDP_Command* data)
{
    Mem_Copy((u8*)data, (u8*)g_VDP_Command, 15);
    VPD_CommandSetupR32();
}*/

//-----------------------------------------------------------------------------
// Function: VDP_CommandCustomR36
// Set a command from a given buffer starting mapped from VDP register R36 (11 bytes buffer)
/*inline void VDP_CommandCustomR36(struct VDP_Command36* data)
{
    Mem_Copy((u8*)data, (u8*)g_VDP_Command+4, 11);
    VPD_CommandSetupR36();
}*/