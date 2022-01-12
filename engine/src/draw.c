// ____________________________
// ██▀███▀██▀▀▀▀▀▀▀█▀▀█        │  ▄▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ██ █ ██▄▀ ▄▀██ █ ██
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ██▄▀ ██   ▀▄██ █▀█▀
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘  
//  by Guillaume 'Aoineko' Blanchard under CC-BY-AS license
//─────────────────────────────────────────────────────────────────────────────
#include "draw.h"

#if (MSX_VERSION >= MSX_2)

//-----------------------------------------------------------------------------
/// Draw a line (pixel unit)
void Draw_Line(UX x1, UY y1, UX x2, UY y2, u8 color, u8 op)
{
	u16 dx, dy, nx, ny;
	u8 arg;

	arg = 0;
	if(x1 > x2)
	{
		arg |= VDP_ARG_DIX_LEFT;
		dx = x1 - x2 /*+ 1*/;
	}
	else // (x1 <= x2)
	{
		arg |= VDP_ARG_DIX_RIGHT;
		dx = x2 - x1 /*+ 1*/;
	}
	if(y1 > y2)
	{
		arg |= VDP_ARG_DIY_UP;
		dy = y1 - y2 /*+ 1*/;
	}
	else // (y1 <= y2)
	{
		arg |= VDP_ARG_DIY_DOWN;
		dy = y2 - y1 /*+ 1*/;
	}
	if(dx > dy)
	{
		arg |= VDP_ARG_MAJ_H;
		nx = dx;
		ny = dy;
	}
	else // (dx <= dy)
	{
		arg |= VDP_ARG_MAJ_V;
		nx = dy;
		ny = dx;
	}

	VDP_CommandLINE(x1, y1, nx, ny, color, arg, op);	
}

//-----------------------------------------------------------------------------
/// Draw an horizontal line (pixel unit)
void Draw_LineH(UX x1, UX x2, UY y, u8 color, u8 op)
{
	u16 dx, nx;
	if(x1 > x2)
	{
		dx = x2;
		nx = x1 - x2 + 1;
	}
	else
	{
		dx = x1;
		nx = x2 - x1 + 1;
	}
	VDP_CommandLMMV(dx, y, nx, 1, color, op);	
}

//-----------------------------------------------------------------------------
/// Draw a vertical line (pixel unit)
void Draw_LineV(UX x, UY y1, UY y2, u8 color, u8 op)
{
	u16 dy, ny;
	if(y1 < y2)
	{
		dy = y1;
		ny = y2 - y1 + 1;
	}
	else
	{
		dy = y2;
		ny = y1 - y2 + 1;
	}
	VDP_CommandLMMV(x, dy, 1, ny, color, op);	
}

//-----------------------------------------------------------------------------
/// Draw a box (pixel unit)
void Draw_Box(UX x1, UY y1, UX x2, UY y2, u8 color, u8 op)
{
	Draw_LineH(x1, x2, y1, color, op);
	Draw_LineH(x1, x2, y2, color, op);
	Draw_LineV(x1, y1, y2, color, op);
	Draw_LineV(x2, y1, y2, color, op);
}

//-----------------------------------------------------------------------------
/// Draw a circle (pixel unit)
/// @author		Ibán Cereijo (https://ibancg.github.io/A-fast-circle-algorithm-for-ZX-Spectrum)
void Draw_Circle(UX dx, UY dy, u8 radius, u8 color, u8 op)
{
	i16 x = radius;
	i16 y = 0;
	i16 e = 0;

	while(1)
	{
		// VDP_CommandPSET(dx + x, dy + y, color, op);
		g_VDP_Command.DX = dx + x;
		g_VDP_Command.DY = dy + y;
		g_VDP_Command.CLR = color;
		g_VDP_Command.ARG = 0;
		g_VDP_Command.CMD = VDP_CMD_PSET + op;
		VPD_CommandSetupR36();
		// VDP_CommandPSET(dx + x, dy - y, color, op);
		g_VDP_Command.DY = dy - y;
		VPD_CommandSetupR36();
		// VDP_CommandPSET(dx - x, dy - y, color, op);
		g_VDP_Command.DX = dx - x;
		VPD_CommandSetupR36();
		// VDP_CommandPSET(dx - x, dy + y, color, op);
		g_VDP_Command.DY = dy + y;
		VPD_CommandSetupR36();
		// VDP_CommandPSET(dx + y, dy + x, color, op);
		g_VDP_Command.DX = dx + y;
		g_VDP_Command.DY = dy + x;
		VPD_CommandSetupR36();
		// VDP_CommandPSET(dx + y, dy - x, color, op);
		g_VDP_Command.DY = dy - x;
		VPD_CommandSetupR36();
		// VDP_CommandPSET(dx - y, dy - x, color, op);
		g_VDP_Command.DX = dx - y;
		VPD_CommandSetupR36();
		// VDP_CommandPSET(dx - y, dy + x, color, op);
		g_VDP_Command.DY = dy + x;
		VPD_CommandSetupR36();
		
		if (x <= y)
			break;
		e += (2 * y) + 1;
		y++;
		if (e > x)
		{
			e += 1 - (2 * x);
			x--;
		}
	}	
}

#endif // (MSX_VERSION >= MSX_2)
