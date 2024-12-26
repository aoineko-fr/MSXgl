// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Print text sample
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"

//=============================================================================
// DEFINES
//=============================================================================

// Font info
struct FontEntry
{
	const c8* Name;
	const u8* Font;
};

// Screen mode info
struct ModeEntry
{
	const c8* Name;
	const u8  Mode;
	const u8* Font;
	const u8  ColorText;
	const u8  ColorBG;
	const u8  ColorGray;
	const u8  ColorAlt;
	const u8  ColorShade[12];
	const u8  ColorSprite[12];
};

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Include font data
#include "font/font_mgl_std0.h"
#include "font/font_mgl_symbol1.h"
#if (TARGET_TYPE != TYPE_BIN)
	#include "font/font_carwar.h"
	#include "font/font_mgl_mini1.h"
	#include "font/font_mgl_big1.h"
	#include "font/font_mgl_curs1b.h"
	#include "font/font_mgl_rune2b.h"
	#include "font/font_mgl_std2.h"
	#include "font/font_mgl_std3.h"
	#include "font/font_mgl_mini2.h"
	#include "font/font_mgl_mini3.h"
	#include "font/font_mgl_neon1b.h"
	#include "font/font_acme.h"
	#include "font/font_darkrose.h"
//	#include "font/font_oxygene.h"
//	#include "font/font_tsm9900.h"
	// #include "font/font_ibm.h"
#endif

// Trigo
#include "mathtable/mt_trigo_32.inc"

// Long sample text
const c8* g_SampleText =
	"\nEquation: (x+7)*42=(10h>>x)^2-3\n"
	"\"Nous sommes au 21e siecle ; toute la Gaule est occupee par les PC, Mac, Xbox, Switch et autres Playstation..."
	"Toute ? Non ! Car une association de fanatiques resiste encore et toujours a l'envahisseur, "
	"en proposant un site qui fleure bon les annees 80, "
	"entierement consacre au culte d'un standard fabuleux : le MSX !\"\n"
	"Bienvenue au MSX Village, le site des irreductibles Gaulois du MSX !";

// Short sample text
const c8* g_SampleTextShort = "Bienvenue au MSX Village, le site des irreductibles Gaulois du MSX !";

// Sprite sample data
const c8* g_SpriteText = "SPRITE!";

// Font entries table
const struct FontEntry g_Fonts[] =
{
	{ "Main-ROM [6*8]",			NULL },
	{ "C-MSX Standard 0 [6*8]",	g_Font_MGL_Std0 },
	{ "C-MSX Symbol 1 [8*8]",	g_Font_MGL_Symbol1 },	
#if (TARGET_TYPE != TYPE_BIN) // No enough free RAM in Basic to load all fonts
	{ "CARWAR [8*8]",			g_Font_Carwar },
	{ "C-MSX Mini 1 [4*6]",		g_Font_MGL_Mini1 },
	{ "C-MSX Big 1 [8*11]",		g_Font_MGL_Big1 },
	{ "C-MSX Cursive 1B [8*8]",	g_Font_MGL_Curs1B },
	{ "C-MSX Rune 2B [8*8]",	g_Font_MGL_Rune2B },
	{ "C-MSX Standard 2 [6*8]",	g_Font_MGL_Std2 },
	{ "C-MSX Standard 3 [6*8]",	g_Font_MGL_Std3 },
	{ "C-MSX Mini 2 [5*6]",		g_Font_MGL_Mini2 },
	{ "C-MSX Mini 3 [5*6]",		g_Font_MGL_Mini3 },
	{ "C-MSX Neon 1B [8*8]",	g_Font_MGL_Neon1B },
	{ "ACME [8*8]",				g_Font_Acme },
	{ "Darkrose [8*8]",			g_Font_Darkrose },
//	{ "OXYGENE [8*8]",			g_Font_Oxygene },
//	{ "TMS9900 [6*8]",			g_Font_TMS9900 },
	// { "IBM VGA [8*8]",			g_Font_IBM },
#endif
};

//
const struct ModeEntry g_Modes[] =
{
	{ "G4", VDP_MODE_GRAPHIC4, g_Font_MGL_Std0, 0xF, 0x0, 0xE, 0xB, { 3, 2, 12, 2, 3, 2, 12, 2, 3, 2, 12, 2 }, { 3, 2, 12, 2, 3, 2, 12, 2, 3, 2, 12, 2 } },
	{ "G5", VDP_MODE_GRAPHIC5, NULL,             0x3, 0x0, 0x2, 0x2, { 3, 3,  2, 2, 3, 3,  2, 2, 3, 3,  2, 2 }, { 15, 15, 10, 10, 15, 15, 10, 10, 15, 15, 10, 10 } },
	{ "G6", VDP_MODE_GRAPHIC6, g_Font_MGL_Std0, 0xF, 0x0, 0xE, 0xB, { 3, 2, 12, 2, 3, 2, 12, 2, 3, 2, 12, 2 }, { 3, 2, 12, 2, 3, 2, 12, 2, 3, 2, 12, 2 } },
	{ "G7",	VDP_MODE_GRAPHIC7, g_Font_MGL_Std0, COLOR8_WHITE, COLOR8_BLACK, RGB8(4,4,2), RGB8(6,6,2), 
		{ COLOR8_DEFAULT3, COLOR8_DEFAULT2, COLOR8_DEFAULT12, COLOR8_DEFAULT2, 
		  COLOR8_DEFAULT3, COLOR8_DEFAULT2, COLOR8_DEFAULT12, COLOR8_DEFAULT2, 
		  COLOR8_DEFAULT3, COLOR8_DEFAULT2, COLOR8_DEFAULT12, COLOR8_DEFAULT2 },
		  { 12, 12, 4, 4, 12, 12, 4, 4, 12, 12, 4, 4 } },
};

// Character animation
const u8 chrAnim[] = { '|', '\\', '-', '/' };

//=============================================================================
// MEMORY DATA
//=============================================================================

u8 g_FontIndex = 0;
u8 g_ModeIndex = 0;
u8 g_StartTime = 0;

//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
///
void PrintHeader()
{
	VDP_SetMode(g_Modes[g_ModeIndex].Mode);
	VDP_SetColor(g_Modes[g_ModeIndex].ColorBG);
	VDP_EnableSprite(FALSE);
	VDP_SetSpriteTables(0x1C000, 0x1CA00);

	Print_Initialize();
	Print_SetColor(0xF, 0x0);
	Print_Clear();
	Print_SetTabSize(16);

	Print_SetColor(g_Modes[g_ModeIndex].ColorText, g_Modes[g_ModeIndex].ColorBG);
	Print_SetFont(g_Modes[g_ModeIndex].Font);
	Print_SetPosition(0, 0);
	Print_DrawText("MSXgl - PRINT SAMPLE (");
	Print_DrawText(g_Modes[g_ModeIndex].Name);
	Print_DrawText(")\n");
	Draw_LineH(0, g_PrintData.ScreenWidth - 1, 12, 0xFF, 0);

	g_StartTime = g_JIFFY;
}

//-----------------------------------------------------------------------------
///
void PrintFooter()
{
	u8 elapsedTime = g_JIFFY - g_StartTime;

	Print_SetColor(g_Modes[g_ModeIndex].ColorGray, g_Modes[g_ModeIndex].ColorBG);
	Print_SetFont(g_Modes[g_ModeIndex].Font);
	Print_SetPosition(0, 204);
	Print_DrawText("F1:List F2:Text F3:FX ");
	Print_SetFont(g_Font_MGL_Symbol1);
	Print_DrawChar(61);
	Print_SetFont(g_Modes[g_ModeIndex].Font);
	Print_DrawText(" Font ");
	Print_SetFont(g_Font_MGL_Symbol1);
	Print_DrawChar(60);
	Print_SetFont(g_Modes[g_ModeIndex].Font);
	Print_DrawText(" Mode  #");
	Print_DrawInt(elapsedTime);
}

//-----------------------------------------------------------------------------
///
void PrintList()
{
	PrintHeader();

	Print_SetPosition(0, 24);
	Print_DrawText("List:\n\n");
	for(u8 i = 0; i < numberof(g_Fonts); ++i)
	{
		Print_SetFont(g_Fonts[i].Font);
		Print_SetColor((i == g_FontIndex) ? g_Modes[g_ModeIndex].ColorAlt : g_Modes[g_ModeIndex].ColorText, g_Modes[g_ModeIndex].ColorBG);
		Print_DrawText("\t");
		Print_DrawText(g_Fonts[i].Name);
		Print_DrawText("\n");
	}

	PrintFooter();
}

//-----------------------------------------------------------------------------
///
void PrintSample()
{
	PrintHeader();

	Print_SetPosition(0, 24);
	Print_DrawText("Font: ");
	Print_DrawText(g_Fonts[g_FontIndex].Name);
	Print_SetFont(g_Fonts[g_FontIndex].Font);

	Print_SetPosition(0, 40);
	Print_SetColor(g_Modes[g_ModeIndex].ColorAlt, g_Modes[g_ModeIndex].ColorBG);
	const struct Print_Data* data = Print_GetFontInfo();
	for(u16 i = data->CharFirst; i <= data->CharLast; ++i)
		Print_DrawChar(i);

	Print_Return();
	Print_SetColor(g_Modes[g_ModeIndex].ColorText, g_Modes[g_ModeIndex].ColorBG);
	Print_DrawText(g_SampleText);

	PrintFooter();
}

//-----------------------------------------------------------------------------
///
void PrintEffect()
{
	PrintHeader();

	Print_SetPosition(0, 24);
	Print_DrawText("Font: ");
	Print_DrawText(g_Fonts[g_FontIndex].Name);

	Print_SetFont(g_Fonts[g_FontIndex].Font);

	//-------------------------------------------------------------------------
	// Multi-color
	Print_SetPosition(0, 40);
	Print_SetColor(g_Modes[g_ModeIndex].ColorText, g_Modes[g_ModeIndex].ColorBG);
	Print_DrawText("Multi-color: ");
#if (PRINT_COLOR_NUM > 1)
	Print_SetColorShade(g_Modes[g_ModeIndex].ColorShade);
	Print_DrawText(g_SampleTextShort);
#else
	Print_DrawText("Disabled! (see config)");
#endif
	Print_Return();
	Print_Return();

	//-------------------------------------------------------------------------
	// Shadow
	Print_SetColor(g_Modes[g_ModeIndex].ColorText, g_Modes[g_ModeIndex].ColorBG);
	Print_DrawText("Shadow: ");
#if (PRINT_USE_FX_SHADOW)
	#if (PRINT_USE_2_PASS_FX)
	Print_DrawTextShadow(g_SampleTextShort, 1, 1, g_Modes[g_ModeIndex].ColorGray);
	#else
	Print_SetShadow(TRUE, 1, 1, g_Modes[g_ModeIndex].ColorGray);
	Print_DrawText(g_SampleTextShort);
	Print_EnableShadow(FALSE);
	#endif
#else
	Print_DrawText("Disabled! (see config)");
#endif
	Print_Return();
	Print_Return();

	//-------------------------------------------------------------------------
	// Outine
	Print_SetColor(g_Modes[g_ModeIndex].ColorText, g_Modes[g_ModeIndex].ColorBG);
	Print_DrawText("Outline: ");
#if (PRINT_USE_FX_OUTLINE)
	#if (PRINT_USE_2_PASS_FX)
	Print_DrawTextOutline(g_SampleTextShort, g_Modes[g_ModeIndex].ColorGray);
	#else
	Print_SetOutline(TRUE, g_Modes[g_ModeIndex].ColorGray);
	Print_DrawText(g_SampleTextShort);
	Print_EnableOutline(FALSE);
	#endif
#else
	Print_DrawText("Disabled! (see config)");
#endif
	Print_Return();
	Print_Return();

	//-------------------------------------------------------------------------
	// VRAM font
	Print_SetColor(g_Modes[g_ModeIndex].ColorText, g_Modes[g_ModeIndex].ColorBG);
	Print_DrawText("VRAM: ");
#if (PRINT_USE_VRAM)
	Print_DrawText("Loading...");
	Print_SetVRAMFont(g_Fonts[g_FontIndex].Font, 212, g_Modes[g_ModeIndex].ColorAlt);
	Print_Backspace(String_Length("Loading..."));
	Print_DrawText(g_SampleTextShort);
#else
	Print_DrawText("Disabled! (see config)");
#endif
	Print_Return();
	Print_Return();

	//-------------------------------------------------------------------------
	// Sprite font
	Print_SetMode(PRINT_MODE_BITMAP);
	Print_SetColor(g_Modes[g_ModeIndex].ColorText, g_Modes[g_ModeIndex].ColorBG);

	Print_DrawText("Sprite: ");
#if (PRINT_USE_SPRITE)
	VDP_EnableSprite(TRUE);
	VDP_SetSpriteTables(0x1C000, 0x1CA00);
	VDP_SetSpriteFlag(VDP_SPRITE_SCALE_2);

	Print_SetSpriteFont(g_Fonts[g_FontIndex].Font, 0, 0);
	#if (PRINT_COLOR_NUM > 1)
		Print_SetColorShade(g_Modes[g_ModeIndex].ColorSprite);
	#else
		Print_SetColor(g_Modes[g_ModeIndex].ColorSprite[0], 0);
	#endif	
	Print_SetCharSize(g_PrintData.UnitX*2, g_PrintData.UnitY*2);
	Print_SetPosition(64, 180);
	Print_DrawText(g_SpriteText);
#else
	Print_DrawText("Disabled! (see config)");
#endif

	Print_SetMode(PRINT_MODE_BITMAP);
	PrintFooter();
}

//-----------------------------------------------------------------------------
///
void PrintBenchmark()
{
	PrintHeader();

	Print_SetPosition(0, 24);
	Print_DrawText("Font: ");
	Print_DrawText(g_Fonts[g_FontIndex].Name);
	
#if (PRINT_USE_VRAM)
	////////////////////////
    static const char text1[]="BONJOUR LE FUTUR\n";
    static const char text2[]="ICI LE MSX QUI VOUS PARLE\n";
    static const char text3[]="DEPUIS L'ANNEE 1985\n";
    static const char text4[]="IL PARAIT QUE LES VIEUX\n";
    static const char text5[]="JOUENT ENCORE AVEC MOI\n";
    static const char text6[]="DANS LES ANNEES 2020 ?\n";
    static const char text7[]="Incroyable !\n";

	Print_DrawText("\n\nLoading...");
	Print_SetColor(g_Modes[g_ModeIndex].ColorAlt, g_Modes[g_ModeIndex].ColorBG);
	Print_SetVRAMFont(g_Fonts[g_FontIndex].Font, 212, g_Modes[g_ModeIndex].ColorAlt);
	Print_Backspace(String_Length("Loading..."));
	u8 startTime = g_JIFFY;
	Print_DrawText(text1);
	Print_DrawText(text2);
	Print_DrawText(text3);
	Print_DrawText(text4);
	Print_DrawText(text5);
	Print_DrawText(text6);
	Print_DrawText(text7);
	u8 elapsedTime = g_JIFFY - startTime;
	Print_DrawInt(elapsedTime);
	////////////////////////
#endif
	
	Print_SetMode(PRINT_MODE_BITMAP);
	PrintFooter();
}

//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
/// Program entry point
void main()
{
	callback cb = PrintList;
	cb();

	u8 count = 0;
	bool bContinue = TRUE;
	while(bContinue)
	{
		if(Keyboard_IsKeyPressed(KEY_ESC))
			bContinue = FALSE;

		u8 row = Keyboard_Read(KEY_ROW(KEY_F1));
		
		if(IS_KEY_PRESSED(row, KEY_F1))
		{
			cb = PrintList;
			cb();
		}
		else if(IS_KEY_PRESSED(row, KEY_F2))
		{
			cb = PrintSample;
			cb();
		}
		else if(IS_KEY_PRESSED(row, KEY_F3))
		{
			cb = PrintEffect;
			cb();
		}
		else if(IS_KEY_PRESSED(row, KEY_CODE))
		{
			cb = PrintBenchmark;
			cb();
		}		

		row = Keyboard_Read(KEY_ROW(KEY_RIGHT));

		if(IS_KEY_PRESSED(row, KEY_RIGHT))
		{
			if(g_FontIndex < numberof(g_Fonts) - 1)
				g_FontIndex++;
			else
				g_FontIndex = 0;			
			cb();
		}
		else if(IS_KEY_PRESSED(row, KEY_LEFT))
		{
			if(g_FontIndex > 0)
				g_FontIndex--;
			else
				g_FontIndex = numberof(g_Fonts) - 1;			
			cb();
		}
		if(IS_KEY_PRESSED(row, KEY_UP))
		{
			if(g_ModeIndex < numberof(g_Modes) - 1)
				g_ModeIndex++;
			else
				g_ModeIndex = 0;			
			cb();
		}
		else if(IS_KEY_PRESSED(row, KEY_DOWN))
		{
			if(g_ModeIndex > 0)
				g_ModeIndex--;
			else
				g_ModeIndex = numberof(g_Modes) - 1;			
			cb();
		}

		Print_SetPosition(g_PrintData.ScreenWidth - 8, 0);
		Print_DrawChar(chrAnim[count & 0x03]);

		#if (PRINT_USE_SPRITE)
		if(cb == PrintEffect)
		{
			for(u8 i = 0; i < String_Length(g_SpriteText); ++i)
			{
				VDP_SetSpritePositionY(i, 155 + ((i16)(g_Sinus32[((count >> 2) + i) % 32]) >> 8));
			}
		}
		#endif

		count++;
		
		// Halt();
	}

	Bios_Exit(0);
}