// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Hello world sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "tool/qrcode.h"

u8 qr0[QRCODE_BUFFER_LEN_MAX];
u8 tempBuffer[QRCODE_BUFFER_LEN_MAX];

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
	VDP_SetMode(VDP_MODE_SCREEN5); // Initialize screen mode 5
	VDP_SetColor(COLOR_BLACK);
	VDP_ClearVRAM();

	Print_SetBitmapFont(PRINT_DEFAULT_FONT); // Initialize font (use BIOS font)
	Print_SetColor(0xFF, 0x11);
	// Print_Clear();
	Print_DrawText("MSXgl - QR Code Sample\n");
	Print_DrawText("----------------------------------------\n");	
	Print_DrawFormat("Version Min:%i, Max:%i, Buf size:%i\n", QRCODE_VERSION_MIN, QRCODE_VERSION_MAX, QRCODE_BUFFER_LEN_MAX);

	// Text data
	Print_DrawText("Encoding...\n");
	bool ok = QRCode_EncodeText("Hello, MSXgl world!\nWelcome to the 8-bits party", tempBuffer, qr0, QRCODE_ECC_MEDIUM, QRCODE_MASK_0, FALSE);
	Print_DrawFormat("Result: %s\n", ok ? "Ok" : "Error!");

	i16 size = QRCode_GetSize(qr0);
	Print_DrawFormat("Size: %i\n", size);
	Print_DrawText("Drawing...\n");
	QRCode_DrawBitmap(8, 128, qr0);
		
	// Text data
	Print_DrawText("Encoding...\n");
	ok = QRCode_EncodeText("https://aoineko.org/msxgl", tempBuffer, qr0, QRCODE_ECC_MEDIUM, QRCODE_MASK_0, FALSE);
	Print_DrawFormat("Result: %s\n", ok ? "Ok" : "Error!");

	size = QRCode_GetSize(qr0);
	Print_DrawFormat("Size: %i\n", size);
	Print_DrawText("Drawing...\n");
	QRCode_DrawBitmap(128, 128, qr0);

	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		Halt();
	}
}