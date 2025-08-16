// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Hello world sample
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"
#include "tool/qrcode.h"

//=============================================================================
// DEFINES
//=============================================================================

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font/font_mgl_sample6.h"

// Bitmaps by GrafxKid (https://opengameart.org/content/super-random-sprites)
#include "content/data_bmp_4b.h"

//=============================================================================
// MEMORY DATA
//=============================================================================

u8 g_QR[QRCODE_BUFFER_LEN_MAX];
u8 g_Buffer[QRCODE_BUFFER_LEN_MAX];
u8 g_LMMC4b[16*16];

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
void QRCode_DrawBitmap(u8 x, u8 y, const u8* qrc)
{
	i16 size = QRCode_GetSize(qrc);
	for(i16 j = 0; j < size; j++)
		for(i16 i = 0; i < size; i++) 
			if(QRCode_GetModule(qrc, i, j))
				VDP_CommandPSET(x + i, y + j, COLOR_WHITE, 0);
}

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	VDP_SetMode(VDP_MODE_SCREEN5); // Initialize screen mode 5
	VDP_SetColor(COLOR_BLACK);
	VDP_ClearVRAM();

	Print_SetBitmapFont(g_Font_MGL_Sample6);
	Print_SetColor(0xFF, 0x11);
	Print_Clear();

	Print_SetPosition(0, 0);
	Print_DrawText("\x1\x2\x3\x4\x5\x6 QR CODE SAMPLE");
	Draw_LineH(0, 255, 12, 0xFF, 0);

	Print_SetPosition(0, 16);
	Print_DrawFormat("Version Min:%i, Max:%i, Buf size:%i\n", QRCODE_VERSION_MIN, QRCODE_VERSION_MAX, QRCODE_BUFFER_LEN_MAX);

#if (QRCODE_VERSION_CUSTOM)
	QRCode_SetVersion(1, 20);
#endif

#if (QRCODE_BOOST_ECL == QRCODE_PARAM_CUSTOM)
	QRCode_SetBoostECL(TRUE);
#endif

	Print_SetPosition(0, 32);

	// Text data
	Print_SetColor(0xFF, 0x11);
	Print_DrawText("Encoding...");
	bool ok = QRCode_EncodeText("MSXgl", g_Buffer, g_QR, QRCODE_ECC_MEDIUM, QRCODE_MASK_0);
	Print_DrawFormat("%s\n", ok ? "Ok" : "Error!");
	if(ok)
	{
		Print_SetColor(0xEE, 0x11);
		Print_DrawFormat(" Version:%i, Size:%i\n", QRCode_GetVersion(g_QR), QRCode_GetSize(g_QR));
		Print_DrawText(" Drawing...");
		QRCode_DrawBitmap(150, 32, g_QR);
		Print_DrawText("Ok\n");
	}

	// Text data
	Print_SetColor(0xFF, 0x11);
	Print_DrawText("\nEncoding...");
	ok = QRCode_EncodeText("https://aoineko.org/msxgl", g_Buffer, g_QR, QRCODE_ECC_MEDIUM, QRCODE_MASK_1);
	Print_DrawFormat("%s\n", ok ? "Ok" : "Error!");
	if(ok)
	{
		Print_SetColor(0xEE, 0x11);
		Print_DrawFormat(" Version:%i, Size:%i\n", QRCode_GetVersion(g_QR), QRCode_GetSize(g_QR));
		Print_DrawText(" Drawing...");
		QRCode_DrawBitmap(196, 32, g_QR);
		Print_DrawText("Ok\n");
	}

	// Text data
	Print_SetColor(0xFF, 0x11);
	Print_DrawText("\nEncoding...");
	ok = QRCode_EncodeText("https://github.com/aoineko-fr/MSXgl", g_Buffer, g_QR, QRCODE_ECC_MEDIUM, QRCODE_MASK_2);
	Print_DrawFormat("%s\n", ok ? "Ok" : "Error!");
	if(ok)
	{
		Print_SetColor(0xEE, 0x11);
		Print_DrawFormat(" Version:%i, Size:%i\n", QRCode_GetVersion(g_QR), QRCode_GetSize(g_QR));
		Print_DrawText(" Drawing...");
		QRCode_DrawBitmap(166, 64, g_QR);
		Print_DrawText("Ok\n");
	}

	// Text data
	Print_SetColor(0xFF, 0x11);
	Print_DrawText("\nEncoding...");
	ok = QRCode_EncodeText("Hello, MSXgl world!\nWelcome to the 8-bits party", g_Buffer, g_QR, QRCODE_ECC_MEDIUM, QRCODE_MASK_3);
	Print_DrawFormat("%s\n", ok ? "Ok" : "Error!");
	if(ok)
	{
		Print_SetColor(0xEE, 0x11);
		Print_DrawFormat(" Version:%i, Size:%i\n", QRCode_GetVersion(g_QR), QRCode_GetSize(g_QR));
		Print_DrawText(" Drawing...");
		QRCode_DrawBitmap(255 - QRCode_GetSize(g_QR) - 8, 64, g_QR);
		Print_DrawText("Ok\n");
	}

	// Text data
	const c8* text =	"MSXglのインストール方法\n"
						"以下の手順に従ってください。\n"
						"1️⃣ ディレクトリを作成する。\n"
						"注：フルパスにスペースを入れないディレクトリを使用することを推奨します。\n"
						"2️⃣ エンジンをインストールします。\n"
						"Gitリポジトリをクローンする。 または、最新版のエンジンをダウンロードし、解凍してください。\n"
						"3️⃣ (オプション) エミュレータ自動起動オプションを使用する場合。\n"
						"✅ これで完了です！ライブラリのサンプルを試したり、まったく新しいゲームを作ったりすることができます。";
	Print_SetColor(0xFF, 0x11);
	Print_DrawText("\nEncoding...");
	ok = QRCode_EncodeText(text, g_Buffer, g_QR, QRCODE_ECC_MEDIUM, QRCODE_MASK_4);
	Print_DrawFormat("%s\n", ok ? "Ok" : "Error!");
	if(ok)
	{
		Print_SetColor(0xEE, 0x11);
		Print_DrawFormat(" Version:%i, Size:%i\n", QRCode_GetVersion(g_QR), QRCode_GetSize(g_QR));
		Print_DrawText(" Drawing...");
		QRCode_DrawBitmap(150, 104, g_QR);
		Print_DrawText("Ok\n");
	}

	// Display logo	
	VDP_SetPaletteEntry(0, COLOR16_DEFAULT_0);
	VDP_SetPaletteEntry(1, COLOR16_DEFAULT_1);
	VDP_SetPaletteEntry(2, COLOR16_DEFAULT_2);
	VDP_SetPaletteEntry(3, COLOR16_DEFAULT_3);
	VDP_SetPaletteEntry(13, RGB16(4, 4, 4));
	for(u16 i = 0; i < 16*16*6; ++i)
	{
		u8 c, b = g_DataBmp4b[i >> 1];
		if((i & 0x1) == 0)
			c = b >> 4;
		else // if((i & 0x1) == 1)
			c = b & 0x0F;
		if(c == 3)
			c = 13;
		g_LMMC4b[i] = c;
	}
	VDP_CommandHMMV(150 + 40 - 2, 104 + 40 - 2, 16 + 4, 16 + 4, 0xFF);
	VDP_CommandLMMC_Arg(g_LMMC4b, 150 + 40 + 15, 104 + 40, 16, 16, VDP_OP_TIMP, VDP_ARG_DIX_LEFT);

	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		Halt();
	}
}