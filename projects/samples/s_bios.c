// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  BIOS sample
//─────────────────────────────────────────────────────────────────────────────

#include "msxgl.h"

//=============================================================================
// READ-ONLY DATA
//=============================================================================

const i8 g_Jump[] = { -4, -3, -2, -1, 0, 1, 2, 3, 4 };

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
//
void Bios_PrintText(const c8* str)
{
	while(*str)
		Bios_TextPrintChar(*str++);
}

//-----------------------------------------------------------------------------
//
void Bios_PrintTextAt(u8 x, u8 y, const c8* str)
{
	Bios_SetCursorPosition(x + 1, y + 1);
	while(*str)
		Bios_TextPrintChar(*str++);
}

//-----------------------------------------------------------------------------
//
void Bios_ChangeWidth(u8 width)
{
	switch(g_SCRMOD)
	{
	case 0: g_LINL40 = width; Bios_ChangeMode(g_SCRMOD); break;
	case 1: g_LINL32 = width; Bios_ChangeMode(g_SCRMOD); break;
	};
}

//-----------------------------------------------------------------------------
//
i8 Clamp(i8 val, i8 min, i8 max)
{
	if(val < min)
		return min;
	if(val > max)
		return max;
	return val;
}

void Bios_ChangeColor2(u8 text, u8 back, u8 border, u8 mode)
{
	text;   // A
	back;   // L
	border; // (SP+4)
	__asm
		ld		iy, #4
		add		iy, sp

		ld  	(M_FORCLR), a
		ld  	a, l
		ld  	(M_BAKCLR), a
		ld  	a, 0(iy)
		ld  	(M_BDRCLR), a
		ld  	a, 1(iy)
		call	R_CHGCLR
	__endasm;
}

//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	Bios_ChangeMode(1);
	Bios_ChangeWidth(32);
	Bios_ChangeColor2(COLOR_LIGHT_GREEN, COLOR_BLACK, COLOR_GRAY, 0);
	Bios_SetKeyClick(FALSE);
	
	Bios_PrintTextAt(0, 0,  "MSXgl - BIOS SAMPLE");
	Bios_PrintTextAt(0, 1,  "--------------------------------");
	Bios_PrintTextAt(0, 21, "--------------------------------");
	Bios_PrintTextAt(0, 23, "<||> Move     [space] Jump");

	bool bContinue = TRUE;
	bool bJump = FALSE;
	u8 jumpFrame = 0;
	u8 line = 0xFF;
	i8 x = 0, y = 20, prevX, prevY;
	while(bContinue)
	{
		prevX = x;
		prevY = y;

		if(Bios_IsKeyPressed(KEY_DEL))
			bContinue = FALSE;
		if(Bios_IsKeyPressed(KEY_RIGHT))
			x++;
		else if(Bios_IsKeyPressed(KEY_LEFT))
			x--;
		
		if(bJump)
		{
			y += g_Jump[jumpFrame++ / 2];
			if(jumpFrame >= numberof(g_Jump) * 2)
				bJump = FALSE;
		}
		else if(Bios_IsKeyPressed(KEY_SPACE))
		{
			Bios_Beep();
			bJump = TRUE;
			jumpFrame = 0;
		}
		
		x = Clamp(x, 0, 39);
		y = Clamp(y, 5, 20);
		Bios_PrintTextAt(prevX, prevY, " ");
		Bios_PrintTextAt(x, y, "#");

		Halt();
	}
	
	Bios_Exit(0);
}