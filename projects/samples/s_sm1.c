// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Sprite mode 1 sample
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"

//=============================================================================
// DEFINES
//=============================================================================

// VRAM Tables Address
#define VRAM_LAYOUT_TABLE			0x3800
#define VRAM_COLOR_TABLE			0x2000
#define VRAM_PATTERN_TABLE			0x0000
#define VRAM_SPRITE_ATTRIBUTE_0		0x3E00
#define VRAM_SPRITE_ATTRIBUTE_1		0x3F00
#define VRAM_SPRITE_PATTERN			0x1800

// Pattern enum
#define PATTERN_8_NUM			(u8)32
#define SPRITE_8_NUM			(u8)32

// SAT MODE
enum SAT_MODE
{
	SAT_STATIC = 0,
	SAT_FLIP,
	SAT_DOUBLE,
};

// Vector structure
struct Vector
{
	i8 x, y;
};

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font/font_mgl_sample8.h"
#include "font/font_mgl_symbol1.h"

// Sprite movement
const struct Vector s_DirMove[] = 
{
	{ (i8)1,  (i8)1 },
	{ (i8)1,  (i8)0 },
	{ (i8)1,  (i8)-1 },
	{ (i8)0,  (i8)-1 },
	{ (i8)-1, (i8)-1 },
	{ (i8)-1, (i8)0 },
	{ (i8)-1, (i8)1 },
	{ (i8)0,  (i8)1 },
};

// Sprite Attribute Table
const u16 s_AddrSAT[] = { VRAM_SPRITE_ATTRIBUTE_0, VRAM_SPRITE_ATTRIBUTE_1 };

//=============================================================================
// MEMORY DATA
//=============================================================================

// Screen mode setting index
u8 g_VBlank = 0;
u8 g_Frame = 0;
u8 g_Size = 0;
u8 g_Scale = 0;
u8 g_SATMode = SAT_STATIC;
u8 g_Move = 0;
u8 g_Count = SPRITE_8_NUM;

// Sprite data
struct VDP_Sprite g_Sprite[32];

//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// H_TIMI interrupt hook
void VBlankHook()
{
	g_VBlank = 1;
}

//-----------------------------------------------------------------------------
// Wait for V-Blank period
void WaitVBlank()
{
	while(g_VBlank == 0) {}
	g_VBlank = 0;
	g_Frame++;
}

//-----------------------------------------------------------------------------
//
void UpdateSpriteFlag()
{
	u8 flag = 0;
	
	Print_SetPosition(10, 19);
	if(g_Size)
	{
		flag |= VDP_SPRITE_SIZE_16;
		Print_DrawText("16px");
	}
	else
		Print_DrawText("8px ");
	
	Print_SetPosition(10, 20);
	if(g_Scale)
	{
		flag |= VDP_SPRITE_SCALE_2;
		Print_DrawText("x2");
	}
	else
		Print_DrawText("x1");
	
	VDP_SetSpriteFlag(flag);
}

//-----------------------------------------------------------------------------
//
void UpdateFlipSAT(u8 mode)
{
	g_SATMode = mode;
	Print_SetPosition(10, 21);
	switch(g_SATMode)
	{
	case SAT_STATIC: Print_DrawText("Static"); break;
	case SAT_FLIP:   Print_DrawText("Flip  "); break;
	case SAT_DOUBLE: Print_DrawText("Double"); break;
	}
	
	VDP_SetSpriteAttributeTable(VRAM_SPRITE_ATTRIBUTE_0);
}

//-----------------------------------------------------------------------------
//
void UpdateMove(u8 move)
{
	g_Move = move;
	Print_SetPosition(10, 22);
	if(g_Move)
		Print_DrawText("\xC");
	else
		Print_DrawText("\xB");
}

//-----------------------------------------------------------------------------
//
void UpdateCount(u8 count)
{
	if(count < 1)
		return;
	if(count > SPRITE_8_NUM)
		return;
	g_Count = count;
	Print_SetPosition(10, 23);
	Print_DrawInt(count);
	Print_DrawText("   ");
}



//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	// Setup screen
	VDP_SetMode(VDP_MODE_GRAPHIC2);
	VDP_SetColor(0x44);
	// Setup VRAM layout
	VDP_SetLayoutTable(VRAM_LAYOUT_TABLE);
	VDP_SetColorTable(VRAM_COLOR_TABLE);
	VDP_SetPatternTable(VRAM_PATTERN_TABLE);
	VDP_SetSpritePatternTable(VRAM_SPRITE_PATTERN);
	VDP_SetSpriteAttributeTable(VRAM_SPRITE_ATTRIBUTE_0);

	VDP_FillVRAM_16K(0, 0x0000, 0x4000);  // Clear 16 KB of VRAM
	
	// Setup print
	Print_SetTextFont(g_Font_MGL_Sample8, 0);
	Print_SetColor(0xF, 0x4);
	Print_SetPosition(0, 0);
	Print_DrawText("\x2\x3\x4\x5 SPRITE MODE 1 (G2)\n");
	Print_DrawLineH(0, 1, 32);
	
	Print_DrawLineH(0, 18, 32);
	Print_SetPosition(0, 19);
	Print_DrawText("F1:Size\n");
	Print_DrawText("F2:Scale\n");
	Print_DrawText("F3:SAT\n");
	Print_DrawText("F4:Move\n");
	Print_DrawText("\x81\x80:Count\n");

	// Load 8x8 sprites
	VDP_LoadSpritePattern(g_Font_MGL_Symbol1 + 4 + (16 * 10 * 8), 0, PATTERN_8_NUM);
	
	// Initialize 8x8 sprites
	for(u8 i = 0; i < SPRITE_8_NUM; ++i)
	{
		u16 rnd = Math_GetRandom16();
		struct VDP_Sprite* sprt = &g_Sprite[i];
		sprt->Y = rnd & 0x7F;
		sprt->X = (rnd >> 7) & 0xFF;
		sprt->Pattern = (rnd >> 4) % PATTERN_8_NUM;
		sprt->Color = ((rnd >> 12) % 11) + 5;

		g_SpriteAtributeLow = VRAM_SPRITE_ATTRIBUTE_0;
		VDP_SetSpriteSM1(i, sprt->X, sprt->Y, sprt->Pattern, sprt->Color);

		g_SpriteAtributeLow = VRAM_SPRITE_ATTRIBUTE_1;
		VDP_SetSpriteSM1(SPRITE_8_NUM - 1 - i, sprt->X, sprt->Y, sprt->Pattern, sprt->Color);
	}
	
	UpdateSpriteFlag();
	UpdateFlipSAT(SAT_STATIC);
	UpdateMove(1);
	UpdateCount(SPRITE_8_NUM);

	Bios_SetHookCallback(H_TIMI, VBlankHook);
	VDP_EnableVBlank(TRUE);

	u8 prevRow6 = 0xFF, prevRow7 = 0xFF, prevRow8 = 0xFF;
	bool bContinue = TRUE;
	while(bContinue)
	{
		// VDP_SetColor(COLOR_LIGHT_BLUE);
		WaitVBlank();
		// VDP_SetColor(COLOR_DARK_BLUE);
		
		// Set current SAT
		u16 vram;
		i8 offset;
		if(g_SATMode == SAT_STATIC)
		{
			vram = VRAM_SPRITE_ATTRIBUTE_0;
			offset = 4;
		}
		else // if(g_SATMode != SAT_STATIC)
		{
			if((g_Frame & 1) == 0)
			{
				VDP_SetSpriteAttributeTable(VRAM_SPRITE_ATTRIBUTE_0);
				vram = VRAM_SPRITE_ATTRIBUTE_0;
				offset = 4;
			}
			else
			{
				VDP_SetSpriteAttributeTable(VRAM_SPRITE_ATTRIBUTE_1);
				vram = VRAM_SPRITE_ATTRIBUTE_1 + ((SPRITE_8_NUM - 1) * 4);
				offset = -4;
			}
		}
		
		// Update sprites attribute
		struct VDP_Sprite* sprt = &g_Sprite[0];
		for(u8 i = 0; i < SPRITE_8_NUM; i++)
		{
			if(g_Move)
			{
				const struct Vector* mov = &s_DirMove[i & 0x7];
				sprt->X = sprt->X + mov->x;
				sprt->Y = (sprt->Y + mov->y) & 0x7F;
			}
			if(i < g_Count)
				VDP_WriteVRAM_16K((const u8*)sprt, vram, 2);
			else
				VDP_FillVRAM_16K(213, vram, 2);

			sprt++;
			vram += offset;
		}
		
		// Handle user input
		u8 row6 = Keyboard_Read(6);
		if(!IS_KEY_PRESSED(prevRow6, KEY_F1) && IS_KEY_PRESSED(row6, KEY_F1))
		{
			g_Size = 1 - g_Size;
			UpdateSpriteFlag();
		}
		if(!IS_KEY_PRESSED(prevRow6, KEY_F2) && IS_KEY_PRESSED(row6, KEY_F2))
		{
			g_Scale = 1 - g_Scale;
			UpdateSpriteFlag();
		}
		if(!IS_KEY_PRESSED(prevRow6, KEY_F3) && IS_KEY_PRESSED(row6, KEY_F3))
		{
			UpdateFlipSAT((g_SATMode + 1) % 2);
		}		
		prevRow6 = row6;
		
		u8 row7 = Keyboard_Read(7);
		if(!IS_KEY_PRESSED(prevRow7, KEY_F4) && IS_KEY_PRESSED(row7, KEY_F4))
		{
			UpdateMove(1 - g_Move);
		}
		if(IS_KEY_PRESSED(prevRow7, KEY_ESC))
			bContinue = FALSE;
		prevRow7 = row7;		

		u8 row8 = Keyboard_Read(8);
		if(IS_KEY_PRESSED(row8, KEY_RIGHT))
		{
			UpdateCount(g_Count + 1);
		}
		if(IS_KEY_PRESSED(row8, KEY_LEFT))
		{
			UpdateCount(g_Count - 1);
		}
		prevRow8 = row8;
	}

	Bios_ClearHook(H_TIMI);
	Bios_Exit(0);
}


