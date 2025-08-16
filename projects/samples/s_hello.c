// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Hello world sample
//─────────────────────────────────────────────────────────────────────────────
#include "msxgl.h"
#include "tool/disk_save.h"

// Animation characters
const u8 g_ChrAnim[] = { '|', '\\', '-', '/' };

//
u8 g_Slot = 1;

//
u8 SaveBuffer[128];

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	VDP_SetMode(VDP_MODE_SCREEN0); // Initialize screen mode 0 (text)
	VDP_ClearVRAM();

	Print_SetTextFont(PRINT_DEFAULT_FONT, 1); // Initialize font (use BIOS font)
	Print_DrawTextAt(0, 0, "DISK SAVE SAMPLE");
	Print_DrawLineH(0, 1, 40);
	Print_DrawTextAt(0, 2, "1. Select slot 1");
	Print_DrawTextAt(0, 3, "2. Select slot 2");
	Print_DrawTextAt(0, 4, "3. Select slot 3");
	Print_DrawTextAt(0, 5, "4. Select slot 4");
	Print_DrawTextAt(0, 6, "C. Check slot");
	Print_DrawTextAt(0, 7, "S. Save data to disk");
	Print_DrawTextAt(0, 8, "L. Load data from disk");

	Print_DrawTextAt(0, 10, "Current slot:");
	Print_DrawHex8At(14, 10, g_Slot);

	DiskSave_SetName("HELLO"); // Set the base name of the save data file
	DiskSave_SetExtension("BIN"); // Set the extension of the save data file

	u8 count = 0;
	while(!Keyboard_IsKeyPressed(KEY_ESC))
	{
		Print_SetPosition(39, 0);
		Print_DrawChar(g_ChrAnim[count++ & 0x03]);

		if(Keyboard_IsKeyPressed(KEY_1))
		{
			g_Slot = 1;
			Print_DrawHex8At(14, 10, g_Slot);
		}
		else if(Keyboard_IsKeyPressed(KEY_2))
		{
			g_Slot = 2;
			Print_DrawHex8At(14, 10, g_Slot);
		}
		else if(Keyboard_IsKeyPressed(KEY_3))
		{
			g_Slot = 3;
			Print_DrawHex8At(14, 10, g_Slot);
		}
		else if(Keyboard_IsKeyPressed(KEY_4))
		{
			g_Slot = 4;
			Print_DrawHex8At(14, 10, g_Slot);
		}
		else if(Keyboard_IsKeyPressed(KEY_C))
		{
			Print_DrawCharXAt(0, 12, ' ', 40); // Clear line
			u8 state = DiskSave_Check(g_Slot - 1);
			Print_DrawTextAt(0, 12, "Slot state: ");
			Print_DrawHex8At(13, 12, state);
		}
		else if(Keyboard_IsKeyPressed(KEY_S))
		{
			Print_DrawCharXAt(0, 12, ' ', 40); // Clear line
			if(DiskSave_Save(g_Slot, (const u8*)"Hello!", 6))
				Print_DrawTextAt(0, 12, "Data saved successfully.");
			else
				Print_DrawTextAt(0, 12, "Failed to save data.");
		}
		else if(Keyboard_IsKeyPressed(KEY_L))
		{
			Print_DrawCharXAt(0, 12, ' ', 40); // Clear line
			Mem_Set(0, &SaveBuffer, 128); // Clear the buffer
			if(DiskSave_Load(g_Slot, SaveBuffer, 6))
				Print_DrawTextAt(0, 12, SaveBuffer);
			else
				Print_DrawTextAt(0, 12, "Failed to load data.");
		}
	}
}