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
#include "tool/qrcode_tiny.h"

//=============================================================================
// DEFINES
//=============================================================================

#define POS_X_1		128
#define POS_Y_1		16
#define POS_X_2		196
#define POS_Y_2		(16 + 32)
#define POS_X_3		128
#define POS_Y_3		(16 + 64)
#define POS_X_4		196
#define POS_Y_4		(16 + 96)
#define POS_X_5		128
#define POS_Y_5		(16 + 128)

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font/font_mgl_sample6.h"

//=============================================================================
// MEMORY DATA
//=============================================================================

u8 g_QR[QRCODE_TINY_BUFFER_LEN];
u8 g_Buffer[QRCODE_TINY_BUFFER_LEN];

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
void QRCode_DrawBitmap(u8 x, u8 y, const u8* qrc)
{
	i16 size = QRCode_GetSize(qrc);
	for (i16 j = 0; j < size; j++)
		for (i16 i = 0; i < size; i++) 
			if(QRCode_GetModule(qrc, i, j))
				VDP_CommandPSET(x + i, y + j, COLOR_WHITE, 0);
}

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	// Initialize screen mode
	VDP_SetMode(VDP_MODE_SCREEN5);
	VDP_SetColor(COLOR_BLACK);
	VDP_ClearVRAM();

	// Initialize font
	Print_SetBitmapFont(g_Font_MGL_Sample6);
	Print_SetColor(0xFF, 0x11);
	Print_Clear();

	// Display title
	Print_SetPosition(0, 0);
	Print_DrawText("\x1\x2\x3\x4\x5\x6 QR CODE SAMPLE");
	Draw_LineH(0, 255, 12, 0xFF, 0);

	// Display QR Code Tiny parameters
	Print_SetPosition(0, 16);
	Print_DrawFormat("Version: %i\n", QRCODE_TINY_VERSION);
	Print_DrawFormat("Size:    %i\n", QRCODE_TINY_SIZE);
	Print_DrawFormat("Buffer:  %i\n", QRCODE_TINY_BUFFER_LEN);
	Print_DrawFormat("ECC:     %s\n", QRCODE_TINY_ECC_NAME);
	Print_DrawFormat("Mask:    %i\n", QRCODE_TINY_MASK);
	
	bool ok;
	Print_SetPosition(0, 72);

	// Text data
	Print_SetColor(0xFF, 0x11);
	Print_DrawText("Encoding...");
	ok = QRCode_EncodeText("MSXgl", g_Buffer, g_QR);
	Print_DrawFormat("%s\n", ok ? "Ok" : "Error!");
	if(ok)
	{
		Print_SetColor(0xEE, 0x11);
		Print_DrawText(" Drawing...");
		QRCode_DrawBitmap(POS_X_1, POS_Y_1, g_QR);
		Print_DrawText("Ok\n");
	}

	// Text data
	Print_SetColor(0xFF, 0x11);
	Print_DrawText("\nEncoding...");
	ok = QRCode_EncodeText("https://aoineko.org/msxgl", g_Buffer, g_QR);
	Print_DrawFormat("%s\n", ok ? "Ok" : "Error!");
	if(ok)
	{
		Print_SetColor(0xEE, 0x11);
		Print_DrawText(" Drawing...");
		QRCode_DrawBitmap(POS_X_2, POS_Y_2, g_QR);
		Print_DrawText("Ok\n");
	}

	// Text data
	Print_SetColor(0xFF, 0x11);
	Print_DrawText("\nEncoding...");
	ok = QRCode_EncodeText("https://github.com/aoineko-fr/MSXgl", g_Buffer, g_QR);
	Print_DrawFormat("%s\n", ok ? "Ok" : "Error!");
	if(ok)
	{
		Print_SetColor(0xEE, 0x11);
		Print_DrawText(" Drawing...");
		QRCode_DrawBitmap(POS_X_3, POS_Y_3, g_QR);
		Print_DrawText("Ok\n");
	}

	// Text data
	Print_SetColor(0xFF, 0x11);
	Print_DrawText("\nEncoding...");
	ok = QRCode_EncodeText("Hello, MSXgl world!\nWelcome to the 8-bits party", g_Buffer, g_QR);
	Print_DrawFormat("%s\n", ok ? "Ok" : "Error!");
	if(ok)
	{
		Print_SetColor(0xEE, 0x11);
		Print_DrawText(" Drawing...");
		QRCode_DrawBitmap(POS_X_4, POS_Y_4, g_QR);
		Print_DrawText("Ok\n");
	}

	// Text data
	const c8* text =	"MSXglのINSTALL方法\n"
						"以下の手順に従ってください。\n"
						"1️⃣ ディレクトリを作成する。\n"
						"注:FILE PATHにSPACEを入れないディレクトリを使用することを推奨します。\n"
						"2️⃣ ENGINEをINSTALLします。";
	Print_SetColor(0xFF, 0x11);
	Print_DrawText("\nEncoding...");
	ok = QRCode_EncodeText(text, g_Buffer, g_QR);
	Print_DrawFormat("%s\n", ok ? "Ok" : "Error!");
	if(ok)
	{
		Print_SetColor(0xEE, 0x11);
		Print_DrawText(" Drawing...");
		QRCode_DrawBitmap(POS_X_5, POS_Y_5, g_QR);
		Print_DrawText("Ok\n");
	}

	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		Halt();
	}
}