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

inline u8 VDP_GetMode()
{
	return g_VDP_Data.Mode;
}

//-----------------------------------------------------------------------------
//
// VDP COMMANDS
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// High speed move CPU to VRAM.
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
/// High speed move VRAM to VRAM, Y coordinate only.
/// @param	sy		Y coordinate of the source area
/// @param	dx,dy	X/Y coordinate of the destination area
/// @param	ny		Y size of the area to move
/// @param	dir		Direction of the move (from the destination)
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
/// High speed move VRAM to VRAM
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
/// High speed move VDP to VRAM
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
/// Logical move CPU to VRAM
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
/// Logical move VRAM to CPU
inline void VDP_CommandLMCM()
{
}

//-----------------------------------------------------------------------------
/// Logical move VRAM to VRAM
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
/// Logical move VDP to VRAM
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
/// Draw straight line in VRAM
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
/// Search for the specific color in VRAM to the right or left of the starting point
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
/// Draw a dot in VRAM 
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
/// Read the color of the specified dot located in VRAM 
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
/// Abort current command
inline void VDP_CommandSTOP()
{
	VDP_RegWrite(46, VDP_CMD_STOP);
}