// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Real-time clock module sample
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"

//=============================================================================
// DEFINES
//=============================================================================

// Font info
struct FontData
{
	const u8* data;
	const c8* name;
};

// Screen mode infp
struct ModeData
{
	const c8* name;
	u8        mode;
};

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts data
#include "font\font_mgl_std0.h"
#include "font\font_mgl_std1.h"
#include "font\font_mgl_std2.h"
#include "font\font_mgl_std3.h"
#include "font\font_mgl_mini2.h"
#include "font\font_mgl_sample6.h"
#include "font\font_tsm9900.h"

// Font list
const struct FontData g_Fonts[] = 
{
	{ NULL,					"Main-ROM system font" },
	{ g_Font_MGL_Std0,		"MGL Standard 0" },
	{ g_Font_MGL_Std1,		"MGL Standard 1" },
	{ g_Font_MGL_Std2,		"MGL Standard 2" },
	{ g_Font_MGL_Std3,		"MGL Standard 3" },
	{ g_Font_MGL_Mini2,		"MGL Mini 2" },
	{ g_Font_MGL_Sample6,	"MGL Sample 6px" },
	{ g_Font_TMS9900,		"TMS 9900" },
};

// Modes data
const struct ModeData g_Modes[] =
{
	{ "Text1 (SC0 W40)", VDP_MODE_TEXT1		},
	{ "Graph1 (SC1)",    VDP_MODE_GRAPHIC1	},
	{ "Graph2 (SC2)",    VDP_MODE_GRAPHIC2	},
#if (MSX_VERSION >= MSX_2)
	{ "Text2 (SC0 W80)", VDP_MODE_TEXT2		},
	{ "Graph3 (SC4)",    VDP_MODE_GRAPHIC3	},
#endif
};

// Sample text
const c8* g_SampleText =
	"(x+7)*42=(10h>>x)^2-3\n\n"
	"\"Nous sommes au 21e siecle ; toute la Gaule est occupee par les PC, Mac, Xbox, Switch et autres Playstation..."
	"Toute ? Non ! Car une association de fanatiques resiste encore et toujours a l'envahisseur, "
	"en proposant un site qui fleure bon les annees 80, "
	"entierement consacre au culte d'un standard fabuleux : le MSX !\"\n"
	"Bienvenue au MSX Village, le site des irreductibles Gaulois du MSX !";

// Animation characters
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

//=============================================================================
// MEMORY DATA
//=============================================================================

u8 g_CurrentFont = 0;
u8 g_CurrentMode = 0;
bool g_DisplayFont = 0;

//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Draw page
void DrawPage()
{
	VDP_SetMode(g_Modes[g_CurrentMode].mode);
	VDP_SetColor(0xF0);
	VDP_FillVRAM_16K(0, 0x0000, 0x4000); // Clear VRAM
	
	Print_SetTextFont(g_Fonts[g_CurrentFont].data, 1);
	Print_SetColor(0xF, 0x0);

	Print_SetPosition(0, 0);
	Print_DrawText("MSXgl - TEXT MODE SAMPLE");
	Print_SetPosition(0, 1);
	Print_DrawCharX('-', g_PrintData.ScreenWidth);

	Print_DrawText("\n\nMode: ");
	Print_DrawText(g_Modes[g_CurrentMode].name);

	Print_DrawText("\nFont: ");
	Print_DrawText(g_Fonts[g_CurrentFont].name);

	if(g_DisplayFont)
	{
		Print_DrawText("\n\n");
		const struct Print_Data* data = Print_GetFontInfo();
		for(u8 j = 0; j < 16; ++j)
		{
			Print_DrawHex8(j * 16);
			Print_DrawText("  ");
			for(u8 i = 0; i < 16; ++i)
			{
				u8 chr = j * 16 + i;
				if((chr >= data->CharFirst) && (chr <= data->CharLast))
					Print_DrawChar(chr);
				else
					Print_Space();
			}
			Print_Return();
		}
	}
	else
	{
		Print_DrawText("\n\nSample: ");
		Print_DrawText(g_SampleText);
	}
	
	Print_SetPosition(0, 23);
	Print_DrawText("<> Scr Mode  ^v Font  F1 Sample");
}

//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	DrawPage();

	u8 count = 0;
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		// Change screen mode
		if(Keyboard_IsKeyPressed(KEY_RIGHT))
		{
			g_CurrentMode++;
			g_CurrentMode %= numberof(g_Modes);
			DrawPage();
		}
		else if(Keyboard_IsKeyPressed(KEY_LEFT))
		{
			g_CurrentMode += numberof(g_Modes) - 1;
			g_CurrentMode %= numberof(g_Modes);
			DrawPage();
		}

		// Change font
		if(Keyboard_IsKeyPressed(KEY_UP))
		{
			g_CurrentFont += numberof(g_Fonts) - 1;
			g_CurrentFont %= numberof(g_Fonts);
			DrawPage();
		}
		else if(Keyboard_IsKeyPressed(KEY_DOWN))
		{
			g_CurrentFont++;
			g_CurrentFont %= numberof(g_Fonts);
			DrawPage();
		}
		if(Keyboard_IsKeyPressed(KEY_F1))
		{
			g_DisplayFont = 1 - g_DisplayFont;
			DrawPage();
		}
		
		// Update character animation
		Print_SetPosition(g_PrintData.ScreenWidth-1, 0);
		Print_DrawChar(g_ChrAnim[count++ & 0x03]);
	}

	Bios_Exit(0);
}