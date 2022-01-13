// __________________________
// ██▀█▀██▀▀▀█▀▀█▀█  ▄▄▄ ▄▄  │   ▄▄▄                ▄▄      
// █  ▄ █▄ ▀██▄ ▀▄█ ██   ██  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █▀▀ ▄█  █ █ ▀█▄█ ██▄▄│  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀──────────┘                 ▀▀
//  MGLimg unpack sample
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"

//=============================================================================
// DEFINES
//=============================================================================

///
typedef struct 
{
	const u8* Data;
	u16       Size;

} BitmapData;

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font\font_mgl_std0.h"

// Bitmap
#include "content\8b\data_bmp_8b_no.h"
#include "content\8b\data_bmp_8b_c16.h"
#include "content\8b\data_bmp_8b_cl16.h"
#include "content\8b\data_bmp_8b_c32.h"
#include "content\8b\data_bmp_8b_cl32.h"
#include "content\8b\data_bmp_8b_c256.h"
#include "content\8b\data_bmp_8b_cl256.h"
#include "content\8b\data_bmp_8b_rle0.h"
#include "content\8b\data_bmp_8b_rle4.h"
#include "content\8b\data_bmp_8b_rle8.h"

#include "content\4b\data_bmp_4b_no.h"
#include "content\4b\data_bmp_4b_c16.h"
#include "content\4b\data_bmp_4b_cl16.h"
#include "content\4b\data_bmp_4b_c32.h"
#include "content\4b\data_bmp_4b_cl32.h"
#include "content\4b\data_bmp_4b_c256.h"
#include "content\4b\data_bmp_4b_cl256.h"
#include "content\4b\data_bmp_4b_rle0.h"
#include "content\4b\data_bmp_4b_rle4.h"
#include "content\4b\data_bmp_4b_rle8.h"

#include "content\2b\data_bmp_2b_no.h"
#include "content\2b\data_bmp_2b_c16.h"
#include "content\2b\data_bmp_2b_cl16.h"
#include "content\2b\data_bmp_2b_c32.h"
#include "content\2b\data_bmp_2b_cl32.h"
#include "content\2b\data_bmp_2b_c256.h"
#include "content\2b\data_bmp_2b_cl256.h"
#include "content\2b\data_bmp_2b_rle0.h"
#include "content\2b\data_bmp_2b_rle4.h"
#include "content\2b\data_bmp_2b_rle8.h"

#include "content\1b\data_bmp_1b_no.h"
#include "content\1b\data_bmp_1b_c16.h"
#include "content\1b\data_bmp_1b_cl16.h"
#include "content\1b\data_bmp_1b_c32.h"
#include "content\1b\data_bmp_1b_cl32.h"
#include "content\1b\data_bmp_1b_c256.h"
#include "content\1b\data_bmp_1b_cl256.h"
// #include "content\1b\data_bmp_1b_rle0.h"
// #include "content\1b\data_bmp_1b_rle4.h"
// #include "content\1b\data_bmp_1b_rle8.h"

// Animation characters
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

// 
const c8* g_CompressorName[] = 
{
	"None",
	"Crop16",
	"CropLine16",
	"Crop32",
	"CropLine32",
	"Crop256",
	"CropLinel256",
	"RLE0",
	"RLE4",
	"RLE8",
};

///
const BitmapData g_Data[] = 
{
	{ g_DataBmp8b_no,    sizeof(g_DataBmp1b_no) },
	{ g_DataBmp8b_c16,   sizeof(g_DataBmp8b_c16) },
	{ g_DataBmp8b_cl16,  sizeof(g_DataBmp8b_cl16) },
	{ g_DataBmp8b_c32,   sizeof(g_DataBmp8b_c32) },
	{ g_DataBmp8b_cl32,  sizeof(g_DataBmp8b_cl32) },
	{ g_DataBmp8b_c256,  sizeof(g_DataBmp8b_c256) },
	{ g_DataBmp8b_cl256, sizeof(g_DataBmp8b_cl256) },
	{ g_DataBmp8b_rle0,  sizeof(g_DataBmp8b_rle0) },
	{ g_DataBmp8b_rle4,  sizeof(g_DataBmp8b_rle4) },
	{ g_DataBmp8b_rle8,  sizeof(g_DataBmp8b_rle8) },
};

//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
/// Program entry point
void main()
{
	VDP_SetMode(VDP_MODE_SCREEN5);
	VDP_SetColor(0x4);
	VDP_CommandHMMV(0, 0, 256, 212, 0x44);

	Print_SetBitmapFont(g_Font_MGL_Std0);
	Print_SetPosition(4, 4);
	Print_DrawText("MGL - MGLimg Sample");
	Draw_Box(0, 0, 255, 14, 0x0F, 0);

	u8 count = 0;
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		Print_SetPosition(254-8, 4);
		u8 chr = count++ & 0x03;
		Print_DrawChar(g_ChrAnim[chr]);
	}

	Bios_Exit(0);
}