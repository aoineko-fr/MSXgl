// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Hello world sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "bios.h"
#include "device/printer.h"

//=============================================================================
// DEFINES
//=============================================================================

// Library's logo
#define MSX_GL "\x01\x02\x03\x04\x05\x06"

// Printer access mode
enum PRINTER_MODE
{
	PRINTER_MODE_BIOS = 0,
	PRINTER_MODE_IOPORT,
};

typedef bool (*PrinterCheckCB)();
typedef void (*PrinterSendCB)(u8 chr);

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font/font_mgl_sample6.h"

// Animation characters
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

//=============================================================================
// MEMORY DATA
//=============================================================================

enum PRINTER_MODE g_PrinterMode = PRINTER_MODE_BIOS;
bool g_PrinterReady = TRUE;
u8 g_SendCount = 0;

//=============================================================================
// FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Set printer access mode and update display
bool PrinterCheck()
{
	if (g_PrinterMode == PRINTER_MODE_BIOS)
		return BIOS_IsPrinterReady();
	else
		return Printer_CheckReady();
}

//-----------------------------------------------------------------------------
// Set printer access mode and update display
void PrinterSendChar(u8 chr)
{
	if (g_PrinterMode == PRINTER_MODE_BIOS)
		BIOS_PrinterSendChar(chr);
	else
		Printer_SendChar(chr);
}

//-----------------------------------------------------------------------------
// Set printer access mode and update display
inline void PrinterSendString(const u8* str) { while (*str) PrinterSendChar(*str++); }

//-----------------------------------------------------------------------------
// Set printer access mode and update display
void SetPrinterMode(enum PRINTER_MODE mode)
{
	g_PrinterMode = mode;
	if (mode == PRINTER_MODE_BIOS)
		Print_DrawTextAt(0, 3, "Mode:    BIOS     ");
	else // if (mode == PRINTER_MODE_IOPORT)
		Print_DrawTextAt(0, 3, "Mode:    I/O ports");
}

//-----------------------------------------------------------------------------
// Set printer ready state and update display
void SetPrinterReady(bool ready)
{
	if (g_PrinterReady == ready)
		return;

	g_PrinterReady = ready;
	if (ready)
	{
		Print_DrawTextAt(0, 5, "Printer: Ready    ");
		Print_DrawTextAt(0, 7, "Send:");
		Print_DrawIntAt(9, 7, g_SendCount);
	}
	else
	{
		Print_DrawTextAt(0, 5, "Printer: Not ready");
		Print_DrawTextAt(0, 7, "                  ");
	}
}

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	VDP_SetMode(VDP_MODE_SCREEN0); // Initialize screen mode 0 (text)
	VDP_SetColor(COLOR_BLACK);
	VDP_ClearVRAM();

	Print_SetTextFont(g_Font_MGL_Sample6, 1); // Initialize font
	Print_SetColor(COLOR_WHITE, COLOR_BLACK);
	Print_DrawTextAt(0, 0, MSX_GL" PRINTER SAMPLE"); // Display title
	Print_DrawLineH(0, 1, 40);
	Print_DrawLineH(0, 22, 40);
	Print_DrawTextAt(1, 23, "\x81\x80:Mode  \x83:Send  \x84:New Page");

	SetPrinterMode(PRINTER_MODE_BIOS);
	SetPrinterReady(FALSE);

	u8 frameCount = 0;
	while (1)
	{
		Halt();
		Print_DrawCharAt(39, 0, g_ChrAnim[frameCount++ & 0x03]);

		if (Keyboard_IsKeyPressed(KEY_LEFT))
			SetPrinterMode(PRINTER_MODE_BIOS);
		if (Keyboard_IsKeyPressed(KEY_RIGHT))
			SetPrinterMode(PRINTER_MODE_IOPORT);

		SetPrinterReady(PrinterCheck());
		if (g_PrinterReady)
		{
			if (Keyboard_IsKeyPressed(KEY_SPACE))
			{
				Print_DrawIntAt(9, 7, ++g_SendCount);
				PrinterSendString((g_PrinterMode == PRINTER_MODE_BIOS) ? "BIOS: Hello MSX World!\r\n" : "IO Port: Hello MSX World!\r\n");
				while (Keyboard_IsKeyPressed(KEY_SPACE)) {}
			}
			if (Keyboard_IsKeyPressed(KEY_RETURN))
			{
				PrinterSendString("Change page...");
				PrinterSendChar(ASCII_FORM_FEED);
				while (Keyboard_IsKeyPressed(KEY_RETURN)) {}
			}
		}
	}
}